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
//Modified by Îâ²Æ»ª
#ifndef CRUTIL_SIMPLIYFIER
#define CRUTIL_SIMPLIYFIER 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>

#include <CRUtil/crExport.h>

namespace CRUtil {

/** A simplifier for reducing the number of traingles in CRCore::crGeometry.
  */
class CRUTIL_EXPORT crSimplifier : public CRCore::crNodeVisitor
{
    public:

        crSimplifier(float sampleRatio=1.0f, float maximumError=0.0f);


        void setSampleRatio(float sampleRatio) { m_sampleRatio = sampleRatio; }
        float getSampleRatio() const { return m_sampleRatio; }

        void setMaximumError(float error) { m_maximumError = error; }
        float getMaximumError() const { return m_maximumError; }
            

        class ContinueSimplificationCallback : public CRCore::Referenced
        {
            public:
                /** return true if mesh should be continued to be simplified, return false to stop simplification.*/
                virtual bool continueSimplification(const crSimplifier& simplifier, float nextError, unsigned int numOriginalPrimitives, unsigned int numRemainingPrimitives) const
                {
                    return simplifier.continueSimplificationImplementation(nextError, numOriginalPrimitives, numRemainingPrimitives);
                }
            
            protected:
                virtual ~ContinueSimplificationCallback() {}
        };
        
        void setContinueSimplificationCallback(ContinueSimplificationCallback* cb) { m_continueSimplificationCallback = cb; }
        ContinueSimplificationCallback* getContinueSimplificationCallback() { return m_continueSimplificationCallback.get(); }
        const ContinueSimplificationCallback* getContinueSimplificationCallback() const { return m_continueSimplificationCallback.get(); }
        
        
        bool continueSimplification(float nextError, unsigned int numOriginalPrimitives, unsigned int numRemainingPrimitives) const
        {
            if (m_continueSimplificationCallback.valid()) return m_continueSimplificationCallback->continueSimplification(*this, nextError, numOriginalPrimitives, numRemainingPrimitives);
            else return continueSimplificationImplementation(nextError, numOriginalPrimitives, numRemainingPrimitives);
        }

        virtual bool continueSimplificationImplementation(float nextError, unsigned int numOriginalPrimitives, unsigned int numRemainingPrimitives) const
        {
            if (nextError<=getMaximumError()) return true;
            return (float)numRemainingPrimitives > (float)numOriginalPrimitives * getSampleRatio();
        }


        virtual void apply(CRCore::crObject& geode)
        {
            for(unsigned int i=0;i<geode.getNumDrawables();++i)
            {
                CRCore::crGeometry* geometry = geode.getDrawable(i)->asGeometry();
                if (geometry)
                {
                    simplify(*geometry);
                }
            }
        }

        /** simply the geometry.*/
        void simplify(CRCore::crGeometry& geometry);
        
        typedef std::vector<unsigned int> IndexList; /// a list of point indices

        /** simply the geometry, whilst protecting key points from being modified.*/
        void simplify(CRCore::crGeometry& geometry, const IndexList& protectedPoints);

    protected:
    
        float m_sampleRatio;
        float m_maximumError;
        
        CRCore::ref_ptr<ContinueSimplificationCallback> m_continueSimplificationCallback;
    
};


}

#endif
