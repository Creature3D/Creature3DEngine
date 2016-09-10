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
#include <Plugin cre/crException.h>
#include <Plugin cre/crNode.h>
#include <Plugin cre/crParticleEffect.h>
#include <CRParticle/crEmitter.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>
using namespace cre;
/////////////////////////////////////////
//
//crParticleEffect
//
/////////////////////////////////////////
void crParticleEffect::write(crDataOutputStream* out)
{
    out->writeInt(CREPARTICLEEFFECT);
    CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(this);
    if(node){//不写ChildNode
        static_cast<cre::crNode*>(node)->write(out);
    }
    else
        throw crException("crParticleEffect::write(): Could not cast this crParticleEffect to an crGroup.");
    //ParticleSystem
	CRParticle::crParticleSystem* ps = getParticleSystem();
	if(dynamic_cast<CRParticle::crParticleSystem*>(ps))
	{
		static_cast<cre::crParticleSystem*>(ps)->write(out);
	}

	//Emitter
	CRParticle::crEmitter* emitter = getEmitter();
	if(dynamic_cast<CRParticle::crModularEmitter *>(emitter))
	{
		static_cast<cre::crModularEmitter*>(emitter)->write(out);
	}

	//Program
	CRParticle::crProgram* program = getProgram();
	if(dynamic_cast<CRParticle::crModularProgram *>(program))
	{
		static_cast<cre::crModularProgram*>(program)->write(out);
	}
	else if(dynamic_cast<CRParticle::crFluidProgram *>(program))
	{
		static_cast<cre::crFluidProgram*>(program)->write(out);
	}
}

void crParticleEffect::read(crDataInputStream* in)
{
    int id = in->peekInt();
    if(id == CREPARTICLEEFFECT){
        id = in->readInt();

        CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(this);
        if(node){
            ((cre::crNode*)(node))->read(in);
        }
        else
            throw crException("crParticleEffect::read(): Could not cast this crParticleEffect to an crGroup.");

		//ParticleSystem
		int typeID = in->peekInt();
		if(typeID == CREParticleSystem)
		{
			CRParticle::crParticleSystem *ps = new CRParticle::crParticleSystem();
			((crParticleSystem*)(ps))->read(in);
			setParticleSystem(ps);
		}
		//Emitter
		typeID = in->peekInt();
		if(typeID == CREModularEmitter)
		{
			CRParticle::crModularEmitter *emitter = new CRParticle::crModularEmitter();
			((crModularEmitter*)(emitter))->read(in);
			setEmitter(emitter);
		}
        //Program
		typeID = in->peekInt();
		if(typeID == CREModularProgram)
		{
			CRParticle::crModularProgram *program = new CRParticle::crModularProgram();
			((crModularProgram*)(program))->read(in);
			setProgram(program);
		}
		else if(typeID == CREFluidProgram)
		{
			CRParticle::crFluidProgram *program = new CRParticle::crFluidProgram();
			((crFluidProgram*)(program))->read(in);
			setProgram(program);
		}
    }
    else{
        throw crException("crParticleEffect::read(): Expected ID");
    }
}
/////////////////////////////////////////
//
//crParticleSystem
//
/////////////////////////////////////////
void crParticleSystem::write(crDataOutputStream* out)
{
	out->writeInt(CREParticleSystem);
    out->writeUChar(getParticleAlignment());
	out->writeString(getTraverseString());
	if ( out->getVersion() >= VERSION_0024 )
		out->writeFloat(getLodValue());
	out->writeBool(getDoublePassRendering());//是否渲染深度

	CRCore::crObject *particleModelObject = getParticleModelObject();
	out->writeBool( particleModelObject!=0 );
	if(particleModelObject)
	{
		out->writeNode(particleModelObject);
	}
	else
	{
		out->writeBool( getStateSet()!=0 );
		if(getStateSet())
			out->writeStateSet(getStateSet());
	}

	//DefaultParticleTemplate
	CRParticle::crParticle& def_particle = getDefaultParticleTemplate();
	out->writeUChar(def_particle.getShape());
	out->writeFloat(def_particle.getRadius());
	out->writeFloat(def_particle.getMass());
	//out->writeVec2(CRCore::crVector2(def_particle.getSizeRange().minimum,def_particle.getSizeRange().maximum));
	out->writeVec3(def_particle.getSizeRange().minimum);
	out->writeVec3(def_particle.getSizeRange().maximum);
	out->writeVec2(CRCore::crVector2(def_particle.getAlphaRange().minimum,def_particle.getAlphaRange().maximum));
	out->writeVec4(def_particle.getColorRange().minimum);
	out->writeVec4(def_particle.getColorRange().maximum);
	out->writeFloat(def_particle.getLifeTime());
	if ( out->getVersion() >= VERSION_0022 )
	{
        out->writeInt(def_particle.getTileS());
		out->writeInt(def_particle.getTileT());
		out->writeInt(def_particle.getNumTiles());

		//CRParticle::crInterpolator *interpolator = const_cast<CRParticle::crInterpolator *>(def_particle.getTileInterpolator());
		//out->writeBool(interpolator!=NULL);
		//if(dynamic_cast<CRParticle::crLinearInterpolator *>(interpolator))
		//{
		//	static_cast<cre::crLinearInterpolator*>(interpolator)->write(out);
		//}
	}
	if ( out->getVersion() >= VERSION_0020 )
	{
		//out->writeVec2i(CRCore::crVector2i(def_particle.getDrawableIdRange().minimum,def_particle.getDrawableIdRange().maximum));
		//SizeInterpolator
		CRParticle::crInterpolator *interpolator = const_cast<CRParticle::crInterpolator *>(def_particle.getSizeInterpolator());
		out->writeBool(interpolator!=NULL);
		if(dynamic_cast<CRParticle::crLinearInterpolator *>(interpolator))
		{
			static_cast<cre::crLinearInterpolator*>(interpolator)->write(out);
		}
		//AlphaInterpolator
		interpolator = const_cast<CRParticle::crInterpolator *>(def_particle.getAlphaInterpolator());
		out->writeBool(interpolator!=NULL);
		if(dynamic_cast<CRParticle::crLinearInterpolator *>(interpolator))
		{
			static_cast<cre::crLinearInterpolator*>(interpolator)->write(out);
		}
		//ColorInterpolator
		interpolator = const_cast<CRParticle::crInterpolator *>(def_particle.getColorInterpolator());
		out->writeBool(interpolator!=NULL);
		if(dynamic_cast<CRParticle::crLinearInterpolator *>(interpolator))
		{
			static_cast<cre::crLinearInterpolator*>(interpolator)->write(out);
		}
		//DrawableIdInterpolator
		interpolator = const_cast<CRParticle::crInterpolator *>(def_particle.getDrawableIdInterpolator());
		out->writeBool(interpolator!=NULL);
		if(dynamic_cast<CRParticle::crLinearInterpolator *>(interpolator))
		{
			static_cast<cre::crLinearInterpolator*>(interpolator)->write(out);
		}
	}
	if ( out->getVersion() >= VERSION_0037 )
	{
		out->writeVec3(def_particle.getStaticAngle());
	}
	if ( out->getVersion() >= VERSION_0038 )
	{
		out->writeUChar(def_particle.getAlignedMode());
	}
}

void crParticleSystem::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREParticleSystem)
	{
		id = in->readInt();
		setParticleAlignment((Alignment)(in->readUChar()));
		setTraverseString(in->readString());
		if ( in->getVersion() >= VERSION_0024 )
			setLodValue(in->readFloat());
		setDoublePassRendering(in->readBool());//是否渲染深度

		if(in->readBool())
		{
			setParticleModelObject(dynamic_cast<CRCore::crObject*>(in->readNode()));
		}
		else
		{
            if(in->readBool())
				setStateSet(in->readStateSet());
		}

		//DefaultParticleTemplate
		CRParticle::crParticle& def_particle = getDefaultParticleTemplate();
		def_particle.setShape((CRParticle::crParticle::Shape)(in->readUChar()));
		def_particle.setRadius(in->readFloat());
		def_particle.setMass(in->readFloat());
		CRCore::crVector3 sizemin;
		CRCore::crVector3 sizemax;
		CRCore::crVector2 vec2;
		if ( in->getVersion() >= VERSION_0030 )
		{
			sizemin = in->readVec3();
			sizemax = in->readVec3();
		}
		else
		{
			vec2 = in->readVec2();
			sizemin.set(vec2[0],vec2[0],vec2[0]);
			sizemax.set(vec2[1],vec2[1],vec2[1]);
		}
		def_particle.setSizeRange(CRCore::rangev3(sizemin,sizemax));
		vec2 = in->readVec2();
		def_particle.setAlphaRange(CRCore::rangef(vec2[0],vec2[1]));
		CRCore::crVector4 colormin = in->readVec4();
		CRCore::crVector4 colormax = in->readVec4();
		def_particle.setColorRange(CRCore::rangev4(colormin,colormax));
		def_particle.setLifeTime(in->readFloat());

		if ( in->getVersion() >= VERSION_0022 )
		{
			int tiles = in->readInt();
			int tilet = in->readInt();
			int numtiles = in->readInt();
			def_particle.setTextureTile(tiles,tilet,numtiles);

			if ( in->getVersion() < VERSION_0030 )
			{
				int typeID;
				if(in->readBool())
				{//
					typeID = in->peekInt();
					if(typeID == CRELinearInterpolator)
					{
						CRParticle::crLinearInterpolator *interpolator = new CRParticle::crLinearInterpolator();
						((crLinearInterpolator*)(interpolator))->read(in);
						//def_particle.setTileInterpolator(interpolator);
					}
				}
			}
		}
		if ( in->getVersion() >= VERSION_0020 )
		{
			//crVector2i vec2i = in->readVec2i();
			//def_particle.setDrawableIdRange(CRCore::rangei(vec2i[0],vec2i[1]));
			int typeID;
			if(in->readBool())
			{//SizeInterpolator
				typeID = in->peekInt();
				if(typeID == CRELinearInterpolator)
				{
					CRParticle::crLinearInterpolator *interpolator = new CRParticle::crLinearInterpolator();
					((crLinearInterpolator*)(interpolator))->read(in);
					def_particle.setSizeInterpolator(interpolator);
				}
			}

			if(in->readBool())
			{
				//AlphaInterpolator
				typeID = in->peekInt();
				if(typeID == CRELinearInterpolator)
				{
					CRParticle::crLinearInterpolator *interpolator = new CRParticle::crLinearInterpolator();
					((crLinearInterpolator*)(interpolator))->read(in);
					def_particle.setAlphaInterpolator(interpolator);
				}
			}

			if(in->readBool())
			{
				//ColorInterpolator
				typeID = in->peekInt();
				if(typeID == CRELinearInterpolator)
				{
					CRParticle::crLinearInterpolator *interpolator = new CRParticle::crLinearInterpolator();
					((crLinearInterpolator*)(interpolator))->read(in);
					def_particle.setColorInterpolator(interpolator);
				}
			}

			if(in->readBool())
			{
				//DrawableIdInterpolator
				typeID = in->peekInt();
				if(typeID == CRELinearInterpolator)
				{
					CRParticle::crLinearInterpolator *interpolator = new CRParticle::crLinearInterpolator();
					((crLinearInterpolator*)(interpolator))->read(in);
					def_particle.setDrawableIdInterpolator(interpolator);
				}
			}
		}
		if ( in->getVersion() >= VERSION_0037 )
		{
			def_particle.setStaticAngle(in->readVec3());
		}
		if ( in->getVersion() >= VERSION_0038 )
		{
			def_particle.setAlignedMode((CRParticle::crParticle::AxisAligned)(in->readUChar()));
		}
	}
	else
	{
		throw crException("crParticleSystem::read(): Expected crParticleSystem ID");
	}
}
/////////////////////////////////////////
//
//crModularEmitter
//
/////////////////////////////////////////
void crModularEmitter::write(crDataOutputStream* out)
{
	out->writeInt(CREModularEmitter);
    out->writeBool(isEnabled());
	out->writeBool(isEndless());
	out->writeFloat(getLifeTime());
	out->writeFloat(getStartTime());
	out->writeFloat(getResetTime());
	out->writeFloat(getNumParticlesToCreateMovementCompenstationRatio());
	out->writeUChar(getReferenceFrame());
	//Counter
	CRParticle::crCounter *counter = getCounter();
	if(dynamic_cast<CRParticle::crRandomRateCounter *>(counter))
	{
		static_cast<cre::crRandomRateCounter*>(counter)->write(out);
	}
	else if(dynamic_cast<CRParticle::crConstantRateCounter *>(counter))
	{
		static_cast<cre::crConstantRateCounter*>(counter)->write(out);
	}
	else if(dynamic_cast<CRParticle::crConstantCounter *>(counter))
	{
		static_cast<cre::crConstantCounter*>(counter)->write(out);
	}
	//Placer
	CRParticle::crPlacer *placer = getPlacer();
	if(dynamic_cast<CRParticle::crSectorPlacer *>(placer))
	{
		static_cast<cre::crSectorPlacer*>(placer)->write(out);
	}
	else if(dynamic_cast<CRParticle::crBoxPlacer *>(placer))
	{
		static_cast<cre::crBoxPlacer*>(placer)->write(out);
	}
	else if(dynamic_cast<CRParticle::crPointPlacer *>(placer))
	{
		static_cast<cre::crPointPlacer*>(placer)->write(out);
	}
	else if(dynamic_cast<CRParticle::crMultiSegmentPlacer *>(placer))
	{
		static_cast<cre::crMultiSegmentPlacer*>(placer)->write(out);
	}
	else if(dynamic_cast<CRParticle::crSegmentPlacer *>(placer))
	{
		static_cast<cre::crSegmentPlacer*>(placer)->write(out);
	}
	//Shooter
	CRParticle::crShooter *shooter = getShooter();
	if(dynamic_cast<CRParticle::crRadialShooter *>(shooter))
	{
		static_cast<cre::crRadialShooter*>(shooter)->write(out);
	}
}

void crModularEmitter::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREModularEmitter)
	{
		id = in->readInt();
		setEnabled(in->readBool());
		setEndless(in->readBool());
		setLifeTime(in->readFloat());
        setStartTime(in->readFloat());
		setResetTime(in->readFloat());
		setNumParticlesToCreateMovementCompenstationRatio(in->readFloat());
		setReferenceFrame((ReferenceFrame)(in->readUChar()));

		//Counter
		int typeID = in->peekInt();
		if(typeID == CRERandomRateCounter)
		{
			CRParticle::crRandomRateCounter *counter = new CRParticle::crRandomRateCounter();
			((crRandomRateCounter*)(counter))->read(in);
			setCounter(counter);
		}
		else if(typeID == CREConstantRateCounter)
		{
			CRParticle::crConstantRateCounter *counter = new CRParticle::crConstantRateCounter();
			((crConstantRateCounter*)(counter))->read(in);
			setCounter(counter);
		}
		else if(typeID == CREConstantCounter)
		{
			CRParticle::crConstantCounter *counter = new CRParticle::crConstantCounter();
			((crConstantCounter*)(counter))->read(in);
			setCounter(counter);
		}
		//Placer
		typeID = in->peekInt();
		if(typeID == CRESectorPlacer)
		{
			CRParticle::crSectorPlacer *placer = new CRParticle::crSectorPlacer();
			((crSectorPlacer*)(placer))->read(in);
			setPlacer(placer);
		}
		else if(typeID == CREBoxPlacer)
		{
			CRParticle::crBoxPlacer *placer = new CRParticle::crBoxPlacer();
			((crBoxPlacer*)(placer))->read(in);
			setPlacer(placer);
		}
		else if(typeID == CREPointPlacer)
		{
			CRParticle::crPointPlacer *placer = new CRParticle::crPointPlacer();
			((crPointPlacer*)(placer))->read(in);
			setPlacer(placer);
		}
		else if(typeID == CREMultiSegmentPlacer)
		{
			CRParticle::crMultiSegmentPlacer *placer = new CRParticle::crMultiSegmentPlacer();
			((crMultiSegmentPlacer*)(placer))->read(in);
			setPlacer(placer);
		}
		else if(typeID == CRESegmentPlacer)
		{
			CRParticle::crSegmentPlacer *placer = new CRParticle::crSegmentPlacer();
			((crSegmentPlacer*)(placer))->read(in);
			setPlacer(placer);
		}
		else if(typeID == CRESegmentPlacer)
		{
			CRParticle::crSegmentPlacer *placer = new CRParticle::crSegmentPlacer();
			((crSegmentPlacer*)(placer))->read(in);
			setPlacer(placer);
		}
		//Shooter
		typeID = in->peekInt();
		if(typeID == CRERadialShooter)
		{
			CRParticle::crRadialShooter *shooter = new CRParticle::crRadialShooter();
			((crRadialShooter*)(shooter))->read(in);
			setShooter(shooter);
		}
	}
	else
	{
		throw crException("crModularEmitter::read(): Expected crModularEmitter ID");
	}
}
/////////////////////////////////////////
//
//crRandomRateCounter
//
/////////////////////////////////////////
void crRandomRateCounter::write(crDataOutputStream* out)
{
	out->writeInt(CRERandomRateCounter);
    out->writeVec2(CRCore::crVector2(getRateRange().minimum,getRateRange().maximum));
}

void crRandomRateCounter::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CRERandomRateCounter)
	{
		id = in->readInt();
		CRCore::crVector2 vec2 = in->readVec2();
		setRateRange(vec2[0],vec2[1]);
	}
	else
	{
		throw crException("crRandomRateCounter::read(): Expected crRandomRateCounter ID");
	}
}
/////////////////////////////////////////
//
//crConstantRateCounter
//
/////////////////////////////////////////
void crConstantRateCounter::write(crDataOutputStream* out)
{
	out->writeInt(CREConstantRateCounter);
    out->writeInt(getMinimumNumberOfParticlesToCreate());
	out->writeFloat(getNumberOfParticlesPerSecondToCreate());
}

void crConstantRateCounter::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREConstantRateCounter)
	{
		id = in->readInt();
        setMinimumNumberOfParticlesToCreate(in->readInt());
		setNumberOfParticlesPerSecondToCreate(in->readFloat());
	}
	else
	{
		throw crException("crConstantRateCounter::read(): Expected crConstantRateCounter ID");
	}
}
/////////////////////////////////////////
//
//crConstantCounter
//
/////////////////////////////////////////
void crConstantCounter::write(crDataOutputStream* out)
{
	out->writeInt(CREConstantCounter);
	out->writeInt(getNumberOfParticlesToCreate());
}

void crConstantCounter::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREConstantCounter)
	{
		id = in->readInt();
		setNumberOfParticlesToCreate(in->readInt());
	}
	else
	{
		throw crException("crConstantCounter::read(): Expected crConstantCounter ID");
	}
}
/////////////////////////////////////////
//
//crSectorPlacer
//
/////////////////////////////////////////
void crSectorPlacer::write(crDataOutputStream* out)
{
	out->writeInt(CRESectorPlacer);
	out->writeVec3(getCenter());
	out->writeVec2(CRCore::crVector2(getRadiusRange().minimum,getRadiusRange().maximum));
	out->writeVec2(CRCore::crVector2(getPhiRange().minimum,getPhiRange().maximum));
}

void crSectorPlacer::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CRESectorPlacer)
	{
		id = in->readInt();
        setCenter(in->readVec3());
		CRCore::crVector2 vec2 = in->readVec2();
		setRadiusRange(CRCore::rangef(vec2[0],vec2[1]));
		vec2 = in->readVec2();
		setPhiRange(CRCore::rangef(vec2[0],vec2[1]));
	}
	else
	{
		throw crException("crSectorPlacer::read(): Expected crSectorPlacer ID");
	}
}
/////////////////////////////////////////
//
//crBoxPlacer
//
/////////////////////////////////////////
void crBoxPlacer::write(crDataOutputStream* out)
{
	out->writeInt(CREBoxPlacer);
	out->writeVec3(getCenter());
	out->writeVec2(CRCore::crVector2(getXRange().minimum,getXRange().maximum));
	out->writeVec2(CRCore::crVector2(getYRange().minimum,getYRange().maximum));
	out->writeVec2(CRCore::crVector2(getZRange().minimum,getZRange().maximum));
}

void crBoxPlacer::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREBoxPlacer)
	{
		id = in->readInt();
		setCenter(in->readVec3());
		CRCore::crVector2 vec2 = in->readVec2();
		setXRange(CRCore::rangef(vec2[0],vec2[1]));
		vec2 = in->readVec2();
		setYRange(CRCore::rangef(vec2[0],vec2[1]));
		vec2 = in->readVec2();
		setZRange(CRCore::rangef(vec2[0],vec2[1]));
	}
	else
	{
		throw crException("crBoxPlacer::read(): Expected crBoxPlacer ID");
	}
}
/////////////////////////////////////////
//
//crPointPlacer
//
/////////////////////////////////////////
void crPointPlacer::write(crDataOutputStream* out)
{
	out->writeInt(CREPointPlacer);
	out->writeVec3(getCenter());
}

void crPointPlacer::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREPointPlacer)
	{
		id = in->readInt();
		setCenter(in->readVec3());
	}
	else
	{
		throw crException("crPointPlacer::read(): Expected crPointPlacer ID");
	}
}
/////////////////////////////////////////
//
//crMultiSegmentPlacer
//
/////////////////////////////////////////
void crMultiSegmentPlacer::write(crDataOutputStream* out)
{
	out->writeInt(CREMultiSegmentPlacer);
    int num = numVertices();
	out->writeInt(num);
	for( int i = 0; i<num; ++i )
	{
		out->writeVec3(getVertex(i));
	}
}

void crMultiSegmentPlacer::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREMultiSegmentPlacer)
	{
		id = in->readInt();
        int num = in->readInt();
		for( int i = 0; i<num; ++i )
		{
			addVertex(in->readVec3());
		}
	}
	else
	{
		throw crException("crMultiSegmentPlacer::read(): Expected crMultiSegmentPlacer ID");
	}
}
/////////////////////////////////////////
//
//crSegmentPlacer
//
/////////////////////////////////////////
void crSegmentPlacer::write(crDataOutputStream* out)
{
	out->writeInt(CRESegmentPlacer);
    out->writeVec3(getVertexA());
	out->writeVec3(getVertexB());
}

void crSegmentPlacer::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CRESegmentPlacer)
	{
		id = in->readInt();
		setVertexA(in->readVec3());
		setVertexB(in->readVec3());
	}
	else
	{
		throw crException("crSegmentPlacer::read(): Expected crSegmentPlacer ID");
	}
}
/////////////////////////////////////////
//
//crRadialShooter
//
/////////////////////////////////////////
void crRadialShooter::write(crDataOutputStream* out)
{
	out->writeInt(CRERadialShooter);
	out->writeVec2(CRCore::crVector2(getThetaRange().minimum,getThetaRange().maximum));
	out->writeVec2(CRCore::crVector2(getPhiRange().minimum,getPhiRange().maximum));
	out->writeVec2(CRCore::crVector2(getInitialSpeedRange().minimum,getInitialSpeedRange().maximum));
	out->writeVec3(getInitialRotationalSpeedRange().minimum);
	out->writeVec3(getInitialRotationalSpeedRange().maximum);
}

void crRadialShooter::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CRERadialShooter)
	{
		id = in->readInt();
		CRCore::crVector2 vec2 = in->readVec2();
		setThetaRange(CRCore::rangef(vec2[0],vec2[1]));
		vec2 = in->readVec2();
		setPhiRange(CRCore::rangef(vec2[0],vec2[1]));
		vec2 = in->readVec2();
		setInitialSpeedRange(CRCore::rangef(vec2[0],vec2[1]));
		CRCore::crVector3 rotspeedmin = in->readVec3();
		CRCore::crVector3 rotspeedmax = in->readVec3();
		setInitialRotationalSpeedRange(rotspeedmin,rotspeedmax);
	}
	else
	{
		throw crException("crRadialShooter::read(): Expected crRadialShooter ID");
	}
}
/////////////////////////////////////////
//
//crModularProgram
//
/////////////////////////////////////////
void crModularProgram::write(crDataOutputStream* out)
{
	out->writeInt(CREModularProgram);
    int num = numOperators();
	out->writeInt(num);
	CRParticle::crOperator* op;
	for( int i = 0; i<num; ++i )
	{
        op = getOperator(i);
		if(dynamic_cast<CRParticle::crAccelOperator *>(op))
		{
			static_cast<cre::crAccelOperator*>(op)->write(out);
		}
		else if(dynamic_cast<CRParticle::crAngularAccelOperator *>(op))
		{
			static_cast<cre::crAngularAccelOperator*>(op)->write(out);
		}
		else if(dynamic_cast<CRParticle::crFluidFrictionOperator *>(op))
		{
			static_cast<cre::crFluidFrictionOperator*>(op)->write(out);
		}
		else if(dynamic_cast<CRParticle::crForceOperator *>(op))
		{
			static_cast<cre::crForceOperator*>(op)->write(out);
		}
		else if(dynamic_cast<CRParticle::crAngularDampingOperator *>(op))
		{
			static_cast<cre::crAngularDampingOperator*>(op)->write(out);
		}
		else if(dynamic_cast<CRParticle::crDampingOperator *>(op))
		{
			static_cast<cre::crDampingOperator*>(op)->write(out);
		}
		else if(dynamic_cast<CRParticle::crExplosionOperator *>(op))
		{
			static_cast<cre::crExplosionOperator*>(op)->write(out);
		}
		else if(dynamic_cast<CRParticle::crOrbitOperator *>(op))
		{
			static_cast<cre::crOrbitOperator*>(op)->write(out);
		}
	}
}

void crModularProgram::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREModularProgram)
	{
		id = in->readInt();
		int num = in->readInt();
		for( int i = 0; i<num; i++)
		{
			int typeID = in->peekInt();
			if(typeID == CREAccelOperator)
			{
				CRParticle::crAccelOperator *op = new CRParticle::crAccelOperator();
				((crAccelOperator*)(op))->read(in);
				addOperator(op);
			}
			else if(typeID == CREAngularAccelOperator)
			{
				CRParticle::crAngularAccelOperator *op = new CRParticle::crAngularAccelOperator();
				((crAngularAccelOperator*)(op))->read(in);
				addOperator(op);
			}
			else if(typeID == CREFluidFrictionOperator)
			{
				CRParticle::crFluidFrictionOperator *op = new CRParticle::crFluidFrictionOperator();
				((crFluidFrictionOperator*)(op))->read(in);
				addOperator(op);
			}
			else if(typeID == CREForceOperator)
			{
				CRParticle::crForceOperator *op = new CRParticle::crForceOperator();
				((crForceOperator*)(op))->read(in);
				addOperator(op);
			}
			else if(typeID == CREAngularDampingOperator)
			{
				CRParticle::crAngularDampingOperator *op = new CRParticle::crAngularDampingOperator();
				((crAngularDampingOperator*)(op))->read(in);
				addOperator(op);
			}
			else if(typeID == CREDampingOperator)
			{
				CRParticle::crDampingOperator *op = new CRParticle::crDampingOperator();
				((crDampingOperator*)(op))->read(in);
				addOperator(op);
			}
			else if(typeID == CREExplosionOperator)
			{
				CRParticle::crExplosionOperator *op = new CRParticle::crExplosionOperator();
				((crExplosionOperator*)(op))->read(in);
				addOperator(op);
			}
			else if(typeID == CREOrbitOperator)
			{
				CRParticle::crOrbitOperator *op = new CRParticle::crOrbitOperator();
				((crOrbitOperator*)(op))->read(in);
				addOperator(op);
			}
		}
	}
	else
	{
		throw crException("crModularProgram::read(): Expected crModularProgram ID");
	}
}
/////////////////////////////////////////
//
//crFluidProgram
//
/////////////////////////////////////////
void crFluidProgram::write(crDataOutputStream* out)
{
	out->writeInt(CREFluidProgram);
    out->writeVec3(getWind());
	out->writeUChar(getFluidType());
	if(getFluidType() == UserDefine)
	{
	    out->writeVec3(getAcceleration());
		out->writeFloat(getFluidDensity());
		out->writeFloat(getFluidViscosity());
	}
}

void crFluidProgram::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREFluidProgram)
	{
		id = in->readInt();
		setWind(in->readVec3());
        FluidType ft = (FluidType)in->readUChar();
		setFluidType(ft);
		if(ft == UserDefine)
		{
			setAcceleration(in->readVec3());
			setFluidDensity(in->readFloat());
			setFluidViscosity(in->readFloat());
		}
	}
	else
	{
		throw crException("crFluidProgram::read(): Expected crFluidProgram ID");
	}
}
/////////////////////////////////////////
//
//crAccelOperator
//
/////////////////////////////////////////
void crAccelOperator::write(crDataOutputStream* out)
{
	out->writeInt(CREAccelOperator);
	out->writeVec3(getAcceleration());
}

void crAccelOperator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREAccelOperator)
	{
		id = in->readInt();
		setAcceleration(in->readVec3());
	}
	else
	{
		throw crException("crAccelOperator::read(): Expected crAccelOperator ID");
	}
}
/////////////////////////////////////////
//
//crAngularAccelOperator
//
/////////////////////////////////////////
void crAngularAccelOperator::write(crDataOutputStream* out)
{
	out->writeInt(CREAngularAccelOperator);
    out->writeVec3(getAngularAcceleration());
}

void crAngularAccelOperator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREAngularAccelOperator)
	{
		id = in->readInt();
        setAngularAcceleration(in->readVec3());
	}
	else
	{
		throw crException("crAngularAccelOperator::read(): Expected crAngularAccelOperator ID");
	}
}
/////////////////////////////////////////
//
//crFluidFrictionOperator
//
/////////////////////////////////////////
void crFluidFrictionOperator::write(crDataOutputStream* out)
{
	out->writeInt(CREFluidFrictionOperator);
    out->writeFloat(getFluidDensity());
	out->writeFloat(getFluidViscosity());
	out->writeFloat(getOverrideRadius());
	out->writeVec3(getWind());
}

void crFluidFrictionOperator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREFluidFrictionOperator)
	{
		id = in->readInt();
        setFluidDensity(in->readFloat());
		setFluidViscosity(in->readFloat());
		setOverrideRadius(in->readFloat());
		setWind(in->readVec3());
	}
	else
	{
		throw crException("crFluidFrictionOperator::read(): Expected crFluidFrictionOperator ID");
	}
}
/////////////////////////////////////////
//
//crForceOperator
//
/////////////////////////////////////////
void crForceOperator::write(crDataOutputStream* out)
{
	out->writeInt(CREForceOperator);
    out->writeVec3(getForce());
}

void crForceOperator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREForceOperator)
	{
		id = in->readInt();
		setForce(in->readVec3());
	}
	else
	{
		throw crException("crForceOperator::read(): Expected crForceOperator ID");
	}
}
/////////////////////////////////////////
//
//crAngularDampingOperator
//
/////////////////////////////////////////
void crAngularDampingOperator::write(crDataOutputStream* out)
{
	out->writeInt(CREAngularDampingOperator);
	out->writeVec3(getDamping());
	out->writeFloat(getCutoffLow());
	out->writeFloat(getCutoffHigh());
}

void crAngularDampingOperator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREAngularDampingOperator)
	{
		id = in->readInt();
		setDamping(in->readVec3());
		setCutoffLow(in->readFloat());
		setCutoffHigh(in->readFloat());
	}
	else
	{
		throw crException("crAngularDampingOperator::read(): Expected crAngularDampingOperator ID");
	}
}
/////////////////////////////////////////
//
//crDampingOperator
//
/////////////////////////////////////////
void crDampingOperator::write(crDataOutputStream* out)
{
	out->writeInt(CREDampingOperator);
	out->writeVec3(getDamping());
	out->writeFloat(getCutoffLow());
	out->writeFloat(getCutoffHigh());
}

void crDampingOperator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREDampingOperator)
	{
		id = in->readInt();
		setDamping(in->readVec3());
		setCutoffLow(in->readFloat());
		setCutoffHigh(in->readFloat());
	}
	else
	{
		throw crException("crDampingOperator::read(): Expected crDampingOperator ID");
	}
}
/////////////////////////////////////////
//
//crExplosionOperator
//
/////////////////////////////////////////
void crExplosionOperator::write(crDataOutputStream* out)
{
	out->writeInt(CREExplosionOperator);
	out->writeVec3(getCenter());
	out->writeFloat(getRadius());
	out->writeFloat(getMagnitude());
	out->writeFloat(getEpsilon());
	out->writeFloat(getSigma());
}

void crExplosionOperator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREExplosionOperator)
	{
		id = in->readInt();
		setCenter(in->readVec3());
		setRadius(in->readFloat());
		setMagnitude(in->readFloat());
		setEpsilon(in->readFloat());
		setSigma(in->readFloat());
	}
	else
	{
		throw crException("crExplosionOperator::read(): Expected crExplosionOperator ID");
	}
}
/////////////////////////////////////////
//
//crOrbitOperator
//
/////////////////////////////////////////
void crOrbitOperator::write(crDataOutputStream* out)
{
	out->writeInt(CREOrbitOperator);
	out->writeVec3(getCenter());
	out->writeFloat(getMagnitude());
	out->writeFloat(getEpsilon());
	out->writeFloat(getMaxRadius());
}

void crOrbitOperator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREOrbitOperator)
	{
		id = in->readInt();
		setCenter(in->readVec3());
		setMagnitude(in->readFloat());
		setEpsilon(in->readFloat());
		setMaxRadius(in->readFloat());
	}
	else
	{
		throw crException("crOrbitOperator::read(): Expected crOrbitOperator ID");
	}
}
/////////////////////////////////////////
//
//crLinearInterpolator
//
/////////////////////////////////////////
void crLinearInterpolator::write(crDataOutputStream* out)
{
	out->writeInt(CRELinearInterpolator);
}

void crLinearInterpolator::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CRELinearInterpolator)
	{
		id = in->readInt();
	}
	else
	{
		throw crException("crLinearInterpolator::read(): Expected crLinearInterpolator ID");
	}
}