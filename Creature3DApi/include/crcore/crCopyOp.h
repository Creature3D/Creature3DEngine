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
#ifndef CRCORE_CRCOPYOP
#define CRCORE_CRCOPYOP 1

#include <CRCore\crExport.h>

namespace CRCore {

  class Referenced;
  class crBase;
  
  class crImage;
  class crTexture;
  class crStateSet;
  class crStateAttribute;
  class crNode;
  class crDrawable;
  class Array;
  class crPrimitive;
  class crShape;
  class crUniform;
  class crData;
  class crEventCallback;
  class CR_EXPORT crCopyOp
  {

  public:

    enum Options
    {
        SHALLOW_COPY    	    	= 0,
        DEEP_COPY_BASES 	    	= 1,
        DEEP_COPY_NODES 	    	= 2,
        DEEP_COPY_DRAWABLES     	= 4,
        DEEP_COPY_STATESETS     	= 8,
        DEEP_COPY_STATEATTRIBUTES 	= 16,
        DEEP_COPY_TEXTURES      	= 32,
        DEEP_COPY_IMAGES 	    	= 64,
        DEEP_COPY_ARRAYS 	    	= 128,
        DEEP_COPY_PRIMITIVES    	= 256,
        DEEP_COPY_SHAPES 	    	= 512,
        DEEP_COPY_UNIFORMS 	    	= 1024,
		DEEP_COPY_DATAS             = 2048,
		DEEP_COPY_Callbacks         = 4096,
        DEEP_COPY_ALL   	    	= 0xffffffff
    };

    typedef unsigned int CopyFlags;

    inline crCopyOp(CopyFlags flags=SHALLOW_COPY):m_flags(flags) {}
    virtual ~crCopyOp() {}

    virtual Referenced*     operator() (const Referenced* ref) const;
    virtual crBase*         operator() (const crBase* obj) const;
  
    virtual crNode*           operator() (const crNode* node) const;
    virtual crDrawable*       operator() (const crDrawable* drawable) const;
    virtual crStateSet*       operator() (const crStateSet* stateset) const;
    virtual crStateAttribute* operator() (const crStateAttribute* attr) const;
    virtual crTexture*        operator() (const crTexture* text) const;
    virtual crImage*          operator() (const crImage* image) const;
    virtual Array*          operator() (const Array* array) const;
    virtual crPrimitive*    operator() (const crPrimitive* primitives) const;
    virtual crShape*      	operator() (const crShape* shape) const;
	virtual crUniform*      operator() (const crUniform* shape) const;
	virtual crData*      	operator() (const crData* data) const;
	virtual crEventCallback*  operator() (const crEventCallback* callback) const;
  protected:

    CopyFlags m_flags;
  };

}

#endif

