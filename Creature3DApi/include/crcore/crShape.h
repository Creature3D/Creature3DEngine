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
#ifndef CRCORE_SHAPE
#define CRCORE_SHAPE 1

#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crQuat.h>
#include <CRCore/crPlane.h>
#include <CRCore/crArray.h>
#include <CRCore/crImage.h>

namespace CRCore {

// forward decare visitors.
class crShapeVisitor;
class crConstShapeVisitor;


/** META_StateAttribute macro define the standard clone, isSameKindAs,
  * className and getType methods.
  * Use when subclassing from crBase to make it more convinient to define 
  * the standard pure virtual methods which are required for all crBase 
  * subclasses.*/
#define META_Shape(library,name) \
        virtual CRCore::crBase* cloneType() const { return new name(); } \
        virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new name (*this,copyop); } \
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const name *>(obj)!=NULL; } \
        virtual const char* libraryName() const { return #library; } \
        virtual const char* className() const { return #name; } \
        virtual void accept(CRCore::crShapeVisitor& sv) { sv.apply(*this); } \
        virtual void accept(CRCore::crConstShapeVisitor& csv) const { csv.apply(*this); }
	
/** Base class for all shape types. 
  * Shapes are used to either for culling and collision detection or
  * to define the geometric shape of procedurally generate Geometry.
*/
class CR_EXPORT crShape : public crBase
{
    public:

        crShape() {}
        
        crShape(const crShape& sa,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
            crBase(sa,copyop) {}
        
        /** Clone the type of an attribute, with crBase* return type.
            Must be defined by derived classes.*/
        virtual crBase* cloneType() const = 0;

        /** Clone an attribute, with crBase* return type.
            Must be defined by derived classes.*/
        virtual crBase* clone(const crCopyOp&) const = 0;


        /** return true if this and obj are of the same kind of object.*/
        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crShape*>(obj)!=NULL; }

        /** return the name of the attribute's library.*/
        virtual const char* libraryName() const { return "CRCore"; }

        /** return the name of the attribute's class type.*/
        virtual const char* className() const { return "crShape"; }

    	/** accept a non const shape visitor which can be used on non const shape objects.
            Must be defined by derived classes.*/
    	virtual void accept(crShapeVisitor&)=0;
	
    	/** accept a const shape visitor which can be used on const shape objects.
            Must be defined by derived classes.*/
    	virtual void accept(crConstShapeVisitor&) const =0;

    protected:
    
    	virtual ~crShape();
};

// forward declartions of crShape types.
class crSphere;
class crBox;
class crCone;
class crCylinder;
class crCapsule;
class crInfinitePlane;

class crTriangleMesh;
class crConvexHull;
class crHeightField;

class crCompositeShape;

class CR_EXPORT crShapeVisitor
{
    public:
    
    	crShapeVisitor() {}
    
		virtual void apply(crShape&) {}
    	virtual void apply(crSphere&) {}
    	virtual void apply(crBox&) {}
    	virtual void apply(crCone&) {}
    	virtual void apply(crCylinder&) {}
    	virtual void apply(crCapsule&) {}
    	virtual void apply(crInfinitePlane&) {}

    	virtual void apply(crTriangleMesh&) {}
    	virtual void apply(crConvexHull&) {}
    	virtual void apply(crHeightField&) {}

    	virtual void apply(crCompositeShape&) {}
};

class CR_EXPORT crConstShapeVisitor
{
    public:
    
    	crConstShapeVisitor() {}

		virtual void apply(const crShape&) {}
    	virtual void apply(const crSphere&) {}
    	virtual void apply(const crBox&) {}
    	virtual void apply(const crCone&) {}
    	virtual void apply(const crCylinder&) {}
    	virtual void apply(const crCapsule&) {}
    	virtual void apply(const crInfinitePlane&) {}

    	virtual void apply(const crTriangleMesh&) {}
    	virtual void apply(const crConvexHull&) {}
    	virtual void apply(const crHeightField&) {}

    	virtual void apply(const crCompositeShape&) {}
};

class CR_EXPORT crSphere : public crShape
{
    public:
    	
        crSphere():
	    m_center(0.0f,0.0f,0.0f),
	    m_radius(1.0f) {}
	
        crSphere(const CRCore::crVector3& center,float radius):
	    m_center(center),
	    m_radius(radius) {}

        crSphere(const crSphere& sphere,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crShape(sphere,copyop),
	    m_center(sphere.m_center),
	    m_radius(sphere.m_radius) {}
        
    	META_Shape(CRCore, crSphere);
	
	inline bool valid() const { return m_radius>=0.0f; }
	
	inline void set(const crVector3& center,float radius)
	{
	    m_center = center;
	    m_radius = radius;
	}

	inline void setCenter(const crVector3& center) { m_center = center; }
	inline const crVector3& getCenter() const { return m_center; }
	
	inline void setRadius(float radius) { m_radius = radius; }
	inline float getRadius() const { return m_radius; }

    protected:
    
    	virtual ~crSphere() {}
    
    	crVector3    m_center;
	float	m_radius;
	
};

class CR_EXPORT crBox : public crShape
{
public:
    
    crBox():
	m_center(0.0f,0.0f,0.0f),
	m_halfLengths(0.5f,0.5f,0.5f) {}

    crBox(const CRCore::crVector3& center,float width):
	m_center(center),
	m_halfLengths(width*0.5f,width*0.5f,width*0.5f) {}

    crBox(const CRCore::crVector3& center,float lengthX,float lengthY, float lengthZ):
	m_center(center),
	m_halfLengths(lengthX*0.5f,lengthY*0.5f,lengthZ*0.5f) {}

    crBox(const crBox& box,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crShape(box,copyop),
	m_center(box.m_center),
	m_halfLengths(box.m_halfLengths),
    	m_rotation(box.m_rotation) {}
    
    META_Shape(CRCore, crBox);
	
	inline bool valid() const { return m_halfLengths.x()>=0.0f; }
	
	inline void set(const crVector3& center,const crVector3& halfLengths)
	{
	    m_center = center;
	    m_halfLengths = halfLengths;
	}

	inline void setCenter(const crVector3& center) { m_center = center; }
	inline const crVector3& getCenter() const { return m_center; }
	
	inline void setHalfLengths(const crVector3& halfLengths) { m_halfLengths = halfLengths; }
	inline const crVector3& getHalfLengths() const { return m_halfLengths; }
	
    inline void setRotation(const crQuat& quat) { m_rotation = quat; }
    inline const crQuat&  getRotation() const { return m_rotation; }
    inline crMatrix getRotationMatrix() const { return crMatrix(m_rotation); }
    inline bool zeroRotation() const { return m_rotation.zeroRotation(); } 

protected:

    virtual ~crBox() {}

    crVector3    m_center;
    crVector3    m_halfLengths;
    crQuat	m_rotation;	
	
};



class CR_EXPORT crCone : public crShape
{
public:
    
    crCone():
	m_center(0.0f,0.0f,0.0f),
	m_radius(1.0f),
	m_height(1.0f) {}

    crCone(const CRCore::crVector3& center,float radius,float height):
	m_center(center),
	m_radius(radius),
	m_height(height) {}

    crCone(const crCone& cone,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crShape(cone,copyop),
	m_center(cone.m_center),
	m_radius(cone.m_radius),
	m_height(cone.m_height),
	m_rotation(cone.m_rotation) {}
    
    META_Shape(CRCore, crCone);
	
	inline bool valid() const { return m_radius>=0.0f; }
	
	inline void set(const crVector3& center,float radius, float height)
	{
	    m_center = center;
	    m_radius = radius;
	    m_height = height;
	}

	inline void setCenter(const crVector3& center) { m_center = center; }
	inline const crVector3& getCenter() const { return m_center; }
	
	inline void setRadius(float radius) { m_radius = radius; }
	inline float getRadius() const { return m_radius; }

	inline void setHeight(float height) { m_height = height; }
	inline float getHeight() const { return m_height; }

    inline void setRotation(const crQuat& quat) { m_rotation = quat; }
    inline const crQuat& getRotation() const { return m_rotation; }
    inline crMatrix getRotationMatrix() const { return crMatrix(m_rotation); }
    inline bool zeroRotation() const { return m_rotation.zeroRotation(); } 
	
	inline float getBaseOffsetFactor() const { return 0.25f; } 
	inline float getBaseOffset() const { return -getBaseOffsetFactor()*getHeight(); } 

protected:
    
    virtual ~crCone() {}
    
    crVector3    m_center;
	float 	m_radius;
	float	m_height;
	
    crQuat	m_rotation;	
};

class CR_EXPORT crCylinder : public crShape
{
public:
    
    crCylinder():
	m_center(0.0f,0.0f,0.0f),
	m_radius(1.0f),
	m_height(1.0f) {}

    crCylinder(const CRCore::crVector3& center,float radius,float height):
	m_center(center),
	m_radius(radius),
	m_height(height) {}

    crCylinder(const crCylinder& cylinder,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crShape(cylinder,copyop),
	m_center(cylinder.m_center),
	m_radius(cylinder.m_radius),
	m_height(cylinder.m_height),
	m_rotation(cylinder.m_rotation) {}
    
    META_Shape(CRCore, crCylinder);
	
	inline bool valid() const { return m_radius>=0.0f; }
	
	inline void set(const crVector3& center,float radius, float height)
	{
	    m_center = center;
	    m_radius = radius;
	    m_height = height;
	}

	inline void setCenter(const crVector3& center) { m_center = center; }
	inline const crVector3& getCenter() const { return m_center; }
	
	inline void setRadius(float radius) { m_radius = radius; }
	inline float getRadius() const { return m_radius; }

	inline void setHeight(float height) { m_height = height; }
	inline float getHeight() const { return m_height; }

    inline void setRotation(const crQuat& quat) { m_rotation = quat; }
    inline const crQuat& getRotation() const { return m_rotation; }
    inline crMatrix getRotationMatrix() const { return crMatrix(m_rotation); }
    bool zeroRotation() const { return m_rotation.zeroRotation(); } 

 protected:
    
    virtual ~crCylinder() {}
    
    crVector3    m_center;
	float 	m_radius;
	float	m_height;
	
    crQuat	m_rotation;	
};

class CR_EXPORT crCapsule : public crShape
{
public:
    
    crCapsule():
	m_center(0.0f,0.0f,0.0f),
	m_radius(1.0f),
	m_height(1.0f) {}

    crCapsule(const CRCore::crVector3& center,float radius,float height):
	m_center(center),
	m_radius(radius),
	m_height(height) {}

    crCapsule(const crCapsule& capsule,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crShape(capsule,copyop),
	m_center(capsule.m_center),
	m_radius(capsule.m_radius),
	m_height(capsule.m_height),
	m_rotation(capsule.m_rotation) {}
    
    META_Shape(CRCore, crCapsule);
	
	inline bool valid() const { return m_radius>=0.0f; }
	
	inline void set(const crVector3& center,float radius, float height)
	{
	    m_center = center;
	    m_radius = radius;
	    m_height = height;
	}

	inline void setCenter(const crVector3& center) { m_center = center; }
	inline const crVector3& getCenter() const { return m_center; }
	
	inline void setRadius(float radius) { m_radius = radius; }
	inline float getRadius() const { return m_radius; }

	inline void setHeight(float height) { m_height = height; }
	inline float getHeight() const { return m_height; }

    inline void setRotation(const crQuat& quat) { m_rotation = quat; }
    inline const crQuat& getRotation() const { return m_rotation; }
    inline crMatrix getRotationMatrix() const { return crMatrix(m_rotation); }
    bool zeroRotation() const { return m_rotation.zeroRotation(); } 

protected:

    virtual ~crCapsule() {}

    crVector3    m_center;
	float 	m_radius;
	float	m_height;
	
    crQuat	m_rotation;	
};

class CR_EXPORT crInfinitePlane : public crShape, public crPlane
{
public:
    crInfinitePlane() {}

    crInfinitePlane(const crInfinitePlane& plane,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crShape(plane,copyop),
        crPlane(plane) {}

    META_Shape(CRCore, crInfinitePlane);
    
protected:
    
    virtual ~crInfinitePlane() {}
};

class CR_EXPORT crTriangleMesh : public crShape
{
public:

    crTriangleMesh() {}

    crTriangleMesh(const crTriangleMesh& mesh,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crShape(mesh,copyop),
	m_vertices(mesh.m_vertices),
	m_indices(mesh.m_indices) {}
    
    META_Shape(CRCore, crTriangleMesh);
	
	
	void setVertices(Vec3Array* vertices) { m_vertices = vertices; }
	Vec3Array* getVertices() { return m_vertices.get(); }
	const Vec3Array* getVertices() const { return m_vertices.get(); }
	
	
	void setIndices(IndexArray* indices) { m_indices = indices; }
	IndexArray* getIndices() { return m_indices.get(); }
	const IndexArray* getIndices() const { return m_indices.get(); }
	
protected:
    
    ~crTriangleMesh() {}
	
	ref_ptr<Vec3Array> m_vertices;
	ref_ptr<IndexArray> m_indices;
    	
};

class CR_EXPORT crConvexHull : public crTriangleMesh
{
public:

    crConvexHull() {}

    crConvexHull(const crConvexHull& hull,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crTriangleMesh(hull,copyop) {}
    
    META_Shape(CRCore, crTriangleMesh);

protected:

    ~crConvexHull() {}
};

class CR_EXPORT crHeightField : public crShape
{
public:

    crHeightField():
	    m_columns(0),
	    m_rows(0),
	    m_origin(0.0f,0.0f,0.0f),
	    m_dx(1.0f),
	    m_dy(1.0f),
        m_skirtHeight(0.0f),
        m_borderWidth(0),
		m_maxHeight(0.0f)
	{
		m_heights = new FloatArray;
		m_heightsForOde = new FloatArray;
	}

    crHeightField(const crHeightField& mesh,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crShape(mesh,copyop),
	    m_columns(mesh.m_columns),
	    m_rows(mesh.m_rows),
	    m_origin(mesh.m_origin),
	    m_dx(mesh.m_dx),
	    m_dy(mesh.m_dy),
        m_skirtHeight(mesh.m_skirtHeight),
        m_borderWidth(mesh.m_borderWidth),
        m_heights(new FloatArray(*mesh.m_heights)),
		m_heightsForOde(new FloatArray(*mesh.m_heightsForOde)),
		m_maxHeight(mesh.m_maxHeight),
		m_heightMap(mesh.m_heightMap){}
    
    META_Shape(CRCore, crHeightField)

    //typedef std::vector<float> HeightList;

    void allocate(unsigned int numColumns,unsigned int numRows);

// deprecated.
	inline unsigned int getNumColumns() const { return m_columns; }
	inline unsigned int getNumRows() const { return m_rows; }

    inline void setOrigin(const CRCore::crVector3& origin) { m_origin = origin; }
    inline const CRCore::crVector3& getOrigin() const { return m_origin; }

	inline void setXInterval(float dx) { m_dx = dx; }
	inline float getXInterval() const { return m_dx; }

	inline void setYInterval(float dy) { m_dy = dy; }
	inline float getYInterval() const { return m_dy; }

	/** Get the FloatArray height data.*/
	FloatArray* getFloatArray() { return m_heights.get(); }

	/** Get the const sFloatArray height data.*/
	const FloatArray* getFloatArray() const { return m_heights.get(); }

	/** Get the FloatArray height data.*/
	FloatArray* getOdeFloatArray() { return m_heightsForOde.get(); }
	/** Get the const sFloatArray height data.*/
	const FloatArray* getOdeFloatArray() const { return m_heightsForOde.get(); }

	//HeightList& getHeightList() { return m_heights->asVector(); }

	//const HeightList& getHeightList() const { return m_heights->asVector(); }


    /** Set the height of the skirt to render around the edge of crHeightField.
        * The skirt is used as a means of disguising edge boundaries between adjacent crHeightField, particular
        * of ones with different resolutions.*/
    void setSkirtHeight(float skirtHeight) { m_skirtHeight = skirtHeight; }
    
    /** Get the height of the skirt to render around the edge of crHeightField.*/
    float getSkirtHeight() const { return m_skirtHeight; }
    
    /** Set the width in number of cells in from the edge that the height field should be rendered from.
        * This exists to allow gradient and curvature continutity to be maintained between adjacent crHeightField, where
        * the border cells will overlap adjacent crHeightField.*/
    void setBorderWidth(unsigned int borderWidth) { m_borderWidth = borderWidth; }

    /** Get the width in number of cells in from the edge that the height field should be rendered from.*/
    unsigned int getBorderWidth() const { return m_borderWidth; }

    inline void setRotation(const crQuat& quat) { m_rotation = quat; }
    inline const crQuat& getRotation() const { return m_rotation; }
    inline crMatrix getRotationMatrix() const { return crMatrix(m_rotation); }
    inline bool zeroRotation() const { return m_rotation.zeroRotation(); } 
        

	inline void setHeight(unsigned int c,unsigned int r,float value)
	{
	   (*m_heights)[c+r*m_columns] = value;
       (*m_heightsForOde)[c+(m_rows - r - 1)*m_columns] = value;
	}

	inline float& getHeight(unsigned int c,unsigned int r)
	{
	   return (*m_heights)[c+r*m_columns];
	}

	inline float getHeight(unsigned int c,unsigned int r) const
	{
	   return (*m_heights)[c+r*m_columns];
	}

    inline crVector3 getVertex(unsigned int c,unsigned int r) const
    {
        return crVector3(m_origin.x()+getXInterval()*(float)c,
                        m_origin.y()+getYInterval()*(float)r,
                        m_origin.z()+(*m_heights)[c+r*m_columns]);  
    }
        
    crVector3 getNormal(unsigned int c,unsigned int r) const;

	crVector2 getHeightDelta(unsigned int c,unsigned int r) const;

	void buildFromHeightMap(crImage *heightimage,float maxHeight);
	const std::string &getHeightMap() const;
	float getMaxHeight() const;
	crImage *getHeightImage();
	void setHeightImage(crImage *image);
	void setHeightMapName(const std::string &name);
	void allocateColor();
	inline bool hasColor()const{ return m_colors.valid(); }
	void setColor(unsigned int c,unsigned int r,const crVector4f &color)
	{
		(*m_colors)[c+r*m_columns] = color;
	}
	crVector4f& getColor(unsigned int c,unsigned int r)
	{
		return (*m_colors)[c+r*m_columns];
	}
	const crVector4f& getColor(unsigned int c,unsigned int r)const
	{
		return (*m_colors)[c+r*m_columns];
	}
protected:
    
    virtual ~crHeightField();

    unsigned int    m_columns,m_rows;

	CRCore::crVector3       m_origin;
	float	        m_dx;
	float	        m_dy;
	
    float           m_skirtHeight;
    unsigned int    m_borderWidth;

	crQuat            m_rotation;
	//HeightList      m_heights;
	CRCore::ref_ptr<CRCore::FloatArray>   m_heights;
    CRCore::ref_ptr<CRCore::FloatArray>   m_heightsForOde;
	CRCore::ref_ptr<CRCore::Vec4Array>    m_colors;

	float m_maxHeight;
	std::string m_heightMap;
	ref_ptr<crImage> m_image;
};

typedef crHeightField Grid;


class CR_EXPORT crCompositeShape : public crShape
{
public:


    typedef std::vector< ref_ptr<crShape> > ChildList;

    crCompositeShape() {}

    crCompositeShape(const crCompositeShape& cs,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
        crShape(cs,copyop),
	    m_children(cs.m_children) {}
    
    META_Shape(CRCore, crCompositeShape);

    	/** Set the shape that encloses all of the children.*/	
	void setShape(crShape* shape) { m_shape = shape; }

    	/** Get the shape that encloses all of the children.*/	
	crShape* getShape() { return m_shape.get(); }

    	/** Get the const shape that encloses all of the children.*/	
	const crShape* getShape() const { return m_shape.get(); }
	
	/** Get the number of children of this composite shape.*/
	unsigned int getNumChildren() const { return m_children.size(); }
	
	/** Get a child.*/
	crShape* getChild(unsigned int i) { return m_children[i].get(); }

	/** Get a const child.*/
	const crShape* getChild(unsigned int i) const { return m_children[i].get(); }

	/** Add a child to the list.*/
	void addChild(crShape* shape) { m_children.push_back(shape); }
	
	/** remove a child from the list.*/
	void removeChild(unsigned int i) { m_children.erase(m_children.begin()+i); }

	/** find the index number of child, if child is not found then it returns getNumChildren(),
	  * so should be used in similar sytle of STL's result!=end().*/ 
	unsigned int findChildNo(crShape* shape) const
    { 
        for (unsigned int childNo=0;childNo<m_children.size();++childNo)
        {
            if (m_children[childNo]==shape) return childNo;
        }
        return m_children.size(); // node not found.

    }
	
protected:

    ~crCompositeShape() {}

    ref_ptr<crShape>	m_shape;
	ChildList   	m_children;

};

}

#endif
