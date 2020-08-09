// TransPlugin.cpp : DLL 导出的实现。


#include "stdafx.h"
#include "resource.h"
#include "TransPlugin_i.h"
#include "dllmain.h"
#include "xdlldata.h"

#define PLUGIN_KEY "SOFTWARE\\Dewesoft\\Plugins\\TransPlugin"

void WriteDewesoftReg() 
{ 
	HKEY hKey;
	DWORD disposition;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT(PLUGIN_KEY), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &disposition);
    LPCTSTR RegName = TEXT("TransPlugin");
	LPCTSTR RegVendor = TEXT("Dewesoft");
	LPCTSTR RegVersion = TEXT("1.0.0");
	LPCTSTR RegDescription = TEXT("C++ Trans Plugin");
	LPOLESTR lpoleGuid; StringFromCLSID(CLSID_TransPluginImpl, &lpoleGuid);
    RegSetValueEx(hKey, TEXT("Name"), 0, REG_SZ, (LPBYTE)RegName, _tcslen(RegName) * sizeof (TCHAR));
	RegSetValueEx(hKey, TEXT("Vendor"), 0, REG_SZ, (BYTE*)RegVendor, _tcslen(RegVendor) * sizeof(TCHAR));
	RegSetValueEx(hKey, TEXT("Version"), 0, REG_SZ, (BYTE*)RegVersion, _tcslen(RegVersion) * sizeof(TCHAR));
	RegSetValueEx(hKey, TEXT("Description"), 0, REG_SZ, (BYTE*)RegDescription, _tcslen (RegDescription) * sizeof(TCHAR));
	RegSetValueEx(hKey, TEXT("GUID"), 0, REG_SZ, (BYTE*)lpoleGuid, _tcslen(lpoleGuid) * sizeof(TCHAR)); 
    RegCloseKey(hKey); 
}

// 用于确定 DLL 是否可由 OLE 卸载。
STDAPI DllCanUnloadNow(void)
{
	#ifdef _MERGE_PROXYSTUB
	HRESULT hr = PrxDllCanUnloadNow();
	if (hr != S_OK)
		return hr;
#endif
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
	}

// 返回一个类工厂以创建所请求类型的对象。
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	#ifdef _MERGE_PROXYSTUB
	if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
		return S_OK;
#endif
		return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - 在系统注册表中添加项。
STDAPI DllRegisterServer(void)
{
	WriteDewesoftReg();

	// 注册对象、类型库和类型库中的所有接口
	HRESULT hr = _AtlModule.DllRegisterServer();
	#ifdef _MERGE_PROXYSTUB
	if (FAILED(hr))
		return hr;
	hr = PrxDllRegisterServer();
#endif
		return hr;
}

// DllUnregisterServer - 在系统注册表中移除项。
STDAPI DllUnregisterServer(void)
{
	RegDeleteKey(HKEY_LOCAL_MACHINE, TEXT(PLUGIN_KEY));

	HRESULT hr = _AtlModule.DllUnregisterServer();
	#ifdef _MERGE_PROXYSTUB
	if (FAILED(hr))
		return hr;
	hr = PrxDllRegisterServer();
	if (FAILED(hr))
		return hr;
	hr = PrxDllUnregisterServer();
#endif
		return hr;
}

// DllInstall - 按用户和计算机在系统注册表中逐一添加/移除项。
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != NULL)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}

	if (bInstall)
	{	
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}


