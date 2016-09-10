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
//Modified by Wucaihua
#ifndef CRCORE_CLIPPLANE_H
#define CRCORE_CLIPPLANE_H 1

#include <CRCore/crVector4.h>
#include <CRCore/crPlane.h>
#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** crClipPlane state class which encapsulates OpenGL glClipPlane() functionality.*/
class CR_EXPORT crClipPlane : public crStateAttribute
{
    public :

        crClipPlane();
        inline crClipPlane(unsigned int no,const crVector4d& plane)    { setClipPlaneNum(no); setClipPlane(plane); }
        inline crClipPlane(unsigned int no,const crPlane& plane)   { setClipPlaneNum(no); setClipPlane(plane); }
        inline crClipPlane(unsigned int no,double a,double b,double c,double d)  { setClipPlaneNum(no); setClipPlane(a,b,c,d); }

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crClipPlane(const crClipPlane& cp,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(cp,copyop)
        {
            m_clipPlane[0]=cp.m_clipPlane[0];
            m_clipPlane[1]=cp.m_clipPlane[1];
            m_clipPlane[2]=cp.m_clipPlane[2];
            m_clipPlane[3]=cp.m_clipPlane[3];
            m_clipPlaneNum=cp.m_clipPlaneNum;
        }
        
        META_StateAttribute(CRCore, crClipPlane, CLIPPLANE0+m_clipPlaneNum);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crClipPlane,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_clipPlaneNum)
            COMPARE_StateAttribute_Parameter(m_clipPlane[0])
            COMPARE_StateAttribute_Parameter(m_clipPlane[1])
            COMPARE_StateAttribute_Parameter(m_clipPlane[2])
            COMPARE_StateAttribute_Parameter(m_clipPlane[3])

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crClipPlane,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_clipPlaneNum)
			COMPARE_StateAttribute_Parameter(m_clipPlane[0])
			COMPARE_StateAttribute_Parameter(m_clipPlane[1])
			COMPARE_StateAttribute_Parameter(m_clipPlane[2])
			COMPARE_StateAttribute_Parameter(m_clipPlane[3])

			return 0; // passed all the above comparison macro's, must be equal.
		}

        //virtual bool getModeUsage(ModeUsage& usage) const
        //{
        //    usage.usesMode((GLMode)(GL_CLIP_PLANE0+m_clipPlaneNum));
        //    return true;
        //}
		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back((unsigned int)(GL_CLIP_PLANE0+m_clipPlaneNum));
		}

        inline void setClipPlane(const crVector4d& plane){ m_clipPlane = plane; };

		inline void setClipPlane(const crPlane& plane) 
		{
			m_clipPlane.set(plane[0],plane[1],plane[2],plane[3]);
		}

        inline void setClipPlane(double a,double b,double c,double d)
        {
            m_clipPlane.set(a,b,c,d);           
        }

		const crVector4d& getClipPlane() const { return m_clipPlane; }

		/** Set the clip plane number. */
		void setClipPlaneNum(unsigned int num);

		/** Get the clip plane number. */
		unsigned int getClipPlaneNum() const;

		virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected :
        virtual ~crClipPlane();
        crVector4d          m_clipPlane;
        unsigned int        m_clipPlaneNum;
};

}

#endif
