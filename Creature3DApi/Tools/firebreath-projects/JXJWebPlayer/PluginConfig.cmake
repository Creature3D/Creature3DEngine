#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for JXJWebPlayer
#
#\**********************************************************/

set(PLUGIN_NAME "JXJWebPlayer")
set(PLUGIN_PREFIX "JXJ")
set(COMPANY_NAME "QueGame")

# ActiveX constants:
set(FBTYPELIB_NAME JXJWebPlayerLib)
set(FBTYPELIB_DESC "JXJWebPlayer 1.0 Type Library")
set(IFBControl_DESC "JXJWebPlayer Control Interface")
set(FBControl_DESC "JXJWebPlayer Control Class")
set(IFBComJavascriptObject_DESC "JXJWebPlayer IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "JXJWebPlayer ComJavascriptObject Class")
set(IFBComEventSource_DESC "JXJWebPlayer IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID fdc26413-7645-5b9b-bb7a-d52d95261cbc)
set(IFBControl_GUID 038ebcdc-dc5a-5226-99e4-9616bc35f632)
set(FBControl_GUID 57e7934c-c894-5c62-a541-27c57c323153)
set(IFBComJavascriptObject_GUID c29158a4-a20c-5bae-aa7e-17fb03f4e622)
set(FBComJavascriptObject_GUID b6fea791-e396-57d0-bf5e-fad8a5c138ec)
set(IFBComEventSource_GUID f0c78521-4eee-5f27-83f7-0ed8c0d4d4b2)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 93645ca6-4153-5d63-a827-ca300b3fc32f)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 1b7cd226-61c7-5188-b827-3c174bca8466)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "QueGame.JXJWebPlayer")
if ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "www.quegame.com/JXJWebPlayer")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "www.quegame.com/JXJWebPlayer_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )

# strings
set(FBSTRING_CompanyName "上海鹊游科技")
set(FBSTRING_PluginDescription "将星诀游戏插件")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2017 上海鹊游科技")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}")
if (APPLE)
    # On apple, np is not needed
    set(FBSTRING_PluginFileName "${PLUGIN_NAME}")
endif()
set(FBSTRING_ProductName "JXJWebPlayer")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "JXJWebPlayer")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "JXJWebPlayer_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-jxjwebplayer")

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
