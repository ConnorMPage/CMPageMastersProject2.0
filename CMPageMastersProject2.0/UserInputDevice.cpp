#include "UserInputDevice.h"
#include <array>

std::array<KeyState, NumKeyCodes> gKeyStates;

//Vector2i gMPosition = { 0, 0 };

void InputInit()
{
	for (auto& keyState : gKeyStates)  keyState = NotPressed;
	//gMPosition = { 0, 0 };
}

//Vector2i MousePos()
//{
//	return gMPosition;
//}

void MouseMove(int X, int Y)
{
	//gMPosition += { X, Y };
}

void KeyDownEvent(KeyCode Key)
{
	if (gKeyStates[Key] == NotPressed)
	{
		gKeyStates[Key] = Pressed;
	}
	else
	{
		gKeyStates[Key] = Held;
	}
}

void KeyUpEvent(KeyCode Key)
{
	gKeyStates[Key] = NotPressed;
}

bool KeyHit(KeyCode eKeyCode)
{
	if (gKeyStates[eKeyCode] == Pressed)
	{
		gKeyStates[eKeyCode] = Held;
		return true;
	}
	return false;
}

bool KeyHeld(KeyCode eKeyCode)
{
	if (gKeyStates[eKeyCode] == NotPressed)
	{
		return false;
	}
	gKeyStates[eKeyCode] = Held;
	return true;
}

bool AnyKeyHit()
{
	bool hit = false;
	for (auto& keyState : gKeyStates)
	{
		if (keyState == Pressed)
		{
			keyState = Held;
			hit = true;
		}
	}
	return hit;
}

bool AnyKeyHeld()
{
	bool hit = false;
	for (auto& keyState : gKeyStates)
	{
		if (keyState != NotPressed)
		{
			keyState = Held;
			hit = true;
		}
	}
	return hit;
}
