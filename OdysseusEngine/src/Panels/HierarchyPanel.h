#pragma once
#include <Odysseus.h>

namespace Odysseus
{
	class HierarchyPanel
	{
	public:
		HierarchyPanel() = default;
		HierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Object GetSelectedObject() const { return OSelectionContext; }
		void SetSelectedObject(Object selectedObject) { OSelectionContext = selectedObject; }
	private:
		void DrawObjectNode(Object object);
		void DrawComponents(Object object);
	private:
		Ref<Scene> Context;
		Object OSelectionContext;
	};
}