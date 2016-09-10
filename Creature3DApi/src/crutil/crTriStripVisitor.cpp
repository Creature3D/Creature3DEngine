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
#include <CRCore/crNotify.h>
#include <CRCore/crTriangleIndexFunctor.h>

#include <CRUtil/crTriStripVisitor.h>
#include <CRUtil/crSmoothingVisitor.h>

#include <stdio.h>
#include <algorithm>
#include <map>
#include <iterator>
#include "tristripper/include/tri_stripper.h"
#include "tristripper/src/tri_stripper.cpp"
#include "tristripper/src/policy.cpp"
#include "tristripper/src/connectivity_graph.cpp"
using namespace CRCore;
using namespace CRUtil;

typedef std::vector<unsigned int> IndexList;

class WriteValue : public CRCore::ConstValueVisitor
{
    public:
        WriteValue(std::ostream& o):m_o(o) {}
        
        std::ostream& m_o;
               
        virtual void apply(const signed char& v) { m_o << v; }
        virtual void apply(const short& v) { m_o << v; }
        virtual void apply(const int& v) { m_o << v; }
        virtual void apply(const unsigned short& v) { m_o << v; }
        virtual void apply(const unsigned char& v) { m_o << v; }
        virtual void apply(const unsigned int& v) { m_o << v; }
        virtual void apply(const float& v) { m_o << v; }
        virtual void apply(const crVector4ub& v) { m_o << v; }
        virtual void apply(const crVector2& v) { m_o << v; }
        virtual void apply(const crVector3& v) { m_o << v; }
        virtual void apply(const crVector4& v) { m_o << v; }
};


struct VertexAttribComparitor
{
    VertexAttribComparitor(CRCore::crGeometry& geometry)
    {
        add(geometry.getVertexArray(),CRCore::crGeometry::BIND_PER_VERTEX);
        add(geometry.getNormalArray(),geometry.getNormalBinding());
        add(geometry.getColorArray(),geometry.getColorBinding());
        add(geometry.getSecondaryColorArray(),geometry.getSecondaryColorBinding());
        add(geometry.getFogCoordArray(),geometry.getFogCoordBinding());
        unsigned int i;
        for(i=0;i<geometry.getNumTexCoordArrays();++i)
        {
            add(geometry.getTexCoordArray(i),CRCore::crGeometry::BIND_PER_VERTEX);
        }
        for(i=0;i<geometry.getNumVertexAttribArrays();++i)
        {
            add(geometry.getVertexAttribArray(i),geometry.getVertexAttribBinding(i));
        }
    }
    
    void add(CRCore::Array* array, CRCore::crGeometry::AttributeBinding binding)
    {
        if (binding==CRCore::crGeometry::BIND_PER_VERTEX && array)
            m_arrayList.push_back(array);
    }
    
    typedef std::vector<CRCore::Array*> ArrayList;
    
    ArrayList m_arrayList;
    
    bool operator() (unsigned int lhs, unsigned int rhs) const
    {
        for(ArrayList::const_iterator itr=m_arrayList.begin();
            itr!=m_arrayList.end();
            ++itr)
        {
            int compare = (*itr)->compare(lhs,rhs);
            if (compare==-1) return true;
            if (compare==1) return false;
        }
        return false;
    }   

    int compare(unsigned int lhs, unsigned int rhs)
    {
        for(ArrayList::iterator itr=m_arrayList.begin();
            itr!=m_arrayList.end();
            ++itr)
        {
            int compare = (*itr)->compare(lhs,rhs);
            if (compare==-1) return -1;
            if (compare==1) return 1;
        }
//         
//         WriteValue wv(std::cout);
//         
//         std::cout<<"Values equal"<<std::endl;
//         for(ArrayList::iterator itr=m_arrayList.begin();
//             itr!=m_arrayList.end();
//             ++itr)
//         {
//             std::cout<<"   lhs["<<lhs<<"]="; (*itr)->accept(lhs,wv);
//             std::cout<<"   rhs["<<rhs<<"]="; (*itr)->accept(rhs,wv);
//             std::cout<<std::endl;
//         }
        

        return 0;
    }
    
    void accept(CRCore::ArrayVisitor& av)
    {
        for(ArrayList::iterator itr=m_arrayList.begin();
            itr!=m_arrayList.end();
            ++itr)
        {
            (*itr)->accept(av);
        }
    }

};

class RemapArray : public CRCore::ArrayVisitor
{
    public:
        RemapArray(const IndexList& remapping):m_remapping(remapping) {}
        
        const IndexList& m_remapping;
        
        template<class T>
        inline void remap(T& array)
        {
            for(unsigned int i=0;i<m_remapping.size();++i)
            {
                if (i!=m_remapping[i])
                {
                    array[i] = array[m_remapping[i]];
                }
            }
            array.erase(array.begin()+m_remapping.size(),array.end());
        }
        
        virtual void apply(CRCore::Array&) {}
        virtual void apply(CRCore::ByteArray& array) { remap(array); }
        virtual void apply(CRCore::ShortArray& array) { remap(array); }
        virtual void apply(CRCore::IntArray& array) { remap(array); }
        virtual void apply(CRCore::UByteArray& array) { remap(array); }
        virtual void apply(CRCore::UShortArray& array) { remap(array); }
        virtual void apply(CRCore::UIntArray& array) { remap(array); }
        virtual void apply(CRCore::Vec4ubArray& array) { remap(array); }
        virtual void apply(CRCore::FloatArray& array) { remap(array); }
        virtual void apply(CRCore::Vec2Array& array) { remap(array); }
        virtual void apply(CRCore::Vec3Array& array) { remap(array); }
        virtual void apply(CRCore::Vec4Array& array) { remap(array); }
};

struct MyTriangleOperator
{

    IndexList                                m_remapIndices;
    triangle_stripper::indices m_in_indices;

    inline void operator()(unsigned int p1, unsigned int p2, unsigned int p3)
    {
        if (m_remapIndices.empty())
        {
            m_in_indices.push_back(p1);
            m_in_indices.push_back(p2);
            m_in_indices.push_back(p3);
        }
        else
        {
            m_in_indices.push_back(m_remapIndices[p1]);
            m_in_indices.push_back(m_remapIndices[p2]);
            m_in_indices.push_back(m_remapIndices[p3]);
        }
    }

};
typedef CRCore::crTriangleIndexFunctor<MyTriangleOperator> MyTriangleIndexFunctor;

void crTriStripVisitor::stripify(crGeometry& geom)
{
    if (geom.getNormalBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE ||
        geom.getNormalBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE_SET) return;

    if (geom.getColorBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE ||
        geom.getColorBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE_SET) return;
    
    if (geom.getSecondaryColorBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE ||
        geom.getSecondaryColorBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE_SET) return;

    if (geom.getFogCoordBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE ||
        geom.getFogCoordBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE_SET) return;

    // no point tri stripping if we don't have enough vertices.
    if (geom.getVertexArray()->getNumElements()<3) return;


    // check to see if vertex attributes indices exists, if so expand them to remove them
    if (geom.suitableForOptimization())
    {
        // removing coord indices
       // CRCore::notify(CRCore::INFO)<<"crTriStripVisitor::stripify(crGeometry&): Removing attribute indices"<<std::endl;
        geom.copyToAndOptimize(geom);
    }

    // check for the existance of surface primitives
    unsigned int numSurfacePrimitives = 0;
    unsigned int numNonSurfacePrimitives = 0;
    crGeometry::PrimitiveList& primitives = geom.getPrimitiveList();
    crGeometry::PrimitiveList::iterator itr;
    for(itr=primitives.begin();
        itr!=primitives.end();
        ++itr)
    {
        switch((*itr)->getMode())
        {
            case(crPrimitive::PT_TRIANGLES):
            case(crPrimitive::PT_TRIANGLE_STRIP):
            case(crPrimitive::PT_TRIANGLE_FAN):
            case(crPrimitive::PT_QUADS):
            case(crPrimitive::PT_QUAD_STRIP):
            case(crPrimitive::PT_POLYGON):
                ++numSurfacePrimitives;
                break;
            default:
                ++numNonSurfacePrimitives;
                break;
                
        }
    }
    
    // nothitng to tri strip leave.
    if (!numSurfacePrimitives) return;
    
    // compute duplicate vertices
    
    typedef std::vector<unsigned int> IndexList;
    unsigned int numVertices = geom.getVertexArray()->getNumElements();
    IndexList indices(numVertices);
    unsigned int i,j;
    for(i=0;i<numVertices;++i)
    {
        indices[i] = i;
    }
    
    VertexAttribComparitor arrayComparitor(geom);
    std::sort(indices.begin(),indices.end(),arrayComparitor);

    unsigned int lastUnique = 0;
    unsigned int numUnique = 1;
    unsigned int numDuplicate = 0;
    for(i=1;i<numVertices;++i)
    {
        if (arrayComparitor.compare(indices[lastUnique],indices[i])==0)
        {
            //std::cout<<"  found duplicate "<<indices[lastUnique]<<" and "<<indices[i]<<std::endl;
            ++numDuplicate;
        }
        else 
        {
            //std::cout<<"  unique "<<indices[i]<<std::endl;
            lastUnique = i;
            ++numUnique;
        }
        
    }
//     std::cout<<"  Number of duplicates "<<numDuplicate<<std::endl;
//     std::cout<<"  Number of unique "<<numUnique<<std::endl;
//     std::cout<<"  Total number of vertices required "<<numUnique<<" vs original "<<numVertices<<std::endl;
//     std::cout<<"  % size "<<(float)numUnique/(float)numVertices*100.0f<<std::endl;
    
    IndexList remapDuplicatesToOrignals(numVertices);
    lastUnique = 0;
    for(i=1;i<numVertices;++i)
    {
        if (arrayComparitor.compare(indices[lastUnique],indices[i])!=0)
        {
            // found a new vertex entry, so previous run of duplicates needs
            // to be put together.
            unsigned int min_index = indices[lastUnique];
            for(j=lastUnique+1;j<i;++j)
            {
                min_index = CRCore::minimum(min_index,indices[j]);
            }
            for(j=lastUnique;j<i;++j)
            {
                remapDuplicatesToOrignals[indices[j]]=min_index;
            }
            lastUnique = i;
        }
        
    }
    unsigned int min_index = indices[lastUnique];
    for(j=lastUnique+1;j<i;++j)
    {
        min_index = CRCore::minimum(min_index,indices[j]);
    }
    for(j=lastUnique;j<i;++j)
    {
        remapDuplicatesToOrignals[indices[j]]=min_index;
    }


    // copy the arrays.    
    IndexList finalMapping(numVertices);
    IndexList copyMapping;
    copyMapping.reserve(numUnique);
    unsigned int currentIndex=0;
    for(i=0;i<numVertices;++i)
    {
        if (remapDuplicatesToOrignals[i]==i) 
        {
            finalMapping[i] = currentIndex;
            copyMapping.push_back(i);
            currentIndex++;
        }
    }
    
    for(i=0;i<numVertices;++i)
    {
        if (remapDuplicatesToOrignals[i]!=i) 
        {
            finalMapping[i] = finalMapping[remapDuplicatesToOrignals[i]];
        }
    }
   
    
    MyTriangleIndexFunctor taf;
    taf.m_remapIndices.swap(finalMapping);

    crGeometry::PrimitiveList new_primitives;
    new_primitives.reserve(primitives.size());

    for(itr=primitives.begin();
        itr!=primitives.end();
        ++itr)
    {
        switch((*itr)->getMode())
        {
            case(crPrimitive::PT_TRIANGLES):
            case(crPrimitive::PT_TRIANGLE_STRIP):
            case(crPrimitive::PT_TRIANGLE_FAN):
            case(crPrimitive::PT_QUADS):
            case(crPrimitive::PT_QUAD_STRIP):
            case(crPrimitive::PT_POLYGON):
                (*itr)->accept(taf);
                break;
            default:
                new_primitives.push_back(*itr);
                break;

        }
    }
    
    float minimum_ratio_of_indices_to_unique_vertices = 1;
    float ratio_of_indices_to_unique_vertices = ((float)taf.m_in_indices.size()/(float)numUnique);

    //CRCore::notify(CRCore::INFO)<<"crTriStripVisitor::stripify(crGeometry&): Number of indices"<<taf.m_in_indices.size()<<" numUnique"<< numUnique << std::endl;
    //CRCore::notify(CRCore::INFO)<<"crTriStripVisitor::stripify(crGeometry&):     ratio indices/numUnique"<< ratio_of_indices_to_unique_vertices << std::endl;
    
    // only tri strip if there is point in doing so.
    if (!taf.m_in_indices.empty() && ratio_of_indices_to_unique_vertices>=minimum_ratio_of_indices_to_unique_vertices)
    {
        //CRCore::notify(CRCore::INFO)<<"crTriStripVisitor::stripify(crGeometry&):     doing tri strip"<< std::endl;

        unsigned int in_numVertices = 0;
        for(triangle_stripper::indices::iterator itr=taf.m_in_indices.begin();
            itr!=taf.m_in_indices.end();
            ++itr)
        {
            if (*itr>in_numVertices) in_numVertices=*itr;
        }
        // the largest indice is in_numVertices, but indices start at 0
        // so increment to give to the corrent number of verticies.
        ++in_numVertices;
        
        // remap any shared vertex attributes
        RemapArray ra(copyMapping);
        arrayComparitor.accept(ra);

		//try
		//{
			triangle_stripper::tri_stripper stripifier(taf.m_in_indices);
			stripifier.SetCacheSize(m_cacheSize);
			stripifier.SetMinStripSize(m_minStripSize);

			triangle_stripper::primitive_vector outPrimitives;
			stripifier.Strip(&outPrimitives);
        
			triangle_stripper::primitive_vector::iterator pitr;
			if (m_generateFourPointPrimitivesQuads)
			{
				typedef triangle_stripper::primitive_vector::iterator prim_iterator;
				typedef std::multimap<unsigned int,prim_iterator> QuadMap;
				QuadMap quadMap;

				// pick out quads and place them in the quadMap, and also look for the max 
				for(pitr=outPrimitives.begin();
					pitr!=outPrimitives.end();
					++pitr)
				{
					if (pitr->Indices.size()==4)
					{
						std::swap(pitr->Indices[2],pitr->Indices[3]);
						unsigned int minValue = *(std::max_element(pitr->Indices.begin(),pitr->Indices.end()));
						quadMap.insert(std::pair<unsigned int,prim_iterator>(minValue,pitr));
					}
				}

				// handle the quads
				if (!quadMap.empty())
				{
					IndexList indices;
					indices.reserve(4*quadMap.size());
		            
					// adds all the quads into the quad primitive, in ascending order 
					// and the QuadMap stores the quad's in ascending order.
					for(QuadMap::iterator qitr=quadMap.begin();
						qitr!=quadMap.end();
						++qitr)
					{
						pitr = qitr->second;

						unsigned int min_pos = 0;
						for(i=1;i<4;++i)
						{
							if (pitr->Indices[min_pos]>pitr->Indices[i]) 
								min_pos = i;
						}
						indices.push_back(pitr->Indices[min_pos]);
						indices.push_back(pitr->Indices[(min_pos+1)%4]);
						indices.push_back(pitr->Indices[(min_pos+2)%4]);
						indices.push_back(pitr->Indices[(min_pos+3)%4]);
					}            
		        
					bool inOrder = true;
					unsigned int previousValue = indices.front();
					for(IndexList::iterator qi_itr=indices.begin()+1;
						qi_itr!=indices.end() && inOrder;
						++qi_itr)
					{
						inOrder = (previousValue+1)==*qi_itr;
						previousValue = *qi_itr;
					}
        
					if (inOrder)
					{
						new_primitives.push_back(new DrawArrays(crPrimitive::PT_QUADS,indices.front(),indices.size()));
					}
					else
					{
						unsigned int maxValue = *(std::max_element(indices.begin(),indices.end()));

						if (maxValue>=65536)
						{
							DrawElementsUInt* elements = new DrawElementsUInt(crPrimitive::PT_QUADS);
							DrawElementsUInt::PT_IndexArray &lhs = elements->getIndexArray();
							lhs.reserve(indices.size());
							std::copy(indices.begin(),indices.end(),std::back_inserter(lhs));
							new_primitives.push_back(elements);
						}
						else
						{
							DrawElementsUShort* elements = new DrawElementsUShort(crPrimitive::PT_QUADS);
							DrawElementsUShort::PT_IndexArray &lhs = elements->getIndexArray();
							lhs.reserve(indices.size());
							std::copy(indices.begin(),indices.end(),std::back_inserter(lhs));
							new_primitives.push_back(elements);
						}
					}
				}  
			}
        
			// handle non quad primitives    
			for(pitr=outPrimitives.begin();
				pitr!=outPrimitives.end();
				++pitr)
			{
				if (!m_generateFourPointPrimitivesQuads || pitr->Indices.size()!=4)
				{
					bool inOrder = true;
					unsigned int previousValue = pitr->Indices.front();
					for(triangle_stripper::indices::iterator qi_itr=pitr->Indices.begin()+1;
						qi_itr!=pitr->Indices.end() && inOrder;
						++qi_itr)
					{
						inOrder = (previousValue+1)==*qi_itr;
						previousValue = *qi_itr;
					}

					if (inOrder)
					{
						DrawArrays* elements = new DrawArrays(pitr->Type,0/*pitr->Indices.front()*/,pitr->Indices.size());
						DrawArrays::PT_IndexArray &lhs = elements->getIndexArray();
						lhs.reserve(pitr->Indices.size());
						std::copy(pitr->Indices.begin(),pitr->Indices.end(),std::back_inserter(lhs));
						new_primitives.push_back(elements);
					}
					else
					{
						unsigned int maxValue = *(std::max_element(pitr->Indices.begin(),pitr->Indices.end()));
						if (maxValue>=65536)
						{
							DrawElementsUInt* elements = new DrawElementsUInt(pitr->Type);
							DrawElementsUInt::PT_IndexArray &lhs = elements->getIndexArray();
							lhs.reserve(pitr->Indices.size());
							std::copy(pitr->Indices.begin(),pitr->Indices.end(),std::back_inserter(lhs));
							new_primitives.push_back(elements);
						}
						else
						{
							DrawElementsUShort* elements = new DrawElementsUShort(pitr->Type);
							DrawElementsUShort::PT_IndexArray &lhs = elements->getIndexArray();
							lhs.reserve(pitr->Indices.size());
							std::copy(pitr->Indices.begin(),pitr->Indices.end(),std::back_inserter(lhs));
							new_primitives.push_back(elements);
						}
					}
				}
			}
        
			geom.setPrimitiveList(new_primitives);
        
			#if 0 
			// debugging code for indentifying the tri-strips.       
					CRCore::Vec4Array* colors = new CRCore::Vec4Array(new_primitives.size());
					for(i=0;i<colors->size();++i)
					{
						(*colors)[i].set(((float)rand()/(float)RAND_MAX),
										((float)rand()/(float)RAND_MAX),
										((float)rand()/(float)RAND_MAX),
										1.0f);
					}
					geom.setColorArray(colors);
					geom.setColorBinding(CRCore::crGeometry::BIND_PER_PRIMITIVE_SET);
			#endif        
       //}
	   //catch(const char* errorMessage)
	   //{
		  // CRCore::notify(CRCore::WARN)<<"Warning: '"<<errorMessage<<"' exception thrown from triangle_stripper"<<std::endl;
	   //}
	   //catch(triangle_stripper::tri_stripper::triangles_indices_error&)
	   //{
		  // CRCore::notify(CRCore::WARN)<<"Warning: triangles_indices_error exception thrown from triangle_stripper"<<std::endl;
	   //}
	   //catch(...)
	   //{
		  // CRCore::notify(CRCore::WARN)<<"Warning: Unhandled exception thrown from triangle_stripper"<<std::endl;
	   //}
    }
    //else
    //{
        //CRCore::notify(CRCore::INFO)<<"crTriStripVisitor::stripify(crGeometry&):     not doing tri strip *****************"<< std::endl;
    //}
}

void crTriStripVisitor::stripify()
{
    for(GeometryList::iterator itr=m_geometryList.begin();
        itr!=m_geometryList.end();
        ++itr)
    {
        stripify(*(*itr));
        
        // CRUtil::crSmoothingVisitor sv;
        // sv.smooth(*(*itr));
    }
}

void crTriStripVisitor::apply(crObject& object)
{
    for(unsigned int i = 0; i < object.getNumDrawables(); ++i )
    {
        CRCore::crGeometry* geom = dynamic_cast<CRCore::crGeometry*>(object.getDrawable(i));
        if (geom) m_geometryList.insert(geom);
    }
}
