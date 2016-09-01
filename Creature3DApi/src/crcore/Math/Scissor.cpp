//-----------------------------------------------------------------------------
// File: Framework\Math\Scissor.cpp
// Copyright (c) 2005 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------




#include <CRCore/Math/Scissor.h>
using namespace CRCore;

/*
	Computes the bounding scissor rectangle to a light sphere.

	Extensive mathematical theory about the code below can be found here:
	http://www.gamasutra.com/features/20021011/lengyel_06.htm
*/
bool getScissorFromLight(const mat4 &modelview, const vec3 &lPos, const float r, const float fov, const int width, const int height, int *x, int *y, int *w, int *h){
	vec4 lightPos = modelview * vec4(lPos, 1.0f);
	vec3 dx = modelview.getRightVec();
	vec3 dy = modelview.getUpVec();
	vec3 dz = modelview.getForwardVec();

	float ex = tanf(fov / 2);
	float ey = ex * height / width;

	float Lxz = (lightPos.x * lightPos.x + lightPos.z * lightPos.z);
	float a = -r * lightPos.x / Lxz;
	float b = (r * r - lightPos.z * lightPos.z) / Lxz;
	float f = -b + a * a;

	float lp = 0;
	float rp = 1;
	float bp = 0;
	float tp = 1;

	if (f > 0){
		float Nx0 = -a + sqrtf(f);
		float Nx1 = -a - sqrtf(f);
		float Nz0 = (r - Nx0 * lightPos.x) / lightPos.z;
		float Nz1 = (r - Nx1 * lightPos.x) / lightPos.z;

		float x0 = 0.5f * (1 - Nz0 / (Nx0 * ex));
		float x1 = 0.5f * (1 - Nz1 / (Nx1 * ex));

		float Pz0 = (Lxz - r * r) / (lightPos.z - lightPos.x * Nz0 / Nx0);
		float Pz1 = (Lxz - r * r) / (lightPos.z - lightPos.x * Nz1 / Nx1);

		float Px0 = -(Pz0 * Nz0) / Nx0;
		float Px1 = -(Pz1 * Nz1) / Nx1;

		if (Px0 > lightPos.x) rp = x0;
		if (Px0 < lightPos.x) lp = x0;
		if (Px1 > lightPos.x && x1 < rp) rp = x1;
		if (Px1 < lightPos.x && x1 > lp) lp = x1;
	}

	float Lyz = (lightPos.y * lightPos.y + lightPos.z * lightPos.z);
	a = -r * lightPos.y / Lyz;
	b = (r * r - lightPos.z * lightPos.z) / Lyz;
	f = -b + a * a;

	if (f > 0){
		float Ny0 = -a + sqrtf(f);
		float Ny1 = -a - sqrtf(f);
		float Nz0 = (r - Ny0 * lightPos.y) / lightPos.z;
		float Nz1 = (r - Ny1 * lightPos.y) / lightPos.z;

		float y0 = 0.5f * (1 - Nz0 / (Ny0 * ey));
		float y1 = 0.5f * (1 - Nz1 / (Ny1 * ey));

		float Pz0 = (Lyz - r * r) / (lightPos.z - lightPos.y * Nz0 / Ny0);
		float Pz1 = (Lyz - r * r) / (lightPos.z - lightPos.y * Nz1 / Ny1);

		float Py0 = -(Pz0 * Nz0) / Ny0;
		float Py1 = -(Pz1 * Nz1) / Ny1;

		if (Py0 > lightPos.y) tp = y0;
		if (Py0 < lightPos.y) bp = y0;
		if (Py1 > lightPos.y && y1 < tp) tp = y1;
		if (Py1 < lightPos.y && y1 > bp) bp = y1;
	}

	lp *= width;
	rp *= width;
	tp *= height;
	bp *= height;

	int left   = int(lp);
	int right  = int(rp);
	int top    = int(tp);
	int bottom = int(bp);

	if (right <= left || top <= bottom) return false;

	*x = min(max(int(left),   0), width  - 1);
	*y = min(max(int(bottom), 0), height - 1);
	*w = min(int(right) - *x, width  - *x);
	*h = min(int(top)   - *y, height - *y);

	return (*w > 0 && *h > 0);
}
