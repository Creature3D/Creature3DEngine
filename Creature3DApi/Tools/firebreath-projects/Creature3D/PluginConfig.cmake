#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for Creature3D
#
#\**********************************************************/

set(PLUGIN_NAME "Creature3D")
set(PLUGIN_PREFIX "CRE")
set(COMPANY_NAME "CAI")

# ActiveX constants:
set(FBTYPELIB_NAME Creature3DLib)
set(FBTYPELIB_DESC "Creature3D 1.0 Type Library")
set(IFBControl_DESC "Creature3D Control Interface")
set(FBControl_DESC "Creature3D Control Class")
set(IFBComJavascriptObject_DESC "Creature3D IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "Creature3D ComJavascriptObject Class")
set(IFBComEventSource_DESC "Creature3D IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 03a0334e-8da5-5f97-8d94-c2443274ea5c)
set(IFBControl_GUID 20453bd6-9a91-5939-bd06-4b0eb2344fe2)
set(FBControl_GUID b1c19106-2dfe-537f-99ef-21d047a8ff49)
set(IFBComJavascriptObject_GUID 551ef72e-c1a5-5f76-83b2-e9f67d5f8e27)
set(FBComJavascriptObject_GUID f4e732f7-dbde-5b7e-9707-57813b6b95b6)
set(IFBComEventSource_GUID fc9f4a3b-644b-52db-a65c-f5231fccc9f8)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 04d98de2-9541-5e2e-9e46-66b6abd7bf24)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 96e64f94-bf63-54bf-96a8-84d249ef0efd)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "CAI.Creature3D")
if ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "www.quegame.com/Creature3D")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "www.quegame.com/Creature3D_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )

# strings
set(FBSTRING_CompanyName "CAI工作室")
set(FBSTRING_PluginDescription "Creature3D游戏引擎插件")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2017 CAI工作室")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}")
if (APPLE)
    # On apple, np is not needed
    set(FBSTRING_PluginFileName "${PLUGIN_NAME}")
endif()
set(FBSTRING_ProductName "Creature3D")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "Creature3D")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "Creature3D_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-creature3d")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
