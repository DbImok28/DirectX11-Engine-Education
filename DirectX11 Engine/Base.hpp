#pragma once
#include <exception>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11.h>
#include <wrl/client.h>

//#ifdef _DEBUG
//#	pragma comment(lib, "d3dx11d.lib")
//#else
//#	pragma comment(lib, "d3dx11.lib")
//#endif
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")

template<typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;