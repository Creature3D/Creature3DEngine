/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*/

#ifndef CRCORE_CRWCHMSG_H
#define CRCORE_CRWCHMSG_H 1

#define _crInt8   __int8
#define _crInt16   __int16
#define _crInt32   __int32
#define _crInt64   __int64

#ifndef MAKEINT64
#define MAKEINT64(low, high) (((_crInt64)((_crInt32)(low)) & 0xffffffff) | ((_crInt64)((_crInt32)(high) & 0xffffffff) << 32))
#endif

#ifndef MAKEINT32
#define MAKEINT32(low, high) (((_crInt32)((_crInt16)(low)) & 0xffff) | ((_crInt32)((_crInt16)(high) & 0xffff) << 16))
#endif

#ifndef MAKEINT16
#define MAKEINT16(low, high) (((_crInt16)((_crInt8)(low)) & 0xff) | ((_crInt16)((_crInt8)(high) & 0xff) << 8))
#endif

#define MAKERGBA(r,g,b,a) MAKEINT32(MAKEINT16(r,g),MAKEINT16(b,a))

#ifndef LOINT64
#define LOINT64(l)           ((_crInt32)((_crInt64)(l) & 0xffffffff))
#endif

#ifndef HIINT64
#define HIINT64(l)           ((_crInt32)((_crInt64)(l) >> 32))
#endif

#ifndef LOINT32
#define LOINT32(l)           ((_crInt16)((_crInt32)(l) & 0xffff))
#endif

#ifndef HIINT32
#define HIINT32(l)           ((_crInt16)((_crInt32)(l) >> 16))
#endif

#ifndef LOINT16
#define LOINT16(l)           ((_crInt8)((_crInt16)(l) & 0xff))
#endif

#ifndef HIINT16
#define HIINT16(l)           ((_crInt8)((_crInt16)(l) >> 8))
#endif

#ifndef GETRVALUE
#define GETRVALUE(l)          ((unsigned char)l)
#endif

#ifndef GETGVALUE
#define GETGVALUE(l)          ((unsigned char)((_crInt32)(l) >> 8))
#endif

#ifndef GETBVALUE
#define GETBVALUE(l)          ((unsigned char)((_crInt32)(l) >> 16))
#endif

#ifndef GETAVALUE
#define GETAVALUE(l)          ((unsigned char)((_crInt32)(l) >> 24))
#endif

////基础交互
#define WCH_UP               0x0100
#define WCH_DOWN             0x0101
#define WCH_LEFT             0x0102
#define WCH_RIGHT            0x0103
#define WCH_TURNLEFT         0x0104
#define WCH_TURNRIGHT        0x0105
#define WCH_TURNUP           0x0106
#define WCH_TURNDOWN         0x0107
#define WCH_JUMP             0x0108
#define WCH_USER16           0x0200

#define WCH_INPUTPARAM       0x00150001
#define WCH_OUTPUTPARAM      0x00150002
#define WCH_MOVE             0x00150003
#define WCH_INQUIRE          0x00150004
#define WCH_FEEDBACK         0x00150005

#define WCH_MSGCONTAINER      0x00150006
#define WCH_EXCEVENT          0x00150007
#define WCH_AITASK            0x00150008
#define WCH_INPUTAITASK       0x00150009
#define WCH_DOAITASK          0x0015000A
#define WCH_KEYBOARDMOUSE     0x0015000B
#define WCH_TASKEND           0x0015000C

#define WCH_UPDATEVISITOR     0x00150010
#define WCH_CULLVISITOR       0x00150011
#define WCH_COLLIDE           0x00150012
#define WCH_UPDATE            0x00150013
//#define WCH_AIUPDATE          0x00150014
#define WCH_INVOLUME          0x00150015
#define WCH_OUTVOLUME         0x00150016
#define WCH_ENABLEVOLUME      0x00150017
#define WCH_COLLIDETEST       0x00150018
//Editor
#define WCH_ShowTerrainEditDecal 0x00150019
//LenUI
#define WCH_LenFadeInit       0x00150020
#define WCH_LenFade           0x00150021
#define WCH_AnimationEvent    0x00150022
#define WCH_SceneStart        0x00150023
#define WCH_RunGame			  0x00150024
#define WCH_GameStart		  0x00150025
#define WCH_GameEnd			  0x00150026
#define WCH_DOWNLOAD		  0x00150027
#define WCH_Startup			  0x00150028
#define WCH_ShowCanvasFaild	  0x00150029
#define WCH_ShowCanvasRecover 0x00150030
//physics msg
#define WCH_SETGRAVITYMODE         0x00150101
#define WCH_FORCEANDTORQUE         0x00150102
#define WCH_PHYSICSRESET           0x00150103
//joint msg
#define WCH_JOINTSETPARAM          0x00150104
#define WCH_JOINTPARAM             0x00150105
#define WCH_JOINTFORCE             0x00150106
#define WCH_JOINTTORQUE            0x00150107
//geom/body
#define WCH_GEOMORBODYINFO         0x00150108
#define WCH_GEOMMATRIX             0x00150109
#define WCH_BODYMATRIX             0x00150110
#define WCH_GEOMBOXPARAM           0x00150111
#define WCH_BODYCHANGEMASS         0x00150112

#define WCH_RADARPOSANDDIR         0x00150129
#define WCH_GENERALPARAMTRANSFER   0x00150130
#define WCH_VIEWMATRIXPARAM        0x00150131
#define WCH_VIEWMATRIXOFFSET       0x00150132
#define WCH_VIEWMATRIX             0x00150133
#define WCH_VIEWMOVE               0x00150134
#define WCH_VIEWROTATE             0x00150135

#define WCH_ASYNCDATATRANSFER      0x00150137
#define WCH_ADDWEAPON              0x00150138
#define WCH_SELECTWEAPON           0x00150139
#define WCH_POSITIONANDORIENTATION 0x00150140
#define WCH_LOCKEDTARGETSET        0x00150141
#define WCH_WIEWFILTEREVENT        0x00150142
#define WCH_FLYGAME                0x00150143
#define WCH_SHOWWIEWFILTER         0x00150144
#define WCH_BOTCONTROLLER          0x00150145
#define WCH_LOADPROGRESS           0x00150146
#define WCH_NEXTPROGRESS           0x00150147
#define WCH_DEAD                   0x00150148
#define WCH_RENASCENCE             0x00150149

//weapon event
#define WCH_WEAPONEVENT            0x00150150
#define WCH_WEAPON_FIRE            0x00150151
#define WCH_WEAPON_ESPECIAL        0x00150152
#define WCH_WEAPON_REPLACECLIP     0x00150153
#define WCH_WEAPON_SELECT          0x00150154
#define WCH_WEAPON_FIRE_T          0x00150155
#define WCH_WEAPON_UNSELECT        0x00150156
#define WCH_WEAPONFIRING           0x00150157
#define WCH_BULLET_EXPLODE         0x00150158
#define WCH_BULLET_FLY             0x00150159
#define WCH_BULLET_COLLIDE         0x00150160
#define WCH_WEAPON_FIRE_LT         0x00150161
#define WCH_BULLET_FIRE			   0x00150162
#define WCH_WEAPON_ESPCIALFIRE	   0x00150163
//#define WCH_WEAPON_FIRING          0x00150158
//#define WCH_WEAPON_STOPFIRING      0x00150159

//collide type
#define COLLIDEWITHMG              0x00150190
#define COLLIDEWITHMG_RAY          0x00150191
#define COLLIDEWITHMO              0x00150192
#define COLLIDEWITHMO_RAY          0x00150193
#define COLLIDEWITHEM              0x00150194
#define COLLIDEWITHEM_RAY          0x00150195

//mattergroup joint id msg
#define WCH_MGJOINTID_DEFAULT      0x00150200
#define WCH_MGJOINTID_MOTOR_LINE   0x00150201
#define WCH_MGJOINTID_LMOTOR_LINE   0x00150202
#define WCH_MGJOINTID_RMOTOR_LINE   0x00150203
#define WCH_MGJOINTID_MOTOR_ANGLE  0x00150204
#define WCH_MGJOINTID_STEER        0x00150205
#define WCH_MGJOINTID_RADAR        0x00150206
#define WCH_MGJOINTID_WEAPON       0x00150207

//robot mattergroup joint id msg
#define WCH_MGJOINTID_LARM         0x00150208
#define WCH_MGJOINTID_LHAND        0x00150209
#define WCH_MGJOINTID_RARM         0x00150230
#define WCH_MGJOINTID_RHAND        0x00150231
#define WCH_MGJOINTID_LTHIGH       0x00150232
#define WCH_MGJOINTID_LCALF        0x00150233
#define WCH_MGJOINTID_RTHIGH       0x00150234
#define WCH_MGJOINTID_RCALF        0x00150235
//
#define WCH_GETMATRIX              0x00150800

//nervecell & ai msg
#define WCH_MOVEANDROTATE          0x00151000
//#define WCH_KEYBOARDMOUSE          0x00151001
#define WCH_DETECT                 0x00151002
#define WCH_FINDAI                 0x00151003
#define WCH_FINDNC                 0x00151004
#define WCH_FINDMG                 0x00151005
#define WCH_PATHPROGRAM            0x00151006

#define WCH_COLLIDEAI              0x00151007
#define WCH_COLLIDENC              0x00151008
#define WCH_COLLIDEMG              0x00151009

#define WCH_VISIONSENSEINFORMATIONPARSE      0x00151200
#define WCH_COMPUTERCONTROL                  0x00151201
#define WCH_AIMOVETO               0x00151202
#define WCH_AIINFOTRANSLATE        0x00151203
#define WCH_AIMOVIES               0x00151204
#define WCH_AIMOVBEGIN             0x00151205
#define WCH_AIMOVEND               0x00151206
#define WCH_CHANGECAMERA           0x00151207
#define WCH_HIDEBODY               0x00151208
#define WCH_NEXTGAME               0x00151209
#define WCH_EXITGAME               0x00151210
#define WCH_PLAYSOUND              0x00151211
#define WCH_CHANGECHBODY           0x00151212
#define WCH_STOPBODY               0x00151213
#define WCH_PLAYSEQUENCE           0x00151214
#define WCH_ADDACTOR               0x00151215
#define WCH_PLAYAMBSOUND           0x00151216
#define WCH_STOPAMBSOUND           0x00151217
#define WCH_DOMODALDLG             0x00151218
#define WCH_SHOWDLG                0x00151219
#define WCH_LOADBOT                0x00151220
#define WCH_FRONTCONTACT           0x00151221
#define WCH_FILTERCAUTION          0x00151222
#define WCH_FILTERINFO             0x00151223
#define WCH_FILTERSWITCH           0x00151224
#define WCH_NODESWITCH             0x00151225
#define WCH_AIMOVBEGIN2            0x00151226
#define WCH_WINRESTORED            0x00151227

#define WCH_SOUNDEVENT             0x00152000
#define WCH_SOUNDPLAY              0x00152001
#define WCH_SOUNDSTORP             0x00152002
#define WCH_REPETITIONSOUND        0x00152003

#define WCH_FXEVENT                0x00152050
#define WCH_FXPLAY                 0x00152051
#define WCH_FXSTORP                0x00152052

#define WCH_EXTEVENT               0x00152100
#define WCH_DLGEVENT               0x00152101
#define WCH_ENTEREVENT             0x00152102
#define WCH_KEYEVENTMAP            0x00152103
#define WCH_EVENTKEY               0x00152105

#define WCH_OBJECTEVENT            0x00153000
#define WCH_IDEACENTER             0x00153001

////aircraft Game
#define WCH_STALL                  0x00154000
#define WCH_UNSTALL                0x00154001

#define WCH_SHOWCURSOR             0x00154003
#define WCH_HIDECURSOR             0x00154004

#define WCH_SELECTNODE             0x00154005
#define WCH_UNSELECTNODE           0x00154006
#define WCH_RACKNODE               0x00154007
#define WCH_UNRACKNODE             0x00154008

#define WCH_FILTERGETFOCUS         0x00154009
#define WCH_FILTERLOSTFOCUS        0x00154010
#define WCH_LISTEVENT              0x00154011
#define WCH_NODEGETFOCUS           0x00154012
#define WCH_NODELOSTFOCUS          0x00154013
#define WCH_WIDGETGETCAPTURE       0x00154014
#define WCH_WIDGETLOSTCAPTURE      0x00154015

//UI
#define WCH_UI_LBtnUp              0x00160000
#define WCH_UI_LBtnDown            0x00160001
#define WCH_UI_LBtnDblClk          0x00160002
#define WCH_UI_RBtnUp              0x00160003
#define WCH_UI_RBtnDown            0x00160004
#define WCH_UI_RBtnDblClk          0x00160005
#define WCH_UI_MBtnUp              0x00160006
#define WCH_UI_MBtnDown            0x00160007
#define WCH_UI_MBtnDblClk          0x00160008
#define WCH_UI_MouseMove           0x00160009
#define WCH_UI_MouseWheel          0x00160010
#define WCH_UI_MouseOn             0x00160011
#define WCH_UI_GetFocus            0x00160012
#define WCH_UI_LostFocus           0x00160013
#define WCH_UI_InitWindow          0x00160014
#define WCH_UI_DestroyWindow       0x00160015
#define WCH_UI_InputKey            0x00160016
#define WCH_UI_InputChar           0x00160017
#define WCH_UI_UpdateData          0x00160018
#define WCH_UI_PreDraw             0x00160019
#define WCH_UI_WINFOCUS            0x00160020
#define WCH_UI_Frame               0x00160021
#define WCH_UI_MouseDrag           0x00160022
#define WCH_UI_KeyLeft			   0x00160023
#define WCH_UI_KeyUp			   0x00160024
#define WCH_UI_KeyRight			   0x00160025
#define WCH_UI_KeyDown			   0x00160026
#define WCH_UI_ValueChange		   0x00160027
#define WCH_UI_LBtnDrag		       0x00160028
#define WCH_UI_RBtnDrag		       0x00160029
#define WCH_UI_MBtnDrag		       0x00160030
#define WCH_UI_SelectChange        0x00160031
#define WCH_UI_ListItemDrag        0x00160032
#define WCH_UI_OnShow		       0x00160033
//Dialog Box Command IDs
#define WCH_OK					   0x00160034
#define WCH_CANCEL				   0x00160035
#define WCH_ABORT				   0x00160036
#define WCH_RETRY				   0x00160037
#define WCH_IGNORE				   0x00160038
#define WCH_YES					   0x00160039
#define WCH_NO					   0x00160040
#define WCH_CLOSE				   0x00160041
#define WCH_HELP 				   0x00160042
#define WCH_TRYAGAIN 			   0x00160043
#define WCH_CONTINUE 			   0x00160044
#define WCH_TIMEOUT				   0x00160045
//Game 通用
#define WCH_TALK                   0x00161000
#define WCH_GETTASK                0x00161001
#define WCH_ADVANCETASK            0x00161002
#define WCH_REFERTASK              0x00161003
#define WCH_TASKAWARD              0x00161004

#define WCH_OUTMAPAREA             0x00161017
#define WCH_INMAPAREA              0x00161018

#define WCH_USER                   0x00200000
//2097152
#endif