#pragma once
#include <comdef.h>
#include "Base.hpp"
#include "StringConverter.hpp"

#define COM_ERROR_IF(hr, msg) if(FAILED(hr)) throw COMException(hr, msg, __FILE__, __FUNCTION__, __LINE__)

class COMException
{
private:
	std::wstring whatMsg;
public:
	COMException(HRESULT hr, const std::string& msg, const char* file, const char* function, int line)
	{
		_com_error error(hr);
		std::wstringstream ss;
		ss << L"Msg: " << StringConverter::StringToWide(msg) << L'\n'
			<< error.ErrorMessage()
			<< L"\nFile: " << file
			<< L"\nFunction: " << function
			<< L"\nLine: " << line;
		whatMsg = ss.str();
	}
	COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, const std::string& line)
	{
		_com_error error(hr);
		std::wstringstream ss;
		ss << L"Msg: " << StringConverter::StringToWide(msg) << L'\n'
			<< error.ErrorMessage()
			<< L"\nFile: " << StringConverter::StringToWide(file)
			<< L"\nFunction: " << StringConverter::StringToWide(function)
			<< L"\nLine: " << StringConverter::StringToWide(line);
		whatMsg = ss.str();
	}
	COMException(HRESULT hr, const std::wstring& msg, const std::wstring& file, const std::wstring& function, const std::wstring& line)
	{
		_com_error error(hr);
		std::wstringstream ss;
		ss  << L"Msg: " << msg << L'\n'
			<< error.ErrorMessage()
			<< L"\nFile: " << file
			<< L"\nFunction: " << function
			<< L"\nLine: " << line;
		whatMsg = ss.str();
	}
	const wchar_t* what() const
	{
		return whatMsg.c_str();
	}
};

