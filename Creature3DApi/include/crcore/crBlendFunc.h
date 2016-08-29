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

#ifndef CRCORE_CRBLENDFUNC_H
#define CRCORE_CRBLENDFUNC_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** crBlendFunc - encapsulates the OpenGL blend/transparency state.*/
class CR_EXPORT crBlendFunc : public crStateAttribute
{
    public :

        crBlendFunc();
        
        crBlendFunc(unsigned int source, unsigned int destination);
		crBlendFunc(unsigned int source, unsigned int destination, unsigned int source_alpha, unsigned int destination_alpha);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crBlendFunc(const crBlendFunc& trans,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(trans,copyop),
            m_source_factor(trans.m_source_factor),
            m_destination_factor(trans.m_destination_factor),
			m_source_factor_alpha(trans.m_source_factor_alpha),
			m_destination_factor_alpha(trans.m_destination_factor_alpha) {}

        META_StateAttribute(CRCore, crBlendFunc,BLENDFUNC);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crBlendFunc,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_source_factor)
            COMPARE_StateAttribute_Parameter(m_destination_factor)
			COMPARE_StateAttribute_Parameter(m_source_factor_alpha)
			COMPARE_StateAttribute_Parameter(m_destination_factor_alpha)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crBlendFunc,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_source_factor)
			COMPARE_StateAttribute_Parameter(m_destination_factor)
			COMPARE_StateAttribute_Parameter(m_source_factor_alpha)
			COMPARE_StateAttribute_Parameter(m_destination_factor_alpha)

			return 0; // passed all the above comparison macro's, must be equal.
		}
   /*     virtual bool getModeUsage(ModeUsage& usage) const
        {
            usage.usesMode(GL_BLEND);
            return true;
        }*/
		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_BLEND);
			//modes.push_back(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
		}
        enum BlendFuncMode {
            DST_ALPHA,
            DST_COLOR,
            ONE,
            ONE_MINUS_DST_ALPHA,
            ONE_MINUS_DST_COLOR,
            ONE_MINUS_SRC_ALPHA,
            ONE_MINUS_SRC_COLOR,
            SRC_ALPHA,
            SRC_ALPHA_SATURATE,
            SRC_COLOR,
			CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR,
            CONSTANT_ALPHA,
            ONE_MINUS_CONSTANT_ALPHA,
            ZERO
        };

        inline void setFunction( unsigned int source, unsigned int destination )
        {
            m_source_factor = source;
            m_destination_factor = destination;
			m_source_factor_alpha = source;
			m_destination_factor_alpha = destination;
        }
		inline void setFunction( unsigned int source_rgb, unsigned int destination_rgb, unsigned int source_alpha, unsigned int destination_alpha )
		{
			m_source_factor = source_rgb;
			m_destination_factor = destination_rgb;
			m_source_factor_alpha = source_alpha;
			m_destination_factor_alpha = destination_alpha;
		}

        void setSource(unsigned int source) { m_source_factor = source; }
        inline unsigned int getSource() const { return m_source_factor; }
		void setSourceRGB(unsigned int source) { m_source_factor = source; }
		inline unsigned int getSourceRGB() const { return m_source_factor; }
		void setSourceAlpha(unsigned int source) { m_source_factor_alpha = source; }
		inline unsigned int getSourceAlpha() const { return m_source_factor_alpha; }

        void setDestination(unsigned int destination) { m_destination_factor = destination; }
        inline unsigned int getDestination() const { return m_destination_factor; }
		void setDestinationRGB(unsigned int destination) { m_destination_factor = destination; }
		inline unsigned int getDestinationRGB() const { return m_destination_factor; }
		void setDestinationAlpha(unsigned int destination) { m_destination_factor_alpha = destination; }
		inline unsigned int getDestinationAlpha() const { return m_destination_factor_alpha; }

		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;
		class Extensions : public Referenced
        {
        public:
            Extensions(unsigned int contextID);

            Extensions(const Extensions& rhs);

            void lowestCommonDenominator(const Extensions& rhs);

            void setupGLExtensions(unsigned int contextID);

            void setBlendFuncSeparateSupported(bool flag) { _isBlendFuncSeparateSupported=flag; }
            bool isBlendFuncSeparateSupported() const { return _isBlendFuncSeparateSupported; }

            void glBlendFuncSeparate(GLenum sfactorRGB,
                                     GLenum dfactorRGB,
                                     GLenum sfactorAlpha,
                                     GLenum dfactorAlpha) const;
			static void clearExtensions();
        protected:

            ~Extensions() {}


            typedef void (APIENTRY * GLBlendFuncSeparateProc) (GLenum sfactorRGB,
                                                               GLenum dfactorRGB,
                                                               GLenum sfactorAlpha,
                                                               GLenum dfactorAlpha);
            bool                    _isBlendFuncSeparateSupported;
            GLBlendFuncSeparateProc _glBlendFuncSeparate;

        };
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);
        static void setExtensions(unsigned int contextID,Extensions* extensions);
    protected :
        virtual ~crBlendFunc();
        unsigned int m_source_factor;
        unsigned int m_destination_factor;
		unsigned int m_source_factor_alpha;
		unsigned int m_destination_factor_alpha;
};

}

#endif
