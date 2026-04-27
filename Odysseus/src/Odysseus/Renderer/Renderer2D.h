#pragma once
#include "Odysseus/Renderer/Texture.h"
#include "Odysseus/Renderer/Sprite.h"

#include "Odysseus/Renderer/Camera.h"
#include "Odysseus/Renderer/EditorCamera.h"
#include <Odysseus/Scene/Components.h>

#include "Odysseus/Renderer/UniformBuffer.h"

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
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const Ref<Texture2D>& normalMap = nullptr, const Ref<Texture2D>& ormMap = nullptr, float Roughness = 0.5f, float Metallic = 0.5f, float AO = 0.0f, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		static void DrawCube(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

		static void DrawPointLight(const glm::vec3& position, const glm::vec3& color, float intensity, float radius = 100.0f, int entityID = -1);
		static void DrawDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity, float radius = 100.0f, int entityID = -1);

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