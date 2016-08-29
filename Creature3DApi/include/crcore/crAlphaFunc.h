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

#ifndef CRCORE_ALPHAFUNC_H
#define CRCORE_ALPHAFUNC_H 1

#include <CRCore/crStateAttribute.h>

namespace  CRCore {

/** Encapsulate OpenGL glAlphaFunc.
*/     
class CR_EXPORT crAlphaFunc : public crStateAttribute
{
    public :
    
        crAlphaFunc();
        
        crAlphaFunc(const crAlphaFunc& af,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(af,copyop),
            m_comparisonFunc(af.m_comparisonFunc),
            m_referenceValue(af.m_referenceValue) {}
        
        META_StateAttribute(CRCore,crAlphaFunc,ALPHAFUNC);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const;
		virtual int compare(const crStateAttribute& sa);

        virtual void getAssociatedModes(std::vector<VRMode>& modes) const;

        enum ComparisonFunction {
            NEVER,
            LESS,
            EQUAL,
            LEQUAL,
            GREATER,
            NOTEQUAL,
            GEQUAL,
            ALWAYS
        };

        void setFunction(ComparisonFunction func,float ref);
        
        inline ComparisonFunction getFunction() const { return m_comparisonFunc; }
        
        inline float getReferenceValue() const { return m_referenceValue; }
        
        //virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;
    protected:
    
        virtual ~crAlphaFunc();

        ComparisonFunction m_comparisonFunc;
        float m_referenceValue;

};

}

#endif
