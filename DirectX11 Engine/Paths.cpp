#include "Paths.hpp"

std::wstring Paths::GetPathToExeFolder() noexcept
{
	WCHAR filePath[MAX_PATH];
	GetModuleFileNameW(NULL, filePath, MAX_PATH);

	int i;
	for (i = lstrlen(filePath) - 1; i >= 0; i--)
	{
		if (filePath[i] == L'\\' || filePath[i] == L'/')
		{
			filePath[i + 1] = L'\0';
			break;
		}
	}
	return std::wstring(filePath);
}

std::string Paths::GetDirectoryFromPath(const std::string& filePath)
{
	size_t off1 = filePath.find_last_of('\\');
	size_t off2 = filePath.find_last_of('/');

	if (off1 == std::string::npos && off2 == std::string::npos)
	{
		return{};
	}
	if (off1 == std::string::npos)
	{
		return filePath.substr(0, off2);
	}
	if (off2 == std::string::npos)
	{
		return filePath.substr(0, off1);
	}
	return filePath.substr(0, std::max(off1, off2));
}

std::wstring Paths::GetDirectoryFromPath(const std::wstring& filePath)
{
	size_t off1 = filePath.find_last_of('\\');
	size_t off2 = filePath.find_last_of('/');

	if (off1 == std::wstring::npos && off2 == std::wstring::npos)
	{
		return{};
	}
	if (off1 == std::wstring::npos)
	{
		return filePath.substr(0, off2);
	}
	if (off2 == std::wstring::npos)
	{
		return filePath.substr(0, off1);
	}
	return filePath.substr(0, std::max(off1, off2));
}

std::string Paths::GetFileExtension(const std::string& filePath)
{
	size_t off = filePath.find_last_of('.');
	if (off == std::string::npos)
	{
		return{};
	}
	return std::string(filePath.substr(off + 1));
}

std::wstring Paths::GetFileExtension(const std::wstring& filePath)
{
	size_t off = filePath.find_last_of('.');
	if (off == std::wstring::npos)
	{
		return{};
	}
	return std::wstring(filePath.substr(off + 1));
}

const std::wstring Paths::ExeFolder = GetPathToExeFolder();
const std::wstring Paths::ShaderFolder = ExeFolder;