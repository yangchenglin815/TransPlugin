// TransPluginImpl.cpp : CTransPluginImpl µÄÊµÏÖ

#include "stdafx.h"
#include "TransPluginImpl.h"

STDMETHODIMP CTransPluginImpl::Initiate(IApp * DeweApp)
{
	app = DeweApp;
	return S_OK;
}

STDMETHODIMP CTransPluginImpl::get_Used(VARIANT_BOOL * Value)
{
	*Value = used;
	return S_OK;
}

STDMETHODIMP CTransPluginImpl::put_Used(VARIANT_BOOL Value)
{
	used =  Value;
	return S_OK;
}

STDMETHODIMP CTransPluginImpl::Configure()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TransForm dlg;
	dlg.DoModal();
	return S_OK;
}

STDMETHODIMP CTransPluginImpl::ShowFrame(long Parent, VARIANT_BOOL * Value)
{
	*Value = false;
	return S_OK;
}

STDMETHODIMP CTransPluginImpl::LoadSetup(VARIANT Data)
{
	MountChannels();
	return S_OK;
}

STDMETHODIMP CTransPluginImpl::NewSetup()
{
	MountChannels();
	return S_OK;
}

STDMETHODIMP CTransPluginImpl::OnGetData()
{
	IMasterClock* mClock;
	double time;
	float value;
    app->get_MasterClock(&mClock);
	mClock->GetCurrentTime(&time);
    value = (float)(rand())/RAND_MAX;

	if (time >= 0) 
		ch->AddAsyncSingleSample(value, time);

    return S_OK;
}

STDMETHODIMP CTransPluginImpl::ClearChannelsInstance()
{
	ch = NULL;
	return S_OK;
}

void CTransPluginImpl::MountChannels() 
{ 
	IData* data; 
	IChannelGroups* groups;
	IChannelGroup* group;
	IPluginGroup* pluginGroup;
    app->get_Data(&data);
	data->get_Groups(&groups);
	groups->get_Item(8, &group);
	group->QueryInterface(IID_IPluginGroup, (void **) &pluginGroup);
   
    pluginGroup->MountChannel(5, TRUE, -1, &ch);
	_bstr_t s = "Test channel";
	ch->put_Name(s);
	ch->put_Used(TRUE);
}
// CTransPluginImpl

