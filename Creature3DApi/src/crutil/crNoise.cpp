/************************************************************************
 *                                                                      *
 *                   Copyright (C) 2002  3Dlabs Inc. Ltd.               *
 *                                                                      *
 ************************************************************************/
#include <stdlib.h>
#include <CRCore/crMath.h>
#include <CRCore/crNotify.h>

#include <CRUtil/crNoise.h>

/* Coherent noise function over 1, 2 or 3 dimensions */
/* (copyright Ken Perlin) */

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + NOISE_N;\
	b0 = ((int)t) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - (int)t;\
	r1 = r0 - 1.;
#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

using namespace CRUtil;
using namespace CRCore;

crNoise* crNoise::instance()
{
	static ref_ptr<crNoise> s_noise = new crNoise;
	return s_noise.get();
}

void crNoise::setNoiseFrequency(int frequency)
{
    start = 1;
    B = frequency;
    BM = B-1;
}

double crNoise::noise1(double arg)
{
   int bx0, bx1;
   double rx0, rx1, sx, t, u, v, vec[1];

   vec[0] = arg;
   if (start) {
      start = 0;
      initNoise();
   }

   setup(0,bx0,bx1,rx0,rx1);

   sx = s_curve(rx0);
   u = rx0 * g1[ p[ bx0 ] ];
   v = rx1 * g1[ p[ bx1 ] ];

   return(lerp(sx, u, v));
}

double crNoise::noise2(double vec[2])
{
   int bx0, bx1, by0, by1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
   int i, j;

   if (start) {
      start = 0;
      initNoise();
   }

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   sx = s_curve(rx0);
   sy = s_curve(ry0);

   q = g2[ b00 ] ; u = at2(rx0,ry0);
   q = g2[ b10 ] ; v = at2(rx1,ry0);
   a = lerp(sx, u, v);

   q = g2[ b01 ] ; u = at2(rx0,ry1);
   q = g2[ b11 ] ; v = at2(rx1,ry1);
   b = lerp(sx, u, v);

   return lerp(sy, a, b);
}

double crNoise::noise3(double vec[3])
{
   int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
   int i, j;

   if (start) {
      start = 0;
      initNoise();
   }

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);
   setup(2, bz0,bz1, rz0,rz1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   t  = s_curve(rx0);
   sy = s_curve(ry0);
   sz = s_curve(rz0);

   q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
   q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
   a = lerp(t, u, v);

   q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
   q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
   b = lerp(t, u, v);

   c = lerp(sy, a, b);

   q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
   q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
   a = lerp(t, u, v);

   q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
   q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
   b = lerp(t, u, v);

   d = lerp(sy, a, b);

   //fprintf(stderr, "%f\n", lerp(sz, c, d));

   return lerp(sz, c, d);
}

void crNoise::normalize2(double v[2])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
}

void crNoise::normalize3(double v[3])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
   v[2] = v[2] / s;
}

void crNoise::initNoise(void)
{
   int i, j, k;

   srand(30757);
   for (i = 0 ; i < B ; i++) {
      p[i] = i;
      g1[i] = (double)((rand() % (B + B)) - B) / B;

      for (j = 0 ; j < 2 ; j++)
         g2[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize2(g2[i]);

      for (j = 0 ; j < 3 ; j++)
         g3[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize3(g3[i]);
   }

   while (--i) {
      k = p[i];
      p[i] = p[j = rand() % B];
      p[j] = k;
   }

   for (i = 0 ; i < B + 2 ; i++) {
      p[B + i] = p[i];
      g1[B + i] = g1[i];
      for (j = 0 ; j < 2 ; j++)
         g2[B + i][j] = g2[i][j];
      for (j = 0 ; j < 3 ; j++)
         g3[B + i][j] = g3[i][j];
   }
}

/* --- My harmonic summing functions - PDB --------------------------*/

/*
   In what follows "alpha" is the weight when the sum is formed.
   Typically it is 2, As this approaches 1 the function is noisier.
   "beta" is the harmonic scaling/spacing, typically 2.
*/

double crNoise::perlinNoise1D(double x,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0;
   double p,scale = 1;

   p = x;
   for (i=0;i<n;i++) {
      val = noise1(p);
      sum += val / scale;
      scale *= alpha;
      p *= beta;
   }
   return(sum);
}

double crNoise::perlinNoise2D(double x,double y,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0;
   double p[2],scale = 1;

   p[0] = x;
   p[1] = y;
   for (i=0;i<n;i++) {
      val = noise2(p);
      sum += val / scale;
      scale *= alpha;
      p[0] *= beta;
      p[1] *= beta;
   }
   return(sum);
}

double crNoise::perlinNoise3D(double x,double y,double z,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0;
   double p[3],scale = 1;

   p[0] = x;
   p[1] = y;
   p[2] = z;
   for (i=0;i<n;i++) {
      val = noise3(p);
      sum += val / scale;
      scale *= alpha;
      p[0] *= beta;
      p[1] *= beta;
      p[2] *= beta;
   }
   return(sum);
}

CRCore::crImage* crNoise::make3DNoiseImage(int texSize, int startFrequency, int numOctaves, double amp)
{
	crImage* image = new crImage;
	image->setImage(texSize, texSize, texSize,
		4, GL_RGBA, GL_UNSIGNED_BYTE,
		new unsigned char[4 * texSize * texSize * texSize],
		crImage::USE_NEW_DELETE);

	//const int startFrequency = 4;
	//const int numOctaves = 4;

	int f, i, j, k, inc;
	double ni[3];
	double inci, incj, inck;
	int frequency = startFrequency;
	GLubyte *ptr;
	//double amp = 1.0;//0.5;

	//CRCore::notify(CRCore::INFO) << "creating 3D noise texture... ";

	for (f = 0, inc = 0; f < numOctaves; ++f, frequency *= 2, ++inc, amp *= 0.5)
	{
		setNoiseFrequency(frequency);
		ptr = image->data();
		ni[0] = ni[1] = ni[2] = 0;

		inci = 1.0 / (texSize / frequency);
		for (i = 0; i < texSize; ++i, ni[0] += inci)
		{
			incj = 1.0 / (texSize / frequency);
			for (j = 0; j < texSize; ++j, ni[1] += incj)
			{
				inck = 1.0 / (texSize / frequency);
				for (k = 0; k < texSize; ++k, ni[2] += inck, ptr += 4)
				{
					*(ptr+inc) = (GLubyte) (((noise3(ni) + 1.0) * amp) * 128.0);
				}
			}
		}
	}

	//CRCore::notify(CRCore::INFO) << "DONE" << std::endl;
	return image;        
}

CRCore::crTexture3D* crNoise::make3DNoiseTexture(int texSize, int startFrequency, int numOctaves, double amp )
{
	crTexture3D* noiseTexture = new crTexture3D;
	noiseTexture->setFilter(crTexture3D::MIN_FILTER, crTexture3D::LINEAR);
	noiseTexture->setFilter(crTexture3D::MAG_FILTER, crTexture3D::LINEAR);
	noiseTexture->setWrap(crTexture3D::WRAP_S, crTexture3D::REPEAT);
	noiseTexture->setWrap(crTexture3D::WRAP_T, crTexture3D::REPEAT);
	noiseTexture->setWrap(crTexture3D::WRAP_R, crTexture3D::REPEAT);
	noiseTexture->setImage( make3DNoiseImage(texSize, startFrequency, numOctaves, amp) );
	return noiseTexture;
}

CRCore::crImage* crNoise::make1DSineImage( int texSize )
{
	crImage* image = new crImage;
	image->setImage(texSize, 1, 1,
		4, GL_RGBA, GL_UNSIGNED_BYTE,
		new unsigned char[4 * texSize],
		crImage::USE_NEW_DELETE);

	GLubyte* ptr = image->data();
	float inc = 2. * PI / (float)texSize;
	for(int i = 0; i < texSize; i++)
	{
		*ptr++ = (GLubyte)((sinf(i * inc) * 0.5 + 0.5) * 255.0);
		*ptr++ = 0;
		*ptr++ = 0;
		*ptr++ = 1;
	}
	return image;        
}

crTexture1D* crNoise::make1DSineTexture( int texSize )
{
	crTexture1D* sineTexture = new crTexture1D;
	sineTexture->setWrap(crTexture1D::WRAP_S, crTexture1D::REPEAT);
	sineTexture->setFilter(crTexture1D::MIN_FILTER, crTexture1D::LINEAR);
	sineTexture->setFilter(crTexture1D::MAG_FILTER, crTexture1D::LINEAR);
	sineTexture->setImage( make1DSineImage(texSize) );
	return sineTexture;
}
