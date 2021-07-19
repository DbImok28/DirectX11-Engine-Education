#pragma once
#include "Keyboard/KeyboardClass.hpp"
#include "RenderWindow.hpp"
#include "Mouse/MouseClass.hpp"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	RenderWindow render_window;
	KeyboardClass keyboard;
	MouseClass mouse;
};