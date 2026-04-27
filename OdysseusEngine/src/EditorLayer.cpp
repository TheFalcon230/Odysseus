#include "EditorLayer.h"

#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include <glm/gtc/type_ptr.hpp>

#include "Odysseus/Scene/SceneSerializer.h"
#include "Odysseus/Utils/PlatformUtils.h"


#include "Odysseus/Math/Math.h"
#include <IconsFontAwesome6.h>
#include <imgui_internal.h>

#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
#define PROFILE_FUNCTION() ODC_PROFILE_SCOPE(__FUNCSIG__);

#define ODYSSEUS_DEFAULT_BUTTON_COLOR ImVec4(0.14f, 0.14f, 0.14f, 1.0f)
#define ODYSSEUS_HOVERED_BUTTON_COLOR ImVec4(0.29f, 0.29f, 0.29f, 1.0f)
#define ODYSSEUS_ACTIVE_BUTTON_COLOR ImVec4(0.04f, 0.37f, 0.67f, 1.0f)

namespace Odysseus
{
	extern const std::filesystem::path g_AssetsDirectory;

	EditorLayer::EditorLayer()
		: Layer("Editor Layer"), m_CameraController(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RBGA8,FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		editorScene = CreateRef<Scene>();
		activeScene = editorScene;

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			OpenScene(sceneFilePath);
		}

		mainCameraEditor = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		auto square = activeScene->CreateSquare("Test Square");
		ETestSquare = square;

		hierarchyPanel.SetContext(activeScene);
		windowTitleBar.SetContext(this);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep updateTime)
	{
		PROFILE_FUNCTION();

		time = updateTime;

		activeScene->OnViewportResize((uint32_t)vec_ViewportSize.x, (uint32_t)vec_ViewportSize.y);

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			vec_ViewportSize.x > 0.0f && vec_ViewportSize.y > 0.0f &&
			(spec.Width != vec_ViewportSize.x || spec.Height != vec_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)vec_ViewportSize.x, (uint32_t)vec_ViewportSize.y);
			m_CameraController.OnResize(vec_ViewportSize.x, vec_ViewportSize.y);
			mainCameraEditor.SetViewportSize(vec_ViewportSize.x, vec_ViewportSize.y);
		}

		{
			PROFILE_SCOPE("Camera OnUpdate");
			if (bIsViewportFocused && !bIsUsingGizmo)
			{
				m_CameraController.OnUpdate(updateTime);
			}
			mainCameraEditor.Update(updateTime);
			activeScene->UpdateEditor(updateTime, mainCameraEditor);
		}

		Renderer2D::ResetStats();
		{
			PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
			RenderCommand::Clear();

			// Clear enity ID attachment to -1
			m_Framebuffer->ClearAttachment(1, -1);
		}


		activeScene->UpdateEditor(updateTime, mainCameraEditor);
		{
			PROFILE_SCOPE("Renderer Draw");

			auto [mx, my] = ImGui::GetMousePos();
			mx -= vec_ViewportBounds[0].x;
			my -= vec_ViewportBounds[0].y;
			glm::vec2 viewportSize = vec_ViewportBounds[1] - vec_ViewportBounds[0];
			my = viewportSize.y - my;

			int mouseX = (int)mx;
			int mouseY = (int)my;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				hoveredObject = pixelData == -1 ? Object() : Object((entt::entity)pixelData, activeScene.get());
			}

		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		static float t = 0;
		t += time;

		PROFILE_FUNCTION();

		float yOffset = 0.0f;

		windowTitleBar.OnImGuiRender();

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
			ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + windowTitleBar.height() + yOffset));
			ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - windowTitleBar.height()));
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

		if (bIsProfilerEnabled && ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_NoResize))
		{
			ImGui::BeginChild("Stats", ImVec2(150, 0), ImGuiChildFlags_AutoResizeX);
			ImGui::Text("Render Stats");
			ImGui::Separator();
			char label[50];
			strcpy(label, "%.0f FPS ");
			ImGui::Text(label, 1000.f / time.AsMilliseconds());
			ImGui::Text("Draw calls: %d", Renderer2D::GetStats().DrawCalls);
			ImGui::Text("Quads: %d", Renderer2D::GetStats().QuadCount);
			ImGui::Text("Vertices: %d", Renderer2D::GetStats().GetTotalVertexCount());
			ImGui::Text("Tris: %d", Renderer2D::GetStats().GetTotalTrisCount());
			ImGui::EndChild();
			ImGui::SameLine();

			static ImPlotAxisFlags xflags = ImPlotAxisFlags_None;
			static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit;

			ImGui::BeginChild("Graph", ImVec2(0, 0));
			//Profiling graph
			if (ImPlot::BeginPlot("Graph", ImVec2(-1, -1)))
			{
				ImPlot::SetupAxes("Time (s)", "Execution time (ms)", xflags, yflags);
				ImPlot::SetupAxisLimits(ImAxis_X1, t - 10, t, ImGuiCond_Always);
				const int resultSize = m_ProfileResults.size();

				static std::vector<ScrollingBuffer> sbDatas;
				sbDatas.resize(resultSize);

				for (int i = 0; i < resultSize; i++)
				{
					sbDatas[i].AddPoint(t, m_ProfileResults[i].ExecutionTime);
					//ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.0f);
					ImPlotSpec spec;
					spec.Offset = sbDatas[i].Offset;
					spec.Stride = 2 * sizeof(float);
					spec.FillAlpha = 0.5f;
					ImPlot::PlotShaded(m_ProfileResults[i].Name, &sbDatas[i].Data[0].x, &sbDatas[i].Data[0].y, sbDatas[i].Data.size(), 0, spec);
				}

				m_ProfileResults.clear();
				ImPlot::EndPlot();
			}
			ImGui::EndChild();
			ImGui::End();
		}

		if (bIsCameraDebugEnabled && ImGui::Begin("Camera debug"))
		{
			ImGui::Text("Camera view matrix:");
			ImGui::Text("[%f; %f; %f; %f]", mainCameraEditor.GetViewMatrix()[0].x, mainCameraEditor.GetViewMatrix()[0].y, mainCameraEditor.GetViewMatrix()[0].z, mainCameraEditor.GetViewMatrix()[0].w);
			ImGui::Text("[%f; %f; %f; %f]", mainCameraEditor.GetViewMatrix()[1].x, mainCameraEditor.GetViewMatrix()[1].y, mainCameraEditor.GetViewMatrix()[1].z, mainCameraEditor.GetViewMatrix()[1].w);
			ImGui::Text("[%f; %f; %f; %f]", mainCameraEditor.GetViewMatrix()[2].x, mainCameraEditor.GetViewMatrix()[2].y, mainCameraEditor.GetViewMatrix()[2].z, mainCameraEditor.GetViewMatrix()[2].w);
			ImGui::Text("[%f; %f; %f; %f]", mainCameraEditor.GetViewMatrix()[3].x, mainCameraEditor.GetViewMatrix()[3].y, mainCameraEditor.GetViewMatrix()[3].z, mainCameraEditor.GetViewMatrix()[3].w);
			ImGui::End();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
		ImGui::SetNextWindowClass(&window_class);

		if (ImGui::Begin("##Viewport", (bool*)true))
		{
			const ImVec2 winSize = ImGui::GetWindowSize();
			// Available space in current window
			ImVec2 avail = ImGui::GetContentRegionAvail();
			float IconSize = 16.0f;
			if (ImGui::BeginChild("##ViewportToolbar", ImVec2(avail.x, IconSize + 2.0f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground))
			{
				ImGui::SetCursorPosX(avail.x * 0.75f);
				ImGui::PushFont(io.Fonts->Fonts[1], IconSize);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ODYSSEUS_HOVERED_BUTTON_COLOR);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ODYSSEUS_ACTIVE_BUTTON_COLOR);

				ImGui::PushStyleColor(ImGuiCol_Button, iGizmoType == ImGuizmo::SELECTION ? ODYSSEUS_ACTIVE_BUTTON_COLOR : ODYSSEUS_DEFAULT_BUTTON_COLOR);
				ImGui::PushID("SelectGuizmoButton");
				if (ImGui::Button(ICON_FA_ARROW_POINTER, ImVec2(0, 0)))//Select
				{
					SetSelectionGizmoType();
				}
				ImGui::PopID();
				ImGui::SameLine();
				ImGui::PopStyleColor(1);

				ImGui::PushStyleColor(ImGuiCol_Button, iGizmoType == ImGuizmo::TRANSLATE ? ODYSSEUS_ACTIVE_BUTTON_COLOR : ODYSSEUS_DEFAULT_BUTTON_COLOR);

				ImGui::PushID("TranslateGuizmoButton");
				if (ImGui::Button(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT, ImVec2(0, 0)))//Move
				{
					SetTranslateGizmoType();
				}
				ImGui::PopID();
				ImGui::SameLine();
				ImGui::PopStyleColor(1);

				ImGui::PushStyleColor(ImGuiCol_Button, iGizmoType == ImGuizmo::ROTATE ? ODYSSEUS_ACTIVE_BUTTON_COLOR : ODYSSEUS_DEFAULT_BUTTON_COLOR);

				ImGui::PushID("RotateGuizmoButton");
				if (ImGui::Button(ICON_FA_ROTATE, ImVec2(0, 0)))//Rotate
				{
					SetRotateGizmoType();
				}
				ImGui::PopStyleColor(1);

				ImGui::PopID();
				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, iGizmoType == ImGuizmo::SCALE ? ODYSSEUS_ACTIVE_BUTTON_COLOR : ODYSSEUS_DEFAULT_BUTTON_COLOR);

				ImGui::PushID("ScaleGuizmoButton");
				if (ImGui::Button(ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER, ImVec2(0, 0)))//Scale
				{
					SetScaleGizmoType();
				}
				ImGui::PopID();
				ImGui::PopStyleColor(1);

				ImGui::SameLine(0, 12.0f);
				const char* gizmoModeText = iGizmoMode == ImGuizmo::LOCAL ? ICON_FA_CUBE : ICON_FA_GLOBE;
				if (ImGui::Button(gizmoModeText))
				{
					iGizmoMode = iGizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
				}
				ImGui::SameLine(0, 12.0f);
				ImGui::PushStyleColor(ImGuiCol_Button, GetSnapTranslation() ? ODYSSEUS_ACTIVE_BUTTON_COLOR : ODYSSEUS_DEFAULT_BUTTON_COLOR);
				if (ImGui::Button(ICON_FA_TABLE_CELLS, ImVec2(0, 0)))//Scale
				{
					SetSnapTranslation(!GetSnapTranslation());
				}
				ImGui::SameLine(0, 0);
				ImGui::Text("50");
				ImGui::PopStyleColor(1);

				ImGui::SameLine(0, 12.0f);

				ImGui::PushStyleColor(ImGuiCol_Button, GetSnapRotation() ? ODYSSEUS_ACTIVE_BUTTON_COLOR : ODYSSEUS_DEFAULT_BUTTON_COLOR);
				if (ImGui::Button(ICON_FA_ROTATE, ImVec2(0, 0)))//Scale
				{
					SetSnapRotation(!GetSnapRotation());
				}
				ImGui::SameLine(0, 0);
				ImGui::Text("10");
				ImGui::PopStyleColor(1);

				ImGui::SameLine(0, 12.0f);

				ImGui::PushStyleColor(ImGuiCol_Button, GetSnapScale() ? ODYSSEUS_ACTIVE_BUTTON_COLOR : ODYSSEUS_DEFAULT_BUTTON_COLOR);
				if (ImGui::Button(ICON_FA_MAXIMIZE, ImVec2(0, 0)))//Scale
				{
					SetSnapScale(!GetSnapScale());
				}
				ImGui::SameLine(0, 0);
				ImGui::Text("0.5");
				ImGui::PopStyleColor(3);

				ImGui::SameLine(0, 12.0f);
				if (ImGui::Button(ICON_FA_VIDEO, ImVec2(0, 0)))
				{
					ImGui::OpenPopup("##EditorCameraSettings");
				}
				ImVec2 button_pos = ImGui::GetItemRectMin();
				ImVec2 button_size = ImGui::GetItemRectSize();
				ImGui::SetNextWindowSize(ImVec2(200, 100));
				if (ImGui::BeginPopupContextItem("##EditorCameraSettings"))
				{
					ImGui::SetWindowPos(ImVec2(button_pos.x, button_pos.y + button_size.y));

					ImGui::Text("Camera settings						");
					mainCameraEditor.SetFOV(DrawFloatControl("Field of view", mainCameraEditor.GetFOV(), 45.0f, 0.0f, 100.0f, 10));
					mainCameraEditor.SetFarClip(DrawFloatControl("Far clip", mainCameraEditor.GetFarClip(), 1000.0f, 10.0f, 1500.0f, 10));
					ImGui::EndPopup();
				}
				ImGui::PopFont();

				ImGui::EndChild();
			}

			ImGuiChildFlags viewportRenderFlags = ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY;
			if (ImGui::BeginChild("##ViewportRender", ImVec2(0, 0)))
			{
				auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
				auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
				auto viewportOffset = ImGui::GetWindowPos();
				vec_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
				vec_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

				bIsViewportFocused = ImGui::IsWindowFocused();
				bIsViewportHovered = ImGui::IsWindowHovered();

				Application::Get().GetImGuiLayer()->SetCanBlockEvents(!bIsViewportHovered);

				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				vec_ViewportSize = { viewportSize.x, viewportSize.y };

				uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
				ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2{ vec_ViewportSize.x, vec_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
					if (payload)
					{
						const wchar_t* itemPath = (const wchar_t*)payload->Data;

						OpenScene(std::filesystem::path(g_AssetsDirectory) / itemPath);

					}
					ImGui::EndDragDropTarget();
				}

				bIsUsingGizmo = false;
				Object selectedObject = hierarchyPanel.GetSelectedObject();
				if (selectedObject && iGizmoType != (ImGuizmo::OPERATION)-1)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();
					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

					const glm::mat4& cameraProj = mainCameraEditor.GetProjection();
					glm::mat4 cameraView = mainCameraEditor.GetViewMatrix();

					if (selectedObject.HasComponent<TransformComponent>())
					{
						auto& transformComponent = selectedObject.GetComponent<TransformComponent>();
						glm::mat4 transform = transformComponent.Transform();

						float snapValue = 0.0f;

						if (GetSnapRotation() && iGizmoType == ImGuizmo::ROTATE)
						{
							snapValue = 10.0f;
						}
						else if (GetSnapTranslation() && iGizmoType == ImGuizmo::TRANSLATE)
						{
							snapValue = 0.5f;
						}
						else if (GetSnapScale() && iGizmoType == ImGuizmo::SCALE)
						{
							snapValue = 0.5f;
						}

						float snapValues[3] = { snapValue,snapValue,snapValue };

						ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj)
							, iGizmoType, iGizmoMode, glm::value_ptr(transform), nullptr, snapValues);

						bIsUsingGizmo = ImGuizmo::IsUsing();
						if (bIsUsingGizmo)
						{
							glm::vec3 position, rotation, scale;
							Math::DecomposeTransform(transform, position, rotation, scale);

							glm::vec3 deltaRotation = rotation - transformComponent.Rotation;// Using delta rotation instead of new rotation to avoid Gimbal-Lock
							transformComponent.Position = position;
							transformComponent.Rotation += deltaRotation;
							transformComponent.Scale = scale;
						}
					}
				}
				ImGui::EndChild();
			}
			ImGui::End();
			ImGui::PopStyleVar();
		}

		hierarchyPanel.OnImGuiRender();
		contentBrowserPanel.OnImGuiRender();

		ImGui::End();


	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		if (!bIsUsingGizmo)
		{
			mainCameraEditor.OnEvent(e);
		}

		if (e.GetEventType() == EventType::WindowResize)
		{
			auto& re = (WindowResizeEvent&)e;

			float zoom = (float)re.GetWidth() / 1280.f;

			m_CameraController.SetZoomLevel(zoom);
		}
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(ODC_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(ODC_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);

		switch (e.GetKeyCode())
		{
		case (int)Key::S: // Save
		{
			if (control)
			{
				if (shift)
				{
					SaveSceneAs();
				}
				else
				{
					SaveScene();
				}
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
		case (int)Key::P:// Open
		{
			if (control && alt)
			{
				bIsProfilerEnabled = !bIsProfilerEnabled;
			}
			break;
		}
		case (int)Key::D:// Open
		{
			if (control && shift)
			{
				bIsCameraDebugEnabled = !bIsCameraDebugEnabled;
			}
			break;
		}

		// Gizmos shortcuts
		case (int)Key::Space: // Switching between gizmos
		{
			if (iGizmoType == ImGuizmo::SCALE)
			{
				SetTranslateGizmoType();
			}
			else if (iGizmoType == ImGuizmo::TRANSLATE)
			{
				SetRotateGizmoType();
			}
			else if (iGizmoType == ImGuizmo::ROTATE)
			{
				SetScaleGizmoType();
			}
			break;
		}
		case (int)Key::A: // No gizmo
		{
			SetSelectionGizmoType();
			break;
		}case (int)Key::W: // Position
		{
			SetTranslateGizmoType();
			break;
		}
		case (int)Key::E: // Rotation
		{
			SetRotateGizmoType();
			break;
		}
		case (int)Key::R: // Scale
		{
			SetScaleGizmoType();
			break;
		}

		case (int)Key::Delete:
		{
			if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
			{
				Object selectedEntity = hierarchyPanel.GetSelectedObject();
				if (selectedEntity)
				{
					hierarchyPanel.SetSelectedObject({});
					activeScene->DestroyObject(selectedEntity);
				}
			}
			break;
		}
		}
		return true;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == (int)Mouse::ButtonLeft)
		{
			if (bIsViewportHovered && !ImGuizmo::IsOver())
				hierarchyPanel.SetSelectedObject(hoveredObject);
		}
		return false;
	}

	float EditorLayer::DrawFloatControl(const std::string& label, float currentValue, float resetValue, float min, float max, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImVec2 avail = ImGui::GetContentRegionAvail();

		float value = currentValue;

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, avail.x * 0.5f);
		if (ImGui::Button(label.c_str(), ImVec2(avail.x * 0.5f, 0.0f)))
		{
			value = resetValue;
		}
		ImGui::NextColumn();

		ImGui::SetColumnWidth(0, avail.x * 0.5f);

		float lineHeight = GImGui->Style.FontSizeBase + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::SameLine();
		ImGui::DragFloat("##float", &value, 0.1f, min, max, "%0.2f");

		ImGui::Columns(1);
		ImGui::PopID();

		return value;
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}

	void EditorLayer::NewScene()
	{
		activeScene = CreateRef<Scene>();
		hierarchyPanel.SetContext(activeScene);

		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string openPath = FileDialogs::OpenFile("Odysseus Scene (*.odcmap)\0*.odcmap\0");
		if (!openPath.empty())
		{
			OpenScene(openPath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (!path.empty())
		{
			Ref<Scene> newScene = CreateRef<Scene>();
			SceneSerializer serializer(newScene);
			if (serializer.Deserialize(path.string()))
			{
				activeScene = newScene;
				hierarchyPanel.SetContext(activeScene);

				m_EditorScenePath = path;
			}

		}
	}

	void EditorLayer::SaveScene()
	{
		if (m_EditorScenePath.empty())
		{
			SaveSceneAs();
		}
		else
		{
			SerializeScene(activeScene, m_EditorScenePath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string savePath = FileDialogs::SaveFile("Odysseus Scene (*.odcmap)\0*.odcmap\0");
		if (!savePath.empty())
		{
			SerializeScene(activeScene, savePath);
			m_EditorScenePath = savePath;
		}
	}

}