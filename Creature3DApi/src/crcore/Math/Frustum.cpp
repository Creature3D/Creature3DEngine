//-----------------------------------------------------------------------------
// File: Framework\Math\Frustum.cpp
// Copyright (c) 2005 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------





#include <CRCore/Math/Frustum.h>
using namespace CRCore;

void Frustum::loadFrustum(const mat4 &mvp){
	planes[FRUSTUM_LEFT  ] = Plane(mvp[12] - mvp[0], mvp[13] - mvp[1], mvp[14] - mvp[2],  mvp[15] - mvp[3]);
	planes[FRUSTUM_RIGHT ] = Plane(mvp[12] + mvp[0], mvp[13] + mvp[1], mvp[14] + mvp[2],  mvp[15] + mvp[3]);

	planes[FRUSTUM_TOP   ] = Plane(mvp[12] - mvp[4], mvp[13] - mvp[5], mvp[14] - mvp[6],  mvp[15] - mvp[7]);
	planes[FRUSTUM_BOTTOM] = Plane(mvp[12] + mvp[4], mvp[13] + mvp[5], mvp[14] + mvp[6],  mvp[15] + mvp[7]);

	planes[FRUSTUM_FAR   ] = Plane(mvp[12] - mvp[8], mvp[13] - mvp[9], mvp[14] - mvp[10], mvp[15] - mvp[11]);
	planes[FRUSTUM_NEAR  ] = Plane(mvp[12] + mvp[8], mvp[13] + mvp[9], mvp[14] + mvp[10], mvp[15] + mvp[11]);
}

bool Frustum::isPointInFrustum(const vec3 &pos) const {
    for (unsigned int i = 0; i < 6; i++){
		if (dot(pos, planes[i].normal) + planes[i].offset <= 0) return false;
    }
    return true;
}

bool Frustum::isSphereInFrustum(const vec3 &pos, const float radius) const {
    for (unsigned int i = 0; i < 6; i++){
		if (dot(pos, planes[i].normal) + planes[i].offset <= -radius) return false;
    }
    return true;
}
