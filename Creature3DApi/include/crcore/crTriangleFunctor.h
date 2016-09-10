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
#ifndef CRCORE_CRTRIANGLEFUNCTOR
#define CRCORE_CRTRIANGLEFUNCTOR 1

#include <CRCore/crDrawable.h>
#include <CRCore/crNotify.h>
#include <CRCore/crPrimitive.h>

namespace CRCore{

template<class T>
class crTriangleFunctor : public crDrawable::PrimitiveFunctor, public T
{
public:

    crTriangleFunctor()
    {
        m_vertexArraySize=0;
        m_vertexArrayPtr=0;
        m_modeCache=0;
        m_treatVertexDataAsTemporary=false;
    }
    
    virtual ~crTriangleFunctor() {}
    
    void setTreatVertexDataAsTemporary(bool treatVertexDataAsTemporary) { m_treatVertexDataAsTemporary=treatVertexDataAsTemporary; }
    bool getTreatVertexDataAsTemporary() const { return m_treatVertexDataAsTemporary; }

    virtual void setVertexArray(unsigned int,const crVector2*) 
    {
        notify(WARN)<<"Triangle Functor does not support crVector2* vertex arrays"<<std::endl;
    }

    virtual void setVertexArray(unsigned int count,const crVector3* vertices)
    {
        m_vertexArraySize = count;
        m_vertexArrayPtr = vertices;
    }

    virtual void setVertexArray(unsigned int,const crVector4* ) 
    {
        notify(WARN)<<"Triangle Functor does not support crVector4* vertex arrays"<<std::endl;
    }


    virtual void drawArrays(unsigned int mode,int first,int count)
    {
        if (m_vertexArrayPtr==0 && count==0) return;
    
        switch(mode)
        {
		case(CRCore::crPrimitive::PT_TRIANGLES):
            {
                const crVector3* vlast = &m_vertexArrayPtr[first+count];
                for(const crVector3* vptr=&m_vertexArrayPtr[first];vptr<vlast;vptr+=3)
                    operator()(*(vptr),*(vptr+1),*(vptr+2),m_treatVertexDataAsTemporary);
                break;
            }
            case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
            {
                const crVector3* vptr = &m_vertexArrayPtr[first];
                for(int i=2;i<count;++i,++vptr)
                {
		    if ((i%2)) operator()(*(vptr),*(vptr+2),*(vptr+1),m_treatVertexDataAsTemporary);
		    else       operator()(*(vptr),*(vptr+1),*(vptr+2),m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUADS):
            {
                const crVector3* vptr = &m_vertexArrayPtr[first];
                for(int i=3;i<count;i+=4,vptr+=4)
                {
                    operator()(*(vptr),*(vptr+1),*(vptr+2),m_treatVertexDataAsTemporary);
                    operator()(*(vptr),*(vptr+2),*(vptr+3),m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUAD_STRIP):
            {
                const crVector3* vptr = &m_vertexArrayPtr[first];
                for(int i=3;i<count;i+=2,vptr+=2)
                {
                    operator()(*(vptr),*(vptr+1),*(vptr+2),m_treatVertexDataAsTemporary);
                    operator()(*(vptr+1),*(vptr+3),*(vptr+2),m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POLYGON): // treat polygons as CRCore::crPrimitive::PT_TRIANGLE_FAN
            case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
            {
                const crVector3* vfirst = &m_vertexArrayPtr[first];
                const crVector3* vptr = vfirst+1;
                for(int i=2;i<count;++i,++vptr)
                {
                    operator()(*(vfirst),*(vptr),*(vptr+1),m_treatVertexDataAsTemporary);
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
    
    virtual void drawElements(unsigned int mode,int count,const unsigned char* indices)
    {
        if (indices==0 || count==0) return;
    
        typedef const unsigned char* IndexPointer;
    
        switch(mode)
        {
            case(CRCore::crPrimitive::PT_TRIANGLES):
            {
                IndexPointer ilast = &indices[count];
                for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3)
                    operator()(m_vertexArrayPtr[*iptr],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                break;
            }
            case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
            {
                IndexPointer iptr = indices;
                for(int i=2;i<count;++i,++iptr)
                {
		    if ((i%2)) operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+2)],m_vertexArrayPtr[*(iptr+1)],m_treatVertexDataAsTemporary);
		    else       operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUADS):
            {
                IndexPointer iptr = indices;
                for(int i=3;i<count;i+=4,iptr+=4)
                {
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+2)],m_vertexArrayPtr[*(iptr+3)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUAD_STRIP):
            {
                IndexPointer iptr = indices;
                for(int i=3;i<count;i+=2,iptr+=2)
                {
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                    operator()(m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+3)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POLYGON): // treat polygons as CRCore::crPrimitive::PT_TRIANGLE_FAN
            case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
            {
                IndexPointer iptr = indices;
                const crVector3& vfirst = m_vertexArrayPtr[*iptr];
                ++iptr;
                for(int i=2;i<count;++i,++iptr)
                {
                    operator()(vfirst,m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_treatVertexDataAsTemporary);
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

    virtual void drawElements(unsigned int mode,int count,const unsigned short* indices)
    {
        if (indices==0 || count==0) return;
    
        typedef const unsigned short* IndexPointer;
    
        switch(mode)
        {
            case(CRCore::crPrimitive::PT_TRIANGLES):
            {
                IndexPointer ilast = &indices[count];
                for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3)
                {
                    operator()(m_vertexArrayPtr[*iptr],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                }    
                break;
            }
            case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
            {
                IndexPointer iptr = indices;
                for(int i=2;i<count;++i,++iptr)
                {
		    if ((i%2)) operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+2)],m_vertexArrayPtr[*(iptr+1)],m_treatVertexDataAsTemporary);
		    else       operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUADS):
            {
                IndexPointer iptr = indices;
                for(int i=3;i<count;i+=4,iptr+=4)
                {
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+2)],m_vertexArrayPtr[*(iptr+3)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUAD_STRIP):
            {
                IndexPointer iptr = indices;
                for(int i=3;i<count;i+=2,iptr+=2)
                {
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                    operator()(m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+3)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POLYGON): // treat polygons as CRCore::crPrimitive::PT_TRIANGLE_FAN
            case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
            {
                IndexPointer iptr = indices;
                const crVector3& vfirst = m_vertexArrayPtr[*iptr];
                ++iptr;
                for(int i=2;i<count;++i,++iptr)
                {
                    operator()(vfirst,m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_treatVertexDataAsTemporary);
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

    virtual void drawElements(unsigned int mode,int count,const unsigned int* indices)
    {
        if (indices==0 || count==0) return;
    
        typedef const unsigned int* IndexPointer;
    
        switch(mode)
        {
            case(CRCore::crPrimitive::PT_TRIANGLES):
            {
                IndexPointer ilast = &indices[count];
                for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3)
                    operator()(m_vertexArrayPtr[*iptr],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                break;
            }
            case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
            {
                IndexPointer iptr = indices;
                for(int i=2;i<count;++i,++iptr)
                {
		    if ((i%2)) operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+2)],m_vertexArrayPtr[*(iptr+1)],m_treatVertexDataAsTemporary);
		    else       operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUADS):
            {
                IndexPointer iptr = indices;
                for(int i=3;i<count;i+=4,iptr+=4)
                {
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+2)],m_vertexArrayPtr[*(iptr+3)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_QUAD_STRIP):
            {
                IndexPointer iptr = indices;
                for(int i=3;i<count;i+=2,iptr+=2)
                {
                    operator()(m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                    operator()(m_vertexArrayPtr[*(iptr+1)],m_vertexArrayPtr[*(iptr+3)],m_vertexArrayPtr[*(iptr+2)],m_treatVertexDataAsTemporary);
                }
                break;
            }
            case(CRCore::crPrimitive::PT_POLYGON): // treat polygons as CRCore::crPrimitive::PT_TRIANGLE_FAN
            case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
            {
                IndexPointer iptr = indices;
                const crVector3& vfirst = m_vertexArrayPtr[*iptr];
                ++iptr;
                for(int i=2;i<count;++i,++iptr)
                {
                    operator()(vfirst,m_vertexArrayPtr[*(iptr)],m_vertexArrayPtr[*(iptr+1)],m_treatVertexDataAsTemporary);
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



    /** begin(..),vertex(..) & end() are convinience methods for adapting 
     *  non vertex array primitives to vertex array based primitives.
     * this is done to simplify the implementation of primtive functor
     * subclasses - users only need override drawArray and drawElements.*/
    virtual void begin(unsigned int mode)
    {
        m_modeCache = mode;
        m_vertexCache.clear();
    }

    virtual void vertex(const crVector2& vert) { m_vertexCache.push_back(CRCore::crVector3(vert[0],vert[1],0.0f)); }
    virtual void vertex(const crVector3& vert) { m_vertexCache.push_back(vert); }
    virtual void vertex(const crVector4& vert) { m_vertexCache.push_back(CRCore::crVector3(vert[0],vert[1],vert[2])/vert[3]); }
    virtual void vertex(float x,float y) { m_vertexCache.push_back(CRCore::crVector3(x,y,0.0f)); }
    virtual void vertex(float x,float y,float z) { m_vertexCache.push_back(CRCore::crVector3(x,y,z)); }
    virtual void vertex(float x,float y,float z,float w) { m_vertexCache.push_back(CRCore::crVector3(x,y,z)/w); }
    virtual void end()
    {
        if (!m_vertexCache.empty())
        {
            setVertexArray(m_vertexCache.size(),&m_vertexCache.front());
            m_treatVertexDataAsTemporary = true;
            drawArrays(m_modeCache,0,m_vertexCache.size());
        }
    }

protected:


    unsigned int             m_vertexArraySize;
    const crVector3*         m_vertexArrayPtr;

    unsigned int             m_modeCache;
    std::vector<crVector3>   m_vertexCache;
    bool                     m_treatVertexDataAsTemporary;
};


}

#endif
