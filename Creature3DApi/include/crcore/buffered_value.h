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

#ifndef CRCORE_BUFFERED_VALUE_H
#define CRCORE_BUFFERED_VALUE_H 1

#include <CRCore/crDisplaySettings.h>
#include <CRCore/Referenced.h>
#include <vector>

namespace CRCore {

/** Simple buffered value array which is used for values that need to multibuffered on
  * one per graphics context basis.*/
  
template<class T>
class buffered_value
{
    public:
    
        inline buffered_value():
            _array(crDisplaySettings::instance()->getMaxNumberOfGraphicsContexts(),0)
            {}

        buffered_value& operator = (const buffered_value& rhs)
        {
            _array = rhs._array;
            return *this;
        }
        
        inline void setAllElementsTo(const T& t) { std::fill(_array.begin(),_array.end(),t); }

        inline void clear() { _array.clear(); }

        inline bool empty() const { return _array.empty(); }

        inline unsigned int size() const { return _array.size(); }

		inline void resize(unsigned int newSize) { _array.resize(newSize,0); }

        inline T& operator[] (unsigned int pos)
        {
            // automatically resize array.
            if (_array.size()<=pos)
                _array.resize(pos+1,0);

            return _array[pos];
        }

        inline T operator[] (unsigned int pos) const
        {
            // automatically resize array.
            if (_array.size()<=pos)
                _array.resize(pos+1,0);

            return _array[pos];
        }

    protected:
    
        mutable std::vector<T> _array;
};

template<class T>
class buffered_object : public Referenced
{
public:

	inline buffered_object():
	_array(crDisplaySettings::instance()->getMaxNumberOfGraphicsContexts())
	{}

	buffered_object& operator = (const buffered_object& rhs)
	{
		_array = rhs._array;
		return *this;
	}

	inline void setAllElementsTo(const T& t) { std::fill(_array.begin(),_array.end(),t); }

	inline void clear() { _array.clear(); }

	inline bool empty() const { return _array.empty(); }

	inline unsigned int size() const { return _array.size(); }

	inline void resize(unsigned int newSize) { _array.resize(newSize); }

	inline T& operator[] (unsigned int pos)
	{
		// automatically resize array.
		if (_array.size()<=pos)
			_array.resize(pos+1);

		return _array[pos];
	}

	inline const T& operator[] (unsigned int pos) const
	{
		// automatically resize array.
		if (_array.size()<=pos)
			_array.resize(pos+1);

		return _array[pos];
	}


protected:

	mutable std::vector<T> _array;
};
template<class T>
class buffered_ptrobject : public Referenced
{
public:

	inline buffered_ptrobject():
	_array(crDisplaySettings::instance()->getMaxNumberOfGraphicsContexts(),NULL)
	{}

	buffered_ptrobject& operator = (const buffered_ptrobject& rhs)
	{
		_array = rhs._array;
		return *this;
	}

	inline void setAllElementsTo(const T& t) { std::fill(_array.begin(),_array.end(),t); }

	inline void clear() { _array.clear(); }

	inline bool empty() const { return _array.empty(); }

	inline unsigned int size() const { return _array.size(); }

	inline void resize(unsigned int newSize) { _array.resize(newSize,NULL); }

	inline T& operator[] (unsigned int pos)
	{
		// automatically resize array.
		if (_array.size()<=pos)
			_array.resize(pos+1,NULL);

		return _array[pos];
	}

	inline const T& operator[] (unsigned int pos) const
	{
		// automatically resize array.
		if (_array.size()<=pos)
			_array.resize(pos+1,NULL);

		return _array[pos];
	}


protected:

	mutable std::vector<T> _array;
};
}

#endif
