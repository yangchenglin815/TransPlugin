// TransPluginImpl.h : CTransPluginImpl 的声明

#pragma once
#include "resource.h"       // 主符号
#include "TransForm.h"


#include "TransPlugin_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CTransPluginImpl

class ATL_NO_VTABLE CTransPluginImpl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTransPluginImpl, &CLSID_TransPluginImpl>,
	public IDispatchImpl<ITransPluginImpl, &IID_ITransPluginImpl, &LIBID_TransPluginLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IPlugin2, &__uuidof(IPlugin2), &LIBID_DEWEsoft, /* wMajor = */ 630, /* wMinor = */ 7>
{
public:
	CTransPluginImpl()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_TRANSPLUGINIMPL)


	BEGIN_COM_MAP(CTransPluginImpl)
		COM_INTERFACE_ENTRY(ITransPluginImpl)
		COM_INTERFACE_ENTRY2(IDispatch, IPlugin2)
		COM_INTERFACE_ENTRY(IPlugin2)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

private:
	IApp *app;
	bool used;
	IChannel* ch; 

	void MountChannels();

	// IPlugin2 Methods
public:
	STDMETHOD(Initiate)(IApp * DeweApp);

	STDMETHOD(OnStartAcq)()
	{
		return S_OK;
	}
	STDMETHOD(OnStopAcq)()
	{
		return S_OK;
	}
	STDMETHOD(OnStartStoring)()
	{
		return S_OK;
	}
	STDMETHOD(OnStopStoring)()
	{
		return S_OK;
	}
	STDMETHOD(OnGetData)();

	STDMETHOD(OnTrigger)(double Time)
	{
		return S_OK;
	}
	STDMETHOD(Configure)();

	STDMETHOD(get_Id)(BSTR * Value)
	{
		return S_OK;
	}
	STDMETHOD(SaveSetup)(VARIANT * Data)
	{
		return S_OK;
	}
	STDMETHOD(LoadSetup)(VARIANT Data);

	STDMETHOD(NewSetup)();

	STDMETHOD(ClearChannelsInstance)();

	STDMETHOD(ShowFrame)(long Parent, VARIANT_BOOL * Value);

	STDMETHOD(HideFrame)()
	{
		return S_OK;
	}
	STDMETHOD(UpdateFrame)()
	{
		return S_OK;
	}
	STDMETHOD(get_Used)(VARIANT_BOOL * Value);

	STDMETHOD(put_Used)(VARIANT_BOOL Value);

	STDMETHOD(OnOleMsg)(long Msg, long Param)
	{
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(TransPluginImpl), CTransPluginImpl)
