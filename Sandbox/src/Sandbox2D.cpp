#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include "../imgui/imgui.h"
#include "../implot/implot.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PROFILE_SCOPE(name) Odysseus::InstrumentationTimer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__);

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.f / 720.f)
{

}

void Sandbox2D::OnAttach()
{
	m_Texture = Odysseus::Texture2D::Create("assets/textures/TestImage.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Odysseus::Timestep updateTime)
{
	PROFILE_FUNCTION();
	time = updateTime;

	{
		PROFILE_SCOPE("Camera OnUpdate");
		m_CameraController.OnUpdate(updateTime);
	}

	{
		PROFILE_SCOPE("Renderer Prep");
		Odysseus::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		Odysseus::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Odysseus::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Odysseus::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 0.25f, 1.0f }, m_SquareColor);
		Odysseus::Renderer2D::DrawQuad({ 0.0f,0.0f , -0.1f }, { 20.0f, 20.0f }, m_Texture);
		Odysseus::Renderer2D::EndScene();
	}
	
}

void Sandbox2D::OnImGuiRender()
{
	static float t = 0;
	t += time;

	PROFILE_FUNCTION();
	ImGui::Begin("Details");
	ImGui::ColorEdit4("Base Color", glm::value_ptr(m_SquareColor));


	ImGui::End();

	ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_NoResize);

	if (ImPlot::BeginPlot("Graph", ImVec2(-1, -1)))
	{
		ImPlot::SetupAxes("Time (s)", "Execution time (ms)");
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 10);
		ImPlot::SetupAxisLimits(ImAxis_X1, t - 10, t, ImGuiCond_Always);
		const int resultSize = m_ProfileResults.size();

		static std::vector<ScrollingBuffer> sbDatas;
		sbDatas.resize(resultSize);

		for (int i = 0; i < resultSize; i++)
		{
			sbDatas[i].AddPoint(t, m_ProfileResults[i].Time);
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.0f);
			ImPlot::PlotShaded(m_ProfileResults[i].Name, &sbDatas[i].Data[0].x, &sbDatas[i].Data[0].y, sbDatas[i].Data.size(), -INFINITY, 0, sbDatas[i].Offset, 2 * sizeof(float));
		}

		m_ProfileResults.clear();
		ImPlot::EndPlot();
	}
	ImGui::End();
}

void Sandbox2D::OnEvent(Odysseus::Event& e)
{
	m_CameraController.OnEvent(e);

	if (e.GetEventType() == Odysseus::EventType::WindowResize)
	{
		auto& re = (Odysseus::WindowResizeEvent&)e;

		float zoom = (float)re.GetWidth() / 1280.f;

		m_CameraController.SetZoomLevel(zoom);
	}
}
