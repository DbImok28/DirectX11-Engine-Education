#include "RenderWindow.hpp"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

#define HR_CHECK(check, msg) HRESULT hr = check; if (FAILED(hr)) ErrorLogger::Log(hr, msg);

int APIENTRY wWinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		LPWSTR		lpCmdLine,
	_In_		int			nCmdShow)
{
	RenderWindow window;

	window.Initialize(hInstance,"DirectX11 Engine","directx11_engine",600,400);

	while (window.ProcessMessages() == true)
	{

	}
	return 0;
}