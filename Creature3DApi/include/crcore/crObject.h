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
//Modified by Îâ²Æ»ª
#ifndef CRCORE_CROBJECT_H
#define CRCORE_CROBJECT_H

#include <list>
#include <hash_set>
#include <CRCore\crNode.h>
#include <CRCore/crGroup.h>
//#include <CRCore/crEventGroup.h>
#include <CRCore\crBoundingBox.h>
#include <CRCore\crDrawable.h>
#include <CRCore\crNodeVisitor.h>
#include <CRCore/crEventCallbackManager.h>

namespace CRCore{
class crEventGroup;
class crShapeDrawable;

class CR_EXPORT crObject : public crNode
{
public:
	crObject();
	  
	/** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
	crObject(const crObject&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

	META_Node(CRCore,crObject);

	typedef std::vector< ref_ptr<crDrawable> >    DrawableList;

	/** Add Drawable to Geode.
	*  If gset is not NULL and is not contained in Geode then increment its  
	*  reference count, add it to the drawables list and dirty the bounding 
	*  sphere to force it to recompute on next getBound() and return true for success.
	*  Otherwise return false.
	*/
	virtual bool addDrawable( crDrawable *drawable );

	/** Remove Drawable from Geode.
	* Equivalent to setDrawabke(getDrawableIndex(orignChild),node), 
	* see docs for setNode for futher details on implementation.*/
	virtual bool removeDrawable( crDrawable *drawable );

	/** Remove drawable(s) from the specified position in Geode's drawable list.*/
	virtual bool removeDrawable(unsigned int i,unsigned int numDrawablesToRemove=1);

	/** Replace specified Drawable with another Drawable.
	* Equivalent to setDrawable(getDrawableIndex(orignChild),node), 
	* see docs for setDrawable for futher details on implementation.*/
	virtual bool replaceDrawable( crDrawable *origDraw, crDrawable *newDraw );

	/** set drawable at position i.
	* return true if set correctly, false on failure (if node==NULL || i is out of range).
	*  Decrement the reference count origGSet and increments the
	*  reference count of newGset, and dirty the bounding sphere
	*  to force it to recompute on next getBound() and returns true.
	*  If origDrawable is not found then return false and do not 
	*  add newGset.  If newGset is NULL then return false and do
	*  not remove origGset.
	*/
	virtual bool setDrawable( unsigned int i, crDrawable* drawable );

	/** return the number of drawable's.*/
	inline unsigned int getNumDrawables() const 
	{ return m_drawables.size(); }

	inline unsigned int getNumDrawables() 
	{ return m_drawables.size(); }

	/** return drawable at position i.*/
	inline crDrawable* getDrawable( unsigned int i ) 
	{ return i<m_drawables.size()?m_drawables[i].get():NULL; }

	/** return drawable at position i.*/
	inline const crDrawable* getDrawable( unsigned int i ) const 
	{ return i<m_drawables.size()?m_drawables[i].get():NULL; }

	DrawableList& getDrawableList() { return m_drawables; }
	const DrawableList& getDrawableList()const{ return m_drawables; }

	/** return true if drawable is contained within Geode.*/
	bool containsDrawable(const crDrawable* gset);

	bool containsDrawable(const crDrawable* gset) const;
	 
	/** Get the index number of drawable, return a value between
	* 0 and _drawables.size()-1 if found, if not found then
	* return _drawables.size().*/
	inline unsigned int getDrawableIndex( const crDrawable* node ) const
	{
		for (unsigned int drawableNum=0;drawableNum<m_drawables.size();++drawableNum)
		{
		if (m_drawables[drawableNum]==node) return drawableNum;
		}
		return m_drawables.size(); // node not found.
	}

	inline unsigned int getDrawableIndex( const crDrawable* node )
	{	  
		for (unsigned int drawableNum=0;drawableNum<m_drawables.size();++drawableNum)
		{
			if (m_drawables[drawableNum]==node) return drawableNum;
		}
		return m_drawables.size(); // node not found.
	}

	virtual void swapBuffers(int frameNumber);

	void setTriDataID( void* triDataID) { m_triData = triDataID; }
	void* getTriDataID() { return m_triData; }
	void* getTriDataID()const { return m_triData; }

	virtual void releaseObjects(CRCore::crState* state);
	virtual void releaseOde();
	void compileDrawables(crState& state);

	void setEffectByShadow(bool effectByShadow);
	inline bool getEffectByShadow()const { return m_effectByShadow; }

	void setNeedSelectLight(bool needSelectLight);
	inline bool getNeedSelectLight()const { return m_needSelectLight; }

    typedef stdext::hash_set<std::string> LightNameSet;
	void addAvoidLight(const std::string &lsname);
	inline bool isAvoidLight(const std::string &lsname) const { return m_avoidLight.find(lsname) != m_avoidLight.end(); }
	void addNeedLight(const std::string &lsname);
	inline bool isNeedLight(const std::string &lsname) const { return m_needLight.find(lsname) != m_needLight.end(); }
	void clearNeedLight();
	void clearAvoidLight();
	typedef std::list<crLightSource *> SelectedLightList;
	void resetSelectedLightList();
	void selectedLight(crLightSource *ls);
    SelectedLightList &getSelectedLightList();
	LightNameSet &getAvoidLightSet();
	LightNameSet &getNeedLightSet();
	void setAcceptGI(char gi){ m_acceptGI = gi; }
	inline char getAcceptGI()const{ return m_acceptGI; }
	void setDrawOutLine(char outline);
	inline char getDrawOutLine()const { return m_drawOutLine; }
	enum OutlineColorMode
	{
		OL_Black,
		OL_Red,
		OL_Green,
		OL_Blue,
		OL_Yellow,
		OL_Edit,
		OL_Count,
	};
	void setOutlineColorMode(OutlineColorMode mode);
	inline unsigned char getOutlineColorMode()const { return m_outlineColorMode; }
protected:
  virtual ~crObject();

  virtual bool computeBound() const;
  //crObjectEvent *getObjectEvent(crObjectEvent::EventType et);
  //mutable crBoundingBox         m_bbox;

  bool           m_effectByShadow;
  bool           m_needSelectLight;
  DrawableList   m_drawables;

  //CollideMode    m_collideMode;  
  
  //mutable void*          m_physicsObjID;
  mutable void*          m_triData;

  LightNameSet m_needLight;
  LightNameSet m_avoidLight;
  SelectedLightList m_selectedLightList;
  char m_acceptGI;//-1_NoSkyLight,0Ä¬ÈÏsgi/lightmap£»1gi£»2sceneheightmap
  char m_drawOutLine;//0Ä¬ÈÏ,1¹´±ß,-1½ûÖ¹¹´±ß
  unsigned char m_outlineColorMode;
 // ObjectEventMap m_objectEventMap;
  
  //mutable CRCore::ref_ptr<crShapeDrawable> m_bboxDrawable;
};

}
#endif