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
    float deltaTime = static_cast<float>(timer.GetMilisecondsElapsed() / 1000.0);

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
            gfx.Camera3D.AdjustRotation(e.GetPosY() * cameraRotSpeed, e.GetPosX() * cameraRotSpeed, 0.0f);
        }
    }

    float cameraSpeed = 20.0f;

    if (keyboard.KeyIsPressed(VK_ESCAPE))
    {
        exit(-1);
    }
    if (keyboard.KeyIsPressed('W'))
    {
        gfx.Camera3D.AdjustPosition(gfx.Camera3D.GetForwardVector() * cameraSpeed * deltaTime);
    }
    if (keyboard.KeyIsPressed('S'))
    {
        gfx.Camera3D.AdjustPosition(gfx.Camera3D.GetBackwardVector() * cameraSpeed * deltaTime);
    }
    if (keyboard.KeyIsPressed('A'))
    {
        gfx.Camera3D.AdjustPosition(gfx.Camera3D.GetLeftVector() * cameraSpeed * deltaTime);
    }
    if (keyboard.KeyIsPressed('D'))
    {
        gfx.Camera3D.AdjustPosition(gfx.Camera3D.GetRightVector() * cameraSpeed * deltaTime);
    }
    if (keyboard.KeyIsPressed(VK_SPACE))
    {
        gfx.Camera3D.AdjustPosition(0.0f, cameraSpeed * deltaTime, 0.0f);
    }
    if (keyboard.KeyIsPressed('Z'))
    {
        gfx.Camera3D.AdjustPosition(0.0f, -cameraSpeed * deltaTime, 0.0f);
    }
    if (keyboard.KeyIsPressed('C'))
    {
        auto lpos = gfx.Camera3D.GetPositionVector();
        lpos += gfx.Camera3D.GetForwardVector() * 2;
        gfx.light.SetRotation(gfx.Camera3D.GetRotationFloat3());
        gfx.light.SetPosition(lpos);
    }
}

void Engine::RenderFrame()
{
    gfx.RenderFrame();
}
