/**********************************************************************
 *
 *    FILE:           final.glsl
 *
 *    DESCRIPTION:    
 *
 *    CREATED BY:     Îâ²Æ»ª
 *
 *    HISTORY:        Created 2006.5.20
 *
 *    Copyright 2005  Creature3D
 **********************************************************************/
 
{****Creature3D Vertex shader****}
void main(void)
{
  gl_Position = ftransform();
  gl_TexCoord[0] = gl_MultiTexCoord0;
}

{****Creature3D Fragment shader****}
uniform sampler2D CRE_LDR;//LDR
uniform sampler2D CRE_HDR;//HDR
uniform sampler2D CRE_Distortion;
#if defined(DOF) || defined(OL)
    uniform sampler2D CRE_NormalDepth;
#ifdef DOF 
	uniform vec3 dofFactor;//start,invsize,dofblur
#endif
#ifdef OL
	uniform vec3 olColor;//color
	uniform float olThreshold;
	const float off = 0.001953125;
#endif
#endif
void main(void)
{
  vec2 texCoord = gl_TexCoord[0].st;
  float distortion = texture2D(CRE_Distortion,texCoord).w;
  if(distortion > 0.0 && distortion < 0.5)
  {
      distortion = min(distortion,0.01);
      if(distortion > 0.002) distortion -= 0.007;
      texCoord += vec2(distortion);
  }

  vec4 color = texture2D(CRE_LDR, texCoord);
  vec4 hdr = texture2D(CRE_HDR, texCoord);
  
#ifdef DOF 
  float depth = texture2D(CRE_NormalDepth, texCoord).w;
  if(depth>dofFactor.x)
  {
      float blur = dofFactor.y + (depth-dofFactor.x) * dofFactor.z;
	  color += texture2D(CRE_LDR,texCoord + blur * vec2( 0.8, -1.0));
	  color += texture2D(CRE_LDR,texCoord + blur * vec2(-0.2, -0.8));
	  color += texture2D(CRE_LDR,texCoord + blur * vec2( 0.2, -0.6));
      color += texture2D(CRE_LDR,texCoord + blur * vec2( 1.0, -0.4));
	  color += texture2D(CRE_LDR,texCoord + blur * vec2(-0.6, -0.2));
	  color += texture2D(CRE_LDR,texCoord + blur * vec2( 0.6,  0.2));
	  color += texture2D(CRE_LDR,texCoord + blur * vec2(-1.0,  0.4));
	  color += texture2D(CRE_LDR,texCoord + blur * vec2( 0.4,  0.6));
	  color += texture2D(CRE_LDR,texCoord + blur * vec2(-0.4,  0.8));
	  color += texture2D(CRE_LDR,texCoord + blur * vec2(-0.8,  1.0));
	  color /= 11.0;
  }
#endif
#ifdef OL 
  // Sample neighbor pixels
  float s00 = texture2D(CRE_NormalDepth, texCoord + vec2(-off, -off)).a;
  float s01 = texture2D(CRE_NormalDepth, texCoord + vec2( 0,   -off)).a;
  float s02 = texture2D(CRE_NormalDepth, texCoord + vec2( off, -off)).a;

  float s10 = texture2D(CRE_NormalDepth, texCoord + vec2(-off,  0)).a;
  float s12 = texture2D(CRE_NormalDepth, texCoord + vec2( off,  0)).a;

  float s20 = texture2D(CRE_NormalDepth, texCoord + vec2(-off,  off)).a;
  float s21 = texture2D(CRE_NormalDepth, texCoord + vec2( 0,    off)).a;
  float s22 = texture2D(CRE_NormalDepth, texCoord + vec2( off,  off)).a;
  // Sobel filter in X direction
  float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
  // Sobel filter in Y direction
  float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;
  // Find edge, skip sqrt() to improve performance ...
  float edgeSqr = (sobelX * sobelX + sobelY * sobelY);
  // ... and threshold against a squared value instead.
  //float nrp = 1.0-float(edgeSqr > 0.07 * 0.07);
  //color *= nrp;
  if(edgeSqr > olThreshold)
  {
      color.xyz = olColor;
  }
#endif

  gl_FragColor = color+hdr;
}

{****end****}