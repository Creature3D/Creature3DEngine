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
#ifndef CRCORE_CRMATRIXF_H
#define CRCORE_CRMATRIXF_H 1

#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crQuat.h>

#include <string.h>

#include <iostream>
#include <algorithm>

namespace CRCore {

class crMatrixd;

class CR_EXPORT crMatrixf : public crBase
{

    public:
    
        typedef float value_type;

        inline crMatrixf() { makeIdentity(); }
		inline crMatrixf( const crMatrixf& mat):crBase(mat){ set(mat.ptr()); }
        crMatrixf( const crMatrixd& mat );
        inline explicit crMatrixf( float const * const ptr ) { set(ptr); }
        inline explicit crMatrixf( double const * const ptr ) { set(ptr); }
        inline explicit crMatrixf( const crQuat& quat ) { set(quat); }

        crMatrixf( value_type a00, value_type a01, value_type a02, value_type a03,
                 value_type a10, value_type a11, value_type a12, value_type a13,
                 value_type a20, value_type a21, value_type a22, value_type a23,
                 value_type a30, value_type a31, value_type a32, value_type a33);

        ~crMatrixf() {}

        int compare(const crMatrixf& m) const { return memcmp(m_mat,m.m_mat,sizeof(m_mat)); }
		int compare(const crMatrixf& m) { return memcmp(m_mat,m.m_mat,sizeof(m_mat)); }

        bool operator < (const crMatrixf& m) const { return compare(m)<0; }
        bool operator == (const crMatrixf& m) const { return compare(m)==0; }
        bool operator != (const crMatrixf& m) const { return compare(m)!=0; }

		bool operator < (const crMatrixf& m) { return compare(m)<0; }
		bool operator == (const crMatrixf& m) { return compare(m)==0; }
		bool operator != (const crMatrixf& m) { return compare(m)!=0; }

        inline value_type& operator()(int row, int col) { return m_mat[row][col]; }
        inline value_type operator()(int row, int col) const { return m_mat[row][col]; }

        inline bool valid() const { return !isNaN(); }
        inline bool isNaN() const { return CRCore::isNaN(m_mat[0][0]) || CRCore::isNaN(m_mat[0][1]) || CRCore::isNaN(m_mat[0][2]) || CRCore::isNaN(m_mat[0][3]) ||
                                                 CRCore::isNaN(m_mat[1][0]) || CRCore::isNaN(m_mat[1][1]) || CRCore::isNaN(m_mat[1][2]) || CRCore::isNaN(m_mat[1][3]) ||
                                                 CRCore::isNaN(m_mat[2][0]) || CRCore::isNaN(m_mat[2][1]) || CRCore::isNaN(m_mat[2][2]) || CRCore::isNaN(m_mat[2][3]) ||
                                                 CRCore::isNaN(m_mat[3][0]) || CRCore::isNaN(m_mat[3][1]) || CRCore::isNaN(m_mat[3][2]) || CRCore::isNaN(m_mat[3][3]); }

        inline crMatrixf& operator = (const crMatrixf& rhs)
        {
            if( &rhs == this ) return *this;
            set(rhs.ptr());
            return *this;
        }

        crMatrixf& operator = (const crMatrixd& rhs);

        void set(const crMatrixd& rhs);

        inline void set(const crMatrixf& rhs) { set(rhs.ptr()); }

        inline void set(float const * const ptr)
        {
            value_type* local_ptr = (value_type*)m_mat;
            for(int i=0;i<16;++i) local_ptr[i]=(value_type)ptr[i];
        }
        
        inline void set(double const * const ptr)
        {
            value_type* local_ptr = (value_type*)m_mat;
            for(int i=0;i<16;++i) local_ptr[i]=(value_type)ptr[i];
        }

        void set( value_type a00, value_type a01, value_type a02, value_type a03,
                  value_type a10, value_type a11, value_type a12, value_type a13,
                  value_type a20, value_type a21, value_type a22, value_type a23,
                  value_type a30, value_type a31, value_type a32, value_type a33);
        
		inline void set(const crQuat& q) { makeRotate(q); }
		inline void get(crQuat& q) const { q = getRotate(); }

		void setRotate(const crQuat& q);
        crQuat getRotate() const;

        value_type * ptr() { return (value_type*)m_mat; }
        const value_type * ptr() const { return (const value_type *)m_mat; }

        void makeIdentity();
        
		void makeScale( const crVector3f& );
        void makeScale( const crVector3d& );
        void makeScale( value_type, value_type, value_type );
        
        void makeTranslate( const crVector3f& );
		void makeTranslate( const crVector3d& );
        void makeTranslate( value_type, value_type, value_type );
        
        void makeRotate( const crVector3f& from, const crVector3f& to );
		void makeRotate( const crVector3d& from, const crVector3d& to );
        void makeRotate( value_type angle, const crVector3f& axis );
		void makeRotate( value_type angle, const crVector3d& axis );
        void makeRotate( value_type angle, value_type x, value_type y, value_type z );
        void makeRotate( const crQuat& );
		void makeRotate( value_type angle1, const crVector3f& axis1, 
						 value_type angle2, const crVector3f& axis2,
						 value_type angle3, const crVector3f& axis3);
        void makeRotate( value_type angle1, const crVector3d& axis1, 
                         value_type angle2, const crVector3d& axis2,
                         value_type angle3, const crVector3d& axis3);

        
        
        /** Set to a orthographic projection. See glOrtho for further details.*/
        void makeOrtho(double left, double right,
                       double bottom, double top,
                       double zNear, double zFar);

        /** Get the othorgraphic settings of the orthographic projection matrix. 
          * Note, if matrix is not an orthographic matrix then invalid values will be returned.*/
        bool getOrtho(double& left, double& right,
                      double& bottom, double& top,
                      double& zNear, double& zFar) const;

        /** Set to a 2D orthographic projection. See glOrtho2D for further details.*/
        inline void makeOrtho2D(double left, double right,
                                double bottom, double top)
        {
            makeOrtho(left,right,bottom,top,-1.0,1.0);
        }


        /** Set to a perspective projection. See glFrustum for further details.*/
        void makeFrustum(double left, double right,
                         double bottom, double top,
                         double zNear, double zFar);

        /** Get the frustum setting of a perspective projection matrix.
          * Returns false if matrix is not an orthographic matrix, where parameter values are undefined.*/
        bool getFrustum(double& left, double& right,
                        double& bottom, double& top,
                        double& zNear, double& zFar) const;

        /** Set to a symmetrical perspective projection, See gluPerspective for further details.
          * Aspect ratio is defined as width/height.*/
        void makePerspective(double fovy,double aspectRatio,
                             double zNear, double zFar);

        /** Get the frustum setting of a symetric perspective projection matrix.
          * Returns false if matrix is not a perspective matrix, where parameter values are undefined. 
          * Note, if matrix is not a symetric perspective matrix then the shear will be lost.
          * Asymetric metrices occur when stereo, power walls, caves and reality center display are used.
          * In these configuration one should use the AsFrustum method instead.*/
        bool getPerspective(double& fovy,double& aspectRatio,
                            double& zNear, double& zFar) const;


        /** Set to the position and orientation modelview matrix, using the same convention as gluLookAt. */
        void makeLookAt(const crVector3d& eye,const crVector3d& center,const crVector3d& up);
        
        /** Get to the position and orientation of a modelview matrix, using the same convention as gluLookAt. */
        void getLookAt(crVector3f& eye,crVector3f& center,crVector3f& up,value_type lookDistance=1.0f) const;
		void getLookAt(crVector3d& eye,crVector3d& center,crVector3d& up,value_type lookDistance=1.0f) const;

		/** invert the matrix rhs, automatically select invert_4x3 or invert_4x4. */
		inline bool invert( const crMatrixf& rhs)
		{
			bool is_4x3 = (rhs.m_mat[0][3]==0.0f && rhs.m_mat[1][3]==0.0f &&  rhs.m_mat[2][3]==0.0f && rhs.m_mat[3][3]==1.0f);
			return is_4x3 ? invert_4x3(rhs) :  invert_4x4(rhs);
		}

		/** 4x3 matrix invert, not right hand column is assumed to be 0,0,0,1. */
		bool invert_4x3( const crMatrixf& rhs);

		/** full 4x4 matrix invert. */
		bool invert_4x4( const crMatrixf& rhs);

		/** ortho-normalize the 3x3 rotation & scale matrix */ 
		void orthoNormalize(const crMatrixf& rhs); 

        //basic utility functions to create new matrices
	    inline static crMatrixf identity( void );
        inline static crMatrixf scale( const crVector3d& sv);
        inline static crMatrixf scale( value_type sx, value_type sy, value_type sz);
        inline static crMatrixf translate( const crVector3d& dv);
        inline static crMatrixf translate( value_type x, value_type y, value_type z);
        inline static crMatrixf rotate( const crVector3f& from, const crVector3f& to);
		inline static crMatrixf rotate( const crVector3d& from, const crVector3d& to);
        inline static crMatrixf rotate( value_type angle, value_type x, value_type y, value_type z);
		inline static crMatrixf rotate( value_type angle, const crVector3f& axis);
        inline static crMatrixf rotate( value_type angle, const crVector3d& axis);
		inline static crMatrixf rotate( value_type angle1, const crVector3f& axis1, 
										value_type angle2, const crVector3f& axis2,
										value_type angle3, const crVector3f& axis3);
        inline static crMatrixf rotate( value_type angle1, const crVector3d& axis1, 
                                     value_type angle2, const crVector3d& axis2,
                                     value_type angle3, const crVector3d& axis3);
        inline static crMatrixf rotate( const crQuat& quat);
        inline static crMatrixf inverse( const crMatrixf& matrix);
        
        /** Create a orthographic projection. See glOrtho for further details.*/
        inline static crMatrixf ortho(double left, double right,
                                   double bottom, double top,
                                   double zNear, double zFar);

        /** Create a 2D orthographic projection. See glOrtho for further details.*/
        inline static crMatrixf ortho2D(double left, double right,
                                     double bottom, double top);

        /** Create a perspective projection. See glFrustum for further details.*/
        inline static crMatrixf frustum(double left, double right,
                                     double bottom, double top,
                                     double zNear, double zFar);

        /** Create a symmetrical perspective projection, See gluPerspective for further details.
          * Aspect ratio is defined as width/height.*/
        inline static crMatrixf perspective(double fovy,double aspectRatio,
                                         double zNear, double zFar);

        /** Create the position and orientation as per a camera, using the same convention as gluLookAt. */
        inline static crMatrixf lookAt(const crVector3d& eye,const crVector3d& center,const crVector3d& up);

		inline static crMatrixf pegToFarPlane(const crMatrixf &matrix)
		{
			crMatrixf mat;

			memcpy(mat.m_mat[0], matrix.m_mat[0], 8 * sizeof(float));
			memcpy(mat.m_mat[2], matrix.m_mat[3], 4 * sizeof(float));
			memcpy(mat.m_mat[3], matrix.m_mat[3], 4 * sizeof(float));

			return mat;
		}


        inline crVector3d preMult( const crVector3d& v ) const;
        inline crVector3d postMult( const crVector3d& v ) const;
        inline crVector3d operator* ( const crVector3d& v ) const;
        inline crVector4d preMult( const crVector4d& v ) const;
        inline crVector4d postMult( const crVector4d& v ) const;
        inline crVector4d operator* ( const crVector4d& v ) const;

        void setTrans( value_type tx, value_type ty, value_type tz );
	    void setTrans( const crVector3f& v );
		void setTrans( const crVector3d& v );
        inline crVector3f getTrans() const { return crVector3f(m_mat[3][0],m_mat[3][1],m_mat[3][2]); } 
        
        inline crVector3f getScale() const { return crVector3f(m_mat[0][0],m_mat[1][1],m_mat[2][2]); }
        
    	/** apply apply an 3x3 transform of v*M[0..2,0..2]  */
    	inline static crVector3f transform3x3(const crVector3d& v,const crMatrixf& m);
    	/** apply apply an 3x3 transform of M[0..2,0..2]*v  */
    	inline static crVector3f transform3x3(const crMatrixf& m,const crVector3d& v);


        // basic crMatrixf multiplication, our workhorse methods.
        void mult( const crMatrixf&, const crMatrixf& );
        void preMult( const crMatrixf& );
        void postMult( const crMatrixf& );

		/** Optimized version of preMult(translate(v)); */
		inline void preMultTranslate( const crVector3d& v );
		inline void preMultTranslate( const crVector3f& v );
		/** Optimized version of postMult(translate(v)); */
		inline void postMultTranslate( const crVector3d& v );
		inline void postMultTranslate( const crVector3f& v );

		/** Optimized version of preMult(scale(v)); */
		inline void preMultScale( const crVector3d& v );
		inline void preMultScale( const crVector3f& v );
		/** Optimized version of postMult(scale(v)); */
		inline void postMultScale( const crVector3d& v );
		inline void postMultScale( const crVector3f& v );

		/** Optimized version of preMult(rotate(q)); */
		inline void preMultRotate( const crQuat& q );
		/** Optimized version of postMult(rotate(q)); */
		inline void postMultRotate( const crQuat& q );

        inline void operator *= ( const crMatrixf& other ) 
        {    
			if( this == &other ) 
			{
                crMatrixf temp(other);
                postMult( temp );
            }
            else postMult( other ); 
        }

        inline crMatrixf operator * ( const crMatrixf &m ) const
		{
	        CRCore::crMatrixf r;
            r.mult(*this,m);
	       return  r;
		}

		virtual crBase* cloneType() const { return new crMatrixf(); } 
		virtual crBase* clone(const crCopyOp&) const { return new crMatrixf(*this); }
		virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crMatrixf*>(obj)!=NULL; }
		virtual const char* libraryName() const { return "CRCore"; }
		virtual const char* className() const { return "crMatrixf"; }

		operator const value_type *() const { return (const value_type *) m_mat; }

    protected:
        value_type m_mat[4][4];

};

class RefMatrixf : public crMatrixf
{
    public:
    
        RefMatrixf():crMatrixf() {}
        RefMatrixf( const crMatrixf& other) : crMatrixf(other) {}
        RefMatrixf( const crMatrixd& other) : crMatrixf(other) {}
        RefMatrixf( const RefMatrixf& other) : /*crBase(other),*/ crMatrixf(other) {}
        explicit RefMatrixf( crMatrixf::value_type const * const def ):crMatrixf(def) {}
        RefMatrixf( crMatrixf::value_type a00, crMatrixf::value_type a01, crMatrixf::value_type a02, crMatrixf::value_type a03,
            crMatrixf::value_type a10, crMatrixf::value_type a11, crMatrixf::value_type a12, crMatrixf::value_type a13,
            crMatrixf::value_type a20, crMatrixf::value_type a21, crMatrixf::value_type a22, crMatrixf::value_type a23,
            crMatrixf::value_type a30, crMatrixf::value_type a31, crMatrixf::value_type a32, crMatrixf::value_type a33):
            crMatrixf(a00, a01, a02, a03,
                   a10, a11, a12, a13,
                   a20, a21, a22, a23,
                   a30, a31, a32, a33) {}

        virtual crBase* cloneType() const { return new RefMatrixf(); } 
        virtual crBase* clone(const crCopyOp&) const { return new RefMatrixf(*this); }
        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const RefMatrixf*>(obj)!=NULL; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "Matrix"; }
        
        
    protected:
    
        virtual ~RefMatrixf() {}
};


//static utility methods
inline crMatrixf crMatrixf::identity(void)
{
    crMatrixf m;
    m.makeIdentity();
    return m;
}

inline crMatrixf crMatrixf::scale(value_type sx, value_type sy, value_type sz)
{
    crMatrixf m;
    m.makeScale(sx,sy,sz);
    return m;
}

inline crMatrixf crMatrixf::scale(const crVector3d& v )
{
    return scale(v.x(), v.y(), v.z() );
}

inline crMatrixf crMatrixf::translate(value_type tx, value_type ty, value_type tz)
{
    crMatrixf m;
    m.makeTranslate(tx,ty,tz);
    return m;
}

inline crMatrixf crMatrixf::translate(const crVector3d& v )
{
    return translate(v.x(), v.y(), v.z() );
}

inline crMatrixf crMatrixf::rotate( const crQuat& q )
{
    return crMatrixf(q);
}
inline crMatrixf crMatrixf::rotate(value_type angle, value_type x, value_type y, value_type z )
{
    crMatrixf m;
    m.makeRotate(angle,x,y,z);
    return m;
}
inline crMatrixf crMatrixf::rotate(value_type angle, const crVector3f& axis )
{
	crMatrixf m;
	m.makeRotate(angle,axis);
	return m;
}
inline crMatrixf crMatrixf::rotate(value_type angle, const crVector3d& axis )
{
    crMatrixf m;
    m.makeRotate(angle,axis);
    return m;
}
inline crMatrixf crMatrixf::rotate( value_type angle1, const crVector3f& axis1, 
								   value_type angle2, const crVector3f& axis2,
								   value_type angle3, const crVector3f& axis3)
{
	crMatrixf m;
	m.makeRotate(angle1,axis1,angle2,axis2,angle3,axis3);
	return m;
}
inline crMatrixf crMatrixf::rotate( value_type angle1, const crVector3d& axis1, 
                              value_type angle2, const crVector3d& axis2,
                              value_type angle3, const crVector3d& axis3)
{
    crMatrixf m;
    m.makeRotate(angle1,axis1,angle2,axis2,angle3,axis3);
    return m;
}
inline crMatrixf crMatrixf::rotate(const crVector3f& from, const crVector3f& to )
{
	crMatrixf m;
	m.makeRotate(from,to);
	return m;
}
inline crMatrixf crMatrixf::rotate(const crVector3d& from, const crVector3d& to )
{
    crMatrixf m;
    m.makeRotate(from,to);
    return m;
}

inline crMatrixf crMatrixf::inverse( const crMatrixf& matrix)
{
    crMatrixf m;
    m.invert(matrix);
    return m;
}

inline crMatrixf crMatrixf::ortho(double left, double right,
                            double bottom, double top,
                            double zNear, double zFar)
{
    crMatrixf m;
    m.makeOrtho(left,right,bottom,top,zNear,zFar);
    return m;
}

inline crMatrixf crMatrixf::ortho2D(double left, double right,
                              double bottom, double top)
{
    crMatrixf m;
    m.makeOrtho2D(left,right,bottom,top);
    return m;
}

inline crMatrixf crMatrixf::frustum(double left, double right,
                              double bottom, double top,
                              double zNear, double zFar)
{
    crMatrixf m;
    m.makeFrustum(left,right,bottom,top,zNear,zFar);
    return m;
}

inline crMatrixf crMatrixf::perspective(double fovy,double aspectRatio,
                                  double zNear, double zFar)
{
    crMatrixf m;
    m.makePerspective(fovy,aspectRatio,zNear,zFar);
    return m;
}

inline crMatrixf crMatrixf::lookAt(const crVector3d& eye,const crVector3d& center,const crVector3d& up)
{
    crMatrixf m;
    m.makeLookAt(eye,center,up);
    return m;
}

inline crVector3d crMatrixf::postMult( const crVector3d& v ) const
{
    value_type d = 1.0f/(m_mat[3][0]*v.x()+m_mat[3][1]*v.y()+m_mat[3][2]*v.z()+m_mat[3][3]) ;
    return crVector3d( (m_mat[0][0]*v.x() + m_mat[0][1]*v.y() + m_mat[0][2]*v.z() + m_mat[0][3])*d,
        (m_mat[1][0]*v.x() + m_mat[1][1]*v.y() + m_mat[1][2]*v.z() + m_mat[1][3])*d,
        (m_mat[2][0]*v.x() + m_mat[2][1]*v.y() + m_mat[2][2]*v.z() + m_mat[2][3])*d) ;
}

inline crVector3d crMatrixf::preMult( const crVector3d& v ) const
{
    value_type d = 1.0f/(m_mat[0][3]*v.x()+m_mat[1][3]*v.y()+m_mat[2][3]*v.z()+m_mat[3][3]) ;
    return crVector3d( (m_mat[0][0]*v.x() + m_mat[1][0]*v.y() + m_mat[2][0]*v.z() + m_mat[3][0])*d,
        (m_mat[0][1]*v.x() + m_mat[1][1]*v.y() + m_mat[2][1]*v.z() + m_mat[3][1])*d,
        (m_mat[0][2]*v.x() + m_mat[1][2]*v.y() + m_mat[2][2]*v.z() + m_mat[3][2])*d);
}

inline crVector4d crMatrixf::postMult( const crVector4d& v ) const
{
    return crVector4d( (m_mat[0][0]*v.x() + m_mat[0][1]*v.y() + m_mat[0][2]*v.z() + m_mat[0][3]*v.w()),
        (m_mat[1][0]*v.x() + m_mat[1][1]*v.y() + m_mat[1][2]*v.z() + m_mat[1][3]*v.w()),
        (m_mat[2][0]*v.x() + m_mat[2][1]*v.y() + m_mat[2][2]*v.z() + m_mat[2][3]*v.w()),
        (m_mat[3][0]*v.x() + m_mat[3][1]*v.y() + m_mat[3][2]*v.z() + m_mat[3][3]*v.w())) ;
}

inline crVector4d crMatrixf::preMult( const crVector4d& v ) const
{
    return crVector4d( (m_mat[0][0]*v.x() + m_mat[1][0]*v.y() + m_mat[2][0]*v.z() + m_mat[3][0]*v.w()),
        (m_mat[0][1]*v.x() + m_mat[1][1]*v.y() + m_mat[2][1]*v.z() + m_mat[3][1]*v.w()),
        (m_mat[0][2]*v.x() + m_mat[1][2]*v.y() + m_mat[2][2]*v.z() + m_mat[3][2]*v.w()),
        (m_mat[0][3]*v.x() + m_mat[1][3]*v.y() + m_mat[2][3]*v.z() + m_mat[3][3]*v.w()));
}
inline crVector3f crMatrixf::transform3x3(const crVector3d& v,const crMatrixf& m)
{
    return crVector3f( (m.m_mat[0][0]*v.x() + m.m_mat[1][0]*v.y() + m.m_mat[2][0]*v.z()),
                 (m.m_mat[0][1]*v.x() + m.m_mat[1][1]*v.y() + m.m_mat[2][1]*v.z()),
                 (m.m_mat[0][2]*v.x() + m.m_mat[1][2]*v.y() + m.m_mat[2][2]*v.z()));
}

inline crVector3f crMatrixf::transform3x3(const crMatrixf& m,const crVector3d& v)
{
    return crVector3f( (m.m_mat[0][0]*v.x() + m.m_mat[0][1]*v.y() + m.m_mat[0][2]*v.z()),
                 (m.m_mat[1][0]*v.x() + m.m_mat[1][1]*v.y() + m.m_mat[1][2]*v.z()),
                 (m.m_mat[2][0]*v.x() + m.m_mat[2][1]*v.y() + m.m_mat[2][2]*v.z()) ) ;
}

inline void crMatrixf::preMultTranslate( const crVector3d& v )
{
	for (unsigned i = 0; i < 3; ++i)
	{
		double tmp = v[i];
		if (tmp == 0)
			continue;
		m_mat[3][0] += tmp*m_mat[i][0];
		m_mat[3][1] += tmp*m_mat[i][1];
		m_mat[3][2] += tmp*m_mat[i][2];
		m_mat[3][3] += tmp*m_mat[i][3];
	}
}

inline void crMatrixf::preMultTranslate( const crVector3f& v )
{
	for (unsigned i = 0; i < 3; ++i)
	{
		float tmp = v[i];
		if (tmp == 0)
			continue;
		m_mat[3][0] += tmp*m_mat[i][0];
		m_mat[3][1] += tmp*m_mat[i][1];
		m_mat[3][2] += tmp*m_mat[i][2];
		m_mat[3][3] += tmp*m_mat[i][3];
	}
}

inline void crMatrixf::postMultTranslate( const crVector3d& v )
{
	for (unsigned i = 0; i < 3; ++i)
	{
		double tmp = v[i];
		if (tmp == 0)
			continue;
		m_mat[0][i] += tmp*m_mat[0][3];
		m_mat[1][i] += tmp*m_mat[1][3];
		m_mat[2][i] += tmp*m_mat[2][3];
		m_mat[3][i] += tmp*m_mat[3][3];
	}
}

inline void crMatrixf::postMultTranslate( const crVector3f& v )
{
	for (unsigned i = 0; i < 3; ++i)
	{
		float tmp = v[i];
		if (tmp == 0)
			continue;
		m_mat[0][i] += tmp*m_mat[0][3];
		m_mat[1][i] += tmp*m_mat[1][3];
		m_mat[2][i] += tmp*m_mat[2][3];
		m_mat[3][i] += tmp*m_mat[3][3];
	}
}

inline void crMatrixf::preMultScale( const crVector3d& v )
{
	m_mat[0][0] *= v[0]; m_mat[0][1] *= v[0]; m_mat[0][2] *= v[0]; m_mat[0][3] *= v[0];
	m_mat[1][0] *= v[1]; m_mat[1][1] *= v[1]; m_mat[1][2] *= v[1]; m_mat[1][3] *= v[1];
	m_mat[2][0] *= v[2]; m_mat[2][1] *= v[2]; m_mat[2][2] *= v[2]; m_mat[2][3] *= v[2];
}

inline void crMatrixf::preMultScale( const crVector3f& v )
{
	m_mat[0][0] *= v[0]; m_mat[0][1] *= v[0]; m_mat[0][2] *= v[0]; m_mat[0][3] *= v[0];
	m_mat[1][0] *= v[1]; m_mat[1][1] *= v[1]; m_mat[1][2] *= v[1]; m_mat[1][3] *= v[1];
	m_mat[2][0] *= v[2]; m_mat[2][1] *= v[2]; m_mat[2][2] *= v[2]; m_mat[2][3] *= v[2];
}

inline void crMatrixf::postMultScale( const crVector3d& v )
{
	m_mat[0][0] *= v[0]; m_mat[1][0] *= v[0]; m_mat[2][0] *= v[0]; m_mat[3][0] *= v[0];
	m_mat[0][1] *= v[1]; m_mat[1][1] *= v[1]; m_mat[2][1] *= v[1]; m_mat[3][1] *= v[1];
	m_mat[0][2] *= v[2]; m_mat[1][2] *= v[2]; m_mat[2][2] *= v[2]; m_mat[3][2] *= v[2];
}

inline void crMatrixf::postMultScale( const crVector3f& v )
{
	m_mat[0][0] *= v[0]; m_mat[1][0] *= v[0]; m_mat[2][0] *= v[0]; m_mat[3][0] *= v[0];
	m_mat[0][1] *= v[1]; m_mat[1][1] *= v[1]; m_mat[2][1] *= v[1]; m_mat[3][1] *= v[1];
	m_mat[0][2] *= v[2]; m_mat[1][2] *= v[2]; m_mat[2][2] *= v[2]; m_mat[3][2] *= v[2];
}


inline void crMatrixf::preMultRotate( const crQuat& q )
{
	if (q.zeroRotation())
		return;
	crMatrixf r;
	r.setRotate(q);
	preMult(r);
}

inline void crMatrixf::postMultRotate( const crQuat& q )
{
	if (q.zeroRotation())
		return;
	crMatrixf r;
	r.setRotate(q);
	postMult(r);
}

inline crVector3d operator* (const crVector3d& v, const crMatrixf& m )
{
	return m.preMult(v);
}
inline crVector4d operator* (const crVector4d& v, const crMatrixf& m )
{
	return m.preMult(v);
}

inline crVector3d crMatrixf::operator* (const crVector3d& v) const
{
	return postMult(v);
}
inline crVector4d crMatrixf::operator* (const crVector4d& v) const
{
	return postMult(v);
}

inline std::ostream& operator<< (std::ostream& os, const crMatrixf& m )
{
    os << "{"<<std::endl;
    for(int row=0; row<4; ++row) {
        os << "\t";
        for(int col=0; col<4; ++col)
            os << m(row,col) << " ";
        os << std::endl;
    }
    os << "}" << std::endl;
    return os;
}


} //namespace CRCore


#endif
