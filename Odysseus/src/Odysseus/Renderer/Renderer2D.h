#pragma once
#include "Odysseus/Renderer/Texture.h"
#include "Odysseus/Renderer/Sprite.h"

#include "Odysseus/Renderer/Camera.h"
#include "Odysseus/Renderer/EditorCamera.h"

namespace Odysseus
{
	struct QuadProperties
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec2 scale = glm::vec2(1.0f);
		Ref<Texture2D> texture;
		Ref<Sprite> sprite;
		glm::vec4 baseColor = glm::vec4(1.0f);
		float tilingFactor = 1.0f;
		QuadProperties()
		{
		}
	};

	class ODYSSEUS_API Renderer2D
	{
	public:

		

		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, glm::mat4 transform);
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		// Primitives
		
		/// <summary>
		/// [DEPRECATED]
		/// Draw a quad at a given position with a given size and color.
		/// </summary>
		/// <param name="position">The position of the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="color">The color of the quad.</param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		/// <summary>
		/// [DEPRECATED]
		/// Draw a quad at a given position with a given size and color.
		/// </summary>
		/// <param name="position">The position of the quad.
		/// NB: The Z axis for the position can be related to the ZOrder.</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="color">The color of the quad.</param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		/// <summary>
		/// [DEPRECATED]
		/// Draw a quad at a given position with a given size and texture.
		/// </summary>
		/// <param name="position">The position of the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="texture">The texture for the quad.</param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor = 1.0f);
		/// <summary>
		/// [DEPRECATED]
		/// Draw a quad at a given position with a given size and texture.
		/// </summary>
		/// <param name="position">The position of the quad.
		/// NB: The Z axis for the position can be related to the ZOrder.</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="texture">The texture for the quad.</param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor = 1.0f);
		/// <summary>
		/// Draw a quad with given properties.
		/// </summary>
		/// <param name="Quad">The quad properties.</param>
		static void DrawQuad(const QuadProperties& Quad);

		//Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalTrisCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();
		
	private:
		static void NextBatch();
		static void NewBatch();
	};
}