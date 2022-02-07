#pragma once
#include "Odysseus/Input.h"

namespace Odysseus
{
	class WindowsInput : public Input
	{
	protected:
		// H�rit� via Input
		virtual bool IsKeyPressedImp(int keycode) override;

		// H�rit� via Input
		virtual bool IsMouseButtonPressedImp(int button) override;
		virtual float GetMouseXImp() override;
		virtual float GetMouseYImp() override;

		// H�rit� via Input
		virtual std::pair<float, float> GetMousePosImp() override;
	};
}