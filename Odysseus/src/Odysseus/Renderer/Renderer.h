#pragma once

#include "RenderCommand.h"

#include "Camera.h"
#include "Shader.h"

namespace Odysseus
{

	class  Renderer
	{
	public:
		ODYSSEUS_API static void Init();

		ODYSSEUS_API static void Shutdown();

		ODYSSEUS_API static void OnWindowResize(uint32_t width, uint32_t height);

		ODYSSEUS_API static void BeginScene(OrthographicCamera& camera);

		ODYSSEUS_API static void EndScene();

		ODYSSEUS_API static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		ODYSSEUS_API inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private: 

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}
