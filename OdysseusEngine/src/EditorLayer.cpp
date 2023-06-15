#include "EditorLayer.h"

#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include <glm/gtc/type_ptr.hpp>

#include "Odysseus/Scene/SceneSerializer.h"
#include "Odysseus/Utils/PlatformUtils.h"

#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__);



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

		Sp_Bush_01 = Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 2, 3 }, glm::vec2(128.f));
		s_TextureMap['D'] = Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 6, 11 }, glm::vec2(128.f));
		s_TextureMap['W'] = Sprite::CreateFromCoords(T_Spritesheet, glm::vec2{ 11, 11 }, glm::vec2(128.f));
		testQuad->sprite = Sp_Bush_01;

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
		m_CameraController.SetZoomLevel(5.0f);

		activeScene = CreateRef<Scene>();
		/*auto square = activeScene->CreateObject("Test Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
		ETestSquare = square;

		auto square1 = activeScene->CreateObject("Red Square");
		square1.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		ETestSquare = square1;

		ECamera = activeScene->CreateObject("Main Camera");
		ECamera.AddComponent<CameraComponent>();*/

		hierarchyPanel.SetContext(activeScene);



	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep updateTime)
	{
		PROFILE_FUNCTION();
		time = updateTime;

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			vec_ViewportSize.x > 0.0f && vec_ViewportSize.y > 0.0f &&
			(spec.Width != vec_ViewportSize.x || spec.Height != vec_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)vec_ViewportSize.x, (uint32_t)vec_ViewportSize.y);
			m_CameraController.OnResize(vec_ViewportSize.x, vec_ViewportSize.y);

			activeScene->OnViewportResize((uint32_t)vec_ViewportSize.x, (uint32_t)vec_ViewportSize.y);
		}

		{
			PROFILE_SCOPE("Camera OnUpdate");
			if (bIsViewportFocused)
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
			activeScene->Update(updateTime);
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
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = 32.0f;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveAs();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Close", "Escape", false))
					Application::Get().QuitApplication();
				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}

		if (ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_NoResize))
		{

			ImGui::Text("Render Stats");
			ImGui::Separator();
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
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("Viewport", (bool*)true))
		{
			bIsViewportFocused = ImGui::IsWindowFocused();
			bIsViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetCanBlockEvents(!bIsViewportFocused || !bIsViewportHovered);

			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			vec_ViewportSize = { viewportSize.x, viewportSize.y };

			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ vec_ViewportSize.x, vec_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });			ImGui::End();
			ImGui::PopStyleVar();
		}

		hierarchyPanel.OnImGuiRender();

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

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(ODC_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case (int)Key::S: // Save
		{
			if (control && shift)
			{
				SaveAs();
			}
			break;
		}
		case (int)Key::O:// Open
		{
			if (control)
			{
				OpenScene();
			}
			break;
		}
		case (int)Key::N:// Open
		{
			if (control)
			{
				NewScene();
			}
			break;
		}
		}
	}

	void EditorLayer::NewScene()
	{
		activeScene = CreateRef<Scene>();
		activeScene->OnViewportResize((uint32_t)vec_ViewportSize.x, (uint32_t)vec_ViewportSize.y);
		hierarchyPanel.SetContext(activeScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string openPath = FileDialogs::OpenFile("Odysseus Scene (*.odcmap)\0*.odcmap\0");
		if (!openPath.empty())
		{
			activeScene = CreateRef<Scene>();
			activeScene->OnViewportResize((uint32_t)vec_ViewportSize.x, (uint32_t)vec_ViewportSize.y);
			hierarchyPanel.SetContext(activeScene);

			SceneSerializer serializer(activeScene);
			serializer.Deserialize(openPath);
		}
	}

	void EditorLayer::SaveAs()
	{
		std::string savePath = FileDialogs::SaveFile("Odysseus Scene (*.odcmap)\0*.odcmap\0");
		if (!savePath.empty())
		{
			SceneSerializer serializer(activeScene);
			serializer.Serialize(savePath);
		}
	}

}