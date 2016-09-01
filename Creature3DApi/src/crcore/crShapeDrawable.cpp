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
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

///////////////////////////////////////////////////////////////////////////////
//
// Compute bounding of shape
//

class ComputeBoundShapeVisitor : public crConstShapeVisitor
{
    public:
    
    	ComputeBoundShapeVisitor(crBoundingBox& bb):m_bb(bb) {}
    
    	virtual void apply(const crSphere&);
    	virtual void apply(const crBox&);
    	virtual void apply(const crCone&);
    	virtual void apply(const crCylinder&);
    	virtual void apply(const crCapsule&);
    	virtual void apply(const crInfinitePlane&);

    	virtual void apply(const crTriangleMesh&);
    	virtual void apply(const crConvexHull&);
    	virtual void apply(const crHeightField&);

    	virtual void apply(const crCompositeShape&);
	
	    crBoundingBox&	m_bb;
};


void ComputeBoundShapeVisitor::apply(const crSphere& sphere)
{
    crVector3 halfLengths(sphere.getRadius(),sphere.getRadius(),sphere.getRadius());
    m_bb.expandBy(sphere.getCenter()-halfLengths);
    m_bb.expandBy(sphere.getCenter()+halfLengths);
}

void ComputeBoundShapeVisitor::apply(const crBox& box)
{
    if (box.zeroRotation())
    {
    	m_bb.expandBy(box.getCenter()-box.getHalfLengths());
	    m_bb.expandBy(box.getCenter()+box.getHalfLengths());
    }
    else
    {
    	float x = box.getHalfLengths().x();
    	float y = box.getHalfLengths().y();
    	float z = box.getHalfLengths().z();

    	crVector3 base_1(crVector3(-x,-y,-z));
    	crVector3 base_2(crVector3(x,-y,-z));
    	crVector3 base_3(crVector3(x,y,-z));
    	crVector3 base_4(crVector3(-x,y,-z));
    
    	crVector3 top_1(crVector3(-x,-y,z));
    	crVector3 top_2(crVector3(x,-y,z));
    	crVector3 top_3(crVector3(x,y,z));
    	crVector3 top_4(crVector3(-x,y,z));

        crMatrix matrix = box.getRotationMatrix();
    	m_bb.expandBy(box.getCenter()+base_1*matrix);
    	m_bb.expandBy(box.getCenter()+base_2*matrix);
    	m_bb.expandBy(box.getCenter()+base_3*matrix);
    	m_bb.expandBy(box.getCenter()+base_4*matrix);

    	m_bb.expandBy(box.getCenter()+top_1*matrix);
    	m_bb.expandBy(box.getCenter()+top_2*matrix);
    	m_bb.expandBy(box.getCenter()+top_3*matrix);
    	m_bb.expandBy(box.getCenter()+top_4*matrix);
    }
}

void ComputeBoundShapeVisitor::apply(const crCone& cone)
{
    if (cone.zeroRotation())
    {
    	m_bb.expandBy(cone.getCenter()+crVector3(-cone.getRadius(),-cone.getRadius(),cone.getBaseOffset()));
    	m_bb.expandBy(cone.getCenter()+crVector3(cone.getRadius(),cone.getRadius(),cone.getHeight()+cone.getBaseOffset()));
	
    }
    else
    {
    	crVector3 top(crVector3(cone.getRadius(),cone.getRadius(),cone.getHeight()+cone.getBaseOffset()));
    	crVector3 base_1(crVector3(-cone.getRadius(),-cone.getRadius(),cone.getBaseOffset()));
    	crVector3 base_2(crVector3(cone.getRadius(),-cone.getRadius(),cone.getBaseOffset()));
    	crVector3 base_3(crVector3(cone.getRadius(),cone.getRadius(),cone.getBaseOffset()));
    	crVector3 base_4(crVector3(-cone.getRadius(),cone.getRadius(),cone.getBaseOffset()));
    
        crMatrix matrix = cone.getRotationMatrix();
    	m_bb.expandBy(cone.getCenter()+base_1*matrix);
    	m_bb.expandBy(cone.getCenter()+base_2*matrix);
    	m_bb.expandBy(cone.getCenter()+base_3*matrix);
    	m_bb.expandBy(cone.getCenter()+base_4*matrix);
	    m_bb.expandBy(cone.getCenter()+top*matrix);
    }
}

void ComputeBoundShapeVisitor::apply(const crCylinder& cylinder)
{
    if (cylinder.zeroRotation())
    {
        crVector3 halfLengths(cylinder.getRadius(),cylinder.getRadius(),cylinder.getHeight()*0.5f);
    	m_bb.expandBy(cylinder.getCenter()-halfLengths);
	    m_bb.expandBy(cylinder.getCenter()+halfLengths);

    }
    else
    {
    	float r = cylinder.getRadius();
    	float z = cylinder.getHeight()*0.5f;

    	crVector3 base_1(crVector3(-r,-r,-z));
    	crVector3 base_2(crVector3(r,-r,-z));
    	crVector3 base_3(crVector3(r,r,-z));
    	crVector3 base_4(crVector3(-r,r,-z));
    
    	crVector3 top_1(crVector3(-r,-r,z));
    	crVector3 top_2(crVector3(r,-r,z));
    	crVector3 top_3(crVector3(r,r,z));
    	crVector3 top_4(crVector3(-r,r,z));

        crMatrix matrix = cylinder.getRotationMatrix();
    	m_bb.expandBy(cylinder.getCenter()+base_1*matrix);
    	m_bb.expandBy(cylinder.getCenter()+base_2*matrix);
    	m_bb.expandBy(cylinder.getCenter()+base_3*matrix);
    	m_bb.expandBy(cylinder.getCenter()+base_4*matrix);

    	m_bb.expandBy(cylinder.getCenter()+top_1*matrix);
    	m_bb.expandBy(cylinder.getCenter()+top_2*matrix);
    	m_bb.expandBy(cylinder.getCenter()+top_3*matrix);
    	m_bb.expandBy(cylinder.getCenter()+top_4*matrix);
    }
}

void ComputeBoundShapeVisitor::apply(const crCapsule& capsule)
{
    if (capsule.zeroRotation())
    {
        crVector3 halfLengths(capsule.getRadius(),capsule.getRadius(),capsule.getHeight()*0.5f + capsule.getRadius());
    	m_bb.expandBy(capsule.getCenter()-halfLengths);
	    m_bb.expandBy(capsule.getCenter()+halfLengths);

    }
    else
    {
    	float r = capsule.getRadius();
    	float z = capsule.getHeight()*0.5f + capsule.getRadius();

    	crVector3 base_1(crVector3(-r,-r,-z));
    	crVector3 base_2(crVector3(r,-r,-z));
    	crVector3 base_3(crVector3(r,r,-z));
    	crVector3 base_4(crVector3(-r,r,-z));
    
    	crVector3 top_1(crVector3(-r,-r,z));
    	crVector3 top_2(crVector3(r,-r,z));
    	crVector3 top_3(crVector3(r,r,z));
    	crVector3 top_4(crVector3(-r,r,z));

        crMatrix matrix = capsule.getRotationMatrix();
    	m_bb.expandBy(capsule.getCenter()+base_1*matrix);
    	m_bb.expandBy(capsule.getCenter()+base_2*matrix);
    	m_bb.expandBy(capsule.getCenter()+base_3*matrix);
    	m_bb.expandBy(capsule.getCenter()+base_4*matrix);

    	m_bb.expandBy(capsule.getCenter()+top_1*matrix);
    	m_bb.expandBy(capsule.getCenter()+top_2*matrix);
    	m_bb.expandBy(capsule.getCenter()+top_3*matrix);
    	m_bb.expandBy(capsule.getCenter()+top_4*matrix);
    }
}

void ComputeBoundShapeVisitor::apply(const crInfinitePlane&)
{
    // can't compute the bounding box of an infinite plane!!! :-)
}

void ComputeBoundShapeVisitor::apply(const crTriangleMesh& mesh)
{
    const Vec3Array* vertices = mesh.getVertices();
    const IndexArray* indices = mesh.getIndices();
    
     if (vertices && indices)
     {
	    for(unsigned int i=0;i<indices->getNumElements();++i)
		{
    	    const CRCore::crVector3& v=(*vertices)[indices->index(i)];
    	    m_bb.expandBy(v);
		}
     }
}

void ComputeBoundShapeVisitor::apply(const crConvexHull& hull)
{
    apply((const crTriangleMesh&)hull);
}

void ComputeBoundShapeVisitor::apply(const crHeightField& field)
{
    float zMin=FLT_MAX;
    float zMax=-FLT_MAX;

    for(unsigned int row=0;row<field.getNumRows();++row)
    {
	for(unsigned int col=0;col<field.getNumColumns();++col)
	{
    	float z = field.getHeight(col,row);
	    if (z<zMin) zMin = z;
	    if (z>zMax) zMax = z;
	}
    }

    if (zMin>zMax)
    {
        // no valid entries so don't reset the bounding box
        return;
    }

	    
    if (field.zeroRotation())
    {
	    m_bb.expandBy(field.getOrigin()+CRCore::crVector3(0.0f,0.0f,zMin));
	    m_bb.expandBy(field.getOrigin()+CRCore::crVector3(field.getXInterval()*(field.getNumColumns()-1),field.getYInterval()*(field.getNumRows()-1),zMax));
    }
    else
    {
    	float x = field.getXInterval()*(field.getNumColumns()-1);
    	float y = field.getYInterval()*(field.getNumRows()-1);

    	crVector3 base_1(crVector3(0,0,zMin));
    	crVector3 base_2(crVector3(x,0,zMin));
    	crVector3 base_3(crVector3(x,y,zMin));
    	crVector3 base_4(crVector3(0,y,zMin));

    	crVector3 top_1(crVector3(0,0,zMax));
    	crVector3 top_2(crVector3(x,0,zMax));
    	crVector3 top_3(crVector3(x,y,zMax));
    	crVector3 top_4(crVector3(0,y,zMax));

        crMatrix matrix = field.getRotationMatrix();
    	m_bb.expandBy(field.getOrigin()+base_1*matrix);
    	m_bb.expandBy(field.getOrigin()+base_2*matrix);
    	m_bb.expandBy(field.getOrigin()+base_3*matrix);
    	m_bb.expandBy(field.getOrigin()+base_4*matrix);

    	m_bb.expandBy(field.getOrigin()+top_1*matrix);
    	m_bb.expandBy(field.getOrigin()+top_2*matrix);
    	m_bb.expandBy(field.getOrigin()+top_3*matrix);
    	m_bb.expandBy(field.getOrigin()+top_4*matrix);
    }

}

void ComputeBoundShapeVisitor::apply(const crCompositeShape& group)
{
    for(unsigned int i=0;i<group.getNumChildren();++i)
    {
	group.getChild(i)->accept(*this);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Accept a primtive functor for each of the shapes.
//

class PrimitiveShapeVisitor : public crConstShapeVisitor
{
    public:
    
    	PrimitiveShapeVisitor(crDrawable::PrimitiveFunctor& functor,const crTessellationHints* hints):
            m_functor(functor),
	        m_hints(hints) {}
    
    	virtual void apply(const crSphere&);
    	virtual void apply(const crBox&);
    	virtual void apply(const crCone&);
    	virtual void apply(const crCylinder&);
    	virtual void apply(const crCapsule&);
    	virtual void apply(const crInfinitePlane&);

    	virtual void apply(const crTriangleMesh&);
    	virtual void apply(const crConvexHull&);
    	virtual void apply(const crHeightField&);

    	virtual void apply(const crCompositeShape&);
	
        crDrawable::PrimitiveFunctor& m_functor;
	    const crTessellationHints*  m_hints;
};


void PrimitiveShapeVisitor::apply(const crSphere& sphere)
{
	
    float tx = sphere.getCenter().x();
    float ty = sphere.getCenter().y();
    float tz = sphere.getCenter().z();

    unsigned int numSegments = 40;
    unsigned int numRows = 20;

    float lDelta = CRCore::PI/(float)numRows;
    float vDelta = 1.0f/(float)numRows;

    float angleDelta = CRCore::PI*2.0f/(float)numSegments;
    float texCoordHorzDelta = 1.0f/(float)numSegments;

    float lBase=-CRCore::PI*0.5f;
    float rBase=0.0f;
    float zBase=-sphere.getRadius();
    float vBase=0.0f;

    for(unsigned int rowi=0;
	    rowi<numRows;
	    ++rowi)
    {

	    float lTop = lBase+lDelta;
	    float rTop = cosf(lTop)*sphere.getRadius();
	    float zTop = sinf(lTop)*sphere.getRadius();
	    float vTop = vBase+vDelta;
    	//float nzTop= sinf(lTop);
    	//float nRatioTop= cosf(lTop);

		m_functor.begin(crPrimitive::PT_QUAD_STRIP);

    	float angle = 0.0f;
    	float texCoord = 0.0f;

    	for(unsigned int topi=0;
		    topi<numSegments;
		    ++topi,angle+=angleDelta,texCoord+=texCoordHorzDelta)
    	{

    		float c = cosf(angle);
    		float s = sinf(angle);

		    m_functor.vertex(tx+c*rTop,ty+s*rTop,tz+zTop);
		    m_functor.vertex(tx+c*rBase,ty+s*rBase,tz+zBase);

    	}

    	// do last point by hand to ensure no round off errors.
	    m_functor.vertex(tx+rTop,ty,tz+zTop);
	    m_functor.vertex(ty+rBase,ty,tz+zBase);

	    m_functor.end();
 
	    lBase=lTop;
	    rBase=rTop;
	    zBase=zTop;
	    vBase=vTop;

    }
}

void PrimitiveShapeVisitor::apply(const crBox& box)
{
    float x = box.getHalfLengths().x();
    float y = box.getHalfLengths().y();
    float z = box.getHalfLengths().z();

    crVector3 base_1(-x,-y,-z);
    crVector3 base_2(x,-y,-z);
    crVector3 base_3(x,y,-z);
    crVector3 base_4(-x,y,-z);

    crVector3 top_1(-x,-y,z);
    crVector3 top_2(x,-y,z);
    crVector3 top_3(x,y,z);
    crVector3 top_4(-x,y,z);

    if (box.zeroRotation())
    {
        base_1 += box.getCenter();
        base_2 += box.getCenter();
        base_3 += box.getCenter();
        base_4 += box.getCenter();

        top_1 += box.getCenter();
        top_2 += box.getCenter();
        top_3 += box.getCenter();
        top_4 += box.getCenter();
    }
    else
    {
        crMatrix matrix = box.getRotationMatrix();
        matrix.setTrans(box.getCenter());
        
    	base_1 = base_1*matrix;
    	base_2 = base_2*matrix;
    	base_3 = base_3*matrix;
    	base_4 = base_4*matrix;

    	top_1 = top_1*matrix;
    	top_2 = top_2*matrix;
    	top_3 = top_3*matrix;
    	top_4 = top_4*matrix;
    }

    m_functor.begin(GL_QUADS);
    
    m_functor.vertex(top_1);
    m_functor.vertex(base_1);
    m_functor.vertex(base_2);
    m_functor.vertex(top_2);

    m_functor.vertex(top_2);
    m_functor.vertex(base_2);
    m_functor.vertex(base_3);
    m_functor.vertex(top_3);

    m_functor.vertex(top_3);
    m_functor.vertex(base_3);
    m_functor.vertex(base_4);
    m_functor.vertex(top_4);

    m_functor.vertex(top_4);
    m_functor.vertex(base_4);
    m_functor.vertex(base_1);
    m_functor.vertex(top_1);

    m_functor.vertex(top_4);
    m_functor.vertex(top_1);
    m_functor.vertex(top_2);
    m_functor.vertex(top_3);

    m_functor.vertex(base_4);
    m_functor.vertex(base_3);
    m_functor.vertex(base_2);
    m_functor.vertex(base_1);

    m_functor.end();

}

void PrimitiveShapeVisitor::apply(const crCone& cone)
{
    crMatrix matrix = cone.getRotationMatrix();
    matrix.setTrans(cone.getCenter());


    unsigned int numSegments = 40;
    unsigned int numRows = 10;

    float r = cone.getRadius();
    float h = cone.getHeight();

    float normalz = r/(sqrtf(r*r+h*h));
    float normalRatio = 1.0f/(sqrtf(1.0f+normalz*normalz));
    normalz *= normalRatio;

    float angleDelta = 2.0f*CRCore::PI/(float)numSegments;
    float texCoordHorzDelta = 1.0/(float)numSegments;
    float texCoordRowDelta = 1.0/(float)numRows;
    float hDelta = 	cone.getHeight()/(float)numRows;
    float rDelta = 	cone.getRadius()/(float)numRows;

    float topz=cone.getHeight()+cone.getBaseOffset();
    float topr=0.0f;
    float topv=1.0f;
    float basez=topz-hDelta;
    float baser=rDelta;
    float basev=topv-texCoordRowDelta;
    float angle;
    float texCoord;

    for(unsigned int rowi=0;
	    rowi<numRows;
	    ++rowi,topz=basez, basez-=hDelta, topr=baser, baser+=rDelta, topv=basev, basev-=texCoordRowDelta)
    {
    	// we can't use a fan for the cone top
	// since we need different normals at the top
	// for each face..
	    m_functor.begin(crPrimitive::PT_QUAD_STRIP);

    	angle = 0.0f;
    	texCoord = 0.0f;
    	for(unsigned int topi=0;
		    topi<numSegments;
		    ++topi,angle+=angleDelta,texCoord+=texCoordHorzDelta)
    	{

    		float c = cosf(angle);
    		float s = sinf(angle);

		    m_functor.vertex(crVector3(c*topr,s*topr,topz)*matrix);
		    m_functor.vertex(crVector3(c*baser,s*baser,basez)*matrix);

    	}

    	// do last point by hand to ensure no round off errors.
	    m_functor.vertex(crVector3(topr,0.0f,topz)*matrix);
	    m_functor.vertex(crVector3(baser,0.0f,basez)*matrix);

        m_functor.end();

    }

    // we can't use a fan for the cone top
    // since we need different normals at the top
    // for each face..
    m_functor.begin(crPrimitive::PT_TRIANGLE_FAN);

    angle = CRCore::PI*2.0f;
    texCoord = 1.0f;
    basez = cone.getBaseOffset();

	m_functor.vertex(crVector3(0.0f,0.0f,basez)*matrix);

    for(unsigned int bottomi=0;
	bottomi<numSegments;
	++bottomi,angle-=angleDelta,texCoord-=texCoordHorzDelta)
    {
    	float c = cosf(angle);
    	float s = sinf(angle);

	    m_functor.vertex(crVector3(c*r,s*r,basez)*matrix);
    }

	m_functor.vertex(crVector3(r,0.0f,basez)*matrix);
	
    m_functor.end();
}

void PrimitiveShapeVisitor::apply(const crCylinder& cylinder)
{
    crMatrix matrix = cylinder.getRotationMatrix();
    matrix.setTrans(cylinder.getCenter());

    unsigned int numSegments = 40;

    float angleDelta = 2.0f*CRCore::PI/(float)numSegments;

    float texCoordDelta = 1.0/(float)numSegments;

    float r = cylinder.getRadius();
    float h = cylinder.getHeight();

    float basez = -h*0.5f;
    float topz = h*0.5f;

    // cylinder body
    m_functor.begin(crPrimitive::PT_QUAD_STRIP);

    float angle = 0.0f;
    float texCoord = 0.0f;
    for(unsigned int bodyi=0;
	    bodyi<numSegments;
	    ++bodyi,angle+=angleDelta,texCoord+=texCoordDelta)
    {

    	float c = cosf(angle);
    	float s = sinf(angle);

	    m_functor.vertex(crVector3(c*r,s*r,topz)*matrix);
	    m_functor.vertex(crVector3(c*r,s*r,basez)*matrix);

    }

    	// do last point by hand to ensure no round off errors.
	m_functor.vertex(crVector3(r,0.0f,topz)*matrix);
	m_functor.vertex(crVector3(r,0.0f,basez)*matrix);

    m_functor.end();


    // cylinder top
    m_functor.begin(crPrimitive::PT_TRIANGLE_FAN);

	m_functor.vertex(crVector3(0.0f,0.0f,topz)*matrix);

    angle = 0.0f;
    texCoord = 0.0f;
    for(unsigned int topi=0;
	    topi<numSegments;
	    ++topi,angle+=angleDelta,texCoord+=texCoordDelta)
    {

    	float c = cosf(angle);
    	float s = sinf(angle);

	    m_functor.vertex(crVector3(c*r,s*r,topz)*matrix);

    }

	m_functor.vertex(crVector3(r,0.0f,topz)*matrix);

    m_functor.end();

    // cylinder bottom
    m_functor.begin(crPrimitive::PT_TRIANGLE_FAN);

	m_functor.vertex(crVector3(0.0f,0.0f,basez)*matrix);

    angle = CRCore::PI*2.0f;
    texCoord = 1.0f;
    for(unsigned int bottomi=0;
	   bottomi<numSegments;
	   ++bottomi,angle-=angleDelta,texCoord-=texCoordDelta)
    {

    	float c = cosf(angle);
    	float s = sinf(angle);

	    m_functor.vertex(crVector3(c*r,s*r,basez)*matrix);

    }

	m_functor.vertex(crVector3(r,0.0f,basez)*matrix);

    m_functor.end();
}

void PrimitiveShapeVisitor::apply(const crCapsule& /*capsule*/)
{
#if 0
     notify(NOTICE)<<"Warning: PrimitiveShapeVisitor doesn't implement apply(crCapsule&) (yet)."<<std::endl;
#endif   
}

void PrimitiveShapeVisitor::apply(const crInfinitePlane&)
{
    notify(NOTICE)<<"Warning: PrimitiveShapeVisitor::apply(const crInfinitePlane& plane) not yet implementated. "<<std::endl;
}

void PrimitiveShapeVisitor::apply(const crTriangleMesh& mesh)
{
    const Vec3Array* vertices = mesh.getVertices();
    const IndexArray* indices = mesh.getIndices();
    
     if (vertices && indices)
     {
	m_functor.begin(crPrimitive::PT_TRIANGLES);

	for(unsigned int i=0;i<indices->getNumElements();i+=3)
	{
    	    m_functor.vertex((*vertices)[indices->index(i)]);
    	    m_functor.vertex((*vertices)[indices->index(i+1)]);
    	    m_functor.vertex((*vertices)[indices->index(i+2)]);
	}

        m_functor.end();
     }
}

void PrimitiveShapeVisitor::apply(const crConvexHull& hull)
{
    apply((const crTriangleMesh&)hull);
}

void PrimitiveShapeVisitor::apply(const crHeightField& field)
{
    if (field.getNumColumns()==0 || field.getNumRows()==0) return;
    
    crMatrix matrix = field.getRotationMatrix();
    matrix.setTrans(field.getOrigin());

    float dx = field.getXInterval();
    float dy = field.getYInterval();

    for(unsigned int row=0;row<field.getNumRows()-1;++row)
    {

    	m_functor.begin(crPrimitive::PT_QUAD_STRIP);

	for(unsigned int col=0;col<field.getNumColumns();++col)
	{
    	    crVector3 vertTop(dx*(float)col,dy*(float)row+dy,field.getHeight(col,row+1));
    	    crVector3 vertBase(dx*(float)col,dy*(float)row,field.getHeight(col,row));

	    m_functor.vertex(vertTop*matrix);
	    m_functor.vertex(vertBase*matrix);

	}

	m_functor.end();
    }

}

void PrimitiveShapeVisitor::apply(const crCompositeShape& group)
{
    for(unsigned int i=0;i<group.getNumChildren();++i)
    {
	    group.getChild(i)->accept(*this);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// crShapeDrawable itself..
//


crShapeDrawable::crShapeDrawable()
{
	m_type = SHAPEDRAWABLE;
}

crShapeDrawable::crShapeDrawable(crShape* shape,crTessellationHints* hints):
    m_tessellationHints(hints),
	m_shape(shape)
{
	m_type = SHAPEDRAWABLE;
}

crShapeDrawable::crShapeDrawable(const crShapeDrawable& pg,const crCopyOp& copyop):
    crDrawable(pg,copyop),
    m_tessellationHints(pg.m_tessellationHints),
	m_shape(pg.m_shape)
{

}

crShapeDrawable::~crShapeDrawable()
{
}

void crShapeDrawable::accept(ConstAttributeFunctor&) const
{
}

void crShapeDrawable::accept(PrimitiveFunctor& pf) const
{
    if (m_shape.valid())
    {
        PrimitiveShapeVisitor psv(pf,m_tessellationHints.get());
	    m_shape->accept(psv);
    }
}

bool crShapeDrawable::computeBound() const
{
    m_bbox.init();

    if (m_shape.valid())
    {
        ComputeBoundShapeVisitor cbsv(m_bbox);
    	m_shape->accept(cbsv);
        m_bbox_computed = true;

	    return true;
    }

    return false;
}

#include <Driver/GLDrv/crShapeDrawableDrv.h>