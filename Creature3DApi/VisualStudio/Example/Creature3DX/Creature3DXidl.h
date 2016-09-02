

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Oct 29 16:38:38 2015
 */
/* Compiler settings for Creature3DX.idl:
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


#ifndef __Creature3DXidl_h__
#define __Creature3DXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DCreature3DX_FWD_DEFINED__
#define ___DCreature3DX_FWD_DEFINED__
typedef interface _DCreature3DX _DCreature3DX;
#endif 	/* ___DCreature3DX_FWD_DEFINED__ */


#ifndef ___DCreature3DXEvents_FWD_DEFINED__
#define ___DCreature3DXEvents_FWD_DEFINED__
typedef interface _DCreature3DXEvents _DCreature3DXEvents;
#endif 	/* ___DCreature3DXEvents_FWD_DEFINED__ */


#ifndef __Creature3DX_FWD_DEFINED__
#define __Creature3DX_FWD_DEFINED__

#ifdef __cplusplus
typedef class Creature3DX Creature3DX;
#else
typedef struct Creature3DX Creature3DX;
#endif /* __cplusplus */

#endif 	/* __Creature3DX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Creature3DXLib_LIBRARY_DEFINED__
#define __Creature3DXLib_LIBRARY_DEFINED__

/* library Creature3DXLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_Creature3DXLib;

#ifndef ___DCreature3DX_DISPINTERFACE_DEFINED__
#define ___DCreature3DX_DISPINTERFACE_DEFINED__

/* dispinterface _DCreature3DX */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DCreature3DX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("714D2510-9A0C-4498-BF39-33DB1779EB67")
    _DCreature3DX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DCreature3DXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DCreature3DX * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DCreature3DX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DCreature3DX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DCreature3DX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DCreature3DX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DCreature3DX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DCreature3DX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DCreature3DXVtbl;

    interface _DCreature3DX
    {
        CONST_VTBL struct _DCreature3DXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DCreature3DX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DCreature3DX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DCreature3DX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DCreature3DX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DCreature3DX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DCreature3DX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DCreature3DX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DCreature3DX_DISPINTERFACE_DEFINED__ */


#ifndef ___DCreature3DXEvents_DISPINTERFACE_DEFINED__
#define ___DCreature3DXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DCreature3DXEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DCreature3DXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9666DF6A-6072-4696-98F0-6CA909ECDA27")
    _DCreature3DXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DCreature3DXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DCreature3DXEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DCreature3DXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DCreature3DXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DCreature3DXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DCreature3DXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DCreature3DXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DCreature3DXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DCreature3DXEventsVtbl;

    interface _DCreature3DXEvents
    {
        CONST_VTBL struct _DCreature3DXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DCreature3DXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DCreature3DXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DCreature3DXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DCreature3DXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DCreature3DXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DCreature3DXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DCreature3DXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DCreature3DXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Creature3DX;

#ifdef __cplusplus

class DECLSPEC_UUID("5D2EE22E-ECFE-440A-8D48-86EDDA433995")
Creature3DX;
#endif
#endif /* __Creature3DXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


