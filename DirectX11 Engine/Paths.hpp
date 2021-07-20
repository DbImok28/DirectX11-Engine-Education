#pragma once
#include "Base.hpp"

class Paths
{
private:
	static std::wstring GetPathToExeFolder() noexcept;
public:
	static const std::wstring ExeFolder;
	static const std::wstring ShaderFolder;
};