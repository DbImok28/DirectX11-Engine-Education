#include "Paths.hpp"

std::wstring Paths::GetPathToExeFolder() noexcept
{
	WCHAR filePath[MAX_PATH];
	GetModuleFileNameW(0, filePath, MAX_PATH);

	int i;
	for (i = lstrlen(filePath) - 1; i >= 0; i--)
	{
		if (filePath[i] == L'\\')
		{
			filePath[i + 1] = L'\0';
			break;
		}
	}
	return std::wstring(filePath);
}

const std::wstring Paths::ExeFolder = GetPathToExeFolder();
const std::wstring Paths::ShaderFolder = ExeFolder;