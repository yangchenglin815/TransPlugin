// TransPluginImpl.h : CTransPluginImpl ������

#pragma once
#include "resource.h"       // ������
#include "TransForm.h"


#include "TransPlugin_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
