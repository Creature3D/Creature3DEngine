//-----------------------------------------------------------------------------
// File: Framework\Math\Scissor.h
// Copyright (c) 2005 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------





#ifndef _SCISSOR_H_
#define _SCISSOR_H_

#include <CRCore/crExport.h>
#include <CRCore/Math/Vector.h>

/** Returns the scissor rect bounds of given light's influence */
bool CR_EXPORT getScissorFromLight(const mat4 &modelview, const vec3 &lPos, const float r, const float fov, const int width, const int height, int *x, int *y, int *w, int *h);

#endif // _SCISSOR_H_
