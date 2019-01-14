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
#ifndef CRCORE_CRGROUP_H
#define CRCORE_CRGROUP_H 1

#include <CRCore/crNode.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crVector3.h>

namespace CRCore{
  
typedef std::vector< ref_ptr<crNode> > NodeArray;

class CR_EXPORT crGroup : public crNode
{
public :
  crGroup();

  /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
  crGroup(const crGroup&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

  META_Node(CRCore, crGroup);

  virtual crGroup* asGroup() { return this; }
  virtual const crGroup* asGroup() const { return this; }

  virtual void traverse(crNodeVisitor& nv);

  /** Add Node to Group.
  *  If node is not NULL and is not contained in Group then increment its  
  *  reference count, add it to the child list and dirty the bounding 
  *  sphere to force it to recompute on next getBound() and return true for success.
  *  Otherwise return false. Scene nodes can't be added as child nodes.
  */
  virtual bool addChild( crNode *child );

  /** Insert Node to Group at specific location.
  *  The new child node is inserted into the child list
  *  before the node at the specified index.  No nodes
  *  are removed from the crGroup with this operation. 
  */
  virtual bool insertChild( unsigned int index, crNode *child );

  /** Remove Node from crGroup.
  *  If Node is contained in Group then remove it from the child
  *  list, decrement its reference count, and dirty the 
  *  bounding sphere to force it to recompute on next getBound() and
  *  return true for success.  If Node is not found then return false
  *  and do not change the reference count of the Node.
  */
  virtual bool removeChild( crNode *child );

  virtual bool removeChild(unsigned int pos,unsigned int numChildrenToRemove=1);

  /** Replace specified Node with another Node.
  * Equivalent to setChild(getChildIndex(orignChild),node), 
  * see docs for setChild for further details on implementation.*/
  virtual bool replaceChild( crNode *origChild, crNode* newChild );
  bool replaceChildExt(crNode *origNode, crNode* newNode);//只替换该节点

  /** return the number of children nodes.*/
  inline unsigned int getNumChildren() const { return m_childArray.size(); }
  inline unsigned int getNumChildren() { return m_childArray.size(); }

  /** set child node at position i.
  *  return true if set correctly, false on failure (if node==NULL || i is out of range).
  *  When set can be successful applied, the algorithm is : decrement the reference count origNode and increments the
  *  reference count of newNode, and dirty the bounding sphere
  *  to force it to recompute on next getBound() and returns true.
  *  If origNode is not found then return false and do not 
  *  add newNode.  If newNode is NULL then return false and do
  *  not remove origNode. Also returns false if newChild is a Scene node.
  */
  virtual bool setChild( unsigned  int i, crNode* node );

  /** return child node at position i.*/
  inline crNode* getChild( unsigned  int i ) { return i<m_childArray.size()?m_childArray[i].get():NULL; }

  /** return child node at position i.*/
  inline const crNode* getChild( unsigned  int i ) const { return i<m_childArray.size()?m_childArray[i].get():NULL; }

  inline NodeArray& getChildArray() { return m_childArray; }
  inline const NodeArray& getChildArray()const { return m_childArray; }

  /** return true if node is contained within crGroup.*/
  bool containsNode( const crNode* node );

  bool containsNode( const crNode* node ) const;

  /** Get the index number of child, return a value between
  * 0 and m_children.size()-1 if found, if not found then
  * return m_children.size().*/
  inline unsigned int getChildIndex( const crNode* node ) const
  {
    for (unsigned int childNum = 0;childNum < m_childArray.size();++childNum)
    {
      if (m_childArray[childNum] == node) return childNum;
    }
    return m_childArray.size(); // node not found.
  }

  virtual bool intersects(const CRCore::crBoundingBox& bb)const { return m_bbox.valid()&&m_bbox.intersects(bb); };

  enum AttrMask
  {
	  AttrAll = 0xFFFF,
	  AttrGround = 0x01,
	  AttrAir = 0x02,
	  AttrDeepSea = 0x04
  };

  void setAttributeMask( unsigned int attributeMask ) { m_attributeMask = attributeMask; }
  unsigned int getAttributeMask(){ return m_attributeMask; }
  unsigned int getAttributeMask()const{ return m_attributeMask; }

  virtual void releaseObjects(crState* = 0);

  //virtual void updateCharacterLod(float sqrDistance){}
protected:

  virtual ~crGroup();

  virtual bool computeBound() const;

  NodeArray m_childArray; 

  unsigned int m_attributeMask;//物体属性掩码，用于描述该物体的基本属性

};

}

#endif