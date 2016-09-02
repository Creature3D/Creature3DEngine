

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_Creature3DXLib,0x25A06DDE,0xB9A0,0x47B9,0x90,0x3B,0x16,0x6F,0xBD,0x06,0x44,0x49);


MIDL_DEFINE_GUID(IID, DIID__DCreature3DX,0x714D2510,0x9A0C,0x4498,0xBF,0x39,0x33,0xDB,0x17,0x79,0xEB,0x67);


MIDL_DEFINE_GUID(IID, DIID__DCreature3DXEvents,0x9666DF6A,0x6072,0x4696,0x98,0xF0,0x6C,0xA9,0x09,0xEC,0xDA,0x27);


MIDL_DEFINE_GUID(CLSID, CLSID_Creature3DX,0x5D2EE22E,0xECFE,0x440A,0x8D,0x48,0x86,0xED,0xDA,0x43,0x39,0x95);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



