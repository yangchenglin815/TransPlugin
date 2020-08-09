// dllmain.cpp : DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "TransPlugin_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CTransPluginModule _AtlModule;

class CTransPluginApp : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTransPluginApp, CWinApp)
END_MESSAGE_MAP()

CTransPluginApp theApp;

BOOL CTransPluginApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CTransPluginApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
