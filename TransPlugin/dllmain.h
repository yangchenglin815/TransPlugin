// dllmain.h : 模块类的声明。

class CTransPluginModule : public ATL::CAtlDllModuleT< CTransPluginModule >
{
public :
	DECLARE_LIBID(LIBID_TransPluginLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TRANSPLUGIN, "{9A09B1F5-D866-48B4-8110-64E47A3288B5}")
};

extern class CTransPluginModule _AtlModule;
