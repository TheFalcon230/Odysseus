#pragma once

#include "Odysseus/Core/Core.h"
#include "Odysseus/Core/KeyCode.h"
#include "Odysseus/Core/MouseButtonCodes.h"

namespace Odysseus
{
	class ODYSSEUS_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePos() ;
		static float GetMouseX();
		static float GetMouseY() ;
	};
}