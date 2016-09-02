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

#include <list>

#include <CRCore/crNotify.h>
#include <CRCore/crState.h>
#include <CRCore/crTimer.h>
#include <CRCore/buffered_value.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crProgram.h>
#include <CRCore/crShader.h>
#include <crgl/GL2Extensions.h>

#include <CRCore/thread/crScopedLock.h>

using namespace CRCore;

///////////////////////////////////////////////////////////////////////////
// static cache of glPrograms flagged for deletion, which will actually
// be deleted in the correct GL context.

crMutex    crProgram::s_mutex_deletedGlProgramCache;
crProgram::DeletedGlProgramCache crProgram::s_deletedGlProgramCache;

void crProgram::deleteGlProgram(unsigned int contextID, GLuint program)
{
    if( program )
    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedGlProgramCache);

        // add glProgram to the cache for the appropriate context.
        s_deletedGlProgramCache[contextID].push_back(program);
    }
}

void crProgram::flushDeletedGlPrograms(unsigned int contextID,double /*currentTime*/, double& availableTime)
{
    // if no time available don't try to flush objects.
    if (availableTime<=0.0) return;

    CRCore::ScopedLock<crMutex> lock(s_mutex_deletedGlProgramCache);
    const GL2Extensions* extensions = GL2Extensions::Get(contextID,true);
    if( ! extensions->isGlslSupported() ) return;

    const CRCore::Timer& timer = *CRCore::Timer::instance();
    CRCore::Timer_t start_tick = timer.tick();
    double elapsedTime = 0.0;

    {

        GlProgramHandleList& pList = s_deletedGlProgramCache[contextID];
        for(GlProgramHandleList::iterator titr=pList.begin();
            titr!=pList.end() && elapsedTime<availableTime;
            )
        {
            extensions->glDeleteProgram( *titr );
            titr = pList.erase( titr );
            elapsedTime = timer.delta_s(start_tick,timer.tick());
        }
    }

    availableTime -= elapsedTime;
}
void crProgram::flushAllDeletedGlPrograms(unsigned int contextID)
{
	CRCore::ScopedLock<crMutex> lock(s_mutex_deletedGlProgramCache);
	const GL2Extensions* extensions = GL2Extensions::Get(contextID,true);
	if( ! extensions->isGlslSupported() ) return;
	{

		GlProgramHandleList& pList = s_deletedGlProgramCache[contextID];
		for(GlProgramHandleList::iterator titr=pList.begin();
			titr!=pList.end();
			)
		{
			extensions->glDeleteProgram( *titr );
			titr = pList.erase( titr );
		}
	}
}
void crProgram::discardDeletedGlPrograms(unsigned int contextID)
{
    CRCore::ScopedLock<crMutex> lock(s_mutex_deletedGlProgramCache);
    GlProgramHandleList& pList = s_deletedGlProgramCache[contextID];
    pList.clear();
}


///////////////////////////////////////////////////////////////////////////
// CRCore::crProgram
///////////////////////////////////////////////////////////////////////////

crProgram::crProgram() :
    m_geometryVerticesOut(1), m_geometryInputType(GL_TRIANGLES),
    m_geometryOutputType(GL_TRIANGLE_STRIP)
{
}


crProgram::crProgram(const crProgram& rhs, const CRCore::crCopyOp& copyop):
    CRCore::crStateAttribute(rhs, copyop)
{
    CRCore::notify(CRCore::FATAL) << "how got here?" << std::endl;
    m_geometryVerticesOut = rhs.m_geometryVerticesOut;
    m_geometryInputType = rhs.m_geometryInputType;
    m_geometryOutputType = rhs.m_geometryOutputType;
}


crProgram::~crProgram()
{
    // inform any attached Shaders that we're going away
    for( unsigned int i=0; i < m_shaderList.size(); ++i )
    {
        m_shaderList[i]->removeProgramRef( this );
    }
}


int crProgram::compare(const CRCore::crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Parameter macro's below.
    COMPARE_StateAttribute_Types(crProgram,sa)
    
    if( m_shaderList.size() < rhs.m_shaderList.size() ) return -1;
    if( rhs.m_shaderList.size() < m_shaderList.size() ) return 1;

    if( getName() < rhs.getName() ) return -1;
    if( rhs.getName() < getName() ) return 1;

    if( m_geometryVerticesOut < rhs.m_geometryVerticesOut ) return -1;
    if( rhs.m_geometryVerticesOut < m_geometryVerticesOut ) return 1;

    if( m_geometryInputType < rhs.m_geometryInputType ) return -1;
    if( rhs.m_geometryInputType < m_geometryInputType ) return 1;

    if( m_geometryOutputType < rhs.m_geometryOutputType ) return -1;
    if( rhs.m_geometryOutputType < m_geometryOutputType ) return 1;

    ShaderList::const_iterator litr=m_shaderList.begin();
    ShaderList::const_iterator ritr=rhs.m_shaderList.begin();
    for(;
        litr!=m_shaderList.end();
        ++litr,++ritr)
    {
        int result = (*litr)->compare(*(*ritr));
        if (result!=0) return result;
    }

    return 0; // passed all the above comparison macro's, must be equal.
}

int crProgram::compare(const CRCore::crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Parameter macro's below.
	COMPARE_StateAttribute_Types(crProgram,sa)

		if( m_shaderList.size() < rhs.m_shaderList.size() ) return -1;
	if( rhs.m_shaderList.size() < m_shaderList.size() ) return 1;

	if( getName() < rhs.getName() ) return -1;
	if( rhs.getName() < getName() ) return 1;

	if( m_geometryVerticesOut < rhs.m_geometryVerticesOut ) return -1;
	if( rhs.m_geometryVerticesOut < m_geometryVerticesOut ) return 1;

	if( m_geometryInputType < rhs.m_geometryInputType ) return -1;
	if( rhs.m_geometryInputType < m_geometryInputType ) return 1;

	if( m_geometryOutputType < rhs.m_geometryOutputType ) return -1;
	if( rhs.m_geometryOutputType < m_geometryOutputType ) return 1;

	ShaderList::const_iterator litr=m_shaderList.begin();
	ShaderList::const_iterator ritr=rhs.m_shaderList.begin();
	for(;
		litr!=m_shaderList.end();
		++litr,++ritr)
	{
		int result = (*litr)->compare(*(*ritr));
		if (result!=0) return result;
	}

	return 0; // passed all the above comparison macro's, must be equal.
}

void crProgram::compile( CRCore::crState& state ) const
{
    if( isFixedFunction() ) return;

    const unsigned int contextID = state.getContextID();

    for( unsigned int i=0; i < m_shaderList.size(); ++i )
    {
        m_shaderList[i]->compileShader( contextID );
    }

    getPCP( contextID )->linkProgram();
}

//void crProgram::setThreadSafeRefUnref(bool threadSafe)
//{
//    crStateAttribute::setThreadSafeRefUnref(threadSafe);
//
//    for( unsigned int i=0; i < m_shaderList.size(); ++i )
//    {
//        if (m_shaderList[i].valid()) m_shaderList[i]->setThreadSafeRefUnref(threadSafe);
//    }
//}

void crProgram::dirtyProgram()
{
    // mark our PCPs as needing relink
    for( unsigned int cxt=0; cxt < m_pcpList.size(); ++cxt )
    {
        if( m_pcpList[cxt].valid() ) m_pcpList[cxt]->requestLink();
    }
}


//void crProgram::resizeGLObjectBuffers(unsigned int maxSize)
//{
//    for( unsigned int i=0; i < m_shaderList.size(); ++i )
//    {
//        if (m_shaderList[i].valid()) m_shaderList[i]->resizeGLObjectBuffers(maxSize);
//    }
//
//    m_pcpList.resize(maxSize);
//}

void crProgram::releaseObjects(CRCore::crState* state)
{
/*    for( unsigned int i=0; i < m_shaderList.size(); ++i )
    {
        if (m_shaderList[i].valid()) m_shaderList[i]->releaseObjects(state);
    }

    if (!state) m_pcpList.setAllElementsTo(0);
    else
    {
        unsigned int contextID = state->getContextID();
        m_pcpList[contextID] = 0;
    } */  

	for( unsigned int i=0; i < m_shaderList.size(); ++i )
	{
		m_shaderList[i]->dirtyShader();
	}

	m_pcpList.clear();
}


bool crProgram::addShader( crShader* shader )
{
    if( !shader ) return false;

    // crShader can only be added once to a crProgram
    for( unsigned int i=0; i < m_shaderList.size(); ++i )
    {
        if( shader == m_shaderList[i].get() ) return false;
    }

    // Add shader to PCPs
    for( unsigned int cxt=0; cxt < m_pcpList.size(); ++cxt )
    {
        if( m_pcpList[cxt].valid() ) m_pcpList[cxt]->addShaderToAttach( shader );
    }

    shader->addProgramRef( this );
    m_shaderList.push_back( shader );
    dirtyProgram();
    return true;
}


bool crProgram::removeShader( crShader* shader )
{
    if( !shader ) return false;

    // crShader must exist to be removed.
    for( ShaderList::iterator itr = m_shaderList.begin();
         itr != m_shaderList.end();
         ++itr)
    {
        if( shader == itr->get() )
        {
            // Remove shader from PCPs
            for( unsigned int cxt=0; cxt < m_pcpList.size(); ++cxt )
            {
                if( m_pcpList[cxt].valid() ) m_pcpList[cxt]->addShaderToDetach( shader );
            }

            shader->removeProgramRef( this );
            m_shaderList.erase(itr);

            dirtyProgram();
            return true;
        }
    }

    return false;
}


void crProgram::setParameter( GLenum pname, GLint value )
{
    switch( pname )
    {
        case GL_GEOMETRY_VERTICES_OUT_EXT:
            m_geometryVerticesOut = value;
            dirtyProgram();
            break;
        case GL_GEOMETRY_INPUT_TYPE_EXT:
            m_geometryInputType = value;
            dirtyProgram();    // needed?
            break;
        case GL_GEOMETRY_OUTPUT_TYPE_EXT:
            m_geometryOutputType = value;
            dirtyProgram();    // needed?
            break;
        default:
            CRCore::notify(CRCore::WARN) << "setParameter invalid param " << pname << std::endl;
            break;
    }
}

GLint crProgram::getParameter( GLenum pname ) const
{
    switch( pname )
    {
        case GL_GEOMETRY_VERTICES_OUT_EXT: return m_geometryVerticesOut;
        case GL_GEOMETRY_INPUT_TYPE_EXT:   return m_geometryInputType;
        case GL_GEOMETRY_OUTPUT_TYPE_EXT:  return m_geometryOutputType;
    }
    CRCore::notify(CRCore::WARN) << "getParameter invalid param " << pname << std::endl;
    return 0;
}


void crProgram::addBindAttribLocation( const std::string& name, GLuint index )
{
    m_attribBindingList[name] = index;
    dirtyProgram();
}

void crProgram::removeBindAttribLocation( const std::string& name )
{
    m_attribBindingList.erase(name);
    dirtyProgram();
}

void crProgram::addBindFragDataLocation( const std::string& name, GLuint index )
{
    m_fragDataBindingList[name] = index;
    dirtyProgram();
}

void crProgram::removeBindFragDataLocation( const std::string& name )
{
    m_fragDataBindingList.erase(name);
    dirtyProgram();
}

void crProgram::apply( CRCore::crState& state ) const
{
    const unsigned int contextID = state.getContextID();
    const GL2Extensions* extensions = GL2Extensions::Get(contextID,true);
    if( ! extensions->isGlslSupported() ) return;

    if( isFixedFunction() )
    {
        extensions->glUseProgram( 0 );
        state.setLastAppliedProgramObject(0);
        return;
    }

    PerContextProgram* pcp = getPCP( contextID );
    if( pcp->needsLink() ) compile( state );
    if( pcp->isLinked() )
    {
        // for shader debugging: to minimize performance impact,
        // optionally validate based on notify level.
        // TODO: enable this using notify level, or perhaps its own getenv()?
        //if( CRCore::isNotifyEnabled(CRCore::INFO) )
        //    pcp->validateProgram();

        pcp->useProgram();
        state.setLastAppliedProgramObject(pcp);
    }
    else
    {
        // program not usable, fallback to fixed function.
        extensions->glUseProgram( 0 );
        state.setLastAppliedProgramObject(0);
    }
}


crProgram::PerContextProgram* crProgram::getPCP(unsigned int contextID) const
{
    if( ! m_pcpList[contextID].valid() )
    {
        m_pcpList[contextID] = new PerContextProgram( this, contextID );

        // attach all PCSs to this new PCP
        for( unsigned int i=0; i < m_shaderList.size(); ++i )
        {
            m_pcpList[contextID]->addShaderToAttach( m_shaderList[i].get() );
        }
    }

    return m_pcpList[contextID].get();
}


bool crProgram::isFixedFunction() const
{
    // A crProgram object having no attached Shaders is a special case:
    // it indicates that programmable shading is to be disabled,
    // and thus use GL 1.x "fixed functionality" rendering.
    return m_shaderList.empty();
}


bool crProgram::getGlProgramInfoLog(unsigned int contextID, std::string& log) const
{
    return getPCP( contextID )->getInfoLog( log );
}

const crProgram::ActiveVarInfoMap& crProgram::getActiveUniforms(unsigned int contextID) const
{
    return getPCP( contextID )->getActiveUniforms();
}

const crProgram::ActiveVarInfoMap& crProgram::getActiveAttribs(unsigned int contextID) const
{
    return getPCP( contextID )->getActiveAttribs();
}

///////////////////////////////////////////////////////////////////////////
// CRCore::crProgram::PerContextProgram
// PCP is an OSG abstraction of the per-context glProgram
///////////////////////////////////////////////////////////////////////////

crProgram::PerContextProgram::PerContextProgram(const crProgram* program, unsigned int contextID ) :
        CRCore::Referenced(),
        m_contextID( contextID )
{
    m_program = program;
    m_extensions = GL2Extensions::Get( m_contextID, true );
    m_glProgramHandle = m_extensions->glCreateProgram();
    requestLink();
}

crProgram::PerContextProgram::~PerContextProgram()
{
    crProgram::deleteGlProgram( m_contextID, m_glProgramHandle );
	m_program = NULL;
}


void crProgram::PerContextProgram::requestLink()
{
    m_needsLink = true;
    m_isLinked = false;
}


void crProgram::PerContextProgram::linkProgram()
{
    if( ! m_needsLink ) return;
    m_needsLink = false;

    //CRCore::notify(CRCore::INFO)
    //    << "Linking CRCore::crProgram \"" << m_program->getName() << "\""
    //    << " id=" << m_glProgramHandle
    //    << " contextID=" << m_contextID
    //    <<  std::endl;

    if (m_extensions->isGeometryShader4Supported())
    {
        m_extensions->glProgramParameteri( m_glProgramHandle, GL_GEOMETRY_VERTICES_OUT_EXT, m_program->m_geometryVerticesOut );
        m_extensions->glProgramParameteri( m_glProgramHandle, GL_GEOMETRY_INPUT_TYPE_EXT, m_program->m_geometryInputType );
        m_extensions->glProgramParameteri( m_glProgramHandle, GL_GEOMETRY_OUTPUT_TYPE_EXT, m_program->m_geometryOutputType );
    }
    
    // Detach removed shaders
    for( unsigned int i=0; i < m_shadersToDetach.size(); ++i )
    {
        m_shadersToDetach[i]->detachShader( m_contextID, m_glProgramHandle );
    }
    m_shadersToDetach.clear();
    
    // Attach new shaders
    for( unsigned int i=0; i < m_shadersToAttach.size(); ++i )
    {
        m_shadersToAttach[i]->attachShader( m_contextID, m_glProgramHandle );
    }
    m_shadersToAttach.clear();

    m_uniformInfoMap.clear();
    m_attribInfoMap.clear();
    m_lastAppliedUniformMap.clear();

    // set any explicit vertex attribute bindings
    const AttribBindingList& bindlist = m_program->getAttribBindingList();
    for( AttribBindingList::const_iterator itr = bindlist.begin();
        itr != bindlist.end(); ++itr )
    {
        m_extensions->glBindAttribLocation( m_glProgramHandle, itr->second, itr->first.c_str() );
    }

    // set any explicit frag data bindings
    const FragDataBindingList& fdbindlist = m_program->getFragDataBindingList();
    for( FragDataBindingList::const_iterator itr = fdbindlist.begin();
        itr != fdbindlist.end(); ++itr )
    {
        m_extensions->glBindFragDataLocation( m_glProgramHandle, itr->second, itr->first.c_str() );
    }

    // link the glProgram
    GLint linked = GL_FALSE;
    m_extensions->glLinkProgram( m_glProgramHandle );
    m_extensions->glGetProgramiv( m_glProgramHandle, GL_LINK_STATUS, &linked );
    m_isLinked = (linked == GL_TRUE);
    if( ! m_isLinked )
    {
        CRCore::notify(CRCore::WARN) << "glLinkProgram \""<< m_program->getName() << "\" FAILED" << std::endl;

        std::string infoLog;
        if( getInfoLog(infoLog) )
        {
            CRCore::notify(CRCore::WARN) << "crProgram \""<< m_program->getName() << "\" " 
                                      "infolog:\n" << infoLog << std::endl;
        }
        
        return;
    }
    //else
    //{
    //    std::string infoLog;
    //    if( getInfoLog(infoLog) )
    //    {
    //        CRCore::notify(CRCore::INFO) << "crProgram \""<< m_program->getName() << "\" "<<
    //                                  "link succeded, infolog:\n" << infoLog << std::endl;
    //    }
    //}

    // build m_uniformInfoMap
    GLint numUniforms = 0;
    GLsizei maxLen = 0;
    m_extensions->glGetProgramiv( m_glProgramHandle, GL_ACTIVE_UNIFORMS, &numUniforms );
    m_extensions->glGetProgramiv( m_glProgramHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen );
    if( (numUniforms > 0) && (maxLen > 1) )
    {
        GLint size = 0;
        GLenum type = 0;
        GLchar* name = new GLchar[maxLen];

        for( GLint i = 0; i < numUniforms; ++i )
        {
            m_extensions->glGetActiveUniform( m_glProgramHandle,
                    i, maxLen, 0, &size, &type, name );

            GLint loc = m_extensions->glGetUniformLocation( m_glProgramHandle, name );
            
            if( loc != -1 )
            {
                m_uniformInfoMap[name] = ActiveVarInfo(loc,type,size);

                //CRCore::notify(CRCore::INFO)
                //    << "\tUniform \"" << name << "\""
                //    << " loc="<< loc
                //    << " size="<< size
                //    << " type=" << Uniform::getTypename((Uniform::Type)type)
                //    << std::endl;
            }
        }
        delete [] name;
    }

    // build m_attribInfoMap
    GLint numAttrib = 0;
    m_extensions->glGetProgramiv( m_glProgramHandle, GL_ACTIVE_ATTRIBUTES, &numAttrib );
    m_extensions->glGetProgramiv( m_glProgramHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLen );
    if( (numAttrib > 0) && (maxLen > 1) )
    {
        GLint size = 0;
        GLenum type = 0;
        GLchar* name = new GLchar[maxLen];

        for( GLint i = 0; i < numAttrib; ++i )
        {
            m_extensions->glGetActiveAttrib( m_glProgramHandle,
                    i, maxLen, 0, &size, &type, name );

            GLint loc = m_extensions->glGetAttribLocation( m_glProgramHandle, name );
            
            if( loc != -1 )
            {
                m_attribInfoMap[name] = ActiveVarInfo(loc,type,size);

                //CRCore::notify(CRCore::INFO)
                //    << "\tAttrib \"" << name << "\""
                //    << " loc=" << loc
                //    << " size=" << size
                //    << std::endl;
            }
        }
        delete [] name;
    }
    //CRCore::notify(CRCore::INFO) << std::endl;
}

bool crProgram::PerContextProgram::validateProgram()
{
    GLint validated = GL_FALSE;
    m_extensions->glValidateProgram( m_glProgramHandle );
    m_extensions->glGetProgramiv( m_glProgramHandle, GL_VALIDATE_STATUS, &validated );
    if( validated == GL_TRUE)
        return true;

    CRCore::notify(CRCore::NOTICE)
        << "glValidateProgram FAILED \"" << m_program->getName() << "\""
        << " id=" << m_glProgramHandle
        << " contextID=" << m_contextID
        <<  std::endl;

    std::string infoLog;
    if( getInfoLog(infoLog) )
        CRCore::notify(CRCore::NOTICE) << "infolog:\n" << infoLog << std::endl;

    CRCore::notify(CRCore::NOTICE) << std::endl;
    
    return false;
}

bool crProgram::PerContextProgram::getInfoLog( std::string& infoLog ) const
{
    return m_extensions->getProgramInfoLog( m_glProgramHandle, infoLog );
}

void crProgram::PerContextProgram::useProgram() const
{
    m_extensions->glUseProgram( m_glProgramHandle  );
}
