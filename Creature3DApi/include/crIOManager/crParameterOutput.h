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
#ifndef CRIOMANAGER_PARAMETEROUTPUT
#define CRIOMANAGER_PARAMETEROUTPUT 1

#include <CRIOManager/crOutput.h>

namespace CRIOManager {

class crParameterOutput
{
    public:
    
        crParameterOutput(crOutput& fw):
            m_fw(fw),
            m_numItemsPerLine(fw.getNumIndicesPerLine()),
            m_column(0) {}
            
        crParameterOutput(crOutput& fw,int numItemsPerLine):
            m_fw(fw),
            m_numItemsPerLine(numItemsPerLine),
            m_column(0) {}

        void begin()
        {
            m_fw.indent() << "{"<<std::endl;
            m_fw.moveIn();
        }

        void newLine()
        {
            if (m_column!=0) m_fw << std::endl;
            m_column = 0;
        }

        void end()
        {
            if (m_column!=0) m_fw << std::endl;
            m_fw.moveOut();
            m_fw.indent() << "}"<<std::endl;
            m_column = 0;
        }
    
        template<class T>
        void write(const T& t)
        {
            if (m_column==0) m_fw.indent();

            m_fw << t;
            
            ++m_column;
            if (m_column==m_numItemsPerLine)
            {
                m_fw << std::endl;
                m_column = 0;
            }
            else
            {
                m_fw << " ";
            }

        }
        
        template<class Iterator>
        void write(Iterator first, Iterator last)
        {
            for(Iterator itr=first;
                itr!=last;
                ++itr)
            {
                write(*itr);
            }

        }
        
        template<class Iterator>
        void writeAsInts(Iterator first, Iterator last)
        {
            for(Iterator itr=first;
                itr!=last;
                ++itr)
            {
                write((int)*itr);
            }

        }
        
        
    protected:
    
        crOutput& m_fw;
        int     m_numItemsPerLine;
        int     m_column;
        
};


template<class Iterator>
void writeArray(crOutput& fw, Iterator first, Iterator last,int noItemsPerLine=0)
{
    if (noItemsPerLine==0) noItemsPerLine=fw.getNumIndicesPerLine();

    fw.indent() << "{"<<std::endl;
    fw.moveIn();

    int column=0;
    
    for(Iterator itr=first;
        itr!=last;
        ++itr)
    {
        if (column==0) fw.indent();
        
        fw << *itr;
        
        ++column;
        if (column==noItemsPerLine)
        {
            fw << std::endl;
            column = 0;
        }
        else
        {
            fw << " ";
        }
    }
    if (column!=0) fw << std::endl;
    
    fw.moveOut();
    fw.indent()<<"}"<<std::endl;
    
}


template<class Iterator>
void writeArrayAsInts(crOutput& fw, Iterator first, Iterator last,int noItemsPerLine=0)
{
    if (noItemsPerLine==0) noItemsPerLine=fw.getNumIndicesPerLine();

    fw.indent() << "{"<<std::endl;
    fw.moveIn();

    int column=0;
    
    for(Iterator itr=first;
        itr!=last;
        ++itr)
    {
        if (column==0) fw.indent();
        
        fw << (int)*itr;
        
        ++column;
        if (column==noItemsPerLine)
        {
            fw << std::endl;
            column = 0;
        }
        else
        {
            fw << " ";
        }
    }
    if (column!=0) fw << std::endl;
    
    fw.moveOut();
    fw.indent()<<"}"<<std::endl;
    
}


}

#endif                                            // __SG_OUTPUT_H
