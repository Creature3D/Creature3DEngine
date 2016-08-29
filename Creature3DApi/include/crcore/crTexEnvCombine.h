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
#ifndef CRCORE_TEXENVCOMBINE_H
#define CRCORE_TEXENVCOMBINE_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crVector4.h>

namespace CRCore {

/** TexEnvCombine - encapsulates the OpenGL glTexEnvCombine (texture environment) state.*/
class CR_EXPORT crTexEnvCombine : public crStateAttribute
{
    public :

        crTexEnvCombine();
        
        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTexEnvCombine(const crTexEnvCombine& texenv,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(texenv,copyop),
            m_needsTexEnvCrossbar(texenv.m_needsTexEnvCrossbar),
            m_combine_RGB(texenv.m_combine_RGB),
            m_combine_Alpha(texenv.m_combine_Alpha),
            m_source0_RGB(texenv.m_source0_RGB),
            m_source1_RGB(texenv.m_source1_RGB),
            m_source2_RGB(texenv.m_source2_RGB),
            m_source0_Alpha(texenv.m_source0_Alpha),
            m_source1_Alpha(texenv.m_source1_Alpha),
            m_source2_Alpha(texenv.m_source2_Alpha),
            m_operand0_RGB(texenv.m_operand0_RGB),
            m_operand1_RGB(texenv.m_operand1_RGB),
            m_operand2_RGB(texenv.m_operand2_RGB),
            m_operand0_Alpha(texenv.m_operand0_Alpha),
            m_operand1_Alpha(texenv.m_operand1_Alpha),
            m_operand2_Alpha(texenv.m_operand2_Alpha),
            m_scale_RGB(texenv.m_scale_RGB),
            m_scale_Alpha(texenv.m_scale_Alpha),
            m_constantColor(texenv.m_constantColor) {}


        META_StateAttribute(CRCore, crTexEnvCombine, TEXENV);


        virtual bool isTextureAttribute() const { return true; }

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crTexEnvCombine,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_needsTexEnvCrossbar)
            COMPARE_StateAttribute_Parameter(m_combine_RGB)
            COMPARE_StateAttribute_Parameter(m_combine_Alpha)
            COMPARE_StateAttribute_Parameter(m_source0_RGB)
            COMPARE_StateAttribute_Parameter(m_source1_RGB)
            COMPARE_StateAttribute_Parameter(m_source2_RGB)
            COMPARE_StateAttribute_Parameter(m_source0_Alpha)
            COMPARE_StateAttribute_Parameter(m_source1_Alpha)
            COMPARE_StateAttribute_Parameter(m_source2_Alpha)
            COMPARE_StateAttribute_Parameter(m_operand0_RGB)
            COMPARE_StateAttribute_Parameter(m_operand1_RGB)
            COMPARE_StateAttribute_Parameter(m_operand2_RGB)
            COMPARE_StateAttribute_Parameter(m_operand0_Alpha)
            COMPARE_StateAttribute_Parameter(m_operand1_Alpha)
            COMPARE_StateAttribute_Parameter(m_operand2_Alpha)
            COMPARE_StateAttribute_Parameter(m_scale_RGB)
            COMPARE_StateAttribute_Parameter(m_scale_Alpha)
            COMPARE_StateAttribute_Parameter(m_constantColor)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crTexEnvCombine,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_needsTexEnvCrossbar)
			COMPARE_StateAttribute_Parameter(m_combine_RGB)
			COMPARE_StateAttribute_Parameter(m_combine_Alpha)
			COMPARE_StateAttribute_Parameter(m_source0_RGB)
			COMPARE_StateAttribute_Parameter(m_source1_RGB)
			COMPARE_StateAttribute_Parameter(m_source2_RGB)
			COMPARE_StateAttribute_Parameter(m_source0_Alpha)
			COMPARE_StateAttribute_Parameter(m_source1_Alpha)
			COMPARE_StateAttribute_Parameter(m_source2_Alpha)
			COMPARE_StateAttribute_Parameter(m_operand0_RGB)
			COMPARE_StateAttribute_Parameter(m_operand1_RGB)
			COMPARE_StateAttribute_Parameter(m_operand2_RGB)
			COMPARE_StateAttribute_Parameter(m_operand0_Alpha)
			COMPARE_StateAttribute_Parameter(m_operand1_Alpha)
			COMPARE_StateAttribute_Parameter(m_operand2_Alpha)
			COMPARE_StateAttribute_Parameter(m_scale_RGB)
			COMPARE_StateAttribute_Parameter(m_scale_Alpha)
			COMPARE_StateAttribute_Parameter(m_constantColor)
			return 0; // passed all the above comparison macro's, must be equal.
    	}

        enum CombineParam
        {
            REPLACE ,
            MODULATE,
            ADD,
            ADD_SIGNED,
            INTERPOLATE,
            SUBTRACT,
            DOT3_RGB,
            DOT3_RGBA
        };

        void setCombine_RGB(CombineParam cm);
        void setCombine_Alpha(CombineParam cm);

        CombineParam getCombine_RGB() const { return m_combine_RGB; }
        CombineParam getCombine_Alpha() const { return m_combine_Alpha; }
        
        enum SourceParam
        {
            CONSTANT,
            PRIMARY_COLOR,
            PREVIOUS,
            TEXTURE,
            TEXTURE0,
            TEXTURE1,
            TEXTURE2,
            TEXTURE3,
            TEXTURE4,
            TEXTURE5,
            TEXTURE6,
            TEXTURE7
        };
        
        void setSource0_RGB(SourceParam sp);
        void setSource1_RGB(SourceParam sp);
        void setSource2_RGB(SourceParam sp);
        
        void setSource0_Alpha(SourceParam sp);
        void setSource1_Alpha(SourceParam sp);
        void setSource2_Alpha(SourceParam sp);

        SourceParam getSource0_RGB() const { return m_source0_RGB; }
        SourceParam getSource1_RGB() const { return m_source1_RGB; }
        SourceParam getSource2_RGB() const { return m_source2_RGB; }
        
        SourceParam getSource0_Alpha() const { return m_source0_Alpha; }
        SourceParam getSource1_Alpha() const { return m_source1_Alpha; }
        SourceParam getSource2_Alpha() const { return m_source2_Alpha; }

        enum OperandParam
        {
            SRC_COLOR ,
            ONE_MINUS_SRC_COLOR,
            SRC_ALPHA ,
            ONE_MINUS_SRC_ALPHA
        };
        
        void setOperand0_RGB(OperandParam op);
        void setOperand1_RGB(OperandParam op);
        void setOperand2_RGB(OperandParam op);
        
        void setOperand0_Alpha(OperandParam op);
        void setOperand1_Alpha(OperandParam op);
        void setOperand2_Alpha(OperandParam op);

        OperandParam getOperand0_RGB() const { return m_operand0_RGB; }
        OperandParam getOperand1_RGB() const { return m_operand1_RGB; }
        OperandParam getOperand2_RGB() const { return m_operand2_RGB; }
        
        OperandParam getOperand0_Alpha() const { return m_operand0_Alpha; }
        OperandParam getOperand1_Alpha() const { return m_operand1_Alpha; }
        OperandParam getOperand2_Alpha() const { return m_operand2_Alpha; }


        void setScale_RGB(float scale);
        void setScale_Alpha(float scale);

        float getScale_RGB() const { return m_scale_RGB; }
        float getScale_Alpha() const { return m_scale_Alpha; }

        void setConstantColor( const crVector4& color ) { m_constantColor = color; }
        const crVector4& getConstantColor() const { return m_constantColor; }

		/** Set the constant color attribute to the given light direction
		* for use with DOT3 combine operation. */
		void setConstantColorAsLightDirection(const crVector3& direction)
		{
			m_constantColor.set((direction.x()+1.0f)*0.5f,(direction.y()+1.0f)*0.5f,(direction.z()+1.0f)*0.5f,1.0f);
		}

		crVector3 getConstantColorAsLightDirection() const
		{
			return crVector3(m_constantColor.x()*2.0f-1.0f, m_constantColor.y()*2.0f-1.0f, m_constantColor.z()*2.0f-1.0f);
		}

		bool getNeedsTexEnvCrossbar(){ return m_needsTexEnvCrossbar; }
		bool getNeedsTexEnvCrossbar()const{ return m_needsTexEnvCrossbar; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected :

        virtual ~crTexEnvCombine();

        inline bool needsTexEnvCombiner(int value) const
        {
            switch(value)
            {
                case(CONSTANT):
                case(PRIMARY_COLOR):
                case(PREVIOUS):
                case(TEXTURE):
                    return false;
            }
            return true;
        }

        void computeNeedoForTexEnvCombiners()
        {
            m_needsTexEnvCrossbar = (needsTexEnvCombiner(m_source0_RGB) ||
                                    needsTexEnvCombiner(m_source1_RGB) ||
                                    needsTexEnvCombiner(m_source2_RGB) ||
                                    needsTexEnvCombiner(m_source0_Alpha) ||
                                    needsTexEnvCombiner(m_source1_Alpha) ||
                                    needsTexEnvCombiner(m_source2_Alpha)); 
        }
        

        


        bool            m_needsTexEnvCrossbar;

        CombineParam    m_combine_RGB;
        CombineParam    m_combine_Alpha;
    
        SourceParam     m_source0_RGB;
        SourceParam     m_source1_RGB;
        SourceParam     m_source2_RGB;
        
        SourceParam     m_source0_Alpha;
        SourceParam     m_source1_Alpha;
        SourceParam     m_source2_Alpha;


        OperandParam    m_operand0_RGB;
        OperandParam    m_operand1_RGB;
        OperandParam    m_operand2_RGB;
        
        OperandParam    m_operand0_Alpha;
        OperandParam    m_operand1_Alpha;
        OperandParam    m_operand2_Alpha;


        float           m_scale_RGB;
        float           m_scale_Alpha;
        
        CRCore::crVector4      m_constantColor;

};

}

#endif
