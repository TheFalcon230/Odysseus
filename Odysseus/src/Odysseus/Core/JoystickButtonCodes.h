#pragma once

namespace Odysseus
{
	/*! @defgroup joysticks Joysticks
*  @brief Joystick IDs.
*
*  See [joystick input](@ref joystick) for how these are used.
*
*  @ingroup input
*  @{ */
#define ODC_JOYSTICK_1             0
#define ODC_JOYSTICK_2             1
#define ODC_JOYSTICK_3             2
#define ODC_JOYSTICK_4             3
#define ODC_JOYSTICK_5             4
#define ODC_JOYSTICK_6             5
#define ODC_JOYSTICK_7             6
#define ODC_JOYSTICK_8             7
#define ODC_JOYSTICK_9             8
#define ODC_JOYSTICK_10            9
#define ODC_JOYSTICK_11            10
#define ODC_JOYSTICK_12            11
#define ODC_JOYSTICK_13            12
#define ODC_JOYSTICK_14            13
#define ODC_JOYSTICK_15            14
#define ODC_JOYSTICK_16            15
#define ODC_JOYSTICK_LAST          ODC_JOYSTICK_16
/*! @} */

/*! @defgroup gamepad_buttons Gamepad buttons
 *  @brief Gamepad buttons.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define ODC_GAMEPAD_BUTTON_A               0
#define ODC_GAMEPAD_BUTTON_B               1
#define ODC_GAMEPAD_BUTTON_X               2
#define ODC_GAMEPAD_BUTTON_Y               3
#define ODC_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define ODC_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define ODC_GAMEPAD_BUTTON_BACK            6
#define ODC_GAMEPAD_BUTTON_START           7
#define ODC_GAMEPAD_BUTTON_GUIDE           8
#define ODC_GAMEPAD_BUTTON_LEFT_THUMB      9
#define ODC_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define ODC_GAMEPAD_BUTTON_DPAD_UP         11
#define ODC_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define ODC_GAMEPAD_BUTTON_DPAD_DOWN       13
#define ODC_GAMEPAD_BUTTON_DPAD_LEFT       14
#define ODC_GAMEPAD_BUTTON_LAST            ODC_GAMEPAD_BUTTON_DPAD_LEFT

#define ODC_GAMEPAD_BUTTON_CROSS       ODC_GAMEPAD_BUTTON_A
#define ODC_GAMEPAD_BUTTON_CIRCLE      ODC_GAMEPAD_BUTTON_B
#define ODC_GAMEPAD_BUTTON_SQUARE      ODC_GAMEPAD_BUTTON_X
#define ODC_GAMEPAD_BUTTON_TRIANGLE    ODC_GAMEPAD_BUTTON_Y
 /*! @} */

 /*! @defgroup gamepad_axes Gamepad axes
  *  @brief Gamepad axes.
  *
  *  See @ref gamepad for how these are used.
  *
  *  @ingroup input
  *  @{ */
#define ODC_GAMEPAD_AXIS_LEFT_X        0
#define ODC_GAMEPAD_AXIS_LEFT_Y        1
#define ODC_GAMEPAD_AXIS_RIGHT_X       2
#define ODC_GAMEPAD_AXIS_RIGHT_Y       3
#define ODC_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define ODC_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define ODC_GAMEPAD_AXIS_LAST          ODC_GAMEPAD_AXIS_RIGHT_TRIGGER 
}