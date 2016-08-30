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
#ifndef CRPARTICLE_MODULARPROGRAM
#define CRPARTICLE_MODULARPROGRAM 1

#include <CRParticle/crExport.h>
#include <CRParticle/crProgram.h>
#include <CRParticle/crOperator.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crNode.h>
#include <CRCore/crNodeVisitor.h>

namespace CRParticle
{

    /**    A program class for performing operations on particles using a sequence of <I>operators</I>.
        To use a <CODE>crModularProgram</CODE> you have to create some <CODE>crOperator</CODE> objects and 
        add them to the program.
        All operators will be applied to each particle in the same order they've been added to the program.
    */    
    class CRPARTICLE_EXPORT crModularProgram: public crProgram {
    public:
        crModularProgram();
        crModularProgram(const crModularProgram& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRParticle,crModularProgram);
        
        /// Get the number of operators.
        inline int numOperators() const;
        
        /// Add an operator to the list.        
        inline void addOperator(crOperator* o);
        
        /// Get a pointer to an operator in the list.
        inline crOperator* getOperator(int i);
        
        /// Get a const pointer to an operator in the list.
        inline const crOperator* getOperator(int i) const;
        
        /// Remove an operator from the list.
        inline void removeOperator(int i);
		inline void removeLastOperator();
		void replaceOperator(int i, crOperator* o);
		void reset();
    protected:
        virtual ~crModularProgram() {}
        crModularProgram& operator=(const crModularProgram&) { return *this; }
        
        void execute(double dt);
        
    private:
        typedef std::vector<CRCore::ref_ptr<crOperator> > Operator_vector;

        Operator_vector m_operators;
    };
    
    // INLINE FUNCTIONS
    
    inline int crModularProgram::numOperators() const
    {
        return static_cast<int>(m_operators.size());
    }

    inline void crModularProgram::addOperator(crOperator* o)
    {
        m_operators.push_back(o);
    }

    inline crOperator* crModularProgram::getOperator(int i)
    {
        return m_operators[i].get();
    }

    inline const crOperator* crModularProgram::getOperator(int i) const
    {
        return m_operators[i].get();
    }

    inline void crModularProgram::removeOperator(int i)
    {
        m_operators.erase(m_operators.begin()+i);
    }
	inline void crModularProgram::removeLastOperator()
	{
		if(!m_operators.empty())
		    m_operators.pop_back();
	}

}

#endif
