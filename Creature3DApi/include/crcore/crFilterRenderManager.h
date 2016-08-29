/**********************************************************************
*
*	文件:	   crFilterRenderManager.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
#ifndef CRCORE_CRFILTERRENDERMANAGER
#define CRCORE_CRFILTERRENDERMANAGER 1
#include <CRCore/crObject.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crMath.h>
#include <map>

namespace CRCore {

class crMousePickVisitor : public CRCore::crNodeVisitor
{//只拾取带事件的UI节点
public:
	crMousePickVisitor():
		crNodeVisitor(NODE_VISITOR,TRAVERSE_ACTIVE_CHILDREN),
		m_pickedUI(NULL){}

	virtual ~crMousePickVisitor(){}

	void setMouse(const crVector2 &mouse){ m_mouse = mouse; }
	void distillPickedUI(crWidgetNode *&pickedUI){ pickedUI = m_pickedUI; m_pickedUI = NULL; m_matrixList.clear(); }

	typedef std::vector<CRCore::ref_ptr<CRCore::RefMatrix> > MatrixList;
	inline void pushMatrix( CRCore::RefMatrix* matrix)
	{
		m_matrixList.push_back(matrix);
	}
	inline void popMatrix()
	{
		m_matrixList.pop_back();
	}
	inline CRCore::crMatrix topMatrix()
	{
		if(m_matrixList.empty())
			return CRCore::crMatrix::identity();
		else
			return *(m_matrixList.back().get());
	}
	virtual void apply(crTransform& node)
	{
		if(/*!m_pickedUI && */node.getVisiable())
		{
			RefMatrix* matrix = new RefMatrix;
			node.computeLocalToWorldMatrix(*matrix,this);

			if(!m_matrixList.empty())
				matrix->preMult(*(m_matrixList.back().get()));

			pushMatrix(matrix);

			traverse(node);

			popMatrix();
		}
	}
	virtual void apply(crNode& node)
	{
		if(/*!m_pickedUI && */node.getVisiable())
		{
			traverse(node);
		}
	}
	virtual void apply(crCanvasNode& node)
	{
		if(/*!m_pickedUI && */node.getVisiable())
		{
			if(node.getEnableMatrix())
				apply((crTransform &)node);
			else
				apply((crNode&)node);
			if(!m_pickedUI)
				pick(node);
		}
	}
	virtual void apply(crWidgetNode& node)
	{
		if(/*!m_pickedUI && */node.getVisiable())
		{
			pick(node);
			if(node.getEnableMatrix())
				apply((crTransform &)node);
			else
				apply((crNode&)node);
		}
	}
	virtual void pick(crWidgetNode& node)
	{
		if(/*!m_pickedUI && */node.getVisiable() && node.getCanFocus()/* && node.getEnable()*/)
		{
			crBoundingBox bbox = node.getBoundBox();
			if(bbox.valid())
			{
				if(!m_matrixList.empty())
				{
					crMatrix &mat = *(m_matrixList.back().get());
					bbox.m_max = bbox.m_max * mat;
					bbox.m_min = bbox.m_min * mat;
					bbox.correct();
				}
				//CRCore::notify(CRCore::ALWAYS)<<"crMousePickVisitor "<<node.getName()<< std::endl;
				//CRCore::notify(CRCore::ALWAYS)<<bbox.m_min<<bbox.m_max<< std::endl;
				if( m_mouse[0]>=bbox.m_min[0] && m_mouse[0]<=bbox.m_max[0] && 
					m_mouse[1]>=bbox.m_min[1] && m_mouse[1]<=bbox.m_max[1] )
				{
					//if(dynamic_cast<crWindowUIData *>(node.getDataClass()))
					//	m_windowUI = &node;
					//else
					m_pickedUI = &node;
				}
			}
		}
	}
protected:
	MatrixList    m_matrixList;
	crVector2     m_mouse;
	//crObject     *m_pickedObject;
	crWidgetNode *m_pickedUI;
};
enum ShowMode
{
	SH_Hide = 0,
	SH_Show = 1,
	SH_DoModal = 2,
};
class CR_EXPORT crFilterRenderManager : public crNode
{
public :
	crFilterRenderManager();
	static crFilterRenderManager *getInstance();

    crFilterRenderManager(const crFilterRenderManager&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

    META_Node(CRCore, crFilterRenderManager);
    
    virtual void traverse(crNodeVisitor& nv);
	virtual void clear(){};
	void init();

	void createIdenticCamera();

	bool addDrawable( crDrawable *drawable, bool async = false );
	crDrawable *getDrawable(const std::string &name);
	bool removeDrawable( crDrawable *drawable, bool async = false );

	virtual void releaseObjects(crState* = 0);

	//void addFilterNode(crNode *filterNode);
	//crNode *getFilterNode(const std::string &name,crSearchNodeBYNameVisitor::SearchNodeType nodeType = crSearchNodeBYNameVisitor::ALLNODE);
 //   crGroup *getFilterRoot();

	void setFocusNode(crWidgetNode *node);
	inline crWidgetNode* getFocusNode(){ return m_focusNode.get(); }

	void computeMVPW();
	inline const crMatrix &getMVPW() const { return m_mvpw; }
	inline const crMatrix &getInverseMVPW() const { return m_inverseMVPW; }
	crVector3 getIdenticPosition(float x, float y);
	crVector3 getIdenticPosition(const crVector3 &windowPos);
	crVector3 getWindowPosition(const crVector3 &identicPos);

	//typedef std::vector< ref_ptr<crCanvasNode> >CanvasArray;
	//void addCanvas(crCanvasNode *canvas);
	crGroup *getCanvasRoot();
	crCanvasNode *findCanvas(const std::string& id);
	void showCanvas(const std::string& id, bool show);//当canvas=NULL时会调用crBrain::getInstance()->doEvent(WCH_ShowCanvasFaild);
	void showCanvas(crCanvasNode *canvas, bool show);
	void showCanvas(crCanvasNode *canvas, const std::string& id, ShowMode show);//当canvas=NULL时会调用crBrain::getInstance()->doEvent(WCH_ShowCanvasFaild);
	void doModal(const std::string& id);//当canvas=NULL时会调用crBrain::getInstance()->doEvent(WCH_ShowCanvasFaild);
	void doModal(crCanvasNode *canvas);
	void doModal(crCanvasNode *canvas,const std::string& id);//当canvas=NULL时会调用crBrain::getInstance()->doEvent(WCH_ShowCanvasFaild);
	void closeCanvas(const std::string& id);
	void closeCanvas(crCanvasNode *canvas);
	
	void mousePick(const crVector2& mouse,crWidgetNode *&pickedWidget);
	void setMouseCapturer(crWidgetNode *node);
	void setInputCapturer(crWidgetNode *node);
	crWidgetNode *getMouseCapturer();
	crWidgetNode *getInputCapturer();
	void setActiveCanvas(crCanvasNode *node);
	crWidgetNode* getCurrentInputWidget();
	crCanvasNode* getMainCanvas();
	crCanvasNode* getModalCanvas();
	//void createOrthoCamera();
	//typedef std::vector< CRCore::ref_ptr<crDrawable> > DrawableVec;
	//void addMouse(crDrawable *drawable);
	//   void showMouse(int i);
	//
	//void addMousePicMsg(crDrawable *drawable);
	//void showMousePicMsg(int i);

	//typedef std::deque<std::string> DequeString;
	//void addDequeText(crDrawable *drawable);
	//void addDequeString(const std::string &str);
	//void setDequeTextInterval(float interval);
	typedef std::deque< ref_ptr<crCanvasNode> > ActiveCanvasDeque;
	void setMouseCanvas(crCanvasNode *mouseCanvas);
	inline crCanvasNode *getMouseCanvas(){ return m_mouseCanvas.get(); }
	inline void setMousePosition(crVector2 &pos)
	{
		if(m_mouseCanvas.valid())
			m_mouseCanvas->setPosition(crVector3(clampTo(pos[0],-1.0f,1.0f),clampTo(pos[1],-1.0f,1.0f),0.0f));
	}
	void closeAllCanvas();
	void setLoadingCanvas(crCanvasNode *loadingCanvas);
	inline crCanvasNode *getLoadingCanvas(){ return m_loadingCanvas.get(); }
	void removeLoadingUI();
protected :
    virtual ~crFilterRenderManager() {}
	virtual bool computeBound() const;
	//void removeCanvas(const std::string &id);
	//void updateDequeText();
    static ref_ptr<crFilterRenderManager> m_instance;
	//CanvasArray m_canvasArray;
	ref_ptr<crCanvasNode> m_mainCanvas;
	ref_ptr<crCanvasNode> m_modalCanvas;
	ref_ptr<crCanvasNode> m_mouseCanvas;//topmost
	ref_ptr<crCanvasNode> m_loadingCanvas;
	ref_ptr<crWidgetNode> m_currentMouseCapturer;
	ref_ptr<crWidgetNode> m_currentInputCapturer;
	ref_ptr<crWidgetNode> m_focusNode;
	crCriticalMutex m_activeCanvasMutex;
	ActiveCanvasDeque m_activeCanvasDeque;
	ActiveCanvasDeque m_activeCanvasTemp;
	ref_ptr<crCameraNode> m_identicCamera;
	//ref_ptr<crCameraNode> m_orthoCamera;
	ref_ptr<crObject>     m_orthoObject;
	//ref_ptr<crObject>     m_identicObject;
	ref_ptr<crStateSet>   m_cameraStateSet;
	crMatrix m_mvpw;
	crMatrix m_inverseMVPW;
	ref_ptr<crSearchNodeBYNameVisitor> m_searchVisitor;
	crMousePickVisitor m_mousePickVisitor;
	crCriticalMutex m_asyncDrawableListMutex;
	crObject::DrawableList m_asyncAddDrawableList;
	crObject::DrawableList m_asyncRemoveDrawableList;
	//crCriticalMutex m_initMutex;
	bool m_inited;
	std::string m_faildShowCanvas;
	ShowMode m_faildShowType;
 //   DrawableVec m_mouse;
	//DrawableVec m_mousePicMsg;
	//
	//DrawableVec m_dequeTextVec;
	//DequeString m_dequeString;
	//float m_dequeTextInterval;
	//double m_lastFrameTime;

	//int m_lastMouse;
	//int m_lastMousePicMsg;
};

}

#endif
