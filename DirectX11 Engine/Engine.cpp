#include "Engine.hpp"
#include <sstream>

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    if (!render_window.Initialize(this, hInstance, window_title, window_class, width, height))
        return false;
    if (!gfx.Initialize(render_window.GetHWND(), width, height))
        return false;
    return true;
}

bool Engine::ProcessMessages()
{
    return render_window.ProcessMessages();
}

void Engine::Update()
{
    while(!keyboard.CharBufferIsEmpty())
    {
        auto ch = keyboard.ReadChar();
    }
    while (!keyboard.KeyBufferIsEmpty()) 
    {
        auto e = keyboard.ReadKey();
    }
    while (!mouse.EventBufferIsEmpty())
    {
        auto e = mouse.ReadEvent();
    }
}

void Engine::RenderFrame()
{
    gfx.RenderFrame();
}
