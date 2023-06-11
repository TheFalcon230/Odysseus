#pragma once

namespace Odysseus
{

	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

#define ODC_MOUSE_BUTTON_1         0
#define ODC_MOUSE_BUTTON_2         1
#define ODC_MOUSE_BUTTON_3         2
#define ODC_MOUSE_BUTTON_4         3
#define ODC_MOUSE_BUTTON_5         4
#define ODC_MOUSE_BUTTON_6         5
#define ODC_MOUSE_BUTTON_7         6
#define ODC_MOUSE_BUTTON_8         7
#define ODC_MOUSE_BUTTON_LAST      ODC_MOUSE_BUTTON_8
#define ODC_MOUSE_BUTTON_LEFT      ODC_MOUSE_BUTTON_1
#define ODC_MOUSE_BUTTON_RIGHT     ODC_MOUSE_BUTTON_2
#define ODC_MOUSE_BUTTON_MIDDLE    ODC_MOUSE_BUTTON_3 
}