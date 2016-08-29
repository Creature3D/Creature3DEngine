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
#ifndef CRCORE_TRIANGLINDEXEFUNCTOR
#define CRCORE_TRIANGLINDEXEFUNCTOR 1

#include <CRCore/crDrawable.h>
#include <CRCore/crNotify.h>

namespace CRCore {

template<class T>
class crTriangleIndexFunctor : public crDrawable::PrimitiveIndexFunctor, public T
{
public:


    virtual void setVertexArray(unsigned int,const crVector2*) 
    {
    }

    virtual void setVertexArray(unsigned int ,const crVector3* )
    {
    }

    virtual void setVertexArray(unsigned int,const crVector4* ) 
    {
    }

    virtual void begin(unsigned int mode)
    {
        m_modeCache = mode;
        m_indexCache.clear();
    }

    virtual void vertex(unsigned int vert)
    {
        m_indexCache.push_back(vert);
    }

    virtual void end()
    {
        if (!m_indexCache.empty())
        {
            drawElements(m_modeCache,m_indexCache.size(),&m_indexCache.front());
        }
    }

    virtual void drawArrays(unsigned int mode,GLint first,GLsizei count)
    {
        switch(mode)
        {
            case(CRCore::crPrimitive::PT_TRIANGLES):
            {
                unsigned int pos=first;
                for(GLsizei i=2;i<count;i+=3,pos+=3)
                {
                    this->operator()(pos,pos+1,pos+2);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
             {
                unsigned int pos=first;
                for(GLsizei i=2;i<count;++i,++pos)
                {
		    if ((i%2)) this->operator()(pos,pos+2,pos+1);
		    else       this->operator()(pos,pos+1,pos+2);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUADS):
            {
                unsigned int pos=first;
                for(GLsizei i=3;i<count;i+=4,pos+=4)
                {
                    this->operator()(pos,pos+1,pos+2);
                    this->operator()(pos,pos+2,pos+3);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUAD_STRIP):
            {
                unsigned int pos=first;
                for(GLsizei i=3;i<count;i+=2,pos+=2)
                {
                    this->operator()(pos,pos+1,pos+2);
                    this->operator()(pos+1,pos+3,pos+2);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POLYGON): // treat polygons as CRCore::crPrimitive::PT_TRIANGLE_FAN
            case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
            {
                unsigned int pos=first+1;
                for(GLsizei i=2;i<count;++i,++pos)
                {
                    this->operator()(first,pos,pos+1);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POINTS):
            case(CRCore::crPrimitive::PT_LINES):
            case(CRCore::crPrimitive::PT_LINE_STRIP):
            case(CRCore::crPrimitive::PT_LINE_LOOP):
            default:
                // can't be converted into to triangles.
                break;
        }
    }
    
    virtual void drawElements(unsigned int mode,GLsizei count,const GLubyte* indices)
    {
        if (indices==0 || count==0) return;

        typedef const GLubyte* IndexPointer;
    
        switch(mode)
        {
            case(CRCore::crPrimitive::PT_TRIANGLES):
            {
                IndexPointer ilast = &indices[count];
                for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3)
                    this->operator()(*iptr,*(iptr+1),*(iptr+2));
                break;
            }
            case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=2;i<count;++i,++iptr)
                {
		    if ((i%2)) this->operator()(*(iptr),*(iptr+2),*(iptr+1));
		    else       this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUADS):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=3;i<count;i+=4,iptr+=4)
                {
                    this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                    this->operator()(*(iptr),*(iptr+2),*(iptr+3));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUAD_STRIP):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=3;i<count;i+=2,iptr+=2)
                {
                    this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                    this->operator()(*(iptr+1),*(iptr+3),*(iptr+2));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POLYGON): // treat polygons as CRCore::crPrimitive::PT_TRIANGLE_FAN
            case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
            {
                IndexPointer iptr = indices;
                unsigned int first = *iptr;
                ++iptr;
                for(GLsizei i=2;i<count;++i,++iptr)
                {
                    this->operator()(first,*(iptr),*(iptr+1));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POINTS):
            case(CRCore::crPrimitive::PT_LINES):
            case(CRCore::crPrimitive::PT_LINE_STRIP):
            case(CRCore::crPrimitive::PT_LINE_LOOP):
            default:
                // can't be converted into to triangles.
                break;
        }
    }    

    virtual void drawElements(unsigned int mode,GLsizei count,const GLushort* indices)
    {
        if (indices==0 || count==0) return;

        typedef const GLushort* IndexPointer;
    
        switch(mode)
        {
            case(CRCore::crPrimitive::PT_TRIANGLES):
            {
                IndexPointer ilast = &indices[count];
                for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3)
                    this->operator()(*iptr,*(iptr+1),*(iptr+2));
                break;
            }
            case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=2;i<count;++i,++iptr)
                {
		    if ((i%2)) this->operator()(*(iptr),*(iptr+2),*(iptr+1));
		    else       this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUADS):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=3;i<count;i+=4,iptr+=4)
                {
                    this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                    this->operator()(*(iptr),*(iptr+2),*(iptr+3));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUAD_STRIP):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=3;i<count;i+=2,iptr+=2)
                {
                    this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                    this->operator()(*(iptr+1),*(iptr+3),*(iptr+2));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POLYGON): // treat polygons as CRCore::crPrimitive::PT_TRIANGLE_FAN
            case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
            {
                IndexPointer iptr = indices;
                unsigned int first = *iptr;
                ++iptr;
                for(GLsizei i=2;i<count;++i,++iptr)
                {
                    this->operator()(first,*(iptr),*(iptr+1));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POINTS):
            case(CRCore::crPrimitive::PT_LINES):
            case(CRCore::crPrimitive::PT_LINE_STRIP):
            case(CRCore::crPrimitive::PT_LINE_LOOP):
            default:
                // can't be converted into to triangles.
                break;
        }
    }    

    virtual void drawElements(unsigned int mode,GLsizei count,const unsigned int* indices)
    {
        if (indices==0 || count==0) return;
    
        typedef const unsigned int* IndexPointer;
    
        switch(mode)
        {
            case(CRCore::crPrimitive::PT_TRIANGLES):
            {
                IndexPointer ilast = &indices[count];
                for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3)
                    this->operator()(*iptr,*(iptr+1),*(iptr+2));
                break;
            }
            case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=2;i<count;++i,++iptr)
                {
		    if ((i%2)) this->operator()(*(iptr),*(iptr+2),*(iptr+1));
		    else       this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUADS):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=3;i<count;i+=4,iptr+=4)
                {
                    this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                    this->operator()(*(iptr),*(iptr+2),*(iptr+3));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUAD_STRIP):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=3;i<count;i+=2,iptr+=2)
                {
                    this->operator()(*(iptr),*(iptr+1),*(iptr+2));
                    this->operator()(*(iptr+1),*(iptr+3),*(iptr+2));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POLYGON): // treat polygons as CRCore::crPrimitive::PT_TRIANGLE_FAN
            case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
            {
                IndexPointer iptr = indices;
                unsigned int first = *iptr;
                ++iptr;
                for(GLsizei i=2;i<count;++i,++iptr)
                {
                    this->operator()(first,*(iptr),*(iptr+1));
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POINTS):
            case(CRCore::crPrimitive::PT_LINES):
            case(CRCore::crPrimitive::PT_LINE_STRIP):
            case(CRCore::crPrimitive::PT_LINE_LOOP):
            default:
                // can't be converted into to triangles.
                break;
        }
    }    

    unsigned int               m_modeCache;
    std::vector<unsigned int>  m_indexCache;
};

}

#endif
