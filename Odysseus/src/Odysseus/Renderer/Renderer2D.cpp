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
		glm::vec3 Normal;
		float Roughness;
		float Metallic;
		float AO;

		// Editor only
		int EntityID;
	};
	struct CubeVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor only
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuadsPerDrawCall = 10000;
		static const uint32_t MaxVerticesPerDrawCall = MaxQuadsPerDrawCall * 4;
		static const uint32_t MaxIndices = MaxQuadsPerDrawCall * 6;
		static const uint32_t MaxTextureSlot = 32;
		static const uint32_t MaxLights = 16;

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> unlitShader;
		Ref<Shader> PBRShader;
		Ref<Texture2D> defaultAlbedoTexture, defaultNormalTexture, defaultORMTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlot> TextureSlots;
		Ref<Texture2D> NormalMapSlot;
		Ref<Texture2D> ORMMapSlot;
		/// <summary>
		/// Current texture slot index (can only starts at 1 -> 0 is reserved to defaultTexture)
		/// </summary>
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];
		glm::vec4 QuadVertexNormal[4];

		glm::vec4 CubeVertexPosition[36];
		glm::vec4 CubeVertexNormal[36];

		struct CameraData
		{
			glm::mat4 ViewProjection;
			//glm::mat4 ModelMatrix;
			//glm::mat3 NormalMatrix;
			glm::vec3 CameraPosition;
		};

		struct LightData
		{
			glm::vec3 Position = glm::vec3(0.0f);
			glm::vec3 Color = glm::vec3(0.0f);
			float Intensity = 1.0f;

			glm::vec3 DirectionalLightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
			glm::vec3 DirectionalLightColor = glm::vec3(0.0f);
			float DirectionalLightIntensity = 1.0f;
		};

		Ref<UniformBuffer> m_CameraBuffer, m_LightBuffer, m_MaterialBuffer;

		Ref<LightData> LightsToRender;

		CameraData cameraData;

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;
	static glm::vec2 defaultTexCoord[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	void Renderer2D::Init()
	{

		s_Data.quadVertexArray = VertexArray::Create();

		s_Data.vertexBuffer = VertexBuffer::Create(s_Data.MaxVerticesPerDrawCall * sizeof(QuadVertex));
		s_Data.vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Float3, "a_Normal"},
			{ ShaderDataType::Float, "a_Roughness"},
			{ ShaderDataType::Float, "a_Metallic"},
			{ ShaderDataType::Float, "a_AO"},
			{ ShaderDataType::Int, "a_EntityID"},
			});
		s_Data.quadVertexArray->AddVertexBuffer(s_Data.vertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVerticesPerDrawCall];

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
		s_Data.quadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		//Default texture creation.
		s_Data.defaultAlbedoTexture = Texture2D::Create("assets/textures/DefaultWhite.png");

		//Default normal texture (normal pointing up)
		s_Data.defaultNormalTexture = Texture2D::Create("assets/textures/DefaultNormal.png");

		//Default ORM texture (R: 1.0, G: 1.0, B: 0.0)
		s_Data.defaultORMTexture = Texture2D::Create("assets/textures/DefaultWhite.png");

		int32_t samplers[s_Data.MaxTextureSlot];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlot; i++)
			samplers[i] = i;

		//s_Data.unlitShader = Shader::Create("assets/shaders/Unlit.glsl");
		s_Data.PBRShader = Shader::Create("assets/shaders/pbr.glsl");

		s_Data.TextureSlots[0] = s_Data.defaultAlbedoTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.QuadVertexNormal[0] = { 0.0f, 0.0f, 1.0f, 0.0f };
		s_Data.QuadVertexNormal[1] = { 0.0f, 0.0f, 1.0f, 0.0f };
		s_Data.QuadVertexNormal[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
		s_Data.QuadVertexNormal[3] = { 0.0f, 0.0f, 1.0f, 0.0f };

		// Vertices position for a cube (6 faces * 2 triangles * 3 vertices = 36 vertices)
		//front
		/*s_Data.CubeVertexPosition[0] = {-0.5f,  0.5f,  0.5f, 1.0f};
		s_Data.CubeVertexPosition[1] =  { -0.5f, -0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[2] =  {  0.5f,  0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[3] =  {  0.5f,  0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[4] =  { -0.5f, -0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[5] =  {  0.5f, -0.5f,  0.5f, 1.0f };

		//Right
		s_Data.CubeVertexPosition[6] =  {  0.5f,  0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[7] =  {  0.5f, -0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[8] =  {  0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[9] =  {  0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[10] = {  0.5f, -0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[11] = {  0.5f, -0.5f, -0.5f, 1.0f };

		//Back
		s_Data.CubeVertexPosition[12] = {  0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[13] = {  0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[14] = { -0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[15] = { -0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[16] = {  0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[17] = { -0.5f, -0.5f, -0.5f, 1.0f };

		//Left
		s_Data.CubeVertexPosition[18] = { -0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[19] = { -0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[20] = { -0.5f,  0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[21] = { -0.5f,  0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[22] = { -0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[23] = { -0.5f, -0.5f,  0.5f, 1.0f };

		//Top
		s_Data.CubeVertexPosition[24] = { -0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[25] = { -0.5f,  0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[26] = {  0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[27] = {  0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[28] = { -0.5f,  0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[29] = {  0.5f,  0.5f,  0.5f, 1.0f };

		//Bottom
		s_Data.CubeVertexPosition[30] = { -0.5f, -0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[31] = { -0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[32] = {  0.5f, -0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[33] = {  0.5f, -0.5f,  0.5f, 1.0f };
		s_Data.CubeVertexPosition[34] = { -0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPosition[35] = {  0.5f, -0.5f, -0.5f, 1.0f };
		

		// Normals for each face of the cube (6 faces * 6 vertices per face)
		s_Data.CubeVertexNormal[0] =  {  0.0f,  0.0f,  1.0f,  0.0f };
		s_Data.CubeVertexNormal[1] =  {  0.0f,  0.0f,  1.0f,  0.0f };
		s_Data.CubeVertexNormal[2] =  {  0.0f,  0.0f,  1.0f,  0.0f };
		s_Data.CubeVertexNormal[3] =  {  0.0f,  0.0f,  1.0f,  0.0f };
		s_Data.CubeVertexNormal[4] =  {  0.0f,  0.0f,  1.0f,  0.0f };
		s_Data.CubeVertexNormal[5] =  {  0.0f,  0.0f,  1.0f,  0.0f };

		s_Data.CubeVertexNormal[6] =  {  1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[7] =  {  1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[8] =  {  1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[9] =  {  1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[10] = {  1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[11] = {  1.0f,  0.0f,  0.0f,  0.0f };

		s_Data.CubeVertexNormal[12] = {  0.0f,  0.0f, -1.0f,  0.0f };
		s_Data.CubeVertexNormal[13] = {  0.0f,  0.0f, -1.0f,  0.0f };
		s_Data.CubeVertexNormal[14] = {  0.0f,  0.0f, -1.0f,  0.0f };
		s_Data.CubeVertexNormal[15] = {  0.0f,  0.0f, -1.0f,  0.0f };
		s_Data.CubeVertexNormal[16] = {  0.0f,  0.0f, -1.0f,  0.0f };
		s_Data.CubeVertexNormal[17] = {  0.0f,  0.0f, -1.0f,  0.0f };

		s_Data.CubeVertexNormal[18] = { -1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[19] = { -1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[20] = { -1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[21] = { -1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[22] = { -1.0f,  0.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[23] = { -1.0f,  0.0f,  0.0f,  0.0f };

		s_Data.CubeVertexNormal[24] = {  0.0f,  1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[25] = {  0.0f,  1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[26] = {  0.0f,  1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[27] = {  0.0f,  1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[28] = {  0.0f,  1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[29] = {  0.0f,  1.0f,  0.0f,  0.0f };

		s_Data.CubeVertexNormal[30] = {  0.0f, -1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[31] = {  0.0f, -1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[32] = {  0.0f, -1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[33] = {  0.0f, -1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[34] = {  0.0f, -1.0f,  0.0f,  0.0f };
		s_Data.CubeVertexNormal[35] = {  0.0f, -1.0f,  0.0f,  0.0f };*/

		s_Data.m_CameraBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0, BufferUsageType::DYNAMIC_DRAW);
		s_Data.m_LightBuffer = UniformBuffer::Create(sizeof(Renderer2DData::LightData), 1, BufferUsageType::DYNAMIC_DRAW);

	}

	void Renderer2D::Shutdown()
	{
		//ODC_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{

		s_Data.unlitShader->Bind();
		s_Data.unlitShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}


	void Renderer2D::BeginScene(const Camera& camera, glm::mat4 transform)
	{

		//s_Data.unlitShader->Bind();
		//s_Data.unlitShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		s_Data.cameraData.ViewProjection = camera.GetViewProjection();
		s_Data.cameraData.CameraPosition = camera.GetPosition();
		//s_Data.cameraData.ModelMatrix = modelMatrix;

		s_Data.PBRShader->Bind();
		s_Data.m_CameraBuffer->Bind();
		s_Data.m_CameraBuffer->SetData(0, sizeof(Renderer2DData::CameraData), &(s_Data.cameraData));
		s_Data.m_CameraBuffer->Unbind();

		s_Data.m_LightBuffer->Bind();
		if (s_Data.LightsToRender)
		{
			s_Data.m_LightBuffer->SetData(0, sizeof(glm::vec3), &s_Data.LightsToRender->Position);
			s_Data.m_LightBuffer->SetData(sizeof(glm::vec3), sizeof(glm::vec3), &s_Data.LightsToRender->Color);
			s_Data.m_LightBuffer->SetData(2*sizeof(glm::vec3), sizeof(float), &s_Data.LightsToRender->Intensity);
		}
		s_Data.m_LightBuffer->Unbind();
		s_Data.PBRShader->Unbind();

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
				s_Data.TextureSlots[i]->Bind(0);

			if (s_Data.NormalMapSlot)
				s_Data.NormalMapSlot->Bind(1);
			else
				s_Data.defaultNormalTexture->Bind(1);

			if (s_Data.ORMMapSlot)
				s_Data.ORMMapSlot->Bind(2);
			else
				s_Data.defaultORMTexture->Bind(2);

			s_Data.PBRShader->Bind();


			RenderCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.QuadIndexCount);
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

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		float textureIndex = 0.0f;
		float tilingFactor = 1.0f;
		float Roughness = 1.0f;
		float Metallic = 1.0f;
		float AO = 1.0f;

		s_Data.TextureSlotIndex = textureIndex;

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = defaultTexCoord[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->Normal = transform * s_Data.QuadVertexNormal[i];
			s_Data.QuadVertexBufferPtr->Roughness = Roughness;
			s_Data.QuadVertexBufferPtr->Metallic = Metallic;
			s_Data.QuadVertexBufferPtr->AO = AO;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Odysseus::Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const Ref<Texture2D>& normalMap, const Ref<Texture2D>& ormMap, float Roughness, float Metallic, float AO,float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlot)
				NextBatch();

			// if texture not found, add to slot
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[textureIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.NormalMapSlot = normalMap;
		s_Data.ORMMapSlot = ormMap;
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = defaultTexCoord[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->Normal = transform * s_Data.QuadVertexNormal[i];
			s_Data.QuadVertexBufferPtr->Roughness = Roughness;
			s_Data.QuadVertexBufferPtr->Metallic = Metallic;
			s_Data.QuadVertexBufferPtr->AO = AO;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawCube(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		// Similar to DrawQuad, but using cube vertices and normals

		/*constexpr size_t quadVertexCount = 36;
		float textureIndex = 0.0f;
		float tilingFactor = 1.0f;

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPosition[i];
			s_Data.CubeVertexBufferPtr->Normal = transform * s_Data.CubeVertexNormal[i];
			s_Data.CubeVertexBufferPtr->Color = color;
			s_Data.CubeVertexBufferPtr->TexCoord = defaultTexCoord[i%4];
			s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
			s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.CubeVertexBufferPtr->EntityID = entityID;
			s_Data.CubeVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount+=6;*/
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Albedo)
		{
			DrawQuad(transform, src.Albedo, src.NormalMap, src.ORMMap, src.Roughness, src.Metallic, src.AO,src.TilingFactor, src.Color, entityID);
		}
		else
		{
			DrawQuad(transform, src.Color, entityID);
		}
	}

	void Renderer2D::DrawPointLight(const glm::vec3& position, const glm::vec3& color, float intensity, float radius, int entityID)
	{
		Ref<Renderer2DData::LightData> lightData = CreateRef<Renderer2DData::LightData>();
		lightData->Position = position;
		lightData->Color = color;
		lightData->Intensity = intensity;
		s_Data.LightsToRender = lightData;
	}
	void Renderer2D::DrawDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity, float radius, int entityID)
	{
		// Render directional light;
		Ref<Renderer2DData::LightData> lightData = CreateRef<Renderer2DData::LightData>();
		lightData->DirectionalLightDirection = direction;
		lightData->DirectionalLightColor = color;
		lightData->DirectionalLightIntensity = intensity;
		s_Data.LightsToRender = lightData;
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