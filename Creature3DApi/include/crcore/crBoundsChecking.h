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

#ifndef CRCORE_CRBOUNDSCHECKING_H
#define CRCORE_CRBOUNDSCHECKING_H 1

#include <CRCore/crNotify.h>

namespace CRCore {


/** if value is greater than or equal to minValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class T>
inline void clampGEQUAL(T& value,const T minValue,const char* valueName)
{
    if (value<minValue)
    {
        notify(WARN) << "Warning: "<<valueName<<" of "<<value<<" is below permitted minimum, clampping to "<<minValue<<"."<< std::endl;
        value = minValue;
    }
}

/** if value is less than or equal to maxValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class T>
inline void clampLEQUAL(T& value,const T maxValue,const char* valueName)
{
    if (value>maxValue)
    {
        notify(WARN) << "Warning: "<<valueName<<" of "<<value<<" is above permitted maximum, clampping to "<<maxValue<<"."<< std::endl;
        value = maxValue;
    }
}

/** if value is between or equal to minValue and maxValue do nothing - legal value,
  * otherwise clamp value to specified to range and return warning 
  * with valueName specifying which variable was clamped. Equivilant to
  * calling clampGEQUAL(value,minValue,valueName); clampLEQUAL(value,maxValue,valueName); */
template <class T>
inline void clampBetweenRange(T& value,const T minValue,const T maxValue,const char* valueName)
{
    if (value<minValue)
    {
        notify(WARN) << "Warning: "<<valueName<<" of "<<value<<" is below permitted minimum, clampping to "<<minValue<<"."<< std::endl;
        value = minValue;
    }
    else
    if (value>maxValue)
    {
        notify(WARN) << "Warning: "<<valueName<<" of "<<value<<" is above permitted maximum, clampping to "<<maxValue<<"."<< std::endl;
        value = maxValue;
    }
    
}

/** if array element value[i] is greater than or equal to minValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class A, class T>
inline void clampArrayElementGEQUAL(A& value,unsigned int i,const T minValue,const char* valueName)
{
    if (value[i]<minValue)
    {
        notify(WARN) << "Warning: "<<valueName<<"["<<i<<"] of "<<value[i]<<" is below permitted minimum, clampping to "<<minValue<<"."<< std::endl;
        value[i] = minValue;
    }
}

/** if array element value[i] is less than or equal to maxValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class A, class T>
inline void clampArrayElementLEQUAL(A& value,unsigned int i,const T maxValue,const char* valueName)
{
    if (value[i]>maxValue)
    {
        notify(WARN) << "Warning: "<<valueName<<"["<<i<<"] of "<<value[i]<<" is above permitted maximum, clampping to "<<maxValue<<"."<< std::endl;
        value = maxValue;
    }
}

/** if array element value[i] is between or equal to minValue and maxValue do nothing - legal value,
  * otherwise clamp value to specified to range and return warning 
  * with valueName specifying which variable was clamped. Equivilant to
  * calling clampGEQUAL(value,minValue,valueName); clampLEQUAL(value,maxValue,valueName); */
template <class A, class T>
inline void clampArrayElementBetweenRange(A& value,unsigned int i,const T minValue,const T maxValue,const char* valueName)
{
    if (value[i]<minValue)
    {
        notify(WARN) << "Warning: "<<valueName<<"["<<i<<"] of "<<value[i]<<" is below permitted minimum, clampping to "<<minValue<<"."<< std::endl;
        value[i] = minValue;
    }
    else
    if (value[i]>maxValue)
    {
        notify(WARN) << "Warning: "<<valueName<<"["<<i<<"] of "<<value[i]<<" is above permitted maximum, clampping to "<<maxValue<<"."<< std::endl;
        value[i] = maxValue;
    }
    
}

/** if array elements are greater than or equal to minValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class A, class T>
inline void clampArrayElementsGEQUAL(A& value,unsigned int first,unsigned int last,const T minValue,const char* valueName)
{
    for(unsigned int i=first;i<=last;++i)
        clampArrayElementGEQUAL(value,i,minValue,valueName);
}

/** if array elements are less than or equal to maxValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class A, class T>
inline void clampArrayElementsLEQUAL(A& value,unsigned int first,unsigned int last,const T maxValue,const char* valueName)
{
    for(unsigned int i=first;i<=last;++i)
        clampArrayElementLEQUAL(value,i,maxValue,valueName);
}

/** if array elements are between or equal to minValue and maxValue do nothing - legal value,
  * otherwise clamp value to specified to range and return warning 
  * with valueName specifying which variable was clamped. Equivalent to
  * calling clampGEQUAL(value,minValue,valueName); clampLEQUAL(value,maxValue,valueName); */
template <class A, class T>
inline void clampArrayElementsBetweenRange(A& value,unsigned int first,unsigned int last,const T minValue,const T maxValue,const char* valueName)
{
    for(unsigned int i=first;i<=last;++i)
        clampArrayElementBetweenRange(value,i,minValue,maxValue,valueName);    
}


/** if array4 elements are greater than or equal to minValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class A, class T>
inline void clampArray3GEQUAL(A& value,const T minValue,const char* valueName)
{
    clampArrayElementsGEQUAL(value,0u,2u,minValue,valueName);
}

/** if array4 elements are is less than or equal to maxValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class A, class T>
inline void clampArray3LEQUAL(A& value,const T maxValue,const char* valueName)
{
    clampArrayElementsLEQUAL(value,0u,2u,maxValue,valueName);
}

/** if array4 elements are between or equal to minValue and maxValue do nothing - legal value,
  * otherwise clamp value to specified to range and return warning 
  * with valueName specifying which variable was clamped. Equivilant to
  * calling clampGEQUAL(value,minValue,valueName); clampLEQUAL(value,maxValue,valueName); */
template <class A, class T>
inline void clampArray3BetweenRange(A& value,const T minValue,const T maxValue,const char* valueName)
{
    clampArrayElementsBetweenRange(value,0u,2u,minValue,maxValue,valueName);
}



/** if array4 elements are greater than or equal to minValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class A, class T>
inline void clampArray4GEQUAL(A& value,const T minValue,const char* valueName)
{
    clampArrayElementsGEQUAL(value,0u,3u,minValue,valueName);
}

/** if array4 elements are is less than or equal to maxValue do nothing - legal value,
  * otherwise clamp value to specified maximum value and return warning 
  * with valueName specifying which variable was clamped.*/
template <class A, class T>
inline void clampArray4LEQUAL(A& value,unsigned int first,unsigned int last,const T maxValue,const char* valueName)
{
    clampArrayElementsLEQUAL(value,0u,3u,maxValue,valueName);
}

/** if array4 elements are between or equal to minValue and maxValue do nothing - legal value,
  * otherwise clamp value to specified to range and return warning 
  * with valueName specifying which variable was clamped. Equivilant to
  * calling clampGEQUAL(value,minValue,valueName); clampLEQUAL(value,maxValue,valueName); */
template <class A, class T>
inline void clampArray4BetweenRange(A& value,const T minValue,const T maxValue,const char* valueName)
{
    clampArrayElementsBetweenRange(value,0u,3u,minValue,maxValue,valueName);
}

}

#endif
