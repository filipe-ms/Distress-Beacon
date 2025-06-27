#pragma once

#include "raylib.h"

// Pressed
bool IsInputLeftPressed(void);
bool IsInputRightPressed(void);
bool IsInputUpPressed(void);
bool IsInputDownPressed(void);
bool IsConfirmButtonPressed(void);
bool IsReturnButtonPressed(void);
bool IsPauseButtonPressed(void);
bool IsActionButtonPressed(void);

// Down
bool IsInputLeftDown(void);
bool IsInputRightDown(void);
bool IsInputUpDown(void);
bool IsInputDownDown(void);
bool IsConfirmButtonDown(void);
bool IsReturnButtonDown(void);
bool IsPauseButtonDown(void);
bool IsActionButtonDown(void);

// Released
bool IsInputLeftReleased(void);
bool IsInputRightReleased(void);
bool IsInputUpReleased(void);
bool IsInputDownReleased(void);
bool IsConfirmButtonReleased(void);
bool IsReturnButtonReleased(void);
bool IsPauseButtonReleased(void);
bool IsActionButtonReleased(void);

// Dpad / outro
bool DpadUpPressed(void);
bool DpadDownPressed(void);
bool DpadLeftPressed(void);
bool DpadRightPressed(void);