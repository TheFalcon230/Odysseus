#pragma once

#include "Odysseus.h"


namespace Odysseus
{
	class EditorLayer;

	class WindowTitleBar
	{
	public:
		WindowTitleBar() = default;
		WindowTitleBar(EditorLayer* context);

		void SetContext(EditorLayer* context);
		void OnImGuiRender();
		void OnEvent(Event& e) {};
		inline float height() {
			return m_TitleBarHeight;

		}
	private:
		EditorLayer* Context = nullptr;
		Ref<Texture2D> m_IconTexture;
		float m_TitleBarHeight = 0.0f;
	};
}