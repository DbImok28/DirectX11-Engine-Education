#pragma once
#include <exception>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

#include "Paths.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11.h>
#include <wrl/client.h>

template<typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;