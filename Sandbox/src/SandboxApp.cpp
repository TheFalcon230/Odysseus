#include <Odysseus.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "../imgui/imgui.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Odysseus::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.f / 720.f)
	{

		ODC_INFO("Start creating layer.");
		m_SquareVA.reset(Odysseus::VertexArray::Create());

		float squareVertices[4 * 5] = {
			//Vertices
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Odysseus::Ref<Odysseus::VertexBuffer> squareVB;
		squareVB.reset(Odysseus::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Odysseus::BufferLayout squareLayout =
		{
			{Odysseus::ShaderDataType::Float3, "a_Position"},
			{Odysseus::ShaderDataType::Float2, "a_TexCoord"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 0, 3, 2 };

		Odysseus::Ref<Odysseus::IndexBuffer> squareIB;
		squareIB.reset(Odysseus::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_UnlitShader = m_ShaderLibrary.Load("assets/shaders/Unlit.glsl");

		m_Texture = Odysseus::Texture2D::Create("assets/textures/TestImage.png");

		std::dynamic_pointer_cast<Odysseus::OpenGLShader>(m_UnlitShader)->Bind();
		std::dynamic_pointer_cast<Odysseus::OpenGLShader>(m_UnlitShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Odysseus::Timestep updateTime) override
	{
		m_CameraController.OnUpdate(updateTime);
		//ODC_CORE_TRACE("Delta time: {0}s | FPS: {1}", updateTime.AsSeconds(), 1.f / updateTime.AsSeconds());

		Odysseus::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		Odysseus::RenderCommand::Clear();

		Odysseus::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::vec4 redColor(0.8f, 0.3f, 0.2f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Odysseus::OpenGLShader>(m_UnlitShader)->Bind();
		std::dynamic_pointer_cast<Odysseus::OpenGLShader>(m_UnlitShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		m_Texture->Bind();

		Odysseus::Renderer::Submit(m_UnlitShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.f)));
		Odysseus::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Details");
		ImGui::ColorEdit3("Base Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Odysseus::Event& e) override
	{
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == Odysseus::EventType::WindowResize)
		{
			auto& re = (Odysseus::WindowResizeEvent&)e;

			float zoom = (float)re.GetWidth() / 1280.f;

			m_CameraController.SetZoomLevel(zoom);
		}
	}
private:
	Odysseus::ShaderLibrary m_ShaderLibrary;
	Odysseus::Ref<Odysseus::Shader> m_Shader;
	Odysseus::Ref<Odysseus::VertexBuffer> m_VertexBuffer;
	Odysseus::Ref<Odysseus::IndexBuffer> m_IndexBuffer;
	Odysseus::Ref<Odysseus::VertexArray> m_VertexArray;

	Odysseus::Ref<Odysseus::Texture2D> m_Texture;

	Odysseus::Ref<Odysseus::Shader> m_UnlitShader;
	Odysseus::Ref<Odysseus::VertexArray> m_SquareVA;

	Odysseus::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 1.f,1.f,1.f };
};

class Sandbox : public Odysseus::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}

private:

};

Odysseus::Application* Odysseus::CreateApplication()
{
	return new Sandbox();
}