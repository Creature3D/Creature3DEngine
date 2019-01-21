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
//Modified by Wucaihua
#include <CRCore/crGeometry.h>
#include <CRCore/crNotify.h>
#include <ode/ode.h>
using namespace CRCore;

const crGeometry::ArrayData crGeometry::s_InvalidArrayData;

crGeometry::ArrayData::ArrayData(const ArrayData& data,const crCopyOp& copyop):
array(copyop(data.array.get())),
indices(dynamic_cast<CRCore::IndexArray*>(copyop(data.indices.get()))),
binding(data.binding),
normalize(data.normalize),
offset(data.offset)
{
}

crGeometry::Vec3ArrayData::Vec3ArrayData(const Vec3ArrayData& data,const crCopyOp& copyop):
array(dynamic_cast<CRCore::Vec3Array*>(copyop(data.array.get()))),
indices(dynamic_cast<CRCore::IndexArray*>(copyop(data.indices.get()))),
binding(data.binding),
normalize(data.normalize),
offset(data.offset)
{
}

crGeometry::crGeometry():
    m_fastPath(true),
    m_fastPathHint(true)
{
	m_type = GEOMETRY;
}

crGeometry::crGeometry(const crGeometry& geometry,const crCopyOp& copyop):
crDrawable(geometry,copyop),
m_vertexData(geometry.m_vertexData,copyop),
//m_vertexStride(geometry.m_vertexStride),
//m_vertexBindMode(geometry.m_vertexBindMode),
m_normalData(geometry.m_normalData,copyop),
m_colorData(geometry.m_colorData,copyop),
m_secondaryColorData(geometry.m_secondaryColorData,copyop),
m_fogCoordData(geometry.m_fogCoordData,copyop),
m_fastPath(geometry.m_fastPath),
m_fastPathHint(geometry.m_fastPathHint)
{
	crPrimitive* primitive;
	for(PrimitiveList::const_iterator pitr=geometry.m_primitives.begin();
		pitr!=geometry.m_primitives.end();
		++pitr)
	{
		primitive = copyop(pitr->get());
		if (primitive) m_primitives.push_back(primitive);
	}

    for(ArrayList::const_iterator titr=geometry.m_texCoordList.begin();
        titr!=geometry.m_texCoordList.end();
        ++titr)
    {
        m_texCoordList.push_back(ArrayData(*titr,copyop));
    }

    for(ArrayList::const_iterator vitr=geometry.m_vertexAttribList.begin();
        vitr!=geometry.m_vertexAttribList.end();
        ++vitr)
    {
        m_vertexAttribList.push_back(ArrayData(*vitr,copyop));
    }
}

crGeometry::~crGeometry()
{
    // no need to delete, all automatically handled by ref_ptr :-)
	//for( TriMeshDataList::iterator itr = m_triMeshDataList.begin();
	//	 itr != m_triMeshDataList.end();
	//	 ++itr )
	//{
	//	if(itr->first) dGeomDestroy((dGeomID)(itr->first));
	//	if(itr->second) dGeomTriMeshDataDestroy((dTriMeshDataID)(itr->second));
	//}
}

bool crGeometry::empty() const
{
	if (!m_primitives.empty()) return false;
	if (!m_vertexData.empty()) return false;
	if (!m_normalData.empty()) return false;
	if (!m_colorData.empty()) return false;
	if (!m_secondaryColorData.empty()) return false;
	if (!m_fogCoordData.empty()) return false;
	if (!m_texCoordList.empty()) return false;
	if (!m_vertexAttribList.empty()) return false;
	return true;
}

void crGeometry::setTexCoordData(unsigned int unit,const ArrayData& arrayData)
{
	if (m_texCoordList.size()<=unit)
		m_texCoordList.resize(unit+1);
	
	m_texCoordList[unit] = arrayData;
}

crGeometry::ArrayData& crGeometry::getTexCoordData(unsigned int unit)
{
    if (m_texCoordList.size()<=unit)
        m_texCoordList.resize(unit+1);
        
    return m_texCoordList[unit];
}

const crGeometry::ArrayData& crGeometry::getTexCoordData(unsigned int unit) const
{
    if (m_texCoordList.size()<=unit)
        return s_InvalidArrayData;
        
    return m_texCoordList[unit];
}

void crGeometry::addTexCoordArray(Array* array)
{
    setTexCoordArray(getNumTexCoordArrays(),array);
}

void crGeometry::setTexCoordArray(unsigned int unit,Array* array)
{
    getTexCoordData(unit).binding = BIND_PER_VERTEX;
    getTexCoordData(unit).array = array;

    computeFastPathsUsed();
    dirtyDisplayList();
}

Array* crGeometry::getTexCoordArray(unsigned int unit)
{
    if (unit<m_texCoordList.size()) return m_texCoordList[unit].array.get();
    else return 0;
}

const Array* crGeometry::getTexCoordArray(unsigned int unit) const
{
    if (unit<m_texCoordList.size()) return m_texCoordList[unit].array.get();
    else return 0;
}

void crGeometry::setTexCoordIndices(unsigned int unit,IndexArray* array)
{
    getTexCoordData(unit).indices = array;

    computeFastPathsUsed();
    dirtyDisplayList();
}

IndexArray* crGeometry::getTexCoordIndices(unsigned int unit)
{
    if (unit<m_texCoordList.size()) return m_texCoordList[unit].indices.get();
    else return 0;
}

const IndexArray* crGeometry::getTexCoordIndices(unsigned int unit) const
{
    if (unit<m_texCoordList.size()) return m_texCoordList[unit].indices.get();
    else return 0;
}


void crGeometry::setVertexAttribData(unsigned int index, const crGeometry::ArrayData& attrData)
{
    if (m_vertexAttribList.size()<=index)
        m_vertexAttribList.resize(index+1);
        
    m_vertexAttribList[index] = attrData;
}

crGeometry::ArrayData& crGeometry::getVertexAttribData(unsigned int index)
{
    if (m_vertexAttribList.size()<=index)
        m_vertexAttribList.resize(index+1);
        
    return m_vertexAttribList[index];
}

const crGeometry::ArrayData& crGeometry::getVertexAttribData(unsigned int index) const
{
    if (m_vertexAttribList.size()<=m_vertexAttribList.size())
        return s_InvalidArrayData;
        
    return m_vertexAttribList[index];
}

void crGeometry::setVertexAttribArray(unsigned int index, Array* array)
{
    getVertexAttribData(index).array = array;

    computeFastPathsUsed();
    dirtyDisplayList();
}

Array *crGeometry::getVertexAttribArray(unsigned int index)
{
    if (index<m_vertexAttribList.size()) return m_vertexAttribList[index].array.get();
    else return 0;
}

const Array *crGeometry::getVertexAttribArray(unsigned int index) const
{
    if (index<m_vertexAttribList.size()) return m_vertexAttribList[index].array.get();
    else return 0;
}

void crGeometry::setVertexAttribIndices(unsigned int index,IndexArray* array)
{
    getVertexAttribData(index).indices = array;

    computeFastPathsUsed();
    dirtyDisplayList();
}

IndexArray* crGeometry::getVertexAttribIndices(unsigned int index)
{
    if (index<m_vertexAttribList.size()) return m_vertexAttribList[index].indices.get();
    else return 0;
}

const IndexArray* crGeometry::getVertexAttribIndices(unsigned int index) const
{
    if (index<m_vertexAttribList.size()) return m_vertexAttribList[index].indices.get();
    else return 0;
}

void crGeometry::setVertexAttribBinding(unsigned int index,AttributeBinding ab)
{
    getVertexAttribData(index).binding = ab;

    computeFastPathsUsed();
    dirtyDisplayList();
}

crGeometry::AttributeBinding crGeometry::getVertexAttribBinding(unsigned int index) const
{
    if (index<m_vertexAttribList.size()) return m_vertexAttribList[index].binding;
    else return BIND_OFF;
}

crGeometry::AttributeBinding crGeometry::getVertexAttribBinding(unsigned int index)
{
	if (index<m_vertexAttribList.size()) return m_vertexAttribList[index].binding;
	else return BIND_OFF;
}

void crGeometry::setVertexAttribNormalize(unsigned int index,unsigned char norm)
{
    getVertexAttribData(index).normalize = norm;

    dirtyDisplayList();
}

unsigned char crGeometry::getVertexAttribNormalize(unsigned int index) const
{
    if (index<m_vertexAttribList.size()) return m_vertexAttribList[index].normalize;
    else return false;
}

bool crGeometry::addPrimitive(crPrimitive* primitive)
{
    if (primitive)
    {
        m_primitives.push_back(primitive);
        dirtyDisplayList();
        dirtyBound();
        return true;
    }
    notify(WARN)<<"Warning: invalid index i or primitive passed to CRCore::crGeometry::addPrimitive(i,primitive), ignoring call."<<std::endl;
    return false;
}

bool crGeometry::setPrimitive(unsigned int i,crPrimitive* primitive)
{
    if (i<m_primitives.size() && primitive)
    {
        m_primitives[i] = primitive;
        dirtyDisplayList();
        dirtyBound();
        return true;
    }
    notify(WARN)<<"Warning: invalid index i or primitive passed to CRCore::crGeometry::setPrimitive(i,primitive), ignoring call."<<std::endl;
    return false;
}

bool crGeometry::insertPrimitive(unsigned int i,crPrimitive* primitive)
{
    if (primitive)
    {
        if (i<m_primitives.size())
        {
            m_primitives.insert(m_primitives.begin()+i,primitive);
            dirtyDisplayList();
            dirtyBound();
            return true;
        }
        else if (i==m_primitives.size())
        {
            return addPrimitive(primitive);
        }
    }
    notify(WARN)<<"Warning: invalid index i or primitive passed to CRCore::crGeometry::insertPrimitive(i,primitive), ignoring call."<<std::endl;
    return false;
}

bool crGeometry::removePrimitive(unsigned int i, unsigned int numElementsToRemove)
{
    if (i<m_primitives.size() && numElementsToRemove>0)
    {
        if (i+numElementsToRemove<=m_primitives.size())
        {
            m_primitives.erase(m_primitives.begin()+i,m_primitives.begin()+i+numElementsToRemove);
        }
        else
        {
            // asking to delete too many elements, report a warning, and delete to
            // the end of the primitive list.
            notify(WARN)<<"Warning: CRCore::crGeometry::removePrimitive(i,numElementsToRemove) has been asked to remove more elements than are available,"<<std::endl;
            notify(WARN)<<"         removing on from i to the end of the list of primitive sets."<<std::endl;
            m_primitives.erase(m_primitives.begin()+i,m_primitives.end());
        }
    
        dirtyDisplayList();
        dirtyBound();
        return true;
    }
    notify(WARN)<<"Warning: invalid index i passed to CRCore::crGeometry::removePrimitive(i,numElementsToRemove), ignoring call."<<std::endl;
    return false;
}

unsigned int crGeometry::getPrimitiveIndex(const crPrimitive* primitive) const
{
    for (unsigned int primitiveSetIndex=0;primitiveSetIndex<m_primitives.size();++primitiveSetIndex)
    {
        if (m_primitives[primitiveSetIndex]==primitive) return primitiveSetIndex;
    }
    return m_primitives.size(); // node not found.
}

bool crGeometry::computeFastPathsUsed()
{
    //static bool s_DisableFastPathInDisplayLists = getenv("CREATURE_DISABLE_FAST_PATH_IN_DISPLAY_LISTS");
    //if (m_useDisplayList && s_DisableFastPathInDisplayLists)
    //{
    //    CRCore::notify(CRCore::DEBUG_INFO)<<"crGeometry::computeFastPathsUsed() - Disabling fast paths in display lists"<<std::endl;
    //    m_fastPath = false;
    //    return m_fastPath;
    //}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // check to see if fast path can be used.
    //
    m_fastPath = true;
    if (m_vertexData.indices.valid()) m_fastPath = false;
    else if (m_normalData.binding==BIND_PER_PRIMITIVE || (m_normalData.binding==BIND_PER_VERTEX && m_normalData.indices.valid())) m_fastPath = false;
    else if (m_colorData.binding==BIND_PER_PRIMITIVE || (m_colorData.binding==BIND_PER_VERTEX && m_colorData.indices.valid())) m_fastPath = false;
    else if (m_secondaryColorData.binding==BIND_PER_PRIMITIVE || (m_secondaryColorData.binding==BIND_PER_VERTEX && m_secondaryColorData.indices.valid())) m_fastPath = false;
    else if (m_fogCoordData.binding==BIND_PER_PRIMITIVE || (m_fogCoordData.binding==BIND_PER_VERTEX && m_fogCoordData.indices.valid())) m_fastPath = false;
    else 
    {
        for( unsigned int va = 0; va < m_vertexAttribList.size(); ++va )
        {
            if (m_vertexAttribList[va].binding==BIND_PER_PRIMITIVE)
            {
                m_fastPath = false;
                break;
            }
            else
            {
                const Array * array = m_vertexAttribList[va].array.get();
                const IndexArray * idxArray = m_vertexAttribList[va].indices.get();

                if( m_vertexAttribList[va].binding==BIND_PER_VERTEX && 
                    array && array->getNumElements()>0 &&
                    idxArray && idxArray->getNumElements()>0 ) 
                {
                    m_fastPath = false;
                    break;
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Set up tex coords if required.
    //
    for(unsigned int unit=0;unit<m_texCoordList.size();++unit)
    {
        const ArrayData& texcoordData = m_texCoordList[unit];
        if (texcoordData.array.valid() && texcoordData.array->getNumElements()>0)
        {
            if (texcoordData.indices.valid())
            {
                if (texcoordData.indices->getNumElements()>0)
                {
                    m_fastPath = false;         
                    break;
                }
            }
        }
    }
    
    m_supportsVertexBufferObjects = m_fastPath;
    //setUseVertexBufferObjects(m_fastPath);
    //m_supportsVertexBufferObjects = false;
    //_useVertexBufferObjects = false;

    return m_fastPath;
}

//void crGeometry::drawImplementation(crState& state) const
//{
//	if(m_drawCallback.valid())
//		m_drawCallback->drawImplementation( this, state );
//
//}
unsigned int crGeometry::getObjectSizeHint() const
{
	unsigned int totalSize = 0;
	if (m_vertexData.array.valid()) totalSize += m_vertexData.array->getTotalDataSize();

	if (m_normalData.array.valid()) totalSize += m_normalData.array->getTotalDataSize();

	if (m_colorData.array.valid()) totalSize += m_colorData.array->getTotalDataSize();

	if (m_secondaryColorData.array.valid()) totalSize += m_secondaryColorData.array->getTotalDataSize();

	if (m_fogCoordData.array.valid()) totalSize += m_fogCoordData.array->getTotalDataSize();


	unsigned int unit;
	for(unit=0;unit<m_texCoordList.size();++unit)
	{
		const Array* array = m_texCoordList[unit].array.get();
		if (array) totalSize += array->getTotalDataSize();

	}

	bool handleVertexAttributes = true;
	if (handleVertexAttributes)
	{
		unsigned int index;
		for( index = 0; index < m_vertexAttribList.size(); ++index )
		{
			const Array* array = m_vertexAttribList[index].array.get();
			if (array) totalSize += array->getTotalDataSize();           
		}
	}

	for(PrimitiveList::const_iterator itr=m_primitives.begin();
		itr!=m_primitives.end();
		++itr)
	{

		totalSize += 4*(*itr)->getNumIndices();

	}


	// do a very simply mapping of display list size proportional to vertex datasize.
	return totalSize;
}



class AttrbuteFunctorArrayVisitor : public ArrayVisitor
{
public:

	AttrbuteFunctorArrayVisitor(crDrawable::AttributeFunctor& af):
	  m_af(af) {}

	  virtual ~AttrbuteFunctorArrayVisitor() {}

	  virtual void apply(ByteArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(ShortArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(IntArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(UByteArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(UShortArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(UIntArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(Vec4ubArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(FloatArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(Vec2Array& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(Vec3Array& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(Vec4Array& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }


	  inline void applyArray(crDrawable::AttributeType type,Array* array)
	  {
		  if (array)
		  {
			  m_type = type;
			  array->accept(*this);
		  }
	  }

	  crDrawable::AttributeFunctor&   m_af;
	  crDrawable::AttributeType       m_type;
};

void crGeometry::accept(AttributeFunctor& af)
{
	AttrbuteFunctorArrayVisitor afav(af);

	afav.applyArray(VERTICES,m_vertexData.array.get());
	afav.applyArray(NORMALS,m_normalData.array.get());
	afav.applyArray(COLORS,m_colorData.array.get());

	for(unsigned unit=0;unit<m_texCoordList.size();++unit)
	{
		afav.applyArray((AttributeType)(TEXTURE_COORDS_0+unit),m_texCoordList[unit].array.get());
	}
}

class ConstAttrbuteFunctorArrayVisitor : public ConstArrayVisitor
{
public:

	ConstAttrbuteFunctorArrayVisitor(crDrawable::ConstAttributeFunctor& af):
	  m_af(af) {}

	  virtual ~ConstAttrbuteFunctorArrayVisitor() {}

	  virtual void apply(const ByteArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const ShortArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const IntArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const UByteArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const UShortArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const UIntArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const Vec4ubArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const FloatArray& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const Vec2Array& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const Vec3Array& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }
	  virtual void apply(const Vec4Array& array) {  if (!array.empty()) m_af.apply(m_type,array.size(),&(array.front())); }


	  inline void applyArray(crDrawable::AttributeType type,const Array* array)
	  {
		  if (array)
		  {
			  m_type = type;
			  array->accept(*this);
		  }
	  }

	  crDrawable::ConstAttributeFunctor&    m_af;
	  crDrawable::AttributeType             m_type;
};

void crGeometry::accept(ConstAttributeFunctor& af) const
{
	ConstAttrbuteFunctorArrayVisitor afav(af);

	afav.applyArray(VERTICES,m_vertexData.array.get());
	afav.applyArray(NORMALS,m_normalData.array.get());
	afav.applyArray(COLORS,m_colorData.array.get());

	for(unsigned unit=0;unit<m_texCoordList.size();++unit)
	{
		afav.applyArray((AttributeType)(TEXTURE_COORDS_0+unit),m_texCoordList[unit].array.get());
	}
}

void crGeometry::accept(PrimitiveFunctor& functor) const
{
	if (!m_vertexData.array.valid() || m_vertexData.array->getNumElements()==0) return;

	if (!m_vertexData.indices.valid())
	{
		switch(m_vertexData.array->getType())
		{
		case(Array::Vec2ArrayType): 
			functor.setVertexArray(m_vertexData.array->getNumElements(),static_cast<const crVector2*>(m_vertexData.array->getDataPointer()));
			break;
		case(Array::Vec3ArrayType): 
			functor.setVertexArray(m_vertexData.array->getNumElements(),static_cast<const crVector3*>(m_vertexData.array->getDataPointer()));
			break;
		case(Array::Vec4ArrayType): 
			functor.setVertexArray(m_vertexData.array->getNumElements(),static_cast<const crVector4*>(m_vertexData.array->getDataPointer()));
			break;
		default:
			notify(WARN)<<"Warning: crGeometry::accept(PrimtiveFunctor&) cannot handle Vertex Array type"<<m_vertexData.array->getType()<<std::endl;
			return;
		}

		for(PrimitiveList::const_iterator itr=m_primitives.begin();
			itr!=m_primitives.end();
			++itr)
		{
			(*itr)->accept(functor);
		}
	}
	else
	{
		const Vec2Array* vec2Array = 0;
		const Vec3Array* vec3Array = 0;
		const Vec4Array* vec4Array = 0;
		Array::Type type = m_vertexData.array->getType();
		switch(type)
		{
		case(Array::Vec2ArrayType): 
			vec2Array = static_cast<const Vec2Array*>(m_vertexData.array.get());
			break;
		case(Array::Vec3ArrayType): 
			vec3Array = static_cast<const Vec3Array*>(m_vertexData.array.get());
			break;
		case(Array::Vec4ArrayType): 
			vec4Array = static_cast<const Vec4Array*>(m_vertexData.array.get());
			break;
		default:
			notify(WARN)<<"Warning: crGeometry::accept(PrimtiveFunctor&) cannot handle Vertex Array type"<<m_vertexData.array->getType()<<std::endl;
			return;
		}


		for(PrimitiveList::const_iterator itr=m_primitives.begin();
			itr!=m_primitives.end();
			++itr)
		{
			const crPrimitive* primitive = itr->get();
			unsigned int mode=primitive->getMode();
			switch(primitive->getType())
			{
			case(crPrimitive::DrawArraysPrimitiveType):
				{
					const DrawArrays* drawArray = static_cast<const DrawArrays*>(primitive);
					functor.begin(mode);

					unsigned int indexEnd = drawArray->getFirst()+drawArray->getCount();
					for(unsigned int vindex=drawArray->getFirst();
						vindex<indexEnd;
						++vindex)
					{
						switch(type)
						{
						case(Array::Vec2ArrayType): 
							functor.vertex((*vec2Array)[m_vertexData.indices->index(vindex)]);
							break;
						case(Array::Vec3ArrayType): 
							functor.vertex((*vec3Array)[m_vertexData.indices->index(vindex)]);
							break;
						case(Array::Vec4ArrayType): 
							functor.vertex((*vec4Array)[m_vertexData.indices->index(vindex)]);
							break;
						default:
							break;
						}

					}

					functor.end();
					break;
				}
			case(crPrimitive::DrawArrayLengthsPrimitiveType):
				{
					const DrawArrayLengths* drawArrayLengths = static_cast<const DrawArrayLengths*>(primitive);
					unsigned int vindex=drawArrayLengths->getFirst();
					for(DrawArrayLengths::PT_IndexArray::const_iterator primItr=drawArrayLengths->getIndexArray().begin();
						primItr!=drawArrayLengths->getIndexArray().end();
						++primItr)
					{

						functor.begin(mode);

						for(int primCount=0;primCount<*primItr;++primCount)
						{
							switch(type)
							{
							case(Array::Vec2ArrayType): 
								functor.vertex((*vec2Array)[m_vertexData.indices->index(vindex)]);
								break;
							case(Array::Vec3ArrayType): 
								functor.vertex((*vec3Array)[m_vertexData.indices->index(vindex)]);
								break;
							case(Array::Vec4ArrayType): 
								functor.vertex((*vec4Array)[m_vertexData.indices->index(vindex)]);
								break;
							default:
								break;
							}
							++vindex;
						}

						functor.end();

					}
					break;
				}
			case(crPrimitive::DrawElementsUBytePrimitiveType):
				{
					const DrawElementsUByte* drawElements = static_cast<const DrawElementsUByte*>(primitive);
					functor.begin(mode);

					unsigned int primCount=0;
					for(DrawElementsUByte::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primCount,++primItr)
					{
						unsigned int vindex=*primItr;
						switch(type)
						{
						case(Array::Vec2ArrayType): 
							functor.vertex((*vec2Array)[m_vertexData.indices->index(vindex)]);
							break;
						case(Array::Vec3ArrayType): 
							functor.vertex((*vec3Array)[m_vertexData.indices->index(vindex)]);
							break;
						case(Array::Vec4ArrayType): 
							functor.vertex((*vec4Array)[m_vertexData.indices->index(vindex)]);
							break;
						default:
							break;
						}
					}

					functor.end();
					break;
				}
			case(crPrimitive::DrawElementsUShortPrimitiveType):
				{
					const DrawElementsUShort* drawElements = static_cast<const DrawElementsUShort*>(primitive);
					functor.begin(mode);

					for(DrawElementsUShort::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primItr)
					{
						unsigned int vindex=*primItr;
						switch(type)
						{
						case(Array::Vec2ArrayType): 
							functor.vertex((*vec2Array)[m_vertexData.indices->index(vindex)]);
							break;
						case(Array::Vec3ArrayType): 
							functor.vertex((*vec3Array)[m_vertexData.indices->index(vindex)]);
							break;
						case(Array::Vec4ArrayType): 
							functor.vertex((*vec4Array)[m_vertexData.indices->index(vindex)]);
							break;
						default:
							break;
						}
					}

					functor.end();
					break;
				}
			case(crPrimitive::DrawElementsUIntPrimitiveType):
				{
					const DrawElementsUInt* drawElements = static_cast<const DrawElementsUInt*>(primitive);
					functor.begin(mode);

					for(DrawElementsUInt::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primItr)
					{
						unsigned int vindex=*primItr;
						switch(type)
						{
						case(Array::Vec2ArrayType): 
							functor.vertex((*vec2Array)[m_vertexData.indices->index(vindex)]);
							break;
						case(Array::Vec3ArrayType): 
							functor.vertex((*vec3Array)[m_vertexData.indices->index(vindex)]);
							break;
						case(Array::Vec4ArrayType): 
							functor.vertex((*vec4Array)[m_vertexData.indices->index(vindex)]);
							break;
						default:
							break;
						}
					}

					functor.end();
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
	return;
}

void crGeometry::accept(PrimitiveIndexFunctor& functor) const
{
	if (!m_vertexData.array.valid() || m_vertexData.array->getNumElements()==0) return;

	switch(m_vertexData.array->getType())
	{
	case(Array::Vec2ArrayType): 
		functor.setVertexArray(m_vertexData.array->getNumElements(),static_cast<const crVector2f*>(m_vertexData.array->getDataPointer()));
		break;
	case(Array::Vec3ArrayType): 
		functor.setVertexArray(m_vertexData.array->getNumElements(),static_cast<const crVector3f*>(m_vertexData.array->getDataPointer()));
		break;
	case(Array::Vec4ArrayType): 
		functor.setVertexArray(m_vertexData.array->getNumElements(),static_cast<const crVector4f*>(m_vertexData.array->getDataPointer()));
		break;
	default:
		notify(WARN)<<"Warning: crGeometry::accept(PrimtiveIndexFunctor&) cannot handle Vertex Array type"<<m_vertexData.array->getType()<<std::endl;
		return;
	}

	if (!m_vertexData.indices.valid())
	{
		for(PrimitiveList::const_iterator itr=m_primitives.begin();
			itr!=m_primitives.end();
			++itr)
		{
			(*itr)->accept(functor);
		}
	}
	else
	{
		for(PrimitiveList::const_iterator itr=m_primitives.begin();
			itr!=m_primitives.end();
			++itr)
		{
			const crPrimitive* primitiveset = itr->get();
			unsigned int mode=primitiveset->getMode();
			switch(primitiveset->getType())
			{
			case(crPrimitive::DrawArraysPrimitiveType):
				{
					const DrawArrays* drawArray = static_cast<const DrawArrays*>(primitiveset);
					functor.begin(mode);

					unsigned int indexEnd = drawArray->getFirst()+drawArray->getCount();
					for(unsigned int vindex=drawArray->getFirst();
						vindex<indexEnd;
						++vindex)
					{
						functor.vertex(m_vertexData.indices->index(vindex));
					}

					functor.end();
					break;
				}
			case(crPrimitive::DrawArrayLengthsPrimitiveType):
				{
					const DrawArrayLengths* drawArrayLengths = static_cast<const DrawArrayLengths*>(primitiveset);
					unsigned int vindex=drawArrayLengths->getFirst();
					for(DrawArrayLengths::PT_IndexArray::const_iterator primItr=drawArrayLengths->getIndexArray().begin();
						primItr!=drawArrayLengths->getIndexArray().end();
						++primItr)
					{

						functor.begin(mode);

						for(GLsizei primCount=0;primCount<*primItr;++primCount)
						{
							functor.vertex(m_vertexData.indices->index(vindex));
							++vindex;
						}

						functor.end();

					}
					break;
				}
			case(crPrimitive::DrawElementsUBytePrimitiveType):
				{
					const DrawElementsUByte* drawElements = static_cast<const DrawElementsUByte*>(primitiveset);
					functor.begin(mode);

					unsigned int primCount=0;
					for(DrawElementsUByte::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primCount,++primItr)
					{
						unsigned int vindex=*primItr;
						functor.vertex(m_vertexData.indices->index(vindex));
					}

					functor.end();
					break;
				}
			case(crPrimitive::DrawElementsUShortPrimitiveType):
				{
					const DrawElementsUShort* drawElements = static_cast<const DrawElementsUShort*>(primitiveset);
					functor.begin(mode);

					for(DrawElementsUShort::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primItr)
					{
						unsigned int vindex=*primItr;
						functor.vertex(m_vertexData.indices->index(vindex));
					}

					functor.end();
					break;
				}
			case(crPrimitive::DrawElementsUIntPrimitiveType):
				{
					const DrawElementsUInt* drawElements = static_cast<const DrawElementsUInt*>(primitiveset);
					functor.begin(mode);

					for(DrawElementsUInt::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primItr)
					{
						unsigned int vindex=*primItr;
						functor.vertex(m_vertexData.indices->index(vindex));
					}

					functor.end();
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
	return;
}



unsigned int _computeNumberOfPrimtives(const CRCore::crGeometry& geom)
{

	unsigned int totalNumberOfPrimitives = 0;

	for(crGeometry::PrimitiveList::const_iterator itr=geom.getPrimitiveList().begin();
		itr!=geom.getPrimitiveList().end();
		++itr)
	{
		const crPrimitive* primitive = itr->get();
		unsigned int mode=primitive->getMode();

		unsigned int primLength;
		switch(mode)
		{
		case(crPrimitive::PT_POINTS):    primLength=1; break;
		case(crPrimitive::PT_LINES):     primLength=2; break;
		case(crPrimitive::PT_TRIANGLES): primLength=3; break;
		case(crPrimitive::PT_QUADS):     primLength=4; break;
		default:            primLength=0; break; // compute later when =0.
		}

		// draw primtives by the more flexible "slow" path,
		// sending OpenGL glBegin/glVertex.../glEnd().
		switch(primitive->getType())
		{
		case(crPrimitive::DrawArrayLengthsPrimitiveType):
			{

				const DrawArrayLengths* drawArrayLengths = static_cast<const DrawArrayLengths*>(primitive);
				for(DrawArrayLengths::PT_IndexArray::const_iterator primItr=drawArrayLengths->getIndexArray().begin();
					primItr!=drawArrayLengths->getIndexArray().end();
					++primItr)
				{
					if (primLength==0) totalNumberOfPrimitives += 1;
					else totalNumberOfPrimitives = *primItr/primLength;
				}
				break;
			}
		default:
			{
				if (primLength==0) totalNumberOfPrimitives += 1;
				else totalNumberOfPrimitives = primitive->getNumIndices()/primLength;

			}
		}
	}

	return totalNumberOfPrimitives;
}

template<class A>
bool _verifyBindings(const CRCore::crGeometry& geom, const A& arrayData)
{
	unsigned int numElements = arrayData.indices.valid()?arrayData.indices->getNumElements():
	arrayData.array.valid()?arrayData.array->getNumElements():0;

	switch(arrayData.binding)
	{
	case(CRCore::crGeometry::BIND_OFF):
		if (numElements>0) return false;
		break;
	case(CRCore::crGeometry::BIND_OVERALL):
		if (numElements!=1) return false;
		break;
	case(CRCore::crGeometry::BIND_PER_PRIMITIVE_SET):
		if (numElements!=geom.getPrimitiveList().size()) return false;
		break;
	case(CRCore::crGeometry::BIND_PER_PRIMITIVE):
		if (numElements!=_computeNumberOfPrimtives(geom)) return false;
		break;
	case(CRCore::crGeometry::BIND_PER_VERTEX):
		{
			unsigned int numVertices = geom.getVertexIndices()?geom.getVertexIndices()->getNumElements():
			geom.getVertexArray()?geom.getVertexArray()->getNumElements():0;
			if (numElements!=numVertices) return false;        
			break;
		}
	} 
	return true;
}

template<class A>
void _computeCorrectBindingsAndArraySizes(const CRCore::crGeometry& geom, A& arrayData, const char* arrayName)
{
	unsigned int numVertices = geom.getVertexIndices()?geom.getVertexIndices()->getNumElements():
	geom.getVertexArray()?geom.getVertexArray()->getNumElements():0;

	if ( numVertices==0 )
	{
		if (arrayData.binding!=CRCore::crGeometry::BIND_OFF)
		{
			arrayData.array = 0;
			arrayData.indices = 0;
			arrayData.binding = CRCore::crGeometry::BIND_OFF;
            #ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() vertex array is empty but "<<std::endl
				<<"         vertex array is empty but"<<arrayName<<" is set"<<std::endl
				<<"         reseting "<<arrayName<< " binding to BIND_OFF and array & indices to 0."<<std::endl;
            #endif
		}
	}

	unsigned int numElements = arrayData.indices.valid()?arrayData.indices->getNumElements():
	arrayData.array.valid()?arrayData.array->getNumElements():0;

	switch(arrayData.binding)
	{
	case(CRCore::crGeometry::BIND_OFF):
		if (numElements!=0)
		{
			arrayData.array = 0;
			arrayData.indices = 0;
            #ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
				<<"         "<<arrayName<<" binding is BIND_OFF but "<<arrayName<< " array is attached"<<std::endl
				<<"         reseting array to 0."<<std::endl;
            #endif
		}
		break;
	case(CRCore::crGeometry::BIND_OVERALL):
		if (numElements==0) 
		{
			arrayData.array = 0;
			arrayData.indices = 0;
			arrayData.binding = CRCore::crGeometry::BIND_OFF;
            #ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
				<<"         "<<arrayName<<" binding is BIND_OVERALL but "<<arrayName<< " array is empty"<<std::endl
				<<"         reseting binding to BIND_OFF "<<arrayName<<"array to 0."<<std::endl;
            #endif
		}
		else if (numElements>1) 
		{
			#ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
				<<"         "<<arrayName<<" binding is BIND_OVERALL but "<<std::endl
				<<"         "<<arrayName<< " contains more than one entry"<<std::endl;
			#endif
		}
		break;
	case(CRCore::crGeometry::BIND_PER_PRIMITIVE_SET):
		if (numElements==0)
		{
			arrayData.array = 0;
			arrayData.indices = 0;
			arrayData.binding = CRCore::crGeometry::BIND_OFF;
			#ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
				<<"         "<<arrayName<<" binding is BIND_PER_PRIMITIVE_SET but "<<std::endl
				<<"         "<<arrayName<< " array is not attached"<<std::endl
				<<"         reseting binding to BIND_OFF."<<std::endl;
			#endif
		}
		else if (numElements<geom.getPrimitiveList().size()) 
		{
			arrayData.array = 0;
			arrayData.indices = 0;
			arrayData.binding = CRCore::crGeometry::BIND_OFF;
			#ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
				<<"         "<<arrayName<<" binding is BIND_PER_PRIMITIVE_SET but "<<std::endl
				<<"         "<<arrayName<< " array contains too few entries"<<std::endl
				<<"         reseting binding to BIND_OFF "<<arrayName<<"array to 0."<<std::endl;
			#endif
		}
		else if (numElements>geom.getPrimitiveList().size()) 
		{
			#ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
				<<"         "<<arrayName<<" binding is BIND_PER_PRIMITIVE_SET but "<<std::endl
				<<"         "<<arrayName<< " array contains too many entries"<<std::endl;
			#endif
		}
		break;
	case(CRCore::crGeometry::BIND_PER_PRIMITIVE):
		{
			unsigned int numPrimitives = _computeNumberOfPrimtives(geom);
			if (numElements==0)
			{
				arrayData.array = 0;
				arrayData.indices = 0;
				arrayData.binding = CRCore::crGeometry::BIND_OFF;
				#ifdef _DEBUG
				notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
					<<"         "<<arrayName<<" binding is BIND_PER_PRIMITIVE but "<<std::endl
					<<"         "<<arrayName<< " array is not attached"<<std::endl
					<<"         reseting binding to BIND_OFF."<<std::endl;
				#endif
			}
			else if (numElements<numPrimitives)
			{
				arrayData.array = 0;
				arrayData.indices = 0;
				arrayData.binding = CRCore::crGeometry::BIND_OFF;
				#ifdef _DEBUG
				notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
					<<"         "<<arrayName<<" binding is BIND_PER_PRIMITIVE but "<<std::endl
					<<"         "<<arrayName<< " array contains too few entries"<<std::endl
					<<"         reseting binding to BIND_OFF "<<arrayName<<"array to 0."<<std::endl;
				#endif
			}
			else if (numElements>numPrimitives)
			{
				#ifdef _DEBUG
				notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
					<<"         "<<arrayName<<" binding is BIND_PER_PRIMITIVE but "<<std::endl
					<<"         "<<arrayName<< " array contains too many entries"<<std::endl;
				#endif
			}

			break;
		}
	case(CRCore::crGeometry::BIND_PER_VERTEX):
		if (numElements==0)
		{
			arrayData.array = 0;
			arrayData.indices = 0;
			arrayData.binding = CRCore::crGeometry::BIND_OFF;
			#ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
				<<"         "<<arrayName<<" binding is BIND_PER_VERTEX but "<<std::endl
				<<"         "<<arrayName<< " array is not attached"<<std::endl
				<<"         reseting binding to BIND_OFF."<<std::endl;
			#endif
		}
		else if (numElements<numVertices) 
		{
			arrayData.array = 0;
			arrayData.indices = 0;
			arrayData.binding = CRCore::crGeometry::BIND_OFF;
			#ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes()"<<std::endl
				<<"         "<<arrayName<<" binding is BIND_PER_VERTEX but "<<std::endl
				<<"         "<<arrayName<< " array contains too few entries"<<std::endl
				<<"         reseting binding to BIND_OFF "<<arrayName<<"array to 0."<<std::endl;
			#endif
		}
		else if (numElements>numVertices) 
		{
			#ifdef _DEBUG
			notify(WARN)<<"Warning: in CRCore::crGeometry::computeCorrectBindingsAndArraySizes() "<<std::endl
				<<"         "<<arrayName<<" binding is BIND_PER_VERTEX but "<<std::endl
				<<"         "<<arrayName<< " array contains too many entries"<<std::endl;
			#endif
		}
		break;
	} 
}        

bool crGeometry::verifyBindings(const ArrayData& arrayData) const
{
	return _verifyBindings(*this,arrayData);
}

bool crGeometry::verifyBindings(const Vec3ArrayData& arrayData) const
{
	return _verifyBindings(*this,arrayData);
}

void crGeometry::computeCorrectBindingsAndArraySizes(ArrayData& arrayData, const char* arrayName)
{
	_computeCorrectBindingsAndArraySizes(*this,arrayData,arrayName);
}

void crGeometry::computeCorrectBindingsAndArraySizes(Vec3ArrayData& arrayData, const char* arrayName)
{
	_computeCorrectBindingsAndArraySizes(*this,arrayData,arrayName);
}

bool crGeometry::verifyBindings() const
{
	if (!verifyBindings(m_normalData)) return false;
	if (!verifyBindings(m_colorData)) return false;
	if (!verifyBindings(m_secondaryColorData)) return false;
	if (!verifyBindings(m_fogCoordData)) return false;

	for(ArrayList::const_iterator titr=m_texCoordList.begin();
		titr!=m_texCoordList.end();
		++titr)
	{
		if (!verifyBindings(*titr)) return false;
	}

	for(ArrayList::const_iterator vitr=m_vertexAttribList.begin();
		vitr!=m_vertexAttribList.end();
		++vitr)
	{
		if (!verifyBindings(*vitr)) return false;
	}

	return true;
}

void crGeometry::computeCorrectBindingsAndArraySizes()
{
	if (verifyBindings()) return;

	computeCorrectBindingsAndArraySizes(m_normalData,"NormalData");
	computeCorrectBindingsAndArraySizes(m_colorData,"ColorData");
	computeCorrectBindingsAndArraySizes(m_secondaryColorData,"SecondaryColorData");
	computeCorrectBindingsAndArraySizes(m_fogCoordData,"FogCoordData");

	for(ArrayList::iterator titr=m_texCoordList.begin();
		titr!=m_texCoordList.end();
		++titr)
	{
		computeCorrectBindingsAndArraySizes(*titr,"TexCoordList[]");
	}

	for(ArrayList::iterator vitr=m_vertexAttribList.begin();
		vitr!=m_vertexAttribList.end();
		++vitr)
	{
		computeCorrectBindingsAndArraySizes(*vitr,"VertAttribList[]");
	}
}

class ExpandIndexedArray : public CRCore::ConstArrayVisitor
{
public:
	ExpandIndexedArray(const CRCore::IndexArray& indices,Array* targetArray):
	  m_indices(indices),
		  m_targetArray(targetArray) {}

		  virtual ~ExpandIndexedArray() {}

		  template <class T,class I>
			  T* create(const T& array,const I& indices)
		  {
			  T* newArray = 0;

			  // if source array type and target array type are equal but arrays arn't equal
			  if (m_targetArray && m_targetArray->getType()==array.getType() && m_targetArray!=(CRCore::Array*)(&array))
			  {
				  // reuse exisiting target array 
				  newArray = static_cast<T*>(m_targetArray);
				  if (newArray->size()!=indices.size())
				  {
					  // make sure its the right size
					  newArray->resize(indices.size());
				  }
			  }
			  else
			  {
				  //  else create a new array.
				  newArray = new T(indices.size());
			  }

			  for(unsigned int i=0;i<indices.size();++i)
			  {
				  (*newArray)[i]= array[indices[i]];
			  }

			  return newArray;
		  }

		  template <class T>
			  T* create(const T& array)
		  {
			  switch(m_indices.getType())
			  {
			  case(CRCore::Array::ByteArrayType): return create(array,static_cast<const CRCore::ByteArray&>(m_indices));
			  case(CRCore::Array::ShortArrayType): return create(array,static_cast<const CRCore::ShortArray&>(m_indices));
			  case(CRCore::Array::IntArrayType): return create(array,static_cast<const CRCore::IntArray&>(m_indices));
			  case(CRCore::Array::UByteArrayType): return create(array,static_cast<const CRCore::UByteArray&>(m_indices));
			  case(CRCore::Array::UShortArrayType): return create(array,static_cast<const CRCore::UShortArray&>(m_indices));
			  case(CRCore::Array::UIntArrayType): return create(array,static_cast<const CRCore::UIntArray&>(m_indices));
			  default: return 0;
			  }

		  }

		  virtual void apply(const CRCore::ByteArray& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::ShortArray& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::IntArray& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::UByteArray& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::UShortArray& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::UIntArray& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::Vec4ubArray& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::FloatArray& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::Vec2Array& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::Vec3Array& array) { m_targetArray = create(array); }
		  virtual void apply(const CRCore::Vec4Array& array) { m_targetArray = create(array); }

		  const CRCore::IndexArray& m_indices;
		  CRCore::Array* m_targetArray;
};

bool crGeometry::suitableForOptimization() const
{
	bool hasIndices = false;

	if (getVertexIndices()) hasIndices = true;

	if (getNormalIndices()) hasIndices = true;

	if (getColorIndices()) hasIndices = true;

	if (getSecondaryColorIndices()) hasIndices = true;

	if (getFogCoordIndices()) hasIndices = true;

	for(unsigned int ti=0;ti<getNumTexCoordArrays();++ti)
	{
		if (getTexCoordIndices(ti)) hasIndices = true;
	}

	for(unsigned int vi=0;vi<getNumVertexAttribArrays();++vi)
	{
		if (getVertexAttribIndices(vi)) hasIndices = true;
	}

	return hasIndices;
}

void crGeometry::copyToAndOptimize(crGeometry& target)
{
	bool copyToSelf = (this==&target);

	// copy over primitive sets.
	if (!copyToSelf) target.getPrimitiveList() = getPrimitiveList();

	// copy over attribute arrays.
	if (getVertexIndices())
	{
		ExpandIndexedArray eia(*(getVertexIndices()),target.getVertexArray());
		getVertexArray()->accept(eia);

		target.setVertexArray(eia.m_targetArray);
		target.setVertexIndices(0);
	}
	else if (getVertexArray())
	{
		if (!copyToSelf) target.setVertexArray(getVertexArray());
	}

	target.setNormalBinding(getNormalBinding());
	if (getNormalIndices())
	{
		ExpandIndexedArray eia(*(getNormalIndices()),target.getNormalArray());
		getNormalArray()->accept(eia);

		target.setNormalArray(dynamic_cast<CRCore::Vec3Array*>(eia.m_targetArray));
		target.setNormalIndices(0);
	}
	else if (getNormalArray())
	{
		if (!copyToSelf) target.setNormalArray(getNormalArray());
	}

	target.setColorBinding(getColorBinding());
	if (getColorIndices())
	{
		ExpandIndexedArray eia(*(getColorIndices()),target.getColorArray());
		getColorArray()->accept(eia);

		target.setColorArray(eia.m_targetArray);
		target.setColorIndices(0);
	}
	else if (getColorArray())
	{
		if (!copyToSelf) target.setColorArray(getColorArray());
	}

	target.setSecondaryColorBinding(getSecondaryColorBinding());
	if (getSecondaryColorIndices())
	{
		ExpandIndexedArray eia(*(getSecondaryColorIndices()),target.getSecondaryColorArray());
		getSecondaryColorArray()->accept(eia);

		target.setSecondaryColorArray(eia.m_targetArray);
		target.setSecondaryColorIndices(0);
	}
	else if (getSecondaryColorArray())
	{
		if (!copyToSelf) target.setSecondaryColorArray(getSecondaryColorArray());
	}

	target.setFogCoordBinding(getFogCoordBinding());
	if (getFogCoordIndices())
	{
		ExpandIndexedArray eia(*(getFogCoordIndices()),target.getFogCoordArray());
		getFogCoordArray()->accept(eia);

		target.setFogCoordArray(eia.m_targetArray);
		target.setFogCoordIndices(0);
	}
	else if (getFogCoordArray())
	{
		if (!copyToSelf) target.setFogCoordArray(getFogCoordArray());
	}

	for(unsigned int ti=0;ti<getNumTexCoordArrays();++ti)
	{
		if (getTexCoordIndices(ti))
		{
			ExpandIndexedArray eia(*(getTexCoordIndices(ti)),target.getTexCoordArray(ti));
			getTexCoordArray(ti)->accept(eia);

			target.setTexCoordArray(ti,eia.m_targetArray);
			target.setTexCoordIndices(ti,0);
		}
		else if (getTexCoordArray(ti)) 
		{
			if (!copyToSelf) target.setTexCoordArray(ti,getTexCoordArray(ti));
		}
	}

	for(unsigned int vi=0;vi<m_vertexAttribList.size();++vi)
	{
		ArrayData& arrayData = m_vertexAttribList[vi];
		if (arrayData.indices.valid())
		{
			ExpandIndexedArray eia(*arrayData.indices,target.getVertexAttribArray(vi));
			arrayData.array->accept(eia);
			target.setVertexAttribData(vi,ArrayData(eia.m_targetArray, 0, arrayData.binding, arrayData.normalize));
		}
		else if (arrayData.array.valid())
		{
			if (!copyToSelf) target.setVertexAttribData(vi,arrayData);
		}
	}
}

void crGeometry::computeInternalOptimizedGeometry()
{
	if (suitableForOptimization())
	{
		if (!m_internalOptimizedGeometry) m_internalOptimizedGeometry = new crGeometry;

		copyToAndOptimize(*m_internalOptimizedGeometry);
	}
}

crGeometry* CRCore::createTexturedQuadGeometry(const crVector3f& corner,const crVector3f& widthVec,const crVector3f& heightVec, float l, float b, float r, float t)
{
	crGeometry* geom = new crGeometry;

	Vec3Array* coords = new Vec3Array(4);
	(*coords)[0] = corner+heightVec;
	(*coords)[1] = corner;
	(*coords)[2] = corner+widthVec;
	(*coords)[3] = corner+widthVec+heightVec;
	geom->setVertexArray(coords);

	Vec2Array* tcoords = new Vec2Array(4);
	(*tcoords)[0].set(l,t);
	(*tcoords)[1].set(l,b);
	(*tcoords)[2].set(r,b);
	(*tcoords)[3].set(r,t);
	geom->setTexCoordArray(0,tcoords);

	geom->setColorBinding(crGeometry::BIND_OFF);
    geom->setNormalBinding(crGeometry::BIND_OFF);

	//CRCore::Vec4Array* colours = new CRCore::Vec4Array(1);
	//(*colours)[0].set(1.0f,1.0f,1.0,1.0f);
	//geom->setColorArray(colours);
	//geom->setColorBinding(crGeometry::BIND_OVERALL);

	//CRCore::Vec3Array* normals = new CRCore::Vec3Array(1);
	//(*normals)[0] = widthVec^heightVec;
	//(*normals)[0].normalize();
	//geom->setNormalArray(normals);
	//geom->setNormalBinding(crGeometry::BIND_OVERALL);

	geom->addPrimitive(new DrawArrays(crPrimitive::PT_QUADS,0,4));

	return geom;
}

#define APPEND_Geometry_Data(lhs,rhs) \
    if(rhs && rhs->size() > 0) \
	    lhs->insert(lhs->end(), \
	        rhs->begin(),rhs->end());

#define APPEND_Geometry_PrmL_IA(lhs,rhs) \
	lhs.insert(lhs.end(),\
               rhs.begin(),\
               rhs.end());

bool crGeometry::expand(const crGeometry& source,int& offset)
{//只并入几何信息，对状态信息不做合并
	Vec3Array* lhs = (Vec3Array*)(m_vertexData.array.get());
	Vec3Array* rhs = (Vec3Array*)(source.m_vertexData.array.get());
	if(lhs) offset = lhs->size();
	else offset = 0;
	if(rhs)
	{
		if(!lhs) this->setVertexArray(new Vec3Array(*rhs));
		else APPEND_Geometry_Data(lhs,rhs)
	}

	if(source.m_primitives.size()==0) return false;
	//ref_ptr<crPrimitive> new_prim = NULL;
	crPrimitive* new_prim;
	for(PrimitiveList::const_iterator sitr = source.m_primitives.begin(); 
		sitr!=source.m_primitives.end(); 
		++sitr)
	{
		new_prim = (crPrimitive *)(sitr->get()->clone(crCopyOp::DEEP_COPY_ALL));
		if(offset>0) new_prim->offsetIndices(offset);
        //new_prim = (crPrimitive *)(sitr->get());

		switch(new_prim->getMode()) 
		{
		case crPrimitive::PT_TRIANGLES:
		case crPrimitive::PT_QUADS:
			{
				if(m_primitives.size()==0) 
				{
					this->addPrimitive(new_prim);
					break;
				}

				bool done = false;
				for(PrimitiveList::iterator itr = m_primitives.begin(); 
					itr!=m_primitives.end(); 
					++itr)
				{
                    if((*itr)->getType() == new_prim->getType() &&
						(*itr)->getMode() == new_prim->getMode())
					{
						switch((*itr)->getType()) 
						{
						case crPrimitive::DrawArraysPrimitiveType:
						case crPrimitive::DrawArrayLengthsPrimitiveType:
							{
							    TemplatePrimitive<int> *lprim = (TemplatePrimitive<int> *)(itr->get());
							    TemplatePrimitive<int> *rprim = (TemplatePrimitive<int> *)(new_prim);

								//for( TemplatePrimitive<int>::PT_IndexArray::iterator index_itr = rprim->getIndexArray().begin();
								//	index_itr != rprim->getIndexArray().end();
								//	++index_itr )
								//{
								//	lprim->push_back(*index_itr + offset);
								//}
								APPEND_Geometry_PrmL_IA(lprim->getIndexArray(),rprim->getIndexArray())
							}
							break;
						case crPrimitive::DrawElementsUBytePrimitiveType:
							{
								DrawElementsUByte *lprim = (DrawElementsUByte *)(itr->get());
								DrawElementsUByte *rprim = (DrawElementsUByte *)(new_prim);

								//for( DrawElementsUByte::PT_IndexArray::iterator index_itr = rprim->getIndexArray().begin();
								//	index_itr != rprim->getIndexArray().end();
								//	++index_itr )
								//{
								//	lprim->push_back(*index_itr + (unsigned char)offset);
								//}
								APPEND_Geometry_PrmL_IA(lprim->getIndexArray(),rprim->getIndexArray())
							}
							break;
						case crPrimitive::DrawElementsUShortPrimitiveType:
							{
								DrawElementsUShort *lprim = (DrawElementsUShort *)(itr->get());
								DrawElementsUShort *rprim = (DrawElementsUShort *)(new_prim);

								//for( DrawElementsUShort::PT_IndexArray::iterator index_itr = rprim->getIndexArray().begin();
								//	index_itr != rprim->getIndexArray().end();
								//	++index_itr )
								//{
								//	lprim->push_back(*index_itr + (unsigned short)offset);
								//}
								APPEND_Geometry_PrmL_IA(lprim->getIndexArray(),rprim->getIndexArray())
							}
                            break;
                        case crPrimitive::DrawElementsUIntPrimitiveType:
							{
								DrawElementsUInt *lprim = (DrawElementsUInt *)(itr->get());
								DrawElementsUInt *rprim = (DrawElementsUInt *)(new_prim);

								//for( DrawElementsUInt::PT_IndexArray::iterator index_itr = rprim->getIndexArray().begin();
								//	index_itr != rprim->getIndexArray().end();
								//	++index_itr )
								//{
								//	lprim->push_back(*index_itr + (unsigned int)offset);
								//}
								APPEND_Geometry_PrmL_IA(lprim->getIndexArray(),rprim->getIndexArray())
							}
							break;
						}
                        done = true;
						break;
					}
				}
				if(!done) this->addPrimitive(new_prim);

			}
			break;
		default:
			this->addPrimitive(new_prim);
		}
	}

    lhs = (Vec3Array*)(m_normalData.array.get());
	rhs = (Vec3Array*)(source.m_normalData.array.get());
	if(rhs)
	{
		if(!lhs) this->setNormalArray(new Vec3Array(*rhs));
		else APPEND_Geometry_Data(lhs,rhs)
		m_normalData.binding = source.m_normalData.binding;
	}

	//lhs = (Vec3Array*)(m_colorData.array.get());
	//rhs = (Vec3Array*)(source.m_colorData.array.get());
	//if(rhs)
	//{
	//	if(!lhs) this->setColorArray(new Vec3Array(*rhs));
	//	else APPEND_Geometry_Data(lhs,rhs)
	//	m_colorData.binding = source.m_colorData.binding;
	//}

	lhs = (Vec3Array*)(m_secondaryColorData.array.get());
	rhs = (Vec3Array*)(source.m_secondaryColorData.array.get());
	if(rhs)
	{
		if(!lhs) this->setSecondaryColorArray(new Vec3Array(*rhs));
		else APPEND_Geometry_Data(lhs,rhs)
		m_secondaryColorData.binding = source.m_secondaryColorData.binding;
	}

	Vec2Array *lhs2 = (Vec2Array*)(m_fogCoordData.array.get());
	Vec2Array *rhs2 = (Vec2Array*)(source.m_fogCoordData.array.get());
	if(rhs2)
	{
		if(!lhs2) this->setFogCoordArray(new Vec2Array(*rhs2));
		else APPEND_Geometry_Data(lhs2,rhs2)
		m_fogCoordData.binding = source.m_fogCoordData.binding;
	}

	if(source.m_texCoordList.size() > 0 )
	{
		Vec2Array* lTexCoorArray = NULL;
		Vec2Array* rTexCoorArray = NULL;
		for(int i = 0; i<source.m_texCoordList.size(); i++)
		{
		    lTexCoorArray = (Vec2Array*)getTexCoordArray(i);
			rTexCoorArray = (Vec2Array*)source.getTexCoordArray(i);
			if(rTexCoorArray)
            {
			    if(!lTexCoorArray)
					this->setTexCoordArray(i,rTexCoorArray);
	                //this->setTexCoordArray(i,new Vec2Array(*rTexCoorArray));
				else 
					lTexCoorArray->insert(lTexCoorArray->end(),rTexCoorArray->begin(),rTexCoorArray->end());

				m_texCoordList[i].binding = source.m_texCoordList[i].binding;
			}
		}
	}

    if(source.m_vertexAttribList.size() > 0 )
	{
		Array* lAttribArray = NULL;
		Array* rAttribArray = NULL;
		for(int i = 0; i<source.m_vertexAttribList.size(); i++)
		{
			lAttribArray = (Array*)getVertexAttribArray(i);
			rAttribArray = (Array*)source.getVertexAttribArray(i);
			if(rAttribArray)
			{
				if(!lAttribArray)
				{
					this->setVertexAttribArray(i,rAttribArray);
					this->setVertexAttribBinding(i,BIND_PER_VERTEX);
				}
				else
				{
                    switch(rAttribArray->getType()) 
					{
					case Array::FloatArrayType:
						((FloatArray *)lAttribArray)->insert(((FloatArray *)lAttribArray)->end(),((FloatArray *)rAttribArray)->begin(),((FloatArray *)rAttribArray)->end());
						break;
					case Array::Vec2ArrayType:
						((Vec2Array *)lAttribArray)->insert(((Vec2Array *)lAttribArray)->end(),((Vec2Array *)rAttribArray)->begin(),((Vec2Array *)rAttribArray)->end());
						break;
					case Array::Vec3ArrayType:
						((Vec3Array *)lAttribArray)->insert(((Vec3Array *)lAttribArray)->end(),((Vec3Array *)rAttribArray)->begin(),((Vec3Array *)rAttribArray)->end());
						break;
					case Array::Vec4ArrayType:
						((Vec4Array *)lAttribArray)->insert(((Vec4Array *)lAttribArray)->end(),((Vec4Array *)rAttribArray)->begin(),((Vec4Array *)rAttribArray)->end());
						break;
                    }
				}
			}
		}
	}

	computeFastPathsUsed();
	dirtyDisplayList();
    return true;
}

void crGeometry::releaseObjects(crState* state)
{
	crDrawable::releaseObjects(state);
	int count = m_primitives.size();
	for (int i = 0; i < count; i++)
	{
		if (m_primitives[i].valid())
			m_primitives[i]->releaseObjects(state);
	}
	//for( PrimitiveList::iterator itr = m_primitives.begin();
	//	 itr != m_primitives.end();
	//	 ++itr )
	//{
 //       (*itr)->releaseObjects(state);
	//}
}
#include <Driver/GLDrv/crGeometryDrv.h>