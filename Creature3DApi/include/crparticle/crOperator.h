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
#ifndef CRPARTICLE_OPERATOR
#define CRPARTICLE_OPERATOR 1

#include <CRParticle/crProgram.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crArgumentParser.h>

namespace CRParticle
{

    // forward declaration to avoid including the whole header file
    class crParticle;

    /** An abstract base class used by <CODE>crModularProgram</CODE> to perform operations on particles before they are updated. 
        To implement a new operator, derive from this class and override the <CODE>operate()</CODE> method.
        You should also override the <CODE>beginOperate()</CODE> method to query the calling program for the reference frame
        used, and initialize the right transformations if needed.
    */
    class crOperator: public CRCore::crBase {        
    public:
        inline crOperator();
        inline crOperator(const crOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crOperator"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crOperator* >(obj) != 0; }
        
        /// Get whether this operator is enabled.
        inline bool isEnabled() const;
        
        /// Enable or disable this operator.
        inline void setEnabled(bool v);
        
        /**    Do something on a particle. 
            This method is called by <CODE>crModularProgram</CODE> objects to perform some operations
            on the particles. You must override it in descendant classes. Common operations
            consist of modifying the particle's velocity vector. The <CODE>dt</CODE> parameter is
            the time elapsed from last operation.            
        */
        virtual void operate(crParticle* P, double dt) = 0;
        
        /** Do something before processing particles via the <CODE>operate()</CODE> method.
            Overriding this method could be necessary to query the calling <CODE>crProgram</CODE> object
            for the current reference frame. If the reference frame is RELATIVE_RF, then your
            class should prepare itself to do all operations in local coordinates.
        */
        virtual void beginOperate(crProgram *) {}
        
        /// Do something after all particles have been processed.
        virtual void endOperate() {}

		virtual void addOperatorParam(int i, const std::string &str) = 0;

    protected:
        virtual ~crOperator() {}
        crOperator &operator=(const crOperator &) { return *this; }
        
    private:
        bool m_enabled;        
    };
    
    // INLINE FUNCTIONS
    
    inline crOperator::crOperator()
    : CRCore::crBase(), m_enabled(true)
    {
    }
    
    inline crOperator::crOperator(const crOperator& copy, const CRCore::crCopyOp& copyop)
    : CRCore::crBase(copy, copyop), m_enabled(copy.m_enabled)
    {
    }
    
    inline bool crOperator::isEnabled() const    
    {
        return m_enabled;
    }
    
    inline void crOperator::setEnabled(bool v)
    {
        m_enabled = v;
    }


}

#endif
