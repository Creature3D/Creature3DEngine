/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef CRPHYSICS_CRPHYSICSUPDATEVISITOR_H
#define CRPHYSICS_CRPHYSICSUPDATEVISITOR_H 1

#include <CRPhysics\crExport.h>

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crNode.h>
#include <CRCore/crDB.h>
#include <CRCore/crObject.h>
#include <CRCore/crBillboard.h>
#include <CRCore/crLod.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crTransform.h>
#include <CRCore/crProjection.h>
//#include <CRCore/crImpostor.h>
//#include <CRCore/crOccluderNode.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crMatrix.h>

#include <CRPhysics/crExport.h>
#include <CRPhysics/crMatterObject.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crViewMatterObject.h>
//#include <CRTerrain/crTerrainTile.h>

#include <map>
#include <set>
#include <vector>
#include <stack>

namespace CRPhysics {

class CRPHYSICS_EXPORT crPhysicsUpdateVisitor : public CRCore::crNodeVisitor
{
    public:

        crPhysicsUpdateVisitor();

		virtual void apply(CRCore::crDB& node);

        virtual void apply(CRCore::crObject& node);
	    virtual void apply(CRCore::crBillboard& node);
        //
        //virtual void apply(CRCore::crLightSource& node);
        
		virtual void apply(CRCore::crGroup& node);

		//virtual void apply(CRCore::crOccluderNode& node);
		virtual void apply(CRCore::crSwitch& node);                   
		virtual void apply(CRCore::crSequence& node);
		//virtual void apply(CRCore::crLod& node);
		//virtual void apply(CRCore::crEventGroup& node);

  //      //virtual void apply(CRCore::crImpostor& node);

		//virtual void apply(CRCore::crClearNode& node);

		virtual void apply(CRCore::crTransform& node);

		virtual void apply(crMatterGroup& node);
		
		virtual void apply(crMatterObject& node);
		virtual void apply(crViewMatterObject& node);
		//virtual void apply(CRTerrain::crTerrainTile& node);

    protected:
		virtual ~crPhysicsUpdateVisitor();

        crPhysicsUpdateVisitor& operator = (const crPhysicsUpdateVisitor&) { return *this; }

		typedef std::vector<CRCore::ref_ptr<CRCore::RefMatrix> > MatrixList;
		inline void pushMatrix( CRCore::RefMatrix* matrix)
		{
			m_matrixList.push_back(matrix);
		}
		inline void popMatrix()
		{
			m_matrixList.pop_back();
		}

private:
	CRCore::ref_ptr<CRCore::crDB>           m_currentDB;

	MatrixList                              m_matrixList;

};

}

#endif

