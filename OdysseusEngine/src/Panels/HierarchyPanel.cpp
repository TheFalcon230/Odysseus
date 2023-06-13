#include "HierarchyPanel.h"
#include <imgui/imgui.h>

namespace Odysseus
{
	HierarchyPanel::HierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void HierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		Context = context;
	}

	void HierarchyPanel::OnImGuiRender()
	{
		if (ImGui::Begin("Hierarchy"))
		{
			Context->registry.each([&](auto objectID)
				{
					Object object = Object(objectID, Context.get());
			DrawObjectNode(object);
				}
			);

			ImGui::End();
		}
	}

	void HierarchyPanel::DrawObjectNode(Object object)
	{
		auto& tc = object.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = ((OSelectionContext == object) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, tc.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			OSelectionContext = object;
		}
		if (opened)
		{
			ImGui::TreePop();
		}
	}
}