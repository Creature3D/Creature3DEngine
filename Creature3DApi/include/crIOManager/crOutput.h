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
#ifndef CRIOMANAGER_OUTPUT
#define CRIOMANAGER_OUTPUT 1

#include <CRCore/crBase.h>

#include <CRIOManager/crExport.h>
#include <CRIOManager/crReaderWriter.h>

#include <string>
#include <map>

#include <fstream>

namespace CRIOManager {

/** ofstream wrapper class for adding support for indenting.
    Used in output of .CRCore ASCII files to improve their readability.*/
class /*CRIOMANAGER_EXPORT*/ crOutput : public std::ofstream
{
    public:

        crOutput();
        crOutput(const char* name);
        
        virtual ~crOutput();

		void setOptions(const crReaderWriter::Options* options) { m_options = options; }
		const crReaderWriter::Options* getOptions() const { return m_options.get(); }

        void open(const char *name);
        
        // comment out temporarily to avoid compilation problems, RO Jan 2002.
        // void open(const char *name,int mode);

        crOutput& indent();

    	/** wrap a string with "" quotes and use \" for any internal quotes.*/
    	std::string wrapString(const std::string& str);

        inline void setIndentStep(int step) { m_indentStep = step; }
        inline int getIndentStep() const { return m_indentStep; }

        inline void setIndent(int indent)  { m_indent = indent; }
        inline int getIndent() const { return m_indent; }

        inline void setNumIndicesPerLine(int num) { m_numIndicesPerLine = num; }
        inline int getNumIndicesPerLine() const { return m_numIndicesPerLine; }

        void moveIn();
        void moveOut();
        
        virtual bool writeObject(const CRCore::crBase& obj);
        

        bool getUniqueIDForObject(const CRCore::crBase* obj,std::string& uniqueID);
        bool createUniqueIDForObject(const CRCore::crBase* obj,std::string& uniqueID);
        bool registerUniqueIDForObject(const CRCore::crBase* obj,std::string& uniqueID);

        enum PathNameHint
        {
            AS_IS,
            FULL_PATH,
            RELATIVE_PATH,
            FILENAME_ONLY
        };
        
        inline void setPathNameHint(const PathNameHint pnh) { m_pathNameHint = pnh; }
        inline PathNameHint getPathNameHint() const { return m_pathNameHint; }

        virtual std::string getFileNameForOutput(const std::string& filename) const;
        const std::string& getFileName() const { return m_filename; }

		void setOutputTextureFiles(bool flag) { m_outputTextureFiles = flag; }
		bool getOutputTextureFiles() const { return m_outputTextureFiles; }

		virtual std::string getTextureFileNameForOutput();
        
    protected:


        virtual void init();

		CRCore::ref_ptr<const crReaderWriter::Options> m_options;

        int m_indent;
        int m_indentStep;
        
        int m_numIndicesPerLine;

        typedef std::map<const CRCore::crBase*,std::string> UniqueIDToLabelMapping;
        UniqueIDToLabelMapping m_objectToUniqueIDMap;
        
        std::string m_filename;
        
        PathNameHint m_pathNameHint;
		bool m_outputTextureFiles;
		unsigned int m_textureFileNameNumber;
};

}

#endif                                            // __SG_OUTPUT_H
