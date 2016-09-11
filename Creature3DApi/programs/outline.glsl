/**********************************************************************
 *
 *    FILE:           outline.glsl
 *
 *    DESCRIPTION:    
 *
 *    CREATED BY:     Îâ²Æ»ª
 *
 *    HISTORY:        Created 2012.12.07
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
uniform vec3 CRE_CameraPos;
uniform vec3 outline_color;
varying vec3 vtxPos;
#ifdef NeedPixelDepth
    uniform float maxDepth;
#endif
#ifdef DepthFog
    uniform vec3 fog_color;
    uniform vec4 fog_params;
#endif
void main(void)
{
    vec4 color = vec4(outline_color,1.0);
	vec3 tempVec;
    tempVec = CRE_CameraPos - vtxPos;
	float sqrDepth = dot(tempVec,tempVec);
    
#ifdef NeedPixelDepth
    float depth = min(sqrt(sqrDepth) / maxDepth, 1.0);
    sqrDepth = depth * depth;
#ifdef DepthFog
	float fogheight = vtxPos.z - fog_params.w;   
    if(fogheight<100.0 && sqrDepth >= fog_params.x && sqrDepth <= fog_params.y)
    {
	    float f = exp(-fog_params.z * (sqrDepth-fog_params.x));
	    if(fogheight>0)
	    {
			f = mix(f,1.0,fogheight*0.01);
	    }
	    color.xyz = mix(fog_color,color.xyz,f);
	}
#endif
#endif
        
#ifdef HDR
    gl_FragData[0] = min(color, 1.0);
    gl_FragData[1] = max(color - 1.0, 0.0);
#ifdef NeedNormalDepth
    gl_FragData[2] = vec4(0.0,0.0,0.0,depth);
#endif
#else
	gl_FragColor = color;
#endif

}

{****end****}