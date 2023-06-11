#include "EditorLayer.h"

#include "../imgui/imgui.h"
#include "../implot/implot.h"

#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
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

namespace Odysseus
{
	EditorLayer::EditorLayer()
		: Layer("Editor Layer"), m_CameraController(1280.f / 720.f)
	{

	}

	void EditorLayer::OnAttach()
	{
		testQuad = CreateRef<QuadProperties>();
		m_Texture = Texture2D::Create("assets/textures/TestImage.png");
		T_Spritesheet = Texture2D::Create("assets/textures/RPG_Sheet.png");

		mapWidth = 24;
		mapHeight = strlen(L_TestMap) / mapWidth;
		ODC_CORE_INFO("Creating a map of size {0};{1}", mapWidth, mapHeight);

		Sp_Bush_01 = Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 2, 3 }, glm::vec2(128.f));
		s_TextureMap['D'] = Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 6, 11 }, glm::vec2(128.f));
		s_TextureMap['W'] = Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 11, 11 }, glm::vec2(128.f));
		testQuad->sprite = Sp_Bush_01;

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
		m_CameraController.SetZoomLevel(5.0f);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep updateTime)
	{
		PROFILE_FUNCTION();
		time = updateTime;

		{
			PROFILE_SCOPE("Camera OnUpdate");
			m_CameraController.OnUpdate(updateTime);
		}

		Renderer2D::ResetStats();
		{
			PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
			RenderCommand::Clear();
		}

		{
			PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (uint32_t y = 0; y < mapHeight; y++)
			{
				for (uint32_t x = 0; x < mapWidth; x++)
				{
					char tile = L_TestMap[x + y * mapWidth];
					Ref<Sprite> sprite;
					if (s_TextureMap.find(tile) != s_TextureMap.end())
						sprite = s_TextureMap[tile];
					else
						sprite = Sp_Bush_01;
					testQuad->sprite = sprite;
					testQuad->position = glm::vec3({ (x), (y) , 0.0f });
					Renderer2D::DrawQuad(*testQuad);
				}
			}
			Renderer2D::EndScene();
			m_Framebuffer->Unbind();
		}

	}

	void EditorLayer::OnImGuiRender()
	{
		static float t = 0;
		t += time;

		PROFILE_FUNCTION();



		static bool isDockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &isDockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close", "ESCAPE", false))
					Application::Get().QuitApplication();
				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}

		ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_NoResize);

		ImGui::Separator();
		ImGui::Text("STATS");
		char label[50];
		strcpy(label, "%.0f FPS ");
		ImGui::Text(label, 1000.f / time.AsMilliseconds());
		ImGui::Text("Draw calls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Quads: %d", Renderer2D::GetStats().QuadCount);
		ImGui::Text("Vertices: %d", Renderer2D::GetStats().GetTotalVertexCount());
		ImGui::Text("Tris: %d", Renderer2D::GetStats().GetTotalTrisCount());
		ImGui::Separator();

		//Profiling graph
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

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		if (ImGui::Begin("Viewport"))
		{
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (vec_ViewportSize != *((glm::vec2*)&viewportSize))
			{
				m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				vec_ViewportSize = { viewportSize.x, viewportSize.y };

				m_CameraController.OnResize(vec_ViewportSize.x, vec_ViewportSize.y);
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2(vec_ViewportSize.x, vec_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			ImGui::PopStyleVar();
		}

		ImGui::End();

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == EventType::WindowResize)
		{
			auto& re = (WindowResizeEvent&)e;

			float zoom = (float)re.GetWidth() / 1280.f;

			m_CameraController.SetZoomLevel(zoom);
		}
	}

}