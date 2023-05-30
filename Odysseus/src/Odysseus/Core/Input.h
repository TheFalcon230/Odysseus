#pragma once

#include "Odysseus/Core/Core.h"

namespace Odysseus
{
	class ODYSSEUS_API Input
	{
	public:
		static inline bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImp(keycode); };
		static inline bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImp(button); };

		static inline float GetMouseX() { return s_Instance->GetMouseXImp(); }
		static inline float GetMouseY() { return s_Instance->GetMouseYImp(); }
		static inline  std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImp(); }

	protected:
		virtual bool IsKeyPressedImp(int keycode) = 0;
		virtual bool IsMouseButtonPressedImp(int button) = 0;

		virtual float GetMouseXImp() = 0;
		virtual float GetMouseYImp() = 0;
		virtual std::pair<float, float> GetMousePosImp() = 0;

	private:
		static Input* s_Instance;
	};
}