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
    while(!keyboard.CharBufferIsEmpty()){
        auto ch = keyboard.ReadChar();
        std::string outmsg = "Char: ";
        outmsg += ch;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }
    while (!keyboard.KeyBufferIsEmpty()) {
        auto ch = keyboard.ReadKey();
        std::string outmsg = "Key: ";
        outmsg += ch.GetKeyCode();
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }
}
