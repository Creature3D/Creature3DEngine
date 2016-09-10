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

#ifndef CRCORE_CRBASE_H
#define CRCORE_CRBASE_H 1

#include <CRCore/Referenced.h>	
#include <CRCore/crCopyOp.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crDataManager.h>
#include <string>

namespace CRCore {
  class crState;
#define META_Base(library,name) \
  virtual CRCore::crBase* cloneType() const { return new name (); } \
  virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new name (*this,copyop); } \
  virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const name *>(obj)!=NULL; } \
  virtual const char* libraryName() const { return #library; }\
  virtual const char* className() const { return #name; }

  class CR_EXPORT crBase : public Referenced
  {
  public:
    inline crBase():Referenced(),m_dataVariance(STATIC),m_currentMsg(0),m_swapFrameNumber(0){}

    crBase(const crBase&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

    virtual crBase* cloneType() const = 0;

    virtual crBase* clone(const crCopyOp&) const = 0;

    virtual bool isSameKindAs(const crBase* obj) const { return true; }

    virtual const char* libraryName() const = 0;

    virtual const char* className() const = 0;

	virtual void setStateSet(crStateSet *state){};

	inline void setName( const std::string& name ) { m_name = name; }
	inline void setName( const char* name ) { m_name = name; }

	inline const std::string& getName() const { return m_name; }

    enum DataVariance
    {
      DYNAMIC,
      STATIC
    };
    inline virtual void setDataVariance(DataVariance dv) { m_dataVariance = dv; }
    inline DataVariance getDataVariance() const { return m_dataVariance; }
	inline DataVariance getDataVariance() { return m_dataVariance; }

	virtual void releaseObjects(CRCore::crState* state);

	virtual void addParam(int i, const std::string &str){}

	/////////////////////
	virtual void setDataClass( CRCore::crData *data );
	CRCore::crData *getDataClass();
	virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);
	inline _crInt64 getCurrentMsg() const { return m_currentMsg; }
	CRCore::crHandle* getHandle(__int64 kbmsg, const std::string& name);
	bool hasHandle(__int64 kbmsg);
	void clearData();
	//////////////////////////
	const CRCore::crData *getDataClass() const;
	inline int getSwapFrameNumber(){ return m_swapFrameNumber; }
  protected:
    virtual ~crBase() {}

	std::string m_name;
    DataVariance m_dataVariance;
	ref_ptr<crData> m_dataClass;
	_crInt64 m_currentMsg;
	int m_swapFrameNumber;
  private:

    /** disallow any copy operator.*/
    crBase& operator = (const crBase&) { return *this; }
  };

}

#endif
