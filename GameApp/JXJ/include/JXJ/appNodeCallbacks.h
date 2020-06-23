/**********************************************************************
*
*	文件:	   appNodeCallbacks.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPNODECALLBACKS_H
#define JXJ_APPNODECALLBACKS_H
#include <CRCore/crNode.h>
namespace JXJ{
////////////////////////////////
//crJXJSetDrawableColorCallback
//UpdateCallback
///////////////////////////////////
class crJXJSetDrawableColorCallback : public CRCore::crNodeCallback
{
public:
	crJXJSetDrawableColorCallback();
	crJXJSetDrawableColorCallback(const crJXJSetDrawableColorCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(JXJ, JXJSetDrawableColor)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crVector4 m_color;
	int m_done;//0未开始,1进行中,2已经完成
};
////////////////////////////////
//crJXJScaleBodyCallback
//UpdateCallback
///////////////////////////////////
class crJXJScaleBodyCallback : public CRCore::crNodeCallback
{
public:
	crJXJScaleBodyCallback();
	crJXJScaleBodyCallback(const crJXJScaleBodyCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(JXJ, JXJScaleBody)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	float m_origscale;
	float m_destscale;
	float m_speed;
	float m_scale;
	int m_done;//0未开始,1进行中,2已经完成
};
////////////////////////////////
//crJXJCloseToTargetNodeCallback
//UpdateCallback
///////////////////////////////////
class crJXJCloseToTargetNodeCallback : public CRCore::crNodeCallback
{
public:
	crJXJCloseToTargetNodeCallback();
	crJXJCloseToTargetNodeCallback(const crJXJCloseToTargetNodeCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(JXJ, JXJCloseToTargetNode)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	virtual void releaseObjects(CRCore::crState* state);
	void setTargetNode(CRCore::crMatrixTransform *targetNode);
protected:
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_targetNode;
	float m_speed;
	int m_done;//0未开始,1进行中,2已经完成
};
}
#endif