#include "AdapterReader.hpp"

std::vector<AdapterData> AdapterReader::adapters;

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	if(!adapters.empty())
		return adapters;
	com_ptr<IDXGIFactory> pFactory;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create DXGIFactory for adapters");
		exit(-1);
	}
	IDXGIAdapter* pAdapter;
	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
		++index;
	}
	return adapters;
}

AdapterData::AdapterData(IDXGIAdapter* pAdapter) : pAdapter(pAdapter)
{
	HRESULT hr = pAdapter->GetDesc(&desc);
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to get description IDXGIAdapter");
		exit(-1);
	}
}