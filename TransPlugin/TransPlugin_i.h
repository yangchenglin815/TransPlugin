

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Aug 27 21:07:15 2020
 */
/* Compiler settings for TransPlugin.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __TransPlugin_i_h__
#define __TransPlugin_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITransPluginImpl_FWD_DEFINED__
#define __ITransPluginImpl_FWD_DEFINED__
typedef interface ITransPluginImpl ITransPluginImpl;
#endif 	/* __ITransPluginImpl_FWD_DEFINED__ */


#ifndef __TransPluginImpl_FWD_DEFINED__
#define __TransPluginImpl_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransPluginImpl TransPluginImpl;
#else
typedef struct TransPluginImpl TransPluginImpl;
#endif /* __cplusplus */

#endif 	/* __TransPluginImpl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ITransPluginImpl_INTERFACE_DEFINED__
#define __ITransPluginImpl_INTERFACE_DEFINED__

/* interface ITransPluginImpl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITransPluginImpl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FF87F646-27FD-41EF-BC61-D19595E27BF0")
    ITransPluginImpl : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITransPluginImplVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITransPluginImpl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITransPluginImpl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITransPluginImpl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITransPluginImpl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITransPluginImpl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITransPluginImpl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITransPluginImpl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ITransPluginImplVtbl;

    interface ITransPluginImpl
    {
        CONST_VTBL struct ITransPluginImplVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITransPluginImpl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITransPluginImpl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITransPluginImpl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITransPluginImpl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITransPluginImpl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITransPluginImpl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITransPluginImpl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITransPluginImpl_INTERFACE_DEFINED__ */



#ifndef __TransPluginLib_LIBRARY_DEFINED__
#define __TransPluginLib_LIBRARY_DEFINED__

/* library TransPluginLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_TransPluginLib;

EXTERN_C const CLSID CLSID_TransPluginImpl;

#ifdef __cplusplus

class DECLSPEC_UUID("53220626-63B8-443C-975C-1961FD679EF9")
TransPluginImpl;
#endif
#endif /* __TransPluginLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


