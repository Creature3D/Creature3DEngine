/************************************************************************
 *                                                                      *
 *                   Copyright (C) 2002  3Dlabs Inc. Ltd.               *
 *                                                                      *
 ***********************************************************************/

#ifndef CRUTIL_CRNOISE_H
#define CRUTIL_CRNOISE_H 1

#include <CRUtil/crExport.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crTexture3D.h>
#include <CRCore/crTexture1D.h>

namespace CRUtil {

#define NOISE_MAXB 0x100
#define NOISE_N 0x1000
#define NOISE_NP 12   /* 2^NOISE_N */
#define NOISE_NM 0xfff

class CRUTIL_EXPORT crNoise : public CRCore::Referenced
{
public:
	crNoise(){}
	static crNoise* instance();

	void setNoiseFrequency(int frequency);
	double noise1(double arg);
	double noise2(double vec[2]);
	double noise3(double vec[3]);
	void normalize2(double vec[2]);
	void normalize3(double vec[3]);

	/*
	In what follows "alpha" is the weight when the sum is formed.
	Typically it is 2, As this approaches 1 the function is noisier.
	"beta" is the harmonic scaling/spacing, typically 2.
	*/

	double perlinNoise1D(double x,double alpha, double beta, int n);
	double perlinNoise2D(double x,double y,double alpha, double beta, int n);
	double perlinNoise3D(double x,double y,double z,double alpha, double beta, int n);

	CRCore::crImage* make3DNoiseImage(int texSize, int startFrequency = 4, int numOctaves = 4, double amp = 0.5);
	CRCore::crTexture3D* make3DNoiseTexture(int texSize, int startFrequency = 4, int numOctaves = 4, double amp = 0.5);
	CRCore::crImage* make1DSineImage( int texSize );
	CRCore::crTexture1D* make1DSineTexture( int texSize );
protected:
	virtual ~crNoise(){}
	void initNoise(void);

private:

	int p[NOISE_MAXB + NOISE_MAXB + 2];
	double g3[NOISE_MAXB + NOISE_MAXB + 2][3];
	double g2[NOISE_MAXB + NOISE_MAXB + 2][2];
	double g1[NOISE_MAXB + NOISE_MAXB + 2];
	int start;
	int B;
	int BM;
};

}

#endif
