#pragma once
#include "Camera.h"
#include "Texture.h"

namespace Odysseus
{
	class ODYSSEUS_API Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		
		/// <summary>
		/// Draw a quad at a given position with a given size and color.
		/// </summary>
		/// <param name="position">The position of the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="color">The color of the quad.</param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		/// <summary>
		/// Draw a quad at a given position with a given size and color.
		/// </summary>
		/// <param name="position">The position of the quad.
		/// NB: The Z axis for the position can be related to the ZOrder.</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="color">The color of the quad.</param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		/// <summary>
		/// Draw a quad at a given position with a given size and texture.
		/// </summary>
		/// <param name="position">The position of the quad</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="texture">The texture for the quad.</param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		/// <summary>
		/// Draw a quad at a given position with a given size and texture.
		/// </summary>
		/// <param name="position">The position of the quad.
		/// NB: The Z axis for the position can be related to the ZOrder.</param>
		/// <param name="size">The size of the quad</param>
		/// <param name="texture">The texture for the quad.</param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
	private:

	};
}