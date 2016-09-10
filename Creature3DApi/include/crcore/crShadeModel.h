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
#ifndef CRCORE_SHADEMODEL
#define CRCORE_SHADEMODEL 1


#include <CRCore/crStateAttribute.h>

namespace CRCore {

  /** Class which encapsulates glShadeModel(..).
  */     
  class CR_EXPORT crShadeModel : public crStateAttribute
  {
  public :
	 enum Mode {
		  FLAT,// = GL_FLAT,
		  SMOOTH// = GL_SMOOTH
	 };

    crShadeModel(Mode mode = SMOOTH);

    /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
    crShadeModel(const crShadeModel& sm,const crCopyOp& copyop = crCopyOp::SHALLOW_COPY):
    crStateAttribute(sm,copyop),
      m_mode(sm.m_mode) {}


      META_StateAttribute(CRCore, crShadeModel, SHADEMODEL);

      /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
      virtual int compare(const crStateAttribute& sa) const
      {
        // check the types are equal and then create the rhs variable
        // used by the COMPARE_crStateAttribute_Paramter macro's below.
          COMPARE_StateAttribute_Types(crShadeModel,sa)

          // compare each paramter in turn against the rhs.
          COMPARE_StateAttribute_Parameter(m_mode)

          return 0; // passed all the above comparison macro's, must be equal.
      }

	  virtual int compare(const crStateAttribute& sa)
	  {
		  // check the types are equal and then create the rhs variable
		  // used by the COMPARE_crStateAttribute_Paramter macro's below.
		  COMPARE_StateAttribute_Types(crShadeModel,sa)

		  // compare each paramter in turn against the rhs.
		  COMPARE_StateAttribute_Parameter(m_mode)

		  return 0; // passed all the above comparison macro's, must be equal.
	  }

      inline void setMode(Mode mode) { m_mode = mode; }

      inline Mode getMode() const { return m_mode; }

      virtual void apply(crState& state) const;
	  virtual void addAttributeParam(int i, const std::string &str);
  protected:

    virtual ~crShadeModel();

    Mode m_mode;

  };

}

#endif
