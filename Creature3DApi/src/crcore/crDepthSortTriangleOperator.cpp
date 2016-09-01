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
#include <CRCore/crDepthSortTriangleOperator.h>
#include <CRCore/crMath.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

//void crDepthSortTriangleOperator::operator()(unsigned int p1, unsigned int p2, unsigned int p3)
//{
//	crVector3 center = (m_vertices[p1] + m_vertices[p2] + m_vertices[p3]) / 3.0f;
//    m_depthIndexVec.push_back(DepthIndexPair((m_eyeLocal - center).length(),crVector3i(p1,p2,p3)));
//}

void crDepthSortTriangleOperator::drawElements(unsigned int mode,GLsizei count,const GLubyte* indices)
{
	if (indices==0 || count==0) return;

	typedef const GLubyte* IndexPointer;

	switch(mode)
	{
	case(CRCore::crPrimitive::PT_TRIANGLES):
		{
			//m_depthIndexVec.clear();
			m_depthIndexVec.resize(count / 3);
			IndexPointer ilast = &indices[count];
			int i = 0;
			crVector3 center;
			for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3,i++)
			{
				center = (m_vertices[*iptr] + m_vertices[*(iptr+1)] + m_vertices[*(iptr+2)]) / 3.0f;
                m_depthIndexVec[i] = DepthIndexPair((m_eyeLocal - center).length(),crVector3i(*iptr,*(iptr+1),*(iptr+2)));
				//this->operator()(*iptr,*(iptr+1),*(iptr+2));
			}
			std::sort(m_depthIndexVec.begin(),m_depthIndexVec.end(),BackToFrontSortFunctor<crDepthSortTriangleOperator::DepthIndexPair>());
            GLubyte *_indices = const_cast<GLubyte*>(indices);
			for( i = 0; i<m_depthIndexVec.size(); i++)
			{
				_indices[i * 3    ] = m_depthIndexVec[i].second[0];
				_indices[i * 3 + 1] = m_depthIndexVec[i].second[1];
				_indices[i * 3 + 2] = m_depthIndexVec[i].second[2];
			}
			break;
		}
	case(CRCore::crPrimitive::PT_QUADS):
		{
			//IndexPointer iptr = indices;
			//for(GLsizei i=3;i<count;i+=4,iptr+=4)
			//{
			//	this->operator()(*(iptr),*(iptr+1),*(iptr+2));
			//	this->operator()(*(iptr),*(iptr+2),*(iptr+3));
			//}
			break;
		}
	}
}    

void crDepthSortTriangleOperator::drawElements(unsigned int mode,GLsizei count,const GLushort* indices)
{
	if (indices==0 || count==0) return;

	typedef const GLushort* IndexPointer;

	switch(mode)
	{
	case(CRCore::crPrimitive::PT_TRIANGLES):
		{
			//m_depthIndexVec.clear();
			//m_depthIndexVec.reserve(count / 3);
			m_depthIndexVec.resize(count / 3);
			IndexPointer ilast = &indices[count];
			int i = 0;
			crVector3 center;
			for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3,i++)
			{
				center = (m_vertices[*iptr] + m_vertices[*(iptr+1)] + m_vertices[*(iptr+2)]) / 3.0f;
				m_depthIndexVec[i] = DepthIndexPair((m_eyeLocal - center).length(),crVector3i(*iptr,*(iptr+1),*(iptr+2)));
				//this->operator()(*iptr,*(iptr+1),*(iptr+2));
			}
			std::sort(m_depthIndexVec.begin(),m_depthIndexVec.end(),BackToFrontSortFunctor<crDepthSortTriangleOperator::DepthIndexPair>());
			GLushort *_indices = const_cast<GLushort*>(indices);

			for( i = 0; i<m_depthIndexVec.size(); i++)
			{
				//CRCore::notify(CRCore::FATAL)<<"crDepthSortedObject::depthSorted() m_depthIndexVec: i = "<<i<<" depth = "<<m_depthIndexVec[i].first<<std::endl;

				_indices[i * 3    ] = m_depthIndexVec[i].second[0];
				_indices[i * 3 + 1] = m_depthIndexVec[i].second[1];
				_indices[i * 3 + 2] = m_depthIndexVec[i].second[2];
			}
			break;
		}
	case(CRCore::crPrimitive::PT_QUADS):
		{
			CRCore::notify(CRCore::FATAL)<<"crDepthSortedObject::depthSorted() PT_QUADS"<<std::endl;

			//IndexPointer iptr = indices;
			//for(GLsizei i=3;i<count;i+=4,iptr+=4)
			//{
			//	this->operator()(*(iptr),*(iptr+1),*(iptr+2));
			//	this->operator()(*(iptr),*(iptr+2),*(iptr+3));
			//}
			break;
		}
	}
}    

void crDepthSortTriangleOperator::drawElements(unsigned int mode,GLsizei count,const unsigned int* indices)
{
	if (indices==0 || count==0) return;

	typedef const unsigned int* IndexPointer;

	switch(mode)
	{
	case(CRCore::crPrimitive::PT_TRIANGLES):
		{
			//m_depthIndexVec.clear();
			//m_depthIndexVec.reserve(count / 3);
			m_depthIndexVec.resize(count / 3);
			IndexPointer ilast = &indices[count];
			int i = 0;
			crVector3 center;
			for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3,i++)
			{
				center = (m_vertices[*iptr] + m_vertices[*(iptr+1)] + m_vertices[*(iptr+2)]) / 3.0f;
				m_depthIndexVec[i] = DepthIndexPair((m_eyeLocal - center).length(),crVector3i(*iptr,*(iptr+1),*(iptr+2)));
				//this->operator()(*iptr,*(iptr+1),*(iptr+2));
			}
			std::sort(m_depthIndexVec.begin(),m_depthIndexVec.end(),BackToFrontSortFunctor<crDepthSortTriangleOperator::DepthIndexPair>());
			unsigned int *_indices = const_cast<unsigned int*>(indices);

			for( i = 0; i<m_depthIndexVec.size(); i++)
			{
				_indices[i * 3    ] = m_depthIndexVec[i].second[0];
				_indices[i * 3 + 1] = m_depthIndexVec[i].second[1];
				_indices[i * 3 + 2] = m_depthIndexVec[i].second[2];
			}
			break;
		}
	case(CRCore::crPrimitive::PT_QUADS):
		{
			//IndexPointer iptr = indices;
			//for(GLsizei i=3;i<count;i+=4,iptr+=4)
			//{
			//	this->operator()(*(iptr),*(iptr+1),*(iptr+2));
			//	this->operator()(*(iptr),*(iptr+2),*(iptr+3));
			//}
			break;
		}
	}
}