#include "Engine.hpp"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    timer.Start();
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
    float deltaTime = timer.GetMilisecondsElapsed() / 1000.0;

    timer.Restart();
    while(!keyboard.CharBufferIsEmpty())
    {
        auto ch = keyboard.ReadChar();
    }
    while (!keyboard.KeyBufferIsEmpty()) 
    {
        auto e = keyboard.ReadKey();
    }
    float cameraRotSpeed = 0.005f;
    while (!mouse.EventBufferIsEmpty())
    {
        auto e = mouse.ReadEvent();
        if (!keyboard.KeyIsPressed(VK_SHIFT) && e.GetType() == MouseEvent::EventType::RawMove)
        {
            gfx.camera.AdjustRotation(e.GetPosY() * cameraRotSpeed, e.GetPosX() * cameraRotSpeed, 0.0f);
        }
    }

    float cameraSpeed = 8.0f;

    if (keyboard.KeyIsPressed(VK_ESCAPE))
    {
        exit(-1);
    }
    if (keyboard.KeyIsPressed('W'))
    {
        gfx.camera.AdjustPosition(gfx.camera.GetForwardVector() * cameraSpeed * deltaTime);
    }
    if (keyboard.KeyIsPressed('S'))
    {
        gfx.camera.AdjustPosition(gfx.camera.GetBackwardVector() * cameraSpeed * deltaTime);
    }
    if (keyboard.KeyIsPressed('A'))
    {
        gfx.camera.AdjustPosition(gfx.camera.GetLeftVector() * cameraSpeed * deltaTime);
    }
    if (keyboard.KeyIsPressed('D'))
    {
        gfx.camera.AdjustPosition(gfx.camera.GetRightVector() * cameraSpeed * deltaTime);
    }
    if (keyboard.KeyIsPressed(VK_SPACE))
    {
        gfx.camera.AdjustPosition(0.0f, cameraSpeed * deltaTime, 0.0f);
    }
    if (keyboard.KeyIsPressed('Z'))
    {
        gfx.camera.AdjustPosition(0.0f, -cameraSpeed * deltaTime, 0.0f);
    }
}

void Engine::RenderFrame()
{
    gfx.RenderFrame();
}
