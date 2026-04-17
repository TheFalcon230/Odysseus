#include "odcpch.h"
#include "Odysseus/Renderer/Renderer2D.h"

#include "Odysseus/Renderer/VertexArray.h"
#include "Odysseus/Renderer/Shader.h"
#include "Odysseus/Renderer/UniformBuffer.h"
#include "Odysseus/Renderer/RenderCommand.h"

#include "Odysseus/Scene/Components.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Odysseus
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor only
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuadsPerDrawCall = 20000;
		static const uint32_t MaxVertices = MaxQuadsPerDrawCall * 4;
		static const uint32_t MaxIndices = MaxQuadsPerDrawCall * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> unlitShader;
		Ref<Texture2D> defaultTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		/// <summary>
		/// Current texture slot index (can only starts at 1 -> 0 is reserved to defaultTexture)
		/// </summary>
		uint32_t TextureSlotIndex = 1;

		glm::vec4 VertexPosition[4];

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;
	static glm::vec2 defaultTexCoord[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	void Renderer2D::Init()
	{
		s_Data.vertexArray = VertexArray::Create();

		s_Data.vertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     }
			});
		s_Data.vertexArray->AddVertexBuffer(s_Data.vertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.vertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		//Default texture creation.
		s_Data.defaultTexture = Texture2D::Create(TextureSpecification());
		uint32_t defaultTextureData = 0xffffffff;
		s_Data.defaultTexture->SetData(&defaultTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.unlitShader = Shader::Create("assets/shaders/Unlit.glsl");

		//s_Data.unlitShader->Bind();
		//s_Data.unlitShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlot);

		s_Data.TextureSlots[0] = s_Data.defaultTexture;

		s_Data.VertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.VertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.VertexPosition[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.VertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		//ODC_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}


	void Renderer2D::BeginScene(const Camera& camera, glm::mat4 transform)
	{
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.vertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.unlitShader->Bind();
			RenderCommand::DrawIndexed(s_Data.vertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x , position.y + size.y, 0.0f };;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.unlitShader->SetFloat("u_Tiling", 1.0f);
		/*s_Data.defaultTexture->Bind();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
		s_Data.unlitShader->SetMat4("u_Transform", transform);

		s_Data.vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.vertexArray);*/
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tillingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor)
	{
		constexpr glm::vec4 color = glm::vec4(1.0f);

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[textureIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		/*s_Data.unlitShader->SetFloat4("u_BaseColor", glm::vec4(1.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
		s_Data.unlitShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data.vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.vertexArray);*/
	}
	void Odysseus::Renderer2D::DrawQuad(const glm::mat4& inTransform, const Ref<Texture2D>& inTexture, float inTillingFactor, const glm::vec4& inColor, int inEntityID)
	{
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = defaultTexCoord;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex + 1; i++)
		{
			auto currentTextureSlot = s_Data.TextureSlots[i];
			if (currentTextureSlot)
			{
				if (*currentTextureSlot == *inTexture)
				{
					textureIndex = (float)i;
					break;
				}
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex > Renderer2DData::MaxTextureSlots)
				NextBatch();

			// if texture not found, add to slot
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[textureIndex] = inTexture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = inTransform * s_Data.VertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = inColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = inTillingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = inEntityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;

		/*s_Data.unlitShader->SetFloat4("u_BaseColor", quad.baseColor);
		s_Data.unlitShader->SetFloat("u_Tiling", quad.tilingFactor);
		glm::mat4 location = glm::translate(glm::mat4(1.0f), quad.position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.rotation), { 0.f, 0.f, 1.0f });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { quad.scale.x, quad.scale.y, 1.0f });
		glm::mat4 transform = location * rotation * scale;
		s_Data.unlitShader->SetMat4("u_Transform", transform);

		if (quad.texture != nullptr)
			quad.texture->Bind();
		else
			s_Data.defaultTexture->Bind();

		s_Data.vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.vertexArray);*/
	}

	void Renderer2D::DrawSprite(const glm::mat4 transform, SpriteRendererComponent& sprite, int entityID)
	{
		if (sprite.texture)
		{
			DrawQuad(transform, sprite.texture, sprite.tilling, sprite.Color, entityID);
		}
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
	}
}