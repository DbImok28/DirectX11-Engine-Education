#pragma once
#include "Base.hpp"
#include "Window/WindowContainer.hpp"
#include "Timer.hpp"

class Engine : public WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
private:
	Timer timer;
};