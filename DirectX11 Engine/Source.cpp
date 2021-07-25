#include "Engine.hpp"

#define HR_CHECK(check, msg) HRESULT hr = check; if (FAILED(hr)) ErrorLogger::Log(hr, msg);

int APIENTRY wWinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		LPWSTR		lpCmdLine,
	_In_		int			nCmdShow)
{
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to call CoInitialize.");
		return -1;
	}

	Engine engine;

	if (engine.Initialize(hInstance, "DirectX11 Engine", "directx11_engine", 1080, 720))
	{
		while (engine.ProcessMessages() == true)
		{
			engine.Update();
			engine.RenderFrame();
		}
	}
	return 0;
}