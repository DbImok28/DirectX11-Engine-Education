#include "Engine.hpp"

#define HR_CHECK(check, msg) HRESULT hr = check; if (FAILED(hr)) ErrorLogger::Log(hr, msg);

int APIENTRY wWinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		LPWSTR		lpCmdLine,
	_In_		int			nCmdShow)
{
	Engine engine;

	engine.Initialize(hInstance,"DirectX11 Engine","directx11_engine",600,400);

	while (engine.ProcessMessages() == true)
	{
		engine.Update();
		engine.RenderFrame();
	}
	return 0;
}