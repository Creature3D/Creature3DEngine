/**********************************************************************
 *
 *    FILE:          godray.glsl
 *
 *    DESCRIPTION:   http://fabiensanglard.net/lightScattering/index.php 
 *
 *    CREATED BY:     Îâ²Æ»ª
 *
 *    HISTORY:        Created 2011.9.23
 *
 *    Copyright 2011  Creature3D
 **********************************************************************/
 
{****Creature3D Vertex shader****}
void main(void)
{
  gl_Position = ftransform();
  gl_TexCoord[0] = gl_MultiTexCoord0;
}

{****Creature3D Fragment shader****}
 uniform float exposure;
 uniform float decay;
 uniform float density;
 uniform float weight;
 uniform vec2 lightPositionOnScreen;
 uniform sampler2D firstPass;
 const int NUM_SAMPLES = 100 ;

 void main()
 { 
  vec2 deltaTextCoord = vec2( gl_TexCoord[0].st - lightPositionOnScreen.xy );
  vec2 textCoo = gl_TexCoord[0].st;
  deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
  float illuminationDecay = 1.0;

  for(int i=0; i < NUM_SAMPLES ; i++)
   {
     textCoo -= deltaTextCoord;
     vec4 sample = texture2D(firstPass, textCoo );
          sample *= illuminationDecay * weight;
          gl_FragColor += sample;
          illuminationDecay *= decay;
  }
  gl_FragColor *= exposure;
}

{****end****}