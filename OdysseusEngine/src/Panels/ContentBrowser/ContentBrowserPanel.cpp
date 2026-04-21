#include "odcpch.h"

#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

#include <IconsFontAwesome6.h>

namespace Odysseus
{
	extern const std::filesystem::path g_AssetsDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetsDirectory)
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Begin("Content Browser");

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		ImGui::PushFont(io.Fonts->Fonts[1], thumbnailSize / 4);

		if (m_CurrentDirectory != std::filesystem::path(g_AssetsDirectory))
		{
			if (ImGui::Button(ICON_FA_ARROW_TURN_UP"##_PARENTBUTTON"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}
		ImGui::PopFont();

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
		for (auto& p : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			ImGui::PushFont(io.Fonts->Fonts[1], thumbnailSize);
			const auto& path = p.path();
			auto relativePath = std::filesystem::relative(path, g_AssetsDirectory);
			std::string filenameString = relativePath.filename().string();

			const char* icon = p.is_directory() ? ICON_FA_FOLDER : ICON_FA_FILE;

			ImGui::Button((std::string(icon) + "##" + filenameString).c_str(), {thumbnailSize, thumbnailSize});
			ImGui::PopFont();

			if(ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (p.is_directory())
				{
					m_CurrentDirectory /= relativePath;
				}
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::End();
	}
}