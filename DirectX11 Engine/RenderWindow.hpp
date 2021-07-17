#pragma once
#include "ErrorLogger.hpp"

class RenderWindow
{
public:
	RenderWindow(const RenderWindow&) = delete;
	RenderWindow(RenderWindow&&) = delete;
	RenderWindow& operator=(const RenderWindow&) = delete;
	RenderWindow& operator=(RenderWindow&&) = delete;
	RenderWindow() {}
	//RenderWindow(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND hWnd = NULL;
	HINSTANCE hInst = NULL;
	std::string  window_title = "";
	std::wstring window_title_wide = L"";
	std::string  window_class = "";
	std::wstring window_class_wide = L"";
	int width = 0;
	int height = 0;
};

