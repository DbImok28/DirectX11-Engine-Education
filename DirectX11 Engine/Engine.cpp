#include "Engine.hpp"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    return render_window.Initialize(this, hInstance, window_title, window_class, width, height);
}

bool Engine::ProcessMessages()
{
    return render_window.ProcessMessages();
}

void Engine::Update()
{

}
