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
#include <list>

#include <CRCore/crNotify.h>
#include <CRCore/crState.h>
#include <CRCore/crTimer.h>
#include <CRCore/crFrameStamp.h>
#include <CRCore/buffered_value.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crShader.h>
#include <CRCore/crBrain.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/crStreamBuf.h>
using namespace CRCore;

///////////////////////////////////////////////////////////////////////////
// static cache of glShaders flagged for deletion, which will actually
// be deleted in the correct GL context.

crMutex    crShader::s_mutex_deletedGlShaderCache;
crShader::DeletedGlShaderCache  crShader::s_deletedGlShaderCache;
unsigned int crShader::s_numberShaders = 0;

void crShader::deleteGlShader(unsigned int contextID, GLuint shader)
{
    if( shader )
    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedGlShaderCache);

        // add glShader to the cache for the appropriate context.
        s_deletedGlShaderCache[contextID].push_back(shader);
		//char gbuf[256];
		//sprintf(gbuf, "deleteGlShader:%d,%d\n\0", shader,s_deletedGlShaderCache[contextID].size());
		//gDebugInfo->debugInfo(CRCore::NOTICE, gbuf);
    }
}

void crShader::flushDeletedGlShaders(unsigned int contextID,double /*currentTime*/, double& availableTime)
{
    // if no time available don't try to flush objects.
    if (availableTime<=0.0) return;

    const GL2Extensions* extensions = GL2Extensions::Get(contextID,true);
    if( ! extensions->isGlslSupported() ) return;

    const CRCore::Timer& timer = *CRCore::Timer::instance();
    CRCore::Timer_t start_tick = timer.tick();
    double elapsedTime = 0.0;

    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedGlShaderCache);

		GlShaderHandleList& pList = s_deletedGlShaderCache[contextID];
        for(GlShaderHandleList::iterator titr=pList.begin();
            titr!=pList.end() && elapsedTime<availableTime;
            )
        {
			crShader::s_numberShaders--;
            extensions->glDeleteShader( *titr );
            titr = pList.erase( titr );
            elapsedTime = timer.delta_s(start_tick,timer.tick());
        }
    }

    availableTime -= elapsedTime;
}
void crShader::flushAllDeletedGlShaders(unsigned int contextID)
{
	const GL2Extensions* extensions = GL2Extensions::Get(contextID,true);
	if( ! extensions->isGlslSupported() ) return;

	{
		CRCore::ScopedLock<crMutex> lock(s_mutex_deletedGlShaderCache);
		int delcount = 0;
		GlShaderHandleList& pList = s_deletedGlShaderCache[contextID];
		for(GlShaderHandleList::iterator titr=pList.begin();
			titr!=pList.end();
			)
		{
			delcount++;
			extensions->glDeleteShader( *titr );
			titr = pList.erase( titr );
		}
		crShader::s_numberShaders -= delcount;
		char gbuf[256];
		sprintf(gbuf, "É¾³ýShaderÊý:%d,Ê£ÓàShaderÊý:%d\n\0", delcount, crShader::s_numberShaders);
		gDebugInfo->debugInfo(CRCore::NOTICE, gbuf);
	}
}
void crShader::discardDeletedGlShaders(unsigned int contextID)
{
    CRCore::ScopedLock<crMutex> lock(s_mutex_deletedGlShaderCache);

    GlShaderHandleList& pList = s_deletedGlShaderCache[contextID];
    pList.clear();
}

///////////////////////////////////////////////////////////////////////////
// CRCore::crShader
///////////////////////////////////////////////////////////////////////////

crShader::crShader(Type type) :
    m_type(type)
{
}

crShader::crShader(Type type, const std::string& source) :
    m_type(type)
{
    setShaderSource( source);
}

crShader::crShader(const crShader& rhs, const CRCore::crCopyOp& copyop):
    CRCore::crBase( rhs, copyop ),
    m_type(rhs.m_type),
    m_shaderSource(rhs.m_shaderSource)
{
}

crShader::~crShader()
{
	m_programSet.clear();
}

bool crShader::setType( Type t )
{
    if( m_type != UNDEFINED )
    {
        CRCore::notify(CRCore::WARN) << "cannot change type of crShader" << std::endl;
        return false;
    }

    m_type = t;
    return true;
}

int crShader::compare(const crShader& rhs) const
{
    if( this == &rhs ) return 0;

    if( getType() < rhs.getType() ) return -1;
    if( rhs.getType() < getType() ) return 1;

    if( getName() < rhs.getName() ) return -1;
    if( rhs.getName() < getName() ) return 1;

    if( getShaderSource() < rhs.getShaderSource() ) return -1;
    if( rhs.getShaderSource() < getShaderSource() ) return 1;

    return 0;
}

void crShader::setShaderSource( const std::string& sourceText )
{
    m_shaderSource = sourceText;
    dirtyShader();
}

const char* crShader::getTypename() const
{
    switch( getType() )
    {
        case VERTEX:    return "VERTEX";
        case FRAGMENT:  return "FRAGMENT";
        case GEOMETRY:  return "GEOMETRY";
        default:        return "UNDEFINED";
    }
}


crShader::Type crShader::getTypeId( const std::string& tname )
{
    if( tname == "VERTEX" )     return VERTEX;
    if( tname == "FRAGMENT" )   return FRAGMENT;
    if( tname == "GEOMETRY" )   return GEOMETRY;
    return UNDEFINED;
}

void crShader::compileShader( unsigned int contextID ) const
{
    PerContextShader* pcs = getPCS( contextID );
    if( pcs ) pcs->compileShader();
}


crShader::PerContextShader* crShader::getPCS(unsigned int contextID) const
{
    if( getType() == UNDEFINED )
    {
        CRCore::notify(CRCore::WARN) << "crShader type is UNDEFINED" << std::endl;
        return 0;
    }

    if( ! m_pcsList[contextID].valid() )
    {
        m_pcsList[contextID] = new PerContextShader( this, contextID );
    }
    return m_pcsList[contextID].get();
}


void crShader::attachShader(unsigned int contextID, GLuint program) const
{
    PerContextShader* pcs = getPCS( contextID );
    if( pcs ) pcs->attachShader( program );
}

void crShader::detachShader(unsigned int contextID, GLuint program) const
{
    PerContextShader* pcs = getPCS( contextID );
    if( pcs ) pcs->detachShader( program );
}


bool crShader::getGlShaderInfoLog(unsigned int contextID, std::string& log) const
{
    PerContextShader* pcs = getPCS( contextID );
    return (pcs) ? pcs->getInfoLog( log ) : false;
}


/////////////////////////////////////////////////////////////////////////
// A crShader stores pointers to the CRCore::Programs to which it is attached,
// so that if the crShader is marked for recompilation with
// crShader::dirtyShader(), the upstream crProgram can be marked for relinking.
// m_programSet does not use ref_ptrs, as that would cause a cyclical
// dependency, and neither the crProgram nor the crShader would be deleted.

bool crShader::addProgramRef( crProgram* program )
{
    ProgramSet::iterator itr = m_programSet.find(program);
    if( itr != m_programSet.end() ) return false;

    m_programSet.insert( program );
    return true;
}

bool crShader::removeProgramRef( crProgram* program )
{
    ProgramSet::iterator itr = m_programSet.find(program);
    if( itr == m_programSet.end() ) return false;

    m_programSet.erase( itr );
    return true;
}

void crShader::dirtyShader()
{
    // Mark our PCSs as needing recompilation.
    //for( unsigned int cxt=0; cxt < m_pcsList.size(); ++cxt )
    //{
    //    if( m_pcsList[cxt].valid() ) m_pcsList[cxt]->requestCompile();
    //}

    //// Also mark Programs that depend on us as needing relink.
    //for( ProgramSet::iterator itr = m_programSet.begin();
    //    itr != m_programSet.end(); ++itr )
    //{
    //    (*itr)->dirtyProgram();
    //}

	m_pcsList.clear();
	// Also mark Programs that depend on us as needing relink.
	for( ProgramSet::iterator itr = m_programSet.begin();
		itr != m_programSet.end(); ++itr )
	{
		(*itr)->dirtyProgram();
	}
}


/////////////////////////////////////////////////////////////////////////
// CRCore::crShader::PerContextShader
// PCS is the OSG abstraction of the per-context glShader
///////////////////////////////////////////////////////////////////////////

crShader::PerContextShader::PerContextShader(const crShader* shader, unsigned int contextID) :
        CRCore::Referenced(),
        m_contextID( contextID )
{
    m_shader = shader;
    m_extensions = GL2Extensions::Get( m_contextID, true );
    m_glShaderHandle = m_extensions->glCreateShader( shader->getType() );
	crShader::s_numberShaders++;
    requestCompile();
}


crShader::PerContextShader::~PerContextShader()
{
	//char gbuf[256];
	//sprintf(gbuf, "DelPerContextShader:%d,%d\n\0", m_contextID, m_glShaderHandle);
	//gDebugInfo->debugInfo(CRCore::NOTICE, gbuf);
    crShader::deleteGlShader( m_contextID, m_glShaderHandle );
	m_shader = NULL;
}


void crShader::PerContextShader::requestCompile()
{
    m_needsCompile = true;
    m_isCompiled = false;
}


void crShader::PerContextShader::compileShader()
{
    //if( ! m_needsCompile ) return;
    //m_needsCompile = false;

    //CRCore::notify(CRCore::INFO)
    //    << "\nCompiling " << m_shader->getTypename()
    //    << " source:\n" << m_shader->getShaderSource() << std::endl;

    //GLint compiled = GL_FALSE;
    //const char* sourceText = m_shader->getShaderSource().c_str();
    //m_extensions->glShaderSource( m_glShaderHandle, 1, &sourceText, NULL );
    //m_extensions->glCompileShader( m_glShaderHandle );
    //m_extensions->glGetShaderiv( m_glShaderHandle, GL_COMPILE_STATUS, &compiled );

    //m_isCompiled = (compiled == GL_TRUE);
    //if( ! m_isCompiled )
    //{
    //    CRCore::notify(CRCore::WARN) << m_shader->getTypename() << " glCompileShader \""
    //        << m_shader->getName() << "\" FAILED" << std::endl;

    //    std::string infoLog;
    //    if( getInfoLog(infoLog) )
    //    {
    //        CRCore::notify(CRCore::WARN) << m_shader->getTypename() << " crShader \""
    //            << m_shader->getName() << "\" infolog:\n" << infoLog << std::endl;
    //    }
    //}
    //else
    //{
    //    std::string infoLog;
    //    if( getInfoLog(infoLog) )
    //    {
    //        CRCore::notify(CRCore::INFO) << m_shader->getTypename() << " crShader \""
    //            << m_shader->getName() << "\" infolog:\n" << infoLog << std::endl;
    //    }
    //}

	if( ! m_needsCompile ) return;
	m_needsCompile = false;

	//CRCore::notify(CRCore::WARN)<<"Compiling source "<<m_shader->getName()<<std::endl;

	const char* renderer = (const char*) glGetString(GL_RENDERER);
	std::string rendererString(renderer ? renderer : "");
	//CRCore::notify(CRCore::FATAL)<<" renderer= "<<rendererString<<std::endl;
	//std::string rendercore;
	std::string source = m_shader->getShaderSource();
	if (rendererString.find("Radeon")!=std::string::npos || rendererString.find("RADEON")!=std::string::npos)
		source.insert(0,"#define ATI\n",12);
	else if (rendererString.find("GeForce")!=std::string::npos)
		source.insert(0,"#define NV\n",11);

	//if(m_shader->getName().find("final")!=-1)
	//       CRCore::notify(CRCore::FATAL)<<" compileShader = "<<m_shader->getName().c_str()<<std::endl;
	const std::string& shaderName = m_shader->getName();
	crDisplaySettings *ds = crDisplaySettings::instance(); 
	if(shaderName.find("final_fp") != -1)
	{
		//CRCore::notify(CRCore::FATAL)<<" hdr.shader_fp "<<ds->getWaveBlend()<<std::endl;
		if(ds->getDepthOfField())
			source.insert(0,"#define DOF\n",12);
		if(ds->getOutLine())
			source.insert(0,"#define OL\n",11);
		//if(ds->getNeedNormalDepthSample())
		//	source.insert(0,"#define NeedNormalDepth\n",24);
		//if(ds->getNPR())
		//	source.insert(0,"#define NPR\n",12);
	}
	else if(m_shader->getName().find("_cf") != -1 || shaderName.find("outline_fp") != -1)
	{//wch_fp,wch_vp
		if(ds->getNeedPixelDepth())
			source.insert(0,"#define NeedPixelDepth\n",23);

		if(ds->getHDR())
			source.insert(0,"#define HDR\n",12);
		if(crBrain::getInstance()->getFog()==1)
			source.insert(0,"#define DepthFog\n",17);
		if(ds->getNeedNormalDepthSample())
			source.insert(0,"#define NeedNormalDepth\n",24);
	}

	//if(m_shader->getName().find("sky.shader") != -1)
	//{
	//       CRCore::notify(CRCore::FATAL)<<" source= "<<source<<std::endl;
	//}

	//CRCore::notify(CRCore::FATAL)<<" source= "<<source<<std::endl;
	float version = m_extensions->getLanguageVersion() * 100.0f;
	if(version>130.0f)
	    source.insert(0,"#version 130\n",13);
	else
	{
		char glslversion[16];
		sprintf(glslversion,"#version %d\n\0",(int)(floor(version)));
		source.insert(0,glslversion,strlen(glslversion));
	}
	//CRCore::notify(CRCore::ALWAYS)<<" version= "<<version<<std::endl;

	GLint compiled = GL_FALSE;
	const char* sourceText = source.c_str();
	m_extensions->glShaderSource( m_glShaderHandle, 1, &sourceText, NULL );
	m_extensions->glCompileShader( m_glShaderHandle );
	m_extensions->glGetShaderiv( m_glShaderHandle, GL_COMPILE_STATUS, &compiled );

	m_isCompiled = (compiled == GL_TRUE);
	if( ! m_isCompiled )
	{
		CRCore::notify(CRCore::WARN) << m_shader->getTypename() << " glCompileShader \""
			<< m_shader->getName() << "\" FAILED" << std::endl;

		std::string infoLog;
		if( getInfoLog(infoLog) )
		{
			CRCore::notify(CRCore::WARN) << m_shader->getTypename() << " crShader \""
				<< m_shader->getName() << "\" infolog:\n" << infoLog << std::endl;
		}
	}
	//  else
	//  {
	//      std::string infoLog;
	//getInfoLog(infoLog);
	//CRCore::notify(CRCore::ALWAYS) << m_shader->getTypename() << " crShader \""
	//       << m_shader->getName()<<" SUCCESS "<< "\" infolog:\n" << infoLog << std::endl;
	//  }
	//CRCore::notify(CRCore::WARN)<<"Compiling source end "<<m_shader->getName()<<std::endl;
}

bool crShader::PerContextShader::getInfoLog( std::string& infoLog ) const
{
    return m_extensions->getShaderInfoLog( m_glShaderHandle, infoLog );
}

void crShader::PerContextShader::attachShader(GLuint program) const
{
    m_extensions->glAttachShader( program, m_glShaderHandle );
}

void crShader::PerContextShader::detachShader(GLuint program) const
{
    m_extensions->glDetachShader( program, m_glShaderHandle );
}
