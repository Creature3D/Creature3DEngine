/**********************************************************************
 *
 *    FILE:           warfog.glsl
 *
 *    DESCRIPTION:    
 *
 *    CREATED BY:     Îâ²Æ»ª
 *
 *    HISTORY:        Created 2013.04.19
 *
 *    Copyright 2013  Creature3D
 **********************************************************************/
 
{****Creature3D Vertex shader****}
uniform mat4 CRE_InverseViewMatrix;
varying vec3 vtxPos;
void main(void)
{
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
    mat4 mat = CRE_InverseViewMatrix * gl_ModelViewMatrix;
    vec4 pos = mat * gl_Vertex;
    vtxPos = pos.xyz;
}

{****Creature3D Fragment shader****}
uniform sampler2D CRE_BaseMap;
uniform vec4 clearColor;
uniform vec4 coordParam;//
uniform float maxheight;//height
uniform vec3 centerPos;
varying vec3 vtxPos;
void main(void)
{
	vec2 dir = centerPos.xy - vtxPos.xy;
	float l = length(dir);
	dir = normalize(dir);
	vec2 pos,coord;
	bool blocked = false;
	float height;
	for( float i = 0.0; i<l ; i+=1.0)
	{
		pos = vtxPos.xy + dir * i;
		coord = (pos.xy + coordParam.xy) * coordParam.zw;
		height = (texture2D(CRE_BaseMap,coord).y * 2.0 - 1.0) * maxheight;
		if(vtxPos.z<height)
		{
		    blocked = true;
			break;
		}
	}
	gl_FragColor = blocked?clearColor:gl_Color;
}

{****end****}