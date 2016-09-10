/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef CRE_PARTICLEEFFECT
#define CRE_PARTICLEEFFECT 1

#include <CRParticle/crParticleEffect.h>
#include <CRParticle/crParticleSystem.h>
#include <CRParticle/crModularEmitter.h>
#include <CRParticle/crRandomRateCounter.h>
#include <CRParticle/crConstantRateCounter.h>
#include <CRParticle/crSectorPlacer.h>
#include <CRParticle/crBoxPlacer.h>
#include <CRParticle/crPointPlacer.h>
#include <CRParticle/crMultiSegmentPlacer.h>
#include <CRParticle/crSegmentPlacer.h>
#include <CRParticle/crRadialShooter.h>
#include <CRParticle/crModularProgram.h>
#include <CRParticle/crFluidProgram.h>
#include <CRParticle/crAccelOperator.h>
#include <CRParticle/crAngularAccelOperator.h>
#include <CRParticle/crFluidFrictionOperator.h>
#include <CRParticle/crForceOperator.h>
#include <CRParticle/crLinearInterpolator.h>
#include <CRParticle/crAngularDampingOperator.h>
#include <CRParticle/crDampingOperator.h>
#include <CRParticle/crExplosionOperator.h>
#include <CRParticle/crOrbitOperator.h>
#include <CRParticle/crConstantCounter.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crParticleEffect : public CRParticle::crParticleEffect, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
//ParticleSystem
class crParticleSystem : public CRParticle::crParticleSystem, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
//Emitter
class crModularEmitter : public CRParticle::crModularEmitter, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
//Emitter->Counter
class crRandomRateCounter : public CRParticle::crRandomRateCounter, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crConstantRateCounter : public CRParticle::crConstantRateCounter, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crConstantCounter : public CRParticle::crConstantCounter, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
//Emitter->Placer
class crSectorPlacer : public CRParticle::crSectorPlacer, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crBoxPlacer : public CRParticle::crBoxPlacer, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crPointPlacer : public CRParticle::crPointPlacer, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crMultiSegmentPlacer : public CRParticle::crMultiSegmentPlacer, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crSegmentPlacer : public CRParticle::crSegmentPlacer, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
//Emitter->Shooter
class crRadialShooter : public CRParticle::crRadialShooter, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
//Program
class crModularProgram : public CRParticle::crModularProgram, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crFluidProgram : public CRParticle::crFluidProgram, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
//Program->Operator
class crAccelOperator : public CRParticle::crAccelOperator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crAngularAccelOperator : public CRParticle::crAngularAccelOperator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crFluidFrictionOperator : public CRParticle::crFluidFrictionOperator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crForceOperator : public CRParticle::crForceOperator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crAngularDampingOperator : public CRParticle::crAngularDampingOperator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crDampingOperator : public CRParticle::crDampingOperator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crExplosionOperator : public CRParticle::crExplosionOperator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crOrbitOperator : public CRParticle::crOrbitOperator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
///crInterpolator
class crLinearInterpolator : public CRParticle::crLinearInterpolator, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

}

#endif
