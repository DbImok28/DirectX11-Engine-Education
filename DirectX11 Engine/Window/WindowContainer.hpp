#pragma once
#include "RenderWindow.hpp"
#include "../Keyboard/KeyboardClass.hpp"
#include "../Mouse/MouseClass.hpp"
#include "../Graphics/Graphics.hpp"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	RenderWindow render_window;
	KeyboardClass keyboard;
	MouseClass mouse;
	Graphics gfx;
};