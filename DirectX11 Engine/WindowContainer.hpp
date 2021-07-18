#pragma once
#include "Keyboard/KeyboardClass.hpp"
#include "RenderWindow.hpp"

class WindowContainer
{
public:
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	RenderWindow render_window;
	KeyboardClass keyboard;
};