#include <Odysseus.h>

#include "../oui/imgui.h"

class ExampleLayer : public Odysseus::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition({0.0f, 0.0f, 0.0f})
	{

		ODC_INFO("Start creating layer.");
		m_SquareVA.reset(Odysseus::VertexArray::Create());

		float squareVertices[3 * 4] = {
			//Vertices
			 0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		std::shared_ptr<Odysseus::VertexBuffer> squareVB;
		squareVB.reset(Odysseus::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Odysseus::BufferLayout squareLayout =
		{
			{Odysseus::ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<Odysseus::IndexBuffer> squareIB;
		squareIB.reset(Odysseus::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;			

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position,1.0);
			}
		)";

		std::string FragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 finalColor;

			in vec3 v_Position;

			void main()
			{
				finalColor = vec4(0.4f, 0.25f, 0.85f, 1.0f);
			}
		)";

		m_Shader2.reset(new Odysseus::Shader(vertexSrc2, FragmentSrc2));
	}

	void OnUpdate() override
	{
		if (Odysseus::Input::IsKeyPressed( ODC_KEY_Q))
		{
			m_CameraPosition.x -= m_CameraSpeed;
		}
		if (Odysseus::Input::IsKeyPressed(ODC_KEY_D))
		{
			m_CameraPosition.x += m_CameraSpeed;
		}
		if (Odysseus::Input::IsKeyPressed(ODC_KEY_S))
		{
			m_CameraPosition.y -= m_CameraSpeed;
		}
		if (Odysseus::Input::IsKeyPressed(ODC_KEY_Z))
		{
			m_CameraPosition.y += m_CameraSpeed;
		}


		Odysseus::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		Odysseus::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);

		Odysseus::Renderer::BeginScene(m_Camera);

		Odysseus::Renderer::Submit(m_Shader2, m_SquareVA);


		Odysseus::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(Odysseus::Event& event) override
	{
		
	}
private:
	std::shared_ptr<Odysseus::Shader> m_Shader;
	std::shared_ptr<Odysseus::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Odysseus::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Odysseus::VertexArray> m_VertexArray;


	std::shared_ptr<Odysseus::Shader> m_Shader2;
	std::shared_ptr<Odysseus::VertexArray> m_SquareVA;

	Odysseus::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 0.1f;
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