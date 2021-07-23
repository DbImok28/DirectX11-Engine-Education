#pragma once
#include "Exceptions.hpp"
#include "Base.hpp"

class ErrorLogger
{
public:
	static void Log(std::string message);
	static void Log(std::wstring message);
	static void Log(HRESULT hr ,std::string message);
	static void Log(HRESULT hr, std::wstring message);
	static void Log(COMException& exception);
};