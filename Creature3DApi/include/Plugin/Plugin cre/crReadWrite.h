/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#ifndef CRE_READWRITE
#define CRE_READWRITE 1

#include <Plugin cre/crDataInputStream.h>
#include <Plugin cre/crDataOutputStream.h>                       

namespace cre {

#define CRESCRIPTDATA                  0xFFFFFFFF
// Objects and Nodes
#define CREBASE                        0x00000001
#define CRENODE                         0x00000002
#define CREGROUP                        0x00000003
#define CREMATRIXTRANSFORM              0x00000004
#define CRESTATESET                     0x00000005
#define CREOBJECT                        0x00000006
#define CREIMAGE                        0x00000007
#define CRELIGHTSOURCE                  0x00000008
#define CREBILLBOARD                    0x00000010
#define CRELOD                          0x00000011
#define CRESEQUENCE                     0x00000012
#define CRETRANSFORM                    0x00000013
#define CREPOSITIONATTITUDETRANSFORM    0x00000014
#define CREANIMATIONPATH                0x00000015
#define CRESWITCH                       0x00000016
#define CREOCCLUDERNODE                 0x00000017
//#define CREIMPOSTOR                     0x00000018
#define CRECONVEXPLANAROCCLUDER         0x00000019
#define CRECONVEXPLANARPOLYGON          0x00000020
#define CREPAGEDLOD                     0x00000021
#define CREDOFTRANSFORM                 0x00000022
#define CRECOORDINATESYSTEMNODE         0x00000023
#define CREELLIPSOIDMODEL               0x00000024
#define CRETEXGENNODE                   0x00000025
#define CRECLIPNODE                     0x00000026
#define CREPROXYNODE                    0x00000027
#define CREEXTERNNODE                   0x00000028
//#define CRESTATEDATA                    0x00000028
#define CRERESOURCE                     0x00000029
#define CREDB                           0x00000030
#define CRESECTOR                       0x00000031
#define CREBRAIN                        0x00000032
#define CRESECTORGROUP                  0x00000033
#define CREREGULARSECTORGROUP           0x00000034

//#define CRESOUNDNODE                    0x00000035
#define CREWEAPONNODE                   0x00000036
#define CRECAMERAVIEW                   0x00000037
#define CRECAMERANODE                   0x00000038
#define CREMATRIXSEQUENCE               0x00000039
//physics node
#define CREMATTEROBJECT                 0x00000040
#define CREBOXMATTEROBJECT              0x00000041
#define CRESPHEREMATTEROBJECT           0x00000042
#define CRECCYLINDERMATTEROBJECT        0x00000043
#define CREMESHMATTEROBJECT             0x00000044
#define CRERAYMATTEROBJECT              0x00000045
#define CREWEAPONMATTEROBJECT           0x00000046
#define CREBULLETMATTEROBJECT           0x00000047
#define CREMATTERGROUP                  0x00000048

#define CREVIEWMATTEROBJECT             0x00000049
#define CRECARMATTEROBJECT              0x00000149

#define CREAITREEHEAD                   0x00000050
#define CREEVENTGROUP                   0x00000051
//#define CRENERVECELLGROUP               0x00000052
//#define CREAIGROUP                      0x00000053
//#define CREKEYBOARDMOUSEEVENTGROUP      0x00000054
#define CRTRAVERSESTRINGNODE            0x00000052
#define CRWIDGETNODE		            0x00000053
#define CRCANVASNODE					0x00000054
#define	CRSCROLLBARWIDGET				0x00000055
#define	CRSTATICTEXTWIDGET				0x00000056
#define	CREDITWIDGET					0x00000057
#define	CRBUTTONWIDGET					0x00000058
#define	CRELISTBOXWIDGET				0x00000059
#define	CREIMAGEBOXWIDGET				0x00001101
#define	CRECHECKBOXWIDGET				0x00001102
#define CRERADIOWIDGET					0x00001103
#define	CRERADIOGROUPWIDGET			    0x00001104
#define CREPROGRESSWIDGET				0x00001105
#define CRESLIDERWIDGET					0x00001106
#define CRECOMBOBOXWIDGET				0x00001107
#define CRECHOOSECOLORWIDGET			0x00001108
#define CRETABLEWIDGET					0x00001109
#define CRHYPERTEXTWIDGET				0x00001110
#define CRELISTCONTROLWIDGET			0x00001111
#define CRESCISSORWIDGET				0x00001112
//events
#define CREEVENT                        0x00000060
#define CREEVENTMSG                     0x00000061
#define CREIFELSEEVENTMSG               0x00000062
#define CRESWITCHEVENTMSG               0x00000063
#define CREWHILEEVENTMSG                0x00000064
#define CRECONDITION                    0x00000065
#define CRESENDMSG                      0x00000066
#define CREECONDITION                   0x00000067

//ppc
#define CREPPC                          0x00000068
#define CRESETFORCEANDTORQUEPPC         0x00000069
#define CRESETGRAVITYMODEPPC            0x00000070
#define CREJOINTSETPARAMPPC             0x00000071
#define CREPHYSICSRESETPPC              0x00000072
#define CRESETGEOMORBODYINFOTPPC        0x00000073
//#define CREKEYBOARDMOUSEPPC             0x00000074
//#define CRECARMANIPULATORPPC            0x00000075
//#define CRERADARCOLLIDEPPC              0x00000076
//#define CREDISCOVERAIPPC                0x00000076
//#define CRECOLLIDEPPC                   0x00000077
#define CREWEAPONEVENTPPC               0x00000078
#define CREROBOTMANIPULATORPPC          0x00000079
//#define CRECHARACTERMANIPULATORPPC      0x00000080
#define CREGENERALPARAMTRANSFERPPC      0x00000081
#define CREFXEVENTPPC                   0x00000082

// crNode callbacks
#define CRENODECALLBACK                 0x00000090
#define CREANIMATIONPATHCALLBACK        0x00000091
#define CRECLUSTERCULLINGCALLBACK       0x00000092

// State attributes.
#define CRESTATEATTRIBUTE				0x00000100
#define CREALPHAFUNC					0x00000101
#define CREBLENDFUNC					0x00000102
#define CREDEPTH    					0x00000103
#define CREMATERIAL					    0x00000110
#define CRETEXTURE					    0x00000120
#define CRETEXTURE1D					0x00000121
#define CRETEXTURE2D					0x00000122
#define CRETEXTURE3D					0x00000123
#define CRETEXTURECUBEMAP				0x00000124
#define CRETEXENV					    0x00000125
#define CRETEXENVCOMBINE				0x00000126
#define CRETEXGEN					    0x00000127
#define CRECULLFACE					    0x00000128
#define CREPOLYGONOFFSET				0x00000129
#define CRESHADEMODEL					0x0000012A
#define CREPOINT					    0x0000012B
#define CRETEXMAT					    0x0000012C
#define CRELINEWIDTH					0x0000012D
//#define CREFRAGMENTPROGRAM				0x0000012E
//#define CREVERTEXPROGRAM				0x0000012F
#define CREFOG				            0x00000130
#define CRELIGHTMODEL				        0x00001121
#define CRECLIPPLANE				        0x00001122
#define CREFRONTFACE				        0x00001123
#define CREPROGRAM				        0x00001124
#define CRESHADER				        0x00001125
#define CREUNIFORM				        0x00001126
#define CREPOLYGONMODE			        0x00001127
#define CRELIGHT                        0x00001128

#define CRESOUNDSTATE                  0x00001500
#define CRESOUNDOCCLUDECALLBACK        0x00001501

#define CRECOLLIDECALLBACK             0x00001502
#define CRESOUNDCOLLIDECALLBACK        0x00001503


// Drawables
#define CREDRAWABLE					0x00001000
#define CREGEOMETRY					0x00001001
#define CRESHAPEDRAWABLE				0x00001002

// Shapes
#define CRESHAPE					0x00002000
#define CRESPHERE					0x00002001
#define CREBOX					        0x00002002
#define CRECONE					        0x00002004
#define CRECYLINDER					0x00002005
#define CRECAPSULE					0x00002006
#define CREHEIGHTFIELD					0x00002007

// Primitive set
#define CREPRIMITCRESET					0x00010000
#define CREDRAWARRAYS					0x00010001
#define CREDRAWARRAYLENGTHS				0x00010002
#define CREDRAWELEMENTSUSHORT			0x00010003
#define CREDRAWELEMENTSUINT				0x00010004
#define CREDRAWELEMENTSUBYTE				0x00010005

// CRSim classes
//#define CREBLINKSEQUENCE				0x00100001
//#define CREAZIMELEVATIONSECTOR			        0x00100002
//#define CREELEVATIONSECTOR				0x00100003
//#define CREAZIMSECTOR					0x00100004
//#define CRECONESECTOR					0x00100005
//#define CRELIGHTPOINT					0x00100006
//#define CRELIGHTPOINTNODE				0x00100007
#define CREMULTISWITCH				        0x00100008
//CRParticle
#define CREPARTICLEEFFECT			        0x00100009
#define CREParticleSystem			        0x00100010
#define CREModularEmitter			        0x00100011
#define CRERandomRateCounter			    0x00100012
#define CREConstantRateCounter			    0x00100013
#define CRESectorPlacer			            0x00100014
#define CREBoxPlacer                        0x00100015
#define CREPointPlacer                      0x00100016
#define CREMultiSegmentPlacer               0x00100017
#define CRESegmentPlacer                    0x00100018
#define CRERadialShooter                    0x00100019
#define CREModularProgram                   0x00100020
#define CREFluidProgram                     0x00100021
#define CREAccelOperator					0x00100022
#define CREAngularAccelOperator			    0x00100023
#define CREFluidFrictionOperator            0x00100024
#define CREForceOperator					0x00100025
#define CRELinearInterpolator               0x00100026
#define CREAngularDampingOperator           0x00100027
#define CREDampingOperator					0x00100028
#define CREExplosionOperator				0x00100029
#define CREOrbitOperator    				0x00100030
#define CREConstantCounter					0x00100031
//#define CREVISIBILITYGROUP			        0x00100009
//#define CREDIRECTIONALSECTOR                            0x0010000A

// CRFX classes
//#define CREMULTITEXTURECONTROL                0x01000001

//CRText classes
#define CRETEXT                                0x10000001

//CRPhysics class
#define CREJOINTPARAMETER                0x10010000
#define CREJOINT                         0x10010001
#define CREJOINTBALL                     0x10010002
#define CREJOINTHINGE                    0x10010003
#define CREJOINTSLIDER                   0x10010004
#define CREJOINTUNIVERSAL                0x10010005
#define CREJOINTCONTACT                  0x10010006
#define CREJOINTHINGE2                   0x10010007
#define CREJOINTFIXED                    0x10010008
#define CREJOINTAMOTOR                   0x10010009
#define CREJOINTNULL                     0x10010010

#define CRETERRAINTILE                   0x10010011

class crReadWrite{

public:
    virtual void write(crDataOutputStream* out) const = 0;
    virtual void read(crDataInputStream* out) const = 0;
};

}
#endif // CRE_READWRITE
