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

#ifndef CRCORE_CRBILLBOARD_H
#define CRCORE_CRBILLBOARD_H 1

#include <CRCore/crMatrix.h>
#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crNotify.h>

#include <vector>

namespace CRCore {

struct ApplyMatrix : public crDrawable::PrimitiveFunctor, public Referenced
{	
	ApplyMatrix():
        m_vertices(0), 
	    m_outvertices(0),
	    m_offset(0){}
	
	virtual void setVertexArray(unsigned int,const crVector2*) 
	{
		notify(WARN)<<"ComputeBound does not support Vec2* vertex arrays"<<std::endl;
	}

	virtual void setVertexArray(unsigned int, const crVector3* vertices) { m_vertices = vertices; }

	virtual void setVertexArray(unsigned int, const crVector4*) 
	{
		notify(WARN)<<"ComputeBound does not support crVector4* vertex arrays"<<std::endl;
	}

	virtual void drawArrays(unsigned int,int first,int count)
	{
		if (m_vertices)
		{
			for(int i = 0; count>0; --count,++i)
			{
				(*m_outvertices)[first+m_offset+i] = m_vertices[first] * m_mat;
			}
		}
	}

	virtual void drawElements(unsigned int,int count,const unsigned char* indices)
	{
		if (m_vertices)
		{
			for(;count>0;--count,++indices)
			{
				(*m_outvertices)[(*indices)+m_offset] = m_vertices[*indices] * m_mat;
			}
		}
	}

	virtual void drawElements(unsigned int,int count,const unsigned short* indices)
	{
		if (m_vertices)
		{
			for(;count>0;--count,++indices)
			{
				(*m_outvertices)[(*indices)+m_offset] = m_vertices[*indices] * m_mat;
			}
		}
	}

	virtual void drawElements(unsigned int,int count,const unsigned int* indices)
	{
		if (m_vertices)
		{
			for(;count>0;--count,++indices)
			{
			    (*m_outvertices)[(*indices)+m_offset] = m_vertices[*indices] * m_mat;
			}
		}
	}

	const crVector3      *m_vertices;
	Vec3Array            *m_outvertices;
	crMatrix             m_mat;
	int                  m_offset;
};

/** crBillboard - a crObject which orientates its child CRCore::crDrawable's to face
    the eye point.  Typical uses are for trees, or particle explosions.
*/
class CR_EXPORT crBillboard : public crObject
{
    public:

        enum Mode 
		{
            POINT_ROT_EYE,
            POINT_ROT_WORLD,
            AXIAL_ROT
        };

        crBillboard();
        
        crBillboard(const crBillboard&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crBillboard);

        void setMode(Mode mode);
        inline Mode getMode() const { return m_mode; }

        /** Set the axis about which all the billboard's drawable rotate. Only utlized when mode==AXIAL_ROT*/
        void setAxis(const crVector3& axis);
        inline const crVector3& getAxis() const { return m_axis; }

        void setNormal(const crVector3& normal);
        inline const crVector3& getNormal() const { return m_normal; }

        inline void setPos(unsigned int i,const crVector3& pos)      { m_positionList[i] = pos; dirtyBound(); }
        inline const crVector3& getPos(unsigned int i) const         { return m_positionList[i]; }
        
        typedef std::vector<crVector3> PositionList;
        inline PositionList& getPositionList()             { return m_positionList; }        
        inline const PositionList& getPositionList() const { return m_positionList; }

        virtual bool addBBDrawable( crDrawable *gset );
        virtual bool addBBDrawable(crDrawable *gset,const crVector3& pos);

        virtual bool removeDrawable( crDrawable *gset );
                

        bool computeMatrix(crMatrix& modelview, const crVector3& eye_local, int i);


		crDrawable *getComposeDrawable(){ return m_composeGeo.get(); }
		const crDrawable *getComposeDrawable()const{ return m_composeGeo.get(); }
        virtual void init();

		bool expand(const crBillboard &billborad);

		virtual void swapBuffers(int frameNumber);
		virtual void releaseObjects(crState* = 0);
    protected:        

        virtual ~crBillboard();

        virtual bool computeBound() const;
        crBillboard *getOrCreateBillboard(crStateSet *ss);

        enum AxisAligned
        {
            AXIAL_ROT_X_AXIS=AXIAL_ROT+1,
            AXIAL_ROT_Y_AXIS,
            AXIAL_ROT_Z_AXIS,
			POINT_ROT_WORLD_Z_AXIS,
            CACHE_DIRTY
        };


        Mode                                m_mode;
        crVector3                           m_axis;
        crVector3                           m_normal;
        PositionList                        m_positionList;
        
        // used internally as cache of which what m_axis is aligned to help
        // decide which method of rotation to use.
        int                                 m_cachedMode;
        crVector3                           m_side;
        void updateCache();
        ref_ptr<crGeometry>                 m_composeGeo;
		typedef std::vector<int> IndexOffsetList;
        IndexOffsetList                     m_indexOffsetList;

		ref_ptr<ApplyMatrix>                m_applyMatrix;

		bool                                m_init;
		ref_ptr<Vec3Array>                  m_vertices;
		ref_ptr<Vec3Array>                  m_vertices_buf;

		typedef std::vector< ref_ptr<crBillboard> > NewBBVec;
		static NewBBVec                            m_newbbvec;
};

}

#endif
