/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
#ifndef CRPARTICLE_FLUIDPROGRAM
#define CRPARTICLE_FLUIDPROGRAM 1

#include <CRParticle/crExport.h>
#include <CRParticle/crProgram.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crNode.h>
#include <CRCore/crNodeVisitor.h>

namespace CRParticle
{

    /**    A program class for performing operations on particles using a sequence of <I>operators</I>.
        To use a <CODE>crFluidProgram</CODE> you have to create some <CODE>crOperator</CODE> objects and 
        add them to the program.
        All operators will be applied to each particle in the same order they've been added to the program.
    */    
    class CRPARTICLE_EXPORT crFluidProgram: public crProgram {
    public:
        crFluidProgram();
        crFluidProgram(const crFluidProgram& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRParticle,crFluidProgram);
        
        /// Set the viscosity of the fluid.
        inline void setFluidViscosity(float v)
        {
            m_viscosity = v;
            m_viscosityCoefficient = 6 * PI * m_viscosity;
        }
       
        /// Get the viscosity of the fluid.
        inline float getFluidViscosity() const { return m_viscosity; }

        /// Set the density of the fluid.
        inline void setFluidDensity(float d)
        {
            m_density = d;
            m_densityCoefficeint = 0.2f * PI * m_density;
        }

        /// Get the density of the fluid.
        inline float getFluidDensity() const { return m_density; }
        
        
        /// Set the wind vector.
        inline void setWind(const CRCore::crVector3& wind) { m_wind = wind; }
        
        /// Get the wind vector.
        inline const CRCore::crVector3& getWind() const { return m_wind; }
        
        /// Set the acceleration vector.
        inline void setAcceleration(const CRCore::crVector3& v) { m_acceleration = v; }
        
        /// Get the acceleration vector.
        inline const CRCore::crVector3& getAcceleration() const { return m_acceleration; }

        /** Set the acceleration vector to the gravity on earth (0, 0, -9.81).
            The acceleration will be multiplied by the <CODE>scale</CODE> parameter.
        */
        inline void setToGravity(float scale = 1.0f) { m_acceleration.set(0, 0, -9.81f*scale); }
 
		enum FluidType
		{
			Air,
			Water,
			UserDefine
		};
		inline FluidType getFluidType()const { return m_fluidType; }
		void setFluidType(FluidType fluidtype);
    protected:

        virtual ~crFluidProgram() {}
        crFluidProgram& operator=(const crFluidProgram&) { return *this; }
        
        virtual void execute(double dt);
		/// Set the fluid parameters as for air (20°C temperature).
		void setFluidToAir();
		/// Set the fluid parameters as for pure water (20°C temperature).
		void setFluidToWater();

        CRCore::crVector3   m_acceleration;
        float       m_viscosity;
        float       m_density;
        CRCore::crVector3   m_wind;

        float       m_viscosityCoefficient;
        float       m_densityCoefficeint;
        FluidType   m_fluidType;
    };
}

#endif
