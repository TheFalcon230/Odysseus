#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include "../imgui/imgui.h"
#include "../implot/implot.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define PROFILE_SCOPE(name) Odysseus::InstrumentationTimer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__);


static const char* L_TestMap =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWDDWWWWWWWWWW"
"WWWWWWWWWWDDDDDDWWWWWWWW"
"WWWWWWWWWWDDDDDDWWWWWWWW"
"WWWWWWWDDDDDDDDDDDDWWWWW"
"WWWWWDDDDDDDDDDDDDDDWWWW"
"WWWWDDDDDDDDDDDDDDDDDWWW"
"WWWWWDDDDDDDDDDDDDDDWWWW"
"WWWWWWWDDDDDDDDDDWWWWWWW"
"WWWWWWWWDDDDDDDWWWWWWWWW"
"WWWWWWWWWDDDDWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.f / 720.f)
{

}

void Sandbox2D::OnAttach()
{
	testQuad = Odysseus::CreateRef<Odysseus::QuadProperties>();
	m_Texture = Odysseus::Texture2D::Create("assets/textures/TestImage.png");
	T_Spritesheet = Odysseus::Texture2D::Create("assets/textures/RPG_Sheet.png");

	mapWidth = 24;
	mapHeight = strlen(L_TestMap) / mapWidth;
	ODC_CORE_INFO("Creating a map of size {0};{1}", mapWidth, mapHeight);

	Sp_Bush_01 = Odysseus::Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 2, 3 }, glm::vec2(128.f));
	s_TextureMap['D'] = Odysseus::Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 6, 11 }, glm::vec2(128.f));
	s_TextureMap['W'] = Odysseus::Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 11, 11 }, glm::vec2(128.f));
	testQuad->sprite = Sp_Bush_01;

	m_CameraController.SetZoomLevel(5.0f);
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

	Odysseus::Renderer2D::ResetStats();
	{
		PROFILE_SCOPE("Renderer Prep");
		Odysseus::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		Odysseus::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Odysseus::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (uint32_t y = 0; y < mapHeight; y++)
		{
			for (uint32_t x = 0; x < mapWidth; x++)
			{
				char tile = L_TestMap[x + y * mapWidth];
				Odysseus::Ref<Odysseus::Sprite> sprite;
				if (s_TextureMap.find(tile) != s_TextureMap.end())
					sprite = s_TextureMap[tile];
				else
					sprite = Sp_Bush_01;
				testQuad->sprite = sprite;
				testQuad->position = glm::vec3({ (x ), (y ) , 0.0f});
				Odysseus::Renderer2D::DrawQuad(*testQuad);
			}
		}
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

	ImGui::Separator();
	ImGui::Text("STATS");
	char label[50];
	strcpy(label, "%.0f FPS ");
	ImGui::Text(label, 1000.f / time.AsMilliseconds());
	ImGui::Text("Draw calls: %d", Odysseus::Renderer2D::GetStats().DrawCalls);
	ImGui::Text("Quads: %d", Odysseus::Renderer2D::GetStats().QuadCount);
	ImGui::Text("Vertices: %d", Odysseus::Renderer2D::GetStats().GetTotalVertexCount());
	ImGui::Text("Tris: %d", Odysseus::Renderer2D::GetStats().GetTotalTrisCount());
	ImGui::Separator();

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
