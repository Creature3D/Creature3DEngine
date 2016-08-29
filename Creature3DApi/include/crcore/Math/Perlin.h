//-----------------------------------------------------------------------------
// File: Framework\Math\Perlin.h
// Copyright (c) 2005 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------





#ifndef _PERLIN_H_
#define _PERLIN_H_

/** \file
    Perlin noise routines.
*/
#include <CRCore/crExport.h>

float CR_EXPORT noise1(const float x);
float CR_EXPORT noise2(const float x, const float y);
float CR_EXPORT noise3(const float x, const float y, const float z);

float CR_EXPORT turbulence1(const float x, float freq);
float CR_EXPORT turbulence2(const float x, const float y, float freq);
float CR_EXPORT turbulence3(const float x, const float y, const float z, float freq);

float CR_EXPORT tileableNoise1(const float x, const float w);
float CR_EXPORT tileableNoise2(const float x, const float y, const float w, const float h);
float CR_EXPORT tileableNoise3(const float x, const float y, const float z, const float w, const float h, const float d);

float CR_EXPORT tileableTurbulence1(const float x, const float w, float freq);
float CR_EXPORT tileableTurbulence2(const float x, const float y, const float w, const float h, float freq);
float CR_EXPORT tileableTurbulence3(const float x, const float y, const float z, const float w, const float h, const float d, float freq);

/** Initializes Perlin noise tables */
void CR_EXPORT initPerlin();

#endif // _PERLIN_H_
