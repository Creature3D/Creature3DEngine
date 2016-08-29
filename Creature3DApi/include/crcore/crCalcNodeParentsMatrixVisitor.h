/**********************************************************************
*
*	文件:	   crCalcNodeParentsMatrixVisitor.h
*
*	描述:	  
*
*	作者:	   吴财华
*					
*
*	Copyright 2011.10.22 吴财华
**********************************************************************/
#ifndef CRCORE_CRCALCNODEPARENTSMATRIXVISITOR_H
#define CRCORE_CRCALCNODEPARENTSMATRIXVISITOR_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crNode.h>
#include <CRCore/crExport.h>
#include <CRCore/crTransform.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crWidgetNode.h>
namespace CRCore {

class CR_EXPORT crCalcNodeParentsMatrixVisitor : public CRCore::crNodeVisitor
{
public:
	crCalcNodeParentsMatrixVisitor():
	  CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_PARENTS){}

	virtual void apply(CRCore::crTransform& node);
	virtual void apply(CRCore::crWidgetNode& node);
	virtual void apply(CRCore::crFilterRenderManager& node);
	virtual void apply(CRCore::crDB& node){}

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
	inline void reset() { m_result.makeIdentity(); }
	inline CRCore::crMatrix& getResult(){ return m_result; }
protected:
	MatrixList                              m_matrixList;
	CRCore::crMatrix  m_result;
};

}
#endif