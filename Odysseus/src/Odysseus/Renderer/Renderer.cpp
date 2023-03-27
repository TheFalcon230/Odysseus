#include "odcpch.h"
#include "Renderer.h"


namespace Odysseus
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->UploadUniforMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(vertexArray);
	}
}