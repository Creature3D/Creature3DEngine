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
#ifndef CRCORE_MULTISAMPLE_H
#define CRCORE_MULTISAMPLE_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/ref_ptr.h>


namespace CRCore {

/** crMultisample - encapsulates the OpenGL crMultisample state.*/
class CR_EXPORT crMultisample : public crStateAttribute
{
    public :

        enum Mode
        {
            FASTEST, 
            NICEST,
            DONT_CARE 
        };

        crMultisample();
        
        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crMultisample(const crMultisample& trans,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(trans,copyop),
            m_coverage(trans.m_coverage),
            m_invert(trans.m_invert),
            m_mode(trans.m_mode) {}

        META_StateAttribute(CRCore, crMultisample,MULTISAMPLE);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crMultisample,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_coverage)
            COMPARE_StateAttribute_Parameter(m_invert)
            COMPARE_StateAttribute_Parameter(m_mode)

            return 0; // passed all the above comparison macro's, must be equal.
        }
		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crMultisample,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_coverage)
			COMPARE_StateAttribute_Parameter(m_invert)
			COMPARE_StateAttribute_Parameter(m_mode)

			return 0; // passed all the above comparison macro's, must be equal.
		}

        void setSampleCoverage(float coverage, bool invert) 
        {
            m_coverage = coverage; 
            m_invert = invert;
        }
        inline float getCoverage() const { return m_coverage; }
        inline bool getInvert() const { return m_invert; }

        inline void setHint(Mode mode) { m_mode = mode; }
		inline Mode getHint() { return m_mode; }
		inline Mode getHint()const{ return m_mode; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);

        class Extensions : public Referenced
        {
            public:
                Extensions(unsigned int contextID);

                Extensions(const Extensions& rhs);

                void lowestCommonDenominator(const Extensions& rhs);

                void setupGLExtensions(unsigned int contextID);

                void setMultisampleSupported(bool flag) { _isMultisampleSupported=flag; }
                void setMultisampleFilterHintSupported(bool flag) { _isMultisampleFilterHintSupported=flag; }
                bool isMultisampleSupported() const { return _isMultisampleSupported; }
                bool isMultisampleFilterHintSupported() const { return _isMultisampleFilterHintSupported; }

                void glSampleCoverage(GLclampf value, GLboolean invert) const;
				static void clearExtensions();
            protected:

                ~Extensions() {}

                bool    _isMultisampleSupported;
                bool    _isMultisampleFilterHintSupported;

                typedef void (APIENTRY * GLSampleCoverageProc) (GLclampf value, GLboolean invert);
                GLSampleCoverageProc _glSampleCoverage;

        };
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);
        static void setExtensions(unsigned int contextID,Extensions* extensions);
    protected :

        virtual ~crMultisample();

        float m_coverage;
        bool m_invert;
        Mode m_mode;
};

}

#endif



