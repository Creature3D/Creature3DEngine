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
#ifndef CRCORE_CRUNIFORM
#define CRCORE_CRUNIFORM 1

#include <CRCore/ref_ptr.h>
#include <CRCore/crArray.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMatrix.h>
#include <crgl/GL2Extensions.h>
#include <CRCore/crNotify.h>

#include <string>
#include <vector>

namespace CRCore {

// forward declare
class GL2Extensions;
class crNodeVisitor;

///////////////////////////////////////////////////////////////////////////
// C++ classes to represent the GLSL-specific "mat2" & "mat3" types.

class CR_EXPORT crMatrix2
{
    public:
        crMatrix2() { makeIdentity(); }
        crMatrix2( const crMatrix2& mat ) { set(mat.ptr()); }
        crMatrix2( float a00, float a01,
                 float a10, float a11 )
        {
            set( a00, a01, a10, a11 );
        }
        ~crMatrix2() {}
        float& operator()(int row, int col) { return m_mat[row][col]; }
        float operator()(int row, int col) const { return m_mat[row][col]; }

        crMatrix2& operator = (const crMatrix2& rhs)
        {
            if( &rhs == this ) return *this;
            set(rhs.ptr());
            return *this;
        }

        void set(const crMatrix2& rhs) { set(rhs.ptr()); }

        void set(float const * const ptr)
        {
            float* local_ptr = (float*)m_mat;
            for(int i=0;i<4;++i) local_ptr[i]=ptr[i];
        }

        void set(float a00, float a01,
                 float a10, float a11)
         {
             m_mat[0][0]=a00; m_mat[0][1]=a01;
             m_mat[1][0]=a10; m_mat[1][1]=a11;
         }

        float* ptr() { return (float*)m_mat; }
        const float* ptr() const { return (const float*)m_mat; }

        float& operator [] (int i) {return ptr()[i];}
        float operator [] (int i) const {return ptr()[i];}

        void makeIdentity() { set( 1, 0, 0, 1 ); }

    protected:
        float m_mat[2][2];
};


class CR_EXPORT crMatrix3
{
    public:
        crMatrix3() { makeIdentity(); }
        crMatrix3( const crMatrix3& mat ) { set(mat.ptr()); }
        crMatrix3( float a00, float a01, float a02,
                 float a10, float a11, float a12,
                 float a20, float a21, float a22 )
        {
            set( a00, a01, a02, a10, a11, a12, a20, a21, a22 );
        }
        ~crMatrix3() {}
        float& operator()(int row, int col) { return m_mat[row][col]; }
        float operator()(int row, int col) const { return m_mat[row][col]; }

        crMatrix3& operator = (const crMatrix3& rhs)
        {
            if( &rhs == this ) return *this;
            set(rhs.ptr());
            return *this;
        }

        void set(const crMatrix3& rhs) { set(rhs.ptr()); }

        void set(float const * const ptr)
        {
            float* local_ptr = (float*)m_mat;
            for(int i=0;i<9;++i) local_ptr[i]=ptr[i];
        }

        void set(float a00, float a01, float a02,
                 float a10, float a11, float a12,
                 float a20, float a21, float a22 )
         {
             m_mat[0][0]=a00; m_mat[0][1]=a01; m_mat[0][2]=a02;
             m_mat[1][0]=a10; m_mat[1][1]=a11; m_mat[1][2]=a12;
             m_mat[2][0]=a20; m_mat[2][1]=a21; m_mat[2][2]=a22;
         }

        float* ptr() { return (float*)m_mat; }
        const float* ptr() const { return (const float*)m_mat; }

        float& operator [] (int i) {return ptr()[i];}
        float operator [] (int i) const {return ptr()[i];}

        void makeIdentity() { set( 1, 0, 0, 0, 1, 0, 0, 0, 1 ); }

    protected:
        float m_mat[3][3];
};

///////////////////////////////////////////////////////////////////////////

/** crUniform encapsulates glUniform values */
class CR_EXPORT crUniform : public crBase
{
    public:
        enum Type {
            FLOAT = GL_FLOAT,
            FLOAT_VEC2 = GL_FLOAT_VEC2,
            FLOAT_VEC3 = GL_FLOAT_VEC3,
            FLOAT_VEC4 = GL_FLOAT_VEC4,
            INT = GL_INT,
            INT_VEC2 = GL_INT_VEC2,
            INT_VEC3 = GL_INT_VEC3,
            INT_VEC4 = GL_INT_VEC4,
            BOOL = GL_BOOL,
            BOOL_VEC2 = GL_BOOL_VEC2,
            BOOL_VEC3 = GL_BOOL_VEC3,
            BOOL_VEC4 = GL_BOOL_VEC4,
            FLOAT_MAT2 = GL_FLOAT_MAT2,
            FLOAT_MAT3 = GL_FLOAT_MAT3,
            FLOAT_MAT4 = GL_FLOAT_MAT4,
            SAMPLER_1D = GL_SAMPLER_1D,
            SAMPLER_2D = GL_SAMPLER_2D,
            SAMPLER_3D = GL_SAMPLER_3D,
            SAMPLER_CUBE = GL_SAMPLER_CUBE,
            SAMPLER_1D_SHADOW = GL_SAMPLER_1D_SHADOW,
            SAMPLER_2D_SHADOW = GL_SAMPLER_2D_SHADOW,

			SAMPLER_1D_ARRAY  = GL_SAMPLER_1D_ARRAY_EXT,
			SAMPLER_2D_ARRAY  = GL_SAMPLER_2D_ARRAY_EXT,
			SAMPLER_1D_ARRAY_SHADOW = GL_SAMPLER_1D_ARRAY_SHADOW_EXT,
			SAMPLER_2D_ARRAY_SHADOW = GL_SAMPLER_2D_ARRAY_SHADOW_EXT,

			// TODO the following must be integrated fully here and Uniform.cpp
			FLOAT_MAT2x3 = GL_FLOAT_MAT2x3,
			FLOAT_MAT2x4 = GL_FLOAT_MAT2x4,
			FLOAT_MAT3x2 = GL_FLOAT_MAT3x2,
			FLOAT_MAT3x4 = GL_FLOAT_MAT3x4,
			FLOAT_MAT4x2 = GL_FLOAT_MAT4x2,
			FLOAT_MAT4x3 = GL_FLOAT_MAT4x3,
			SAMPLER_BUFFER = GL_SAMPLER_BUFFER_EXT,
			SAMPLER_CUBE_SHADOW = GL_SAMPLER_CUBE_SHADOW_EXT,
			UNSIGNED_INT_VEC2 = GL_UNSIGNED_INT_VEC2_EXT,
			UNSIGNED_INT_VEC3 = GL_UNSIGNED_INT_VEC3_EXT,
			UNSIGNED_INT_VEC4 = GL_UNSIGNED_INT_VEC4_EXT,
			INT_SAMPLER_1D       = GL_INT_SAMPLER_1D_EXT,
			INT_SAMPLER_2D       = GL_INT_SAMPLER_2D_EXT,
			INT_SAMPLER_3D       = GL_INT_SAMPLER_3D_EXT,
			INT_SAMPLER_CUBE     = GL_INT_SAMPLER_CUBE_EXT,
			INT_SAMPLER_2D_RECT  = GL_INT_SAMPLER_2D_RECT_EXT,
			INT_SAMPLER_1D_ARRAY = GL_INT_SAMPLER_1D_ARRAY_EXT,
			INT_SAMPLER_2D_ARRAY = GL_INT_SAMPLER_2D_ARRAY_EXT,
			INT_SAMPLER_BUFFER   = GL_INT_SAMPLER_BUFFER_EXT,
			UNSIGNED_INT_SAMPLER_1D       = GL_UNSIGNED_INT_SAMPLER_1D_EXT,
			UNSIGNED_INT_SAMPLER_2D       = GL_UNSIGNED_INT_SAMPLER_2D_EXT,
			UNSIGNED_INT_SAMPLER_3D       = GL_UNSIGNED_INT_SAMPLER_3D_EXT,
			UNSIGNED_INT_SAMPLER_CUBE     = GL_UNSIGNED_INT_SAMPLER_CUBE_EXT,
			UNSIGNED_INT_SAMPLER_2D_RECT  = GL_UNSIGNED_INT_SAMPLER_2D_RECT_EXT,
			UNSIGNED_INT_SAMPLER_1D_ARRAY = GL_UNSIGNED_INT_SAMPLER_1D_ARRAY_EXT,
			UNSIGNED_INT_SAMPLER_2D_ARRAY = GL_UNSIGNED_INT_SAMPLER_2D_ARRAY_EXT,
			UNSIGNED_INT_SAMPLER_BUFFER   = GL_UNSIGNED_INT_SAMPLER_BUFFER_EXT,

            UNDEFINED = 0x0
        };

    public:
        crUniform();
        crUniform( Type type, const std::string& name, int numElements=1 );

        /** Copy constructor using CopyOp to manage deep vs shallow copy. */
        crUniform(const crUniform& rhs, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

		virtual void setDataVariance(DataVariance dv);

        META_Base(CRCore, crUniform);


        /** Set the type of glUniform, ensuring it is only set once.*/
        bool setType( Type t );

        /** Get the type of glUniform as enum. */
        const Type getType() const { return m_type; }

        /** Set the name of the glUniform, ensuring it is only set once.*/
        void setName( const std::string& name );

        /** Set the length of a uniform, ensuring it is only set once (1==scalar)*/
        void setNumElements( unsigned int numElements );

        /** Get the number of GLSL elements of the CRCore::crUniform (1==scalar) */
        unsigned int getNumElements() const { return m_uniformData->m_numElements; }
		unsigned int getBufNumElements() const { return m_uniformData_buf->m_numElements; }

        /** Get the number of elements required for the internal data array.
          * Returns 0 if the CRCore::crUniform is not properly configured.  */
        unsigned int getInternalArrayNumElements() const;

        /** Return the name of a Type enum as string. */
        static const char* getTypename( Type t );

        /** Return the the number of components for a GLSL type. */
        static int getTypeNumComponents( Type t );

        /** Return the Type enum of a crUniform typename string */
        static crUniform::Type getTypeId( const std::string& tname );

        /** Return the GL API type corresponding to a GLSL type */
        static Type getGlApiType( Type t );

        /** Return the internal data array type corresponding to a GLSL type */
        static GLenum getInternalArrayType( Type t );

        /** convenient scalar (non-array) constructors w/ assignment */
        explicit crUniform( const char* name, float f );
        explicit crUniform( const char* name, int i );
        explicit crUniform( const char* name, bool b );
        crUniform( const char* name, const CRCore::crVector2& v2 );
        crUniform( const char* name, const CRCore::crVector3& v3 );
        crUniform( const char* name, const CRCore::crVector4& v4 );
        crUniform( const char* name, const CRCore::crMatrix2& m2 );
        crUniform( const char* name, const CRCore::crMatrix3& m3 );
        crUniform( const char* name, const CRCore::crMatrixf& m4 );
        crUniform( const char* name, const CRCore::crMatrixd& m4 );
        crUniform( const char* name, int i0, int i1 );
        crUniform( const char* name, int i0, int i1, int i2 );
        crUniform( const char* name, int i0, int i1, int i2, int i3 );
        crUniform( const char* name, bool b0, bool b1 );
        crUniform( const char* name, bool b0, bool b1, bool b2 );
        crUniform( const char* name, bool b0, bool b1, bool b2, bool b3 );

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crUniform& rhs) const;
        virtual int compareData(const crUniform& rhs) const;

        bool operator <  (const crUniform& rhs) const { return compare(rhs)<0; }
        bool operator == (const crUniform& rhs) const { return compare(rhs)==0; }
        bool operator != (const crUniform& rhs) const { return compare(rhs)!=0; }

        void copyData( const crUniform& rhs );


        /** A vector of CRCore::crStateSet pointers which is used to store the parent(s) of this crUniform, the parents could be CRCore::Node or CRCore::Drawable.*/
        typedef std::vector<crStateSet*> ParentList;

        /** Get the parent list of this crUniform. */
        inline const ParentList& getParents() const { return m_parents; }

        /** Get the a copy of parent list of node. A copy is returned to 
          * prevent modification of the parent list.*/
        inline ParentList getParents() { return m_parents; }

        inline crStateSet* getParent(unsigned int i)  { return m_parents[i]; }
        /**
         * Get a single const parent of this crUniform.
         * @param i index of the parent to get.
         * @return the parent i.
         */
        inline const crStateSet* getParent(unsigned int i) const  { return m_parents[i]; }

        /**
         * Get the number of parents of this crUniform.
         * @return the number of parents of this crUniform.
         */
        inline unsigned int getNumParents() const { return m_parents.size(); }


        /** convenient scalar (non-array) value assignment */
        bool set( float f );
        bool set( int i );
        bool set( bool b );
        bool set( const CRCore::crVector2& v2 );
        bool set( const CRCore::crVector3& v3 );
        bool set( const CRCore::crVector4& v4 );
        bool set( const CRCore::crMatrix2& m2 );
        bool set( const CRCore::crMatrix3& m3 );
        bool set( const CRCore::crMatrixf& m4 );
        bool set( const CRCore::crMatrixd& m4 );
        bool set( int i0, int i1 );
        bool set( int i0, int i1, int i2 );
        bool set( int i0, int i1, int i2, int i3 );
        bool set( bool b0, bool b1 );
        bool set( bool b0, bool b1, bool b2 );
        bool set( bool b0, bool b1, bool b2, bool b3 );

        /** convenient scalar (non-array) value query */
        bool get( float& f ) const;
        bool get( int& i ) const;
        bool get( bool& b ) const;
        bool get( CRCore::crVector2& v2 ) const;
        bool get( CRCore::crVector3& v3 ) const;
        bool get( CRCore::crVector4& v4 ) const;
        bool get( CRCore::crMatrix2& m2 ) const;
        bool get( CRCore::crMatrix3& m3 ) const;
        bool get( CRCore::crMatrixf& m4 ) const;
        bool get( CRCore::crMatrixd& m4 ) const;
        bool get( int& i0, int& i1 ) const;
        bool get( int& i0, int& i1, int& i2 ) const;
        bool get( int& i0, int& i1, int& i2, int& i3 ) const;
        bool get( bool& b0, bool& b1 ) const;
        bool get( bool& b0, bool& b1, bool& b2 ) const;
        bool get( bool& b0, bool& b1, bool& b2, bool& b3 ) const;

        /** value assignment for array uniforms */
        bool setElement( unsigned int index, float f );
        bool setElement( unsigned int index, int i );
        bool setElement( unsigned int index, bool b );
        bool setElement( unsigned int index, const CRCore::crVector2& v2 );
        bool setElement( unsigned int index, const CRCore::crVector3& v3 );
        bool setElement( unsigned int index, const CRCore::crVector4& v4 );
        bool setElement( unsigned int index, const CRCore::crMatrix2& m2 );
        bool setElement( unsigned int index, const CRCore::crMatrix3& m3 );
        bool setElement( unsigned int index, const CRCore::crMatrixf& m4 );
        bool setElement( unsigned int index, const CRCore::crMatrixd& m4 );
        bool setElement( unsigned int index, int i0, int i1 );
        bool setElement( unsigned int index, int i0, int i1, int i2 );
        bool setElement( unsigned int index, int i0, int i1, int i2, int i3 );
        bool setElement( unsigned int index, bool b0, bool b1 );
        bool setElement( unsigned int index, bool b0, bool b1, bool b2 );
        bool setElement( unsigned int index, bool b0, bool b1, bool b2, bool b3 );

        /** value query for array uniforms */
        bool getElement( unsigned int index, float& f ) const;
        bool getElement( unsigned int index, int& i ) const;
        bool getElement( unsigned int index, bool& b ) const;
        bool getElement( unsigned int index, CRCore::crVector2& v2 ) const;
        bool getElement( unsigned int index, CRCore::crVector3& v3 ) const;
        bool getElement( unsigned int index, CRCore::crVector4& v4 ) const;
        bool getElement( unsigned int index, CRCore::crMatrix2& m2 ) const;
        bool getElement( unsigned int index, CRCore::crMatrix3& m3 ) const;
        bool getElement( unsigned int index, CRCore::crMatrixf& m4 ) const;
        bool getElement( unsigned int index, CRCore::crMatrixd& m4 ) const;
        bool getElement( unsigned int index, int& i0, int& i1 ) const;
        bool getElement( unsigned int index, int& i0, int& i1, int& i2 ) const;
        bool getElement( unsigned int index, int& i0, int& i1, int& i2, int& i3 ) const;
        bool getElement( unsigned int index, bool& b0, bool& b1 ) const;
        bool getElement( unsigned int index, bool& b0, bool& b1, bool& b2 ) const;
        bool getElement( unsigned int index, bool& b0, bool& b1, bool& b2, bool& b3 ) const;


        struct Callback : public virtual CRCore::crBase
        {
            Callback() {}

            Callback(const Callback&,const crCopyOp&) {}

            META_Base(CRCore,Callback);

            /** do customized update code.*/
            virtual void operator () (crUniform*, crNodeVisitor*) {}
        };

        /** Set the UpdateCallback which allows users to attach customize the updating of an object during the update traversal.*/
        void setUpdateCallback(Callback* uc);

        /** Get the non const UpdateCallback.*/
        Callback* getUpdateCallback() { return m_updateCallback.get(); }

        /** Get the const UpdateCallback.*/
        const Callback* getUpdateCallback() const { return m_updateCallback.get(); }


        /** Set the EventCallback which allows users to attach customize the updating of an object during the Event traversal.*/
        //void setEventCallback(Callback* ec);

        ///** Get the non const EventCallback.*/
        //Callback* getEventCallback() { return m_eventCallback.get(); }

        ///** Get the const EventCallback.*/
        //const Callback* getEventCallback() const { return m_eventCallback.get(); }

        /** Increment the modified count on the crUniform so Programs watching it know it update themselves.
          * NOTE: autotomatically called during CRCore::crUniform::set*();
          * you must call if modifying the internal data array directly. */
        inline void dirty() { ++m_modifiedCount; }

        /** Set the internal data array for a CRCore::crUniform */
        bool setArray( FloatArray* array );
        bool setArray( IntArray* array );

        /** Get the internal data array for a float CRCore::crUniform. */
        FloatArray* getFloatArray() { return m_uniformData->m_floatArray.get(); }
        const FloatArray* getFloatArray() const { return m_uniformData->m_floatArray.get(); }

        /** Get the internal data array for an int CRCore::crUniform. */
        IntArray* getIntArray() { return m_uniformData->m_intArray.get(); }
        const IntArray* getIntArray() const { return m_uniformData->m_intArray.get(); }

        inline void setModifiedCount(unsigned int mc) { m_modifiedCount = mc; }
        inline unsigned int getModifiedCount() const { return m_modifiedCount; }


        void apply(const GL2Extensions* ext, GLint location) const;

		inline virtual void swapBuffers(int frameNumber);
    protected:
    
        virtual ~crUniform();
        crUniform& operator=(const crUniform&) { return *this; }

        bool isCompatibleType( Type t ) const;
        bool isScalar() const { return m_uniformData->m_numElements==1; }
        void allocateDataArray();

        void addParent(CRCore::crStateSet* stateset);
        void removeParent(CRCore::crStateSet* stateset);

        ParentList m_parents;
        friend class CRCore::crStateSet;

        Type                m_type;

        // The internal data for CRCore::Uniforms are stored as an array of
        // getInternalArrayType() of length getInternalArrayNumElements().

		struct UniformData : public Referenced
		{
			UniformData(int n = 0):m_numElements(n){}
			UniformData(const UniformData& rhs):
				m_numElements(rhs.m_numElements)
			{
				if( rhs.m_floatArray.valid() )
				{
					m_floatArray = new FloatArray(rhs.m_floatArray->getNumElements());
					*m_floatArray = *rhs.m_floatArray;
				}
				if( rhs.m_intArray.valid() )
				{
					m_intArray = new IntArray(rhs.m_intArray->getNumElements());
					*m_intArray = *rhs.m_intArray;
				}
			}
			unsigned int        m_numElements;
			ref_ptr<FloatArray> m_floatArray;
			ref_ptr<IntArray>   m_intArray;

			inline bool operator == (const UniformData& uniformData) const 
			{ 
			    if(m_numElements != uniformData.m_numElements) return false;
				if(!(m_floatArray.valid() * uniformData.m_floatArray.valid())) return false;
				if(!(m_intArray.valid() * uniformData.m_intArray.valid())) return false;
				int i;
				if(m_floatArray.valid())
				{
					if(m_floatArray->getNumElements()!=uniformData.m_floatArray->getNumElements())
						return false;
					for(i = 0; i<m_floatArray->getNumElements(); ++i)
						if((*m_floatArray)[i]!=(*uniformData.m_floatArray)[i]) return false;
				}
				if(m_intArray.valid())
				{
					if(m_intArray->getNumElements()!=uniformData.m_intArray->getNumElements())
						return false;
					for(i = 0; i<m_intArray->getNumElements(); ++i)
						if((*m_intArray)[i]!=(*uniformData.m_intArray)[i]) return false;
				}

				return true;
			}
		};

		ref_ptr<UniformData> m_uniformData;
		ref_ptr<UniformData> m_uniformData_buf;

        ref_ptr<Callback>   m_updateCallback;
        ref_ptr<Callback>   m_eventCallback;
        
        unsigned int        m_modifiedCount;
};

inline void crUniform::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		if(m_dataVariance==crBase::DYNAMIC)
		{
			//CRCore::notify(CRCore::ALWAYS) << "uniform swapBuffers name "<<m_name<< std::endl;
			ref_ptr<UniformData> temp = m_uniformData;
			m_uniformData = m_uniformData_buf;
			m_uniformData_buf = temp;
		}
		m_swapFrameNumber = frameNumber;
	}
}

}

#endif
