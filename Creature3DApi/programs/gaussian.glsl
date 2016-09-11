/**********************************************************************
 *
 *    FILE:           gaussian.glsl
 *
 *    DESCRIPTION:    vgaussian.glsl hgaussian.glsl
 *
 *    CREATED BY:     Îâ²Æ»ª
 *
 *    HISTORY:        Created 2006.5.20
 *
 *    Copyright 2005  Creature3D
 **********************************************************************/
 
{****Creature3D Vertex shader****}
uniform vec2 invSize;

varying vec2 texCoordM;
varying vec4 texCoordBF1;
varying vec4 texCoordBF2;
varying vec4 texCoordBF3;
//varying vec4 texCoordBF4;
void main()
{
	gl_Position = ftransform();
	vec2 texCoord = gl_MultiTexCoord0.xy;

	texCoordM = texCoord;
	texCoordBF1 = vec4(texCoord - invSize,texCoord + invSize);
	texCoordBF2 = vec4(texCoord - invSize * 2.0,texCoord + invSize * 2.0);
	texCoordBF3 = vec4(texCoord - invSize * 3.0,texCoord + invSize * 3.0);
	//texCoordBF4 = vec4(texCoord - invSize * 4.0,texCoord + invSize * 4.0);
}

{****Creature3D Fragment shader****}
uniform sampler2D CRE_BaseMap;
                                                                                       
varying vec2 texCoordM;
varying vec4 texCoordBF1;
varying vec4 texCoordBF2;
varying vec4 texCoordBF3;
//varying vec4 texCoordBF4;

void main()
{
	vec4 sampleM  = texture2D(CRE_BaseMap, texCoordM);
	vec4 sampleB1 = texture2D(CRE_BaseMap, texCoordBF1.xy);
	vec4 sampleF1 = texture2D(CRE_BaseMap, texCoordBF1.zw);
	vec4 sampleB2 = texture2D(CRE_BaseMap, texCoordBF2.xy);
	vec4 sampleF2 = texture2D(CRE_BaseMap, texCoordBF2.zw);
	vec4 sampleB3 = texture2D(CRE_BaseMap, texCoordBF3.xy);
	vec4 sampleF3 = texture2D(CRE_BaseMap, texCoordBF3.zw);
	//vec4 sampleB4 = texture2D(CRE_BaseMap, texCoordBF4.xy);
	//vec4 sampleF4 = texture2D(CRE_BaseMap, texCoordBF4.zw);
	gl_FragColor = 0.1752 * sampleM + 0.1658 * (sampleB1 + sampleF1) + 0.1403 * (sampleB2 + sampleF2) + 0.1063 * (sampleB3 + sampleF3);
}

{****end****}
//gl_FragColor = 0.1752 * sampleM + 0.1658 * (sampleB1 + sampleF1) + 0.1403 * (sampleB2 + sampleF2) + 0.1063 * (sampleB3 + sampleF3);	
