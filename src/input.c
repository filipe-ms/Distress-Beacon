#include "input.h"

#define GAMEPAD_ID 0
#define AXIS_SENS 0.2f

#pragma region PRESSED

bool IsInputLeftPressed(void) {
	bool keyboard = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_LEFT) ||
			GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_X) < -AXIS_SENS;
	}

	return keyboard || gamepad;
}

bool IsInputRightPressed(void) {
	bool keyboard = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
			GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_X) > AXIS_SENS;
	}

	return keyboard || gamepad;
}

bool IsInputUpPressed(void) {
	bool keyboard = IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_UP) ||
			GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_Y) < -AXIS_SENS;
	}

	return keyboard || gamepad;
}

bool IsInputDownPressed(void) {
	bool keyboard = IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_DOWN) ||
			GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_Y) > AXIS_SENS;
	}

	return keyboard || gamepad;
}

bool IsConfirmButtonPressed(void) {
	bool keyboard = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
	}

	return keyboard || gamepad;
}

bool IsPauseButtonPressed(void) {
	bool keyboard = IsKeyPressed(KEY_P);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_MIDDLE_RIGHT);
	}

	return keyboard || gamepad;
}

bool IsActionButtonPressed(void) {
	bool keyboard = IsKeyPressed(KEY_Z);
	bool gamepad = false;
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
	}
	return keyboard || gamepad;
}

bool IsActionButton2Pressed(void) {
	bool keyboard = IsKeyPressed(KEY_X);
	bool gamepad = false;
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_UP);
	}
	return keyboard || gamepad;
}

bool IsReturnButtonPressed(void) {
	bool keyboard = IsKeyPressed(KEY_BACKSPACE);
	bool gamepad = false;
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
	}
	return keyboard || gamepad;
}

#pragma endregion

#pragma region DOWN

bool IsInputLeftDown(void) {

	bool keyboard = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonDown(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_LEFT) ||
			GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_X) < -AXIS_SENS;
	}

	return keyboard || gamepad;
}

bool IsInputRightDown(void) {

	bool keyboard = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonDown(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
			GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_X) > AXIS_SENS;
	}

	return keyboard || gamepad;
}

bool IsInputUpDown(void) {

	bool keyboard = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonDown(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_UP) ||
			GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_Y) < -AXIS_SENS;
	}

	return keyboard || gamepad;
}

bool IsInputDownDown(void) {

	bool keyboard = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonDown(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_DOWN) ||
			GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_Y) > AXIS_SENS;
	}

	return keyboard || gamepad;
}

bool IsConfirmButtonDown(void) {

	bool keyboard = IsKeyDown(KEY_ENTER) || IsKeyDown(KEY_KP_ENTER);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonDown(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
	}

	return keyboard || gamepad;
}

bool IsPauseButtonDown(void) {

	bool keyboard = IsKeyDown(KEY_P);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonDown(GAMEPAD_ID, GAMEPAD_BUTTON_MIDDLE_RIGHT);

	}

	return keyboard || gamepad;
}

bool IsActionButtonDown(void) {
	bool keyboard = IsKeyDown(KEY_SPACE);
	bool gamepad = false;
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonDown(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
	}
	return keyboard || gamepad;
}

bool IsReturnButtonDown(void) {
	bool keyboard = IsKeyDown(KEY_BACKSPACE);
	bool gamepad = false;
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonDown(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
	}
	return keyboard || gamepad;
}

#pragma endregion

#pragma region RELEASED

bool IsInputLeftReleased(void) {
	bool keyboard = IsKeyReleased(KEY_LEFT) || IsKeyReleased(KEY_A);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonReleased(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
	}

	return keyboard || gamepad;
}

bool IsInputRightReleased(void) {
	bool keyboard = IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_D);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonReleased(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
	}

	return keyboard || gamepad;
}

bool IsInputUpReleased(void) {
	bool keyboard = IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonReleased(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_UP);
	}

	return keyboard || gamepad;
}

bool IsInputDownReleased(void) {
	bool keyboard =
		IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsKeyReleased(KEY_KP_ENTER);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonReleased(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
	}

	return keyboard || gamepad;
}

bool IsConfirmButtonReleased(void) {
	bool keyboard = IsKeyReleased(KEY_ENTER);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonReleased(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
	}

	return keyboard || gamepad;
}

bool IsPauseButtonReleased(void) {
	bool keyboard = IsKeyReleased(KEY_P);

	bool gamepad = false;

	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonReleased(GAMEPAD_ID, GAMEPAD_BUTTON_MIDDLE_RIGHT);
	}

	return keyboard || gamepad;
}

bool IsActionButtonReleased(void) {
	bool keyboard = IsKeyReleased(KEY_SPACE);
	bool gamepad = false;
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonReleased(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
	}
	return keyboard || gamepad;
}

bool IsReturnButtonReleased(void) {
	bool keyboard = IsKeyReleased(KEY_BACKSPACE);
	bool gamepad = false;
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		gamepad = IsGamepadButtonReleased(GAMEPAD_ID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
	}
	return keyboard || gamepad;
}

#pragma endregion

#pragma region DPAD
bool DpadUpPressed(void) {
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		return IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_UP);
	}
	return false;
}

bool DpadDownPressed(void) {
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		return IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
	}
	return false;
}

bool DpadLeftPressed(void) {
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		return IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
	}
	return false;
}

bool DpadRightPressed(void) {
	if (IsGamepadAvailable(GAMEPAD_ID)) {
		return IsGamepadButtonPressed(GAMEPAD_ID, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
	}
	return false;
}
#pragma endregion