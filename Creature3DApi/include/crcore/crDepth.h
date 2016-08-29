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
#ifndef CRCORE_DEPTH
#define CRCORE_DEPTH 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Encapsulate OpenGL glDepthFunc/Mask/Range functions.
*/     
class CR_EXPORT crDepth : public crStateAttribute
{
    public :
        enum Function
        {
            NEVER,
            LESS,
            EQUAL,
            LEQUAL,
            GREATER,
            NOTEQUAL,
            GEQUAL,
            ALWAYS
        };


        crDepth(Function func=LESS,double zNear=0.0, double zFar=1.0,bool writeMask=true);
        
        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crDepth(const crDepth& dp,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(dp,copyop),
            m_func(dp.m_func),
            m_zNear(dp.m_zNear),
            m_zFar(dp.m_zFar),
            m_depthWriteMask(dp.m_depthWriteMask) {}
        
        
        META_StateAttribute(CRCore, crDepth, DEPTH);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crDepth,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_func)
            COMPARE_StateAttribute_Parameter(m_depthWriteMask)
            COMPARE_StateAttribute_Parameter(m_zNear)
            COMPARE_StateAttribute_Parameter(m_zFar)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crDepth,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_func)
			COMPARE_StateAttribute_Parameter(m_depthWriteMask)
			COMPARE_StateAttribute_Parameter(m_zNear)
			COMPARE_StateAttribute_Parameter(m_zFar)

			return 0; // passed all the above comparison macro's, must be equal.
		}

        //virtual bool getModeUsage(ModeUsage& usage) const
        //{
        //    usage.usesMode(GL_DEPTH_TEST);
        //    return true;
        //}

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			// Have to think about the role of m_colorMode
			// in setting the colormaterial... also need to take the
			// color material enable/disable out of the the apply()...
			modes.push_back(GL_DEPTH_TEST);
		}

        inline void setFunction(Function func) { m_func = func; }
        
        inline Function getFunction() const { return m_func; }
               

        inline void setRange(double zNear, double zFar)
        {
            m_zNear = zNear;
            m_zFar = zFar;
        }

        inline double getZNear() const { return m_zNear; }
        inline double getZFar() const { return m_zFar; }

        inline void setWriteMask(bool mask) { m_depthWriteMask = mask; }
        
        inline bool getWriteMask() const { return m_depthWriteMask; }

		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;
    protected:
    
        virtual ~crDepth();

        Function            m_func;
        
        double              m_zNear;
        double              m_zFar;

        bool                m_depthWriteMask;
};

}

#endif
