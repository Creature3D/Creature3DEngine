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
#include <CRCore/crNotify.h>
#include <CRCore/crBase.h>
#include <CRCore/crImage.h>
//#include <CRCore/crImageStream.h>
#include <CRCore/crNode.h>
#include <CRCore/crGroup.h>
#include <CRCore/crObject.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTextureRectangle.h>

#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crReadFile.h>

using namespace CRCore;
using namespace CRIOManager;

crBase* CRIOManager::readObjectFile(const std::string& filename,const crReaderWriter::Options* options)
{
    crReaderWriter::ReadResult rr = crRegistry::instance()->readObject(filename,options);
    if (rr.validObject()) return rr.takeObject();
    if (rr.error()) notify(WARN) << rr.message() << std::endl;
    return NULL;
}


crImage* CRIOManager::readImageFile(const std::string& filename,const crReaderWriter::Options* options)
{
    crReaderWriter::ReadResult rr = crRegistry::instance()->readImage(filename,options);
    if (rr.validImage()) return rr.takeImage();
    if (rr.error()) notify(WARN) << rr.message() << std::endl;
    return NULL;
}


crHeightField* CRIOManager::readHeightFieldFile(const std::string& filename,const crReaderWriter::Options* options)
{
    crReaderWriter::ReadResult rr = crRegistry::instance()->readHeightField(filename,options);
    if (rr.validHeightField()) return rr.takeHeightField();
    if (rr.error()) notify(WARN) << rr.message() << std::endl;
    return NULL;
}


crNode* CRIOManager::readNodeFile(const std::string& filename,const crReaderWriter::Options* options)
{
    crReaderWriter::ReadResult rr = crRegistry::instance()->readNode(filename,options);
    if (rr.validNode()) return rr.takeNode();
    if (rr.error()) notify(WARN) << rr.message() << std::endl;
    return NULL;
}

crNode* CRIOManager::readNodeFiles(std::vector<std::string>& commandLine,const crReaderWriter::Options* options)
{
    typedef std::vector<CRCore::crNode*> NodeArray;
    NodeArray nodeList;

    // note currently doesn't delete the loaded file entries from the command line yet...

    for(std::vector<std::string>::iterator itr=commandLine.begin();
        itr!=commandLine.end();
        ++itr)
    {
        if ((*itr)[0]!='-')
        {
            // not an option so assume string is a filename.
            CRCore::crNode *node = CRIOManager::readNodeFile( *itr , options );

            if( node != (CRCore::crNode *)0L )
            {
                if (node->getName().empty()) node->setName( *itr );
                nodeList.push_back(node);
            }

        }
    }
    
    if (nodeList.empty())
    {
        return NULL;
    }

    if (nodeList.size()==1)
    {
        return nodeList.front();
    }
    else  // size >1
    {
        CRCore::crGroup* group = new CRCore::crGroup;
        for(NodeArray::iterator itr=nodeList.begin();
            itr!=nodeList.end();
            ++itr)
        {
            group->addChild(*itr);
        }

        return group;
    }
    
}

crNode* CRIOManager::readNodeFiles(CRCore::crArgumentParser& arguments,const crReaderWriter::Options* options)
{

    typedef std::vector< CRCore::ref_ptr<CRCore::crNode> > NodeArray;
    NodeArray nodeList;

    std::string filename;
    while (arguments.read("--image",filename))
    {
        CRCore::ref_ptr<CRCore::crImage> image = readImageFile(filename.c_str(), options);
        if (image.valid()) nodeList.push_back(CRCore::createObjectForImage(image.get()));
    }

    //while (arguments.read("--movie",filename))
    //{
    //    CRCore::ref_ptr<CRCore::crImage> image = readImageFile(filename.c_str(), options);
    //    CRCore::ref_ptr<CRCore::crImageStream> imageStream = dynamic_cast<CRCore::crImageStream*>(image.get());
    //    if (imageStream.valid())
    //    {
    //        // start the stream playing.
    //        imageStream->play();

    //        CRCore::ref_ptr<CRCore::crGeometry> pictureQuad = 0;

    //        bool useTextureRectangle = true;
    //        if (useTextureRectangle)
    //        {
    //            pictureQuad = CRCore::createTexturedQuadGeometry(CRCore::Vec3(0.0f,0.0f,0.0f),
    //                                               CRCore::Vec3(image->s(),0.0f,0.0f),
    //                                               CRCore::Vec3(0.0f,0.0f,image->t()),
    //                                               0.0f,image->t(), image->s(),0.0f);

    //            pictureQuad->getOrCreateStateSet()->setTextureAttributeAndModes(0,
    //                        new CRCore::crTextureRectangle(image.get()),
    //                        CRCore::crStateAttribute::ON);
    //        }
    //        else
    //        {
    //            pictureQuad = CRCore::createTexturedQuadGeometry(CRCore::Vec3(0.0f,0.0f,0.0f),
    //                                               CRCore::Vec3(image->s(),0.0f,0.0f),
    //                                               CRCore::Vec3(0.0f,0.0f,image->t()),
    //                                               0.0f,0.0f, 1.0f,1.0f);

    //            pictureQuad->getOrCreateStateSet()->setTextureAttributeAndModes(0,
    //                        new CRCore::crTexture2D(image.get()),
    //                        CRCore::crStateAttribute::ON);
    //        }

    //        if (pictureQuad.valid())
    //        {
    //            CRCore::ref_ptr<CRCore::crObject> geode = new CRCore::crObject;
    //            geode->addDrawable(pictureQuad.get());
    //            nodeList.push_back(geode.get());

    //        }
    //    }
    //    else if (image.valid())
    //    {
    //        nodeList.push_back(CRCore::createGeodeForImage(image.get()));
    //    }
    //}

    while (arguments.read("--dem",filename))
    {
        CRCore::crHeightField* hf = readHeightFieldFile(filename.c_str(), options);
        if (hf)
        {
            CRCore::crObject* geode = new CRCore::crObject;
            geode->addDrawable(new CRCore::crShapeDrawable(hf));
            nodeList.push_back(geode);
        }
    }

    // note currently doesn't delete the loaded file entries from the command line yet...
    for(int pos=1;pos<arguments.argc();++pos)
    {
        if (!arguments.isOption(pos))
        {
            // not an option so assume string is a filename.
            CRCore::crNode *node = CRIOManager::readNodeFile( arguments[pos], options);

            if(node)
            {
                if (node->getName().empty()) node->setName( arguments[pos] );
                nodeList.push_back(node);
            }

        }
    }
    
    if (nodeList.empty())
    {
        return NULL;
    }

    if (nodeList.size()==1)
    {
        return nodeList.front().release();
    }
    else  // size >1
    {
        CRCore::crGroup* group = new CRCore::crGroup;
        for(NodeArray::iterator itr=nodeList.begin();
            itr!=nodeList.end();
            ++itr)
        {
            group->addChild((*itr).get());
        }

        return group;
    }
    
}
