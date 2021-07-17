#pragma once
#include "RenderWindow.hpp"

class WindowContainer
{
public:
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	RenderWindow render_window;
};