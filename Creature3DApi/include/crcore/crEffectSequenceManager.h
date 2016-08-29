/**********************************************************************
*
*	文件:	   crEffectSequenceManager.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
#ifndef CRCORE_CREFFECTSEQUENCEMANAGER
#define CRCORE_CREFFECTSEQUENCEMANAGER 1

#include <CRCore/crBase.h>
#include <CRCore/crNode.h>
#include <CRCore/crSequence.h>
#include <CRCore/crRange.h>
#include <CRCore/thread/crMutex.h>
#include <map>
#include <string>

namespace CRCore {
class CR_EXPORT crEffectSequenceManager : public CRCore::crNode
{
public :
	crEffectSequenceManager();
	static crEffectSequenceManager *getInstance();
	crEffectSequenceManager(const crEffectSequenceManager&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRCore, crEffectSequenceManager);

	virtual void traverse(CRCore::crNodeVisitor& nv);

	void addEffectNodes( const std::string &name, crSequence *node );
	crSequence *reuseOrgetOrCreateAnIdleEffectNodes(const std::string &name);
	void clearEffectNodes();

	typedef std::multimap< std::string, CRCore::ref_ptr<crSequence> > EffectNodeMap;
	typedef std::pair < std::string, CRCore::ref_ptr<crSequence> > EffectNodeMapPair;

	//virtual void swapBuffers();
	virtual void releaseObjects(CRCore::crState* state);
	virtual void clear();
	CRCore::crGroup *getEffectRoot();
protected :
	virtual ~crEffectSequenceManager();
	static CRCore::ref_ptr<crEffectSequenceManager> m_instance;
	virtual bool computeBound() const;
	CRCore::ref_ptr<CRCore::crGroup> m_effectRoot;
	EffectNodeMap m_effectNodeMap;
	EffectNodeMap m_effectNodeMap_buf;
	rangei m_rangei;
	crMutex m_effectNodeMapMutex;
};

}

#endif
