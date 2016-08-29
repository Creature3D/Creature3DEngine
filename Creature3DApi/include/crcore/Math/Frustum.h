//-----------------------------------------------------------------------------
// File: Framework\Math\Frustum.h
// Copyright (c) 2005 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------





#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

/** \file
    View frustum definition
*/

#include <CRCore/crExport.h>
#include <CRCore/Math/Vector.h>

#define FRUSTUM_LEFT   0
#define FRUSTUM_RIGHT  1
#define FRUSTUM_TOP    2
#define FRUSTUM_BOTTOM 3
#define FRUSTUM_FAR    4
#define FRUSTUM_NEAR   5

/** Standard mathematical plane definition */
struct CR_EXPORT Plane {
	Plane(){};
	Plane(float x, float y, float z, float w){
        normal = vec3(x, y, z);
		float invLen = 1.0f / length(normal);
		normal *= invLen;
		offset = w * invLen;
	}
    vec3 normal;
	float offset;
};

/** 6-planes view frustum class */
class CR_EXPORT Frustum {
public:
	void loadFrustum(const mat4 &mvp);
	const Plane &getPlane(const int index) const { return planes[index]; }

	bool isPointInFrustum(const vec3 &pos) const;
	bool isSphereInFrustum(const vec3 &pos, const float radius) const;
protected:
	Plane planes[6];
};


#endif // _FRUSTUM_H_
