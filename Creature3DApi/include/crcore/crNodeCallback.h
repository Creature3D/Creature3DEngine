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
#ifndef CRCORE_NODECALLBACK_H
#define CRCORE_NODECALLBACK_H 1

#include <CRCore/crBase.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crEventCallbackManager.h>

namespace CRCore {

class crNode;
class crNodeVisitor;

class CR_EXPORT crNodeCallback : public crEventCallback 
{

    public :
        crNodeCallback():m_swapFrameNumber(0){}

        //crNodeCallback(const crNodeCallback&nc,const crCopyOp&):
        //    crNestedCallbackPtr(nc.crNestedCallbackPtr) {}
        //META_Base(CRCore,crNodeCallback)

		crNodeCallback(const crNodeCallback& nc,const crCopyOp&copyop);

		META_EventCallback(CRCore, Node)

        /** Callback method call by the crNodeVisitor when visiting a node.*/
        inline virtual void operator()(crNode* node, crNodeVisitor* nv)
        { 
            // note, callback is repsonsible for scenegraph traversal so
            // should always include call the traverse(node,nv) to ensure 
            // that the rest of cullbacks and the scene graph are traversed.
            traverse(node,nv);
        }
        
        /** Call any nested callbacks and then traverse the scene graph. */
        void traverse(crNode* node,crNodeVisitor* nv);
        
        inline void setNestedCallback(crNodeCallback* nc) { crNestedCallbackPtr = nc; }
        inline crNodeCallback* getNestedCallback() { return crNestedCallbackPtr.get(); }
        
        inline void addNestedCallback(crNodeCallback* nc)
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
        
		inline crNodeCallback *getNestedCallback(const std::string &name)
		{
			if (crNestedCallbackPtr.valid())
			{
				if(name.compare(crNestedCallbackPtr->_name()) == 0)
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
				if(name.compare(crNestedCallbackPtr->_name()) == 0)
			    {
			        crNestedCallbackPtr = crNestedCallbackPtr->getNestedCallback();
			    }
				else
			    {
			        crNestedCallbackPtr->removeNestedCallback(name);
			    }
			}
        }

		//inline virtual void addEventParam(int i, const std::string &str){}

		//与绘制线程的同步
		inline virtual void swapBuffers(int frameNumber)
		{
			if(m_swapFrameNumber != frameNumber)
			{
				if(crNestedCallbackPtr.valid()) crNestedCallbackPtr->swapBuffers(frameNumber);
				m_swapFrameNumber = frameNumber;
			}
		}

		void setCfgFile(const std::string &file) { m_cfgFile = file; }
		std::string getCfgFile() { return m_cfgFile; }
    public:

        ref_ptr<crNodeCallback> crNestedCallbackPtr;
        
    protected:
    
        virtual ~crNodeCallback() {}
		std::string m_cfgFile;
		int m_swapFrameNumber;
};

} // namespace

#endif

