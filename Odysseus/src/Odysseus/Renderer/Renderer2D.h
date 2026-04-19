#pragma once
#include "Odysseus/Renderer/Texture.h"
#include "Odysseus/Renderer/Sprite.h"

#include "Odysseus/Renderer/Camera.h"
#include "Odysseus/Renderer/EditorCamera.h"

#include <Odysseus/Scene/Components.h>

namespace Odysseus
{
	struct QuadProperties
	{
		glm::mat4 transform = glm::mat4(1.0f);
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
		/// Draw a quad with texture and color.
		/// </summary>
		/// <param name="inTransform">The transformation matrix for the quad.</param>
		/// <param name="inTexture">The texture of the quad.</param>
		/// <param name="inTillingFactor">The tiling factor for the texture.</param>
		/// <param name="inColor">The color of the quad.</param>
		/// <param name="inEntityID">The entity ID (if = -1 it's considered as invalid).</param>
		static void DrawQuad(const glm::mat4 & inTransform, const Ref<Texture2D>& inTexture, float inTillingFactor, const glm::vec4 & inColor, int inEntityID = -1);

		/// <summary>
		/// Draw a quad with color only.
		/// </summary>
		/// <param name="inTransform">The transformation matrix for the quad.</param>
		/// <param name="inColor">The color of the quad.</param>
		/// <param name="inEntityID">The entity ID (if = -1 it's considered as invalid).</param>
		static void DrawQuad(const glm::mat4 & inTransform, const glm::vec4 & inColor, int inEntityID = -1);

		/// <summary>
		/// Draw a quad with a sprite. The sprite contains both the texture and the texture coordinates.
		/// </summary>
		/// <param name="transform">The transformation matrix for the quad.</param>
		/// <param name="sprite">The sprite component containing the texture and texture coordinates.</param>
		/// <param name="entityID">The entity ID (if = -1 it's considered as invalid).</param>
		static void DrawSprite(const glm::mat4 transform, SpriteRendererComponent& sprite, int entityID);

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
		static void StartBatch();
	};
}