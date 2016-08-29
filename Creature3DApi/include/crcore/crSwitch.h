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
#ifndef CRCORE_SWITCH
#define CRCORE_SWITCH 1

#include <CRCore/crGroup.h>

namespace CRCore {

	/** crSwitch is a crGroup node which allows switching between children.
	Typical uses would be for objects which might need to be rendered
	differently at different times, for instance a switch could be used 
	to represent the different states of a traffic light.
	*/
	class CR_EXPORT crSwitch : public crGroup
	{
	public :


		crSwitch();

		/** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
		crSwitch(const crSwitch&,const crCopyOp& copyop = crCopyOp::SHALLOW_COPY);

		META_Node(CRCore, crSwitch);

		virtual void traverse(crNodeVisitor& nv);

		void setNewChildDefaultValue(bool value) { m_bNewChildDefaultValue = value; }

		bool getNewChildDefaultValue() const { return m_bNewChildDefaultValue; }

		virtual bool addChild( crNode *child );

		virtual bool addChild( crNode *child, bool value );

		virtual bool insertChild( unsigned int index, crNode *child );

		virtual bool insertChild( unsigned int index, crNode *child, bool value );

		virtual bool removeChild( crNode *child );

		void setValue(unsigned int pos,bool value);

		bool getValue(unsigned int pos) const;

		void setChildValue(const crNode* child,bool value);

		bool getChildValue(const crNode* child) const;

		/** Set all the children off (false), and set the new default child value to off (false).*/
		bool setAllChildrenOff();

		/** Set all the children on (true), and set the new default child value to on (true).*/
		bool setAllChildrenOn();

		/** Set a single child to be on, switch off all other children.*/
		bool setSingleChildOn(unsigned int pos);

#ifdef USE_DEPRECTATED_API
		/**
		* Special values for the crSwitch. Use these if you want to
		* turn on/off all child nodes.
		*/
		enum Values {
			/** All children turned on. */
			ALL_CHILDREN_ON=-1,
			/** All children off. */
			ALL_CHILDREN_OFF=-2,
			/** Multiple children turned on.*/
			MULTIPLE_CHILDREN_ON=-3
		};

		/**
		* Selects the active child crNode or enables a special
		* SwitchType mode. 
		* @param value the number of the active child
		* (first child == number 0) or SwitchType. Invalid values
		* will be ignored.
		*/
		void setValue(int value);

		int getValue() const;

#endif        

		typedef std::vector<bool>   ValueList;

		const ValueList& getValueList() const { return m_ValueArray; }

	protected :

		virtual ~crSwitch() {}

		// this is effectively a bit mask.
		bool        m_bNewChildDefaultValue;
		ValueList   m_ValueArray;
	};

}

#endif
