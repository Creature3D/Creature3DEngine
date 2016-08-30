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
#ifndef CRPARTICLE_FLUIDFRICTIONOPERATOR
#define CRPARTICLE_FLUIDFRICTIONOPERATOR 1

#include <CRParticle/crExport.h>
#include <CRParticle/crOperator.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crMath.h>
#include <CRCore/crArgumentParser.h>

namespace CRParticle
{

    class crParticle;    

    /**    An operator that simulates the friction of a fluid.
        By using this operator you can let the particles move in a fluid of a given <I>density</I>
        and <I>viscosity</I>. There are two functions to quickly setup the parameters for pure water
        and air. You can decide whether to compute the forces using the particle's physical 
        radius or another value, by calling the <CODE>setOverrideRadius()</CODE> method.
    */
    class CRPARTICLE_EXPORT crFluidFrictionOperator: public crOperator {
    public:
        
        crFluidFrictionOperator();
        crFluidFrictionOperator(const crFluidFrictionOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRParticle, crFluidFrictionOperator);
               
        /// Set the density of the fluid.
        inline void setFluidDensity(float d);

        /// Get the density of the fluid.
        inline float getFluidDensity() const;
        
        /// Set the viscosity of the fluid.
        inline void setFluidViscosity(float v);
        
        /// Get the viscosity of the fluid.
        inline float getFluidViscosity() const;
        
        /// Set the wind vector.
        inline void setWind(const CRCore::crVector3& wind) { m_wind = wind; }
        
        /// Get the wind vector.
        inline const CRCore::crVector3& getWind() const { return m_wind; }
               
        /// Set the overriden radius value (pass 0 if you want to use particle's radius).
        inline void setOverrideRadius(float r);

        /// Get the overriden radius value.
        inline float getOverrideRadius() const;        
        
        /// Set the fluid parameters as for air (20°C temperature).
        inline void setFluidToAir();
        
        /// Set the fluid parameters as for pure water (20°C temperature).
        inline void setFluidToWater();
        
        /// Apply the friction forces to a particle. Do not call this method manually.
        void operate(crParticle* P, double dt);
        
        /// Perform some initializations. Do not call this method manually.
        inline void beginOperate(crProgram* prg);

		inline virtual void addOperatorParam(int i, const std::string &str)
		{
			switch(i) 
			{
			case 0:
				//CRCore::crArgumentParser::readKeyValue(str,"FLT",m_coeff_A);
				m_coeff_A = atof(str.c_str());
				break;
			case 1:
				//CRCore::crArgumentParser::readKeyValue(str,"FLT",m_coeff_B);
				m_coeff_B = atof(str.c_str());
				break;
			case 2:
				//CRCore::crArgumentParser::readKeyValue(str,"FLT",m_density);
				m_density = atof(str.c_str());
				break;
			case 3:
				//CRCore::crArgumentParser::readKeyValue(str,"FLT",m_viscosity);
				m_viscosity = atof(str.c_str());
				break;
			case 4:
				//CRCore::crArgumentParser::readKeyValue(str,"FLT",m_ovr_rad);
				m_ovr_rad = atof(str.c_str());
				break;
			case 5:
				CRCore::crArgumentParser::appAtoVec(str,m_wind);
				break;
			}
		}
    protected:
        virtual ~crFluidFrictionOperator() {}
        crFluidFrictionOperator &operator=(const crFluidFrictionOperator &) { return *this; }
        
    private:
        float       m_coeff_A;
        float       m_coeff_B;
        float       m_density;
        float       m_viscosity;
        float       m_ovr_rad;
        CRCore::crVector3   m_wind;
        crProgram*    m_current_program;
    };
    
    // INLINE FUNCTIONS
    
    inline float crFluidFrictionOperator::getFluidDensity() const
    {
        return m_density;
    }

    inline float crFluidFrictionOperator::getFluidViscosity() const
    {
        return m_viscosity;
    }
    
    inline void crFluidFrictionOperator::setFluidDensity(float d)
    {
        m_density = d;
        m_coeff_B = 0.2f * CRCore::PI * m_density;
    }

    inline void crFluidFrictionOperator::setFluidViscosity(float v)
    {
        m_viscosity = v;
        m_coeff_A = 6 * CRCore::PI * m_viscosity;
    }
    
    inline void crFluidFrictionOperator::setFluidToAir()
    {
        setFluidViscosity(1.8e-5f);
        setFluidDensity(1.2929f);
    }

    inline void crFluidFrictionOperator::setFluidToWater()
    {
        setFluidViscosity(1.002e-3f);
        setFluidDensity(1.0f);
    }
    
    inline float crFluidFrictionOperator::getOverrideRadius() const
    {
        return m_ovr_rad;
    }
    
    inline void crFluidFrictionOperator::setOverrideRadius(float r)
    {
        m_ovr_rad = r;
    }
    
    inline void crFluidFrictionOperator::beginOperate(crProgram* prg)
    {
        m_current_program = prg;
    }

}


#endif
