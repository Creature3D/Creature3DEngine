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
//Modified by 吴财华
#ifndef CRCORE_MULTISWITCH
#define CRCORE_MULTISWITCH 1

#include <CRCore/crGroup.h>
#include <CRCore/crExport.h>

namespace CRCore {

/** crMultiSwitch is a crGroup node which allows switching between sets of selected children.
    MultiSwtich is based on the OpenFlight switch behaviour.
*/
class CR_EXPORT crMultiSwitch : public CRCore::crGroup
{
    public :
        

        crMultiSwitch();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crMultiSwitch(const crMultiSwitch&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crMultiSwitch);

        virtual void traverse(CRCore::crNodeVisitor& nv);
        
        void setNewChildDefaultValue(bool value) { m_newChildDefaultValue = value; }
        
        bool getNewChildDefaultValue() const { return m_newChildDefaultValue; }

        virtual bool addChild( CRCore::crNode *child );

        virtual bool insertChild( unsigned int index, CRCore::crNode *child );

        //virtual bool removeChild( CRCore::crNode *child );
		virtual bool removeChild(unsigned int pos,unsigned int numChildrenToRemove=1);

        void setValue(unsigned int switchSet, unsigned int pos,bool value);

        bool getValue(unsigned int switchSet, unsigned int pos) const;

        void setChildValue(const CRCore::crNode* child,unsigned int switchSet, bool value);
        
        bool getChildValue(const CRCore::crNode* child,unsigned int switchSet) const;

        /** Set all the children off (false), and set the new default child value to off (false).*/
        bool setAllChildrenOff(unsigned int switchSet);
        
        /** Set all the children on (true), and set the new default child value to on (true).*/
        bool setAllChildrenOn(unsigned int switchSet);
        
        /** Set a single child to be on, crMultiSwitch off all other children.*/
        bool setSingleChildOn(unsigned int switchSet, unsigned int pos);
        
        /** Set which of the available switch set lists to use.*/
        void setActiveSwitchSet(unsigned int switchSet);
        
        /** Get which of the available switch set lists to use.*/
        unsigned int getActiveSwitchSet() const { return m_activeSwitchSet; }

        typedef std::vector<bool>       ValueList;
        typedef std::vector<ValueList>  SwitchSetList;
        
        /** Set the compile set of different values.*/
        void setSwitchSetList(const SwitchSetList& switchSetList);

        /** Get the compile set of different values.*/
        const SwitchSetList& getSwitchSetList() const { return m_values; }

        /** Set the a single set of different values for a particular switch set.*/
        void setValueList(unsigned int switchSet, const ValueList& values);

        /** Get the a single set of different values for a particular switch set.*/
        const ValueList& getValueList(unsigned int switchSet) const { return m_values[switchSet]; }

		crNode *getFirstActiveChild();

		bool removeSwitchSet(unsigned int switchSet);
		//void asyncSetActiveSwitchSet(unsigned int switchSet);
		virtual void addParam(int i, const std::string &str);
    protected :
    
        virtual ~crMultiSwitch() {}
        
        void expandToEncompassSwitchSet(unsigned int switchSet);
		virtual bool computeBound() const;

        // this is effectively a list of bit mask.
        bool            m_newChildDefaultValue;
        unsigned int    m_activeSwitchSet;
        SwitchSetList   m_values;
};

}

#endif
