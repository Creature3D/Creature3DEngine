/**********************************************************************
 *
 *    FILE:           collidemap.glsl
 *
 *    DESCRIPTION:    
 *
 *    CREATED BY:     Îâ²Æ»ª
 *
 *    HISTORY:        Created 2012.04.03
 *
 *    Copyright 2012  Creature3D
 **********************************************************************/
 
{****Creature3D Vertex shader****}
uniform mat4 CRE_InverseViewMatrix;
varying vec3 vtxPos;
void main(void)
{
	gl_Position = ftransform();
    mat4 mat = CRE_InverseViewMatrix * gl_ModelViewMatrix;
    vec4 pos = mat * gl_Vertex;
    vtxPos = pos.xyz;
}

{****Creature3D Fragment shader****}
varying vec3 vtxPos;
uniform float maxheight;
uniform float walkable;
void main(void)
{
  float h = (vtxPos.z/maxheight)*0.5f + 0.5f;
  gl_FragColor = vec4(walkable,h,0.0,1.0);
}

{****end****}