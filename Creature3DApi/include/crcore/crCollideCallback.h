/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#ifndef CRCORE_CRCOLLIDECALLBACK_H
#define CRCORE_CRCOLLIDECALLBACK_H 1

#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crEventCallbackManager.h>

namespace CRCore {

class CR_EXPORT crCollideCallback : public crEventCallback
{

    public :
    
        crCollideCallback(){}

		//crCollideCallback(const crCollideCallback&cc,const CRCore::crCopyOp&):
  //          crNestedCallbackPtr(cc.crNestedCallbackPtr) {}
  //      META_Base(CRCore,crCollideCallback)
		crCollideCallback(const crCollideCallback& cc,const crCopyOp&);

		META_EventCallback(CRCore, Collide)
        
		virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);

        inline void setNestedCallback(crCollideCallback* nc) { crNestedCallbackPtr = nc; }
        inline crCollideCallback* getNestedCallback() { return crNestedCallbackPtr.get(); }
        
        inline void addNestedCallback(crCollideCallback* nc)
        {
			if (nc)
			{
				if (crNestedCallbackPtr.valid())
				{
					crNestedCallbackPtr->addNestedCallback(nc);
				}
				else
				{
					crNestedCallbackPtr = nc;
				}
			}
        }
        
        inline crCollideCallback *getNestedCallback(const std::string &name)
        {
			if (crNestedCallbackPtr.valid())
			{
				if (name.compare(crNestedCallbackPtr->_name()) == 0)
				{
					return crNestedCallbackPtr.get();
				}
				else
				{
					return crNestedCallbackPtr->getNestedCallback(name);
				}
			}
			return NULL;
        }

		inline void removeNestedCallback(const std::string &name)
		{
			if (crNestedCallbackPtr.valid())
			{
				if (name.compare(crNestedCallbackPtr->_name()) == 0)
				{
					crNestedCallbackPtr = crNestedCallbackPtr->getNestedCallback();
				}
				else
				{
					crNestedCallbackPtr->removeNestedCallback(name);
				}
			}
		}

		// /virtual void addEventParam(int i, const std::string &str){}
    public:

        CRCore::ref_ptr<crCollideCallback> crNestedCallbackPtr;
        
    protected:
    
        virtual ~crCollideCallback() {}
};

}

#endif

