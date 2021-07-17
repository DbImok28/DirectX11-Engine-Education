#include "ErrorLogger.hpp"
#include <comdef.h>

void ErrorLogger::Log(std::string message)
{
	std::string err_msg = "Error: " + message;
	MessageBoxA(NULL, err_msg.c_str(), "Error", MB_ICONERROR);
}
void ErrorLogger::Log(std::wstring message)
{
	std::wstring err_msg = L"Error: " + message;
	MessageBoxW(NULL, err_msg.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring err_msg = L"Error: " + StringConverter::StringToWide(message) + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, err_msg.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring err_msg = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, err_msg.c_str(), L"Error", MB_ICONERROR);
}