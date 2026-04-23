#include "WindowTitleBar.h"
#include <IconsFontAwesome6.h>
#include <Platform/Windows/WindowsWindow.h>
#include "../../EditorLayer.h"

namespace Odysseus
{
	WindowTitleBar::WindowTitleBar(EditorLayer* context)
	{
		SetContext(context);
	}

	void WindowTitleBar::SetContext(EditorLayer* context)
	{
		Context = context;
		m_IconTexture = Texture2D::Create("assets/textures/Odysseus_Icon.png");
	}

	void WindowTitleBar::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 6.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 10.0f));
		ImGuiIO& io = ImGui::GetIO();

		m_TitleBarHeight = ImGui::GetFrameHeight();
		const float titleBarWidth = ImGui::GetContentRegionAvail().x;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 titleBarPos = ImVec2(viewport->Pos.x, viewport->Pos.y);
		ImGui::SetNextWindowPos(titleBarPos);
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, m_TitleBarHeight));
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_MenuBar;
		if (ImGui::Begin("##Titlebar", nullptr, flags))
		{
			bool titleBarHovered = ImGui::IsWindowHovered();

			//Screen space titlebar hitbox
			ImVec2 p0 = ImGui::GetWindowPos();
			ImVec2 p1 = ImVec2(p0.x + ImGui::GetWindowSize().x, p0.y + ImGui::GetWindowSize().y);
			ImVec2 p0Screen = ImVec2(p0.x - viewport->Pos.x, p0.y - viewport->Pos.y);
			ImVec2 p1Screen = ImVec2(p1.x - viewport->Pos.x, p1.y - viewport->Pos.y);

			//debug titlebar hitbox
			/*ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(p0Screen, p1Screen, IM_COL32(255, 0, 255, 255));*/

			const float frameH = ImGui::GetFrameHeight();
			const float iconWidth = frameH - 2.0f;
			const ImVec2 winSize = ImGui::GetWindowSize();
			const float rightButtonsWidth = iconWidth * 3.0f + ImGui::GetStyle().ItemSpacing.x * 2.0f;

			if (ImGui::BeginMenuBar())
			{
				ImGuiStyle& style = ImGui::GetStyle();
				float originalCursorPosY = ImGui::GetCursorPosY();
				ImGui::SetCursorPosY((winSize.y * 0.5f) - (iconWidth * 0.5f));
				ImGui::Image(m_IconTexture->GetRendererID(), ImVec2(iconWidth, iconWidth), ImVec2(0, 0), ImVec2(1, -1));
				ImGui::SameLine(iconWidth + ImGui::GetStyle().ItemSpacing.x);

				ImGui::SetCursorPosY(originalCursorPosY);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3, 3));
				float originalBorder = style.PopupBorderSize;
				style.PopupBorderSize = 0.0f;
				ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(18, 17, 23, 255));
				ImGui::SetNextWindowSizeConstraints(ImVec2(150, 0), ImVec2(FLT_MAX, FLT_MAX));
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(233, 233, 238, 255));

				//Menu
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					{
						Context->NewScene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						Context->SaveSceneAs();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						Context->OpenScene();
					}

					if (ImGui::MenuItem("Close", "Escape", false))
						Application::Get().QuitApplication();
					ImGui::EndMenu();
				}
				ImGui::SetCursorPosY(originalCursorPosY);

				ImGui::SetNextWindowSizeConstraints(ImVec2(150, 0), ImVec2(FLT_MAX, FLT_MAX));

				if (ImGui::BeginMenu("Tools"))
				{
					if (ImGui::MenuItem("Profiler", "Ctrl + Alt + P", Context->GetProfilerEnabled()))
					{
						Context->SetProfilerEnabled(!Context->GetProfilerEnabled());
					}
					if (ImGui::MenuItem("Camera Debug", "Ctrl + Shift + D", Context->GetCameraDebugEnabled()))
					{
						Context->SetCameraDebugEnabled(!Context->GetCameraDebugEnabled());
					}

					ImGui::EndMenu();
				}

				ImGui::GetStyle().PopupBorderSize = originalBorder;
				ImGui::PopStyleVar();
				ImGui::PopStyleColor(2);

				
				std::string AppName = Application::Get().GetAppName();
				std::transform(AppName.begin(), AppName.end(), AppName.begin(),::toupper);
				ImGui::SetCursorPosX((winSize.x) * 0.5f - (ImGui::CalcTextSize(AppName.c_str()).x) * 0.5f);
				ImGui::SetCursorPosY(originalCursorPosY);

				ImGui::TextUnformatted(AppName.c_str());

				ImGui::SetCursorPosY(originalCursorPosY);
				ImGui::SetCursorPosX(winSize.x - rightButtonsWidth);
				ImVec2 buttonSize = ImVec2(frameH * 1.1f, frameH - 2.0f);

				ImGui::PushFont(io.Fonts->Fonts[1], 16.0f);
				if (ImGui::Button(ICON_FA_MINUS, buttonSize))
				{
					//Minimize
					Application::Get().GetWindow().MinimizeWindow();
				}
				ImGui::SameLine(0, 0);
				ImGui::SetCursorPosY(originalCursorPosY);

				const char* maximizeIcon = Application::Get().GetWindow().IsMaximized() ? ICON_FA_WINDOW_RESTORE : ICON_FA_SQUARE;
				if (ImGui::Button(maximizeIcon, buttonSize))
				{
					//Maximize
					if (Application::Get().GetWindow().IsMaximized())
					{
						Application::Get().GetWindow().RestoreWindow();
					}
					else
					{
						Application::Get().GetWindow().MaximizeWindow();
					}

				}
				ImGui::SameLine(0, 0);
				ImGui::SetCursorPosY(originalCursorPosY);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(255, 0, 0, 255));
				if (ImGui::Button(ICON_FA_XMARK, buttonSize))
				{
					//Close
					Application::Get().QuitApplication();
				}
				ImGui::PopStyleColor();
				ImGui::PopFont();
				ImGui::EndMenuBar();
			}

			ImGui::End();
			ImGui::PopStyleVar(2);

			bool HoveringArea = titleBarHovered && !ImGui::IsAnyItemHovered();

		}
	}

}