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
	private:
		void DrawObjectNode(Object object);
		void DrawComponents(Object object);
	private:
		Ref<Scene> Context;
		Object OSelectionContext;
	};
}