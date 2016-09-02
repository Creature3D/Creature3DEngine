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
#ifndef CRCORE_CRMATRIXD_H
#define CRCORE_CRMATRIXD_H


#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crQuat.h>

#include <string.h>

#include <iostream>
#include <algorithm>

namespace CRCore {


class crMatrixf;

class CR_EXPORT crMatrixd : public  crBase
{

    public:
    
        typedef double value_type;

        inline crMatrixd() { makeIdentity(); }
        inline crMatrixd( const crMatrixd& mat):crBase(mat) { set(mat.ptr()); }
        crMatrixd( const crMatrixf& mat );
        inline explicit crMatrixd( float const * const ptr ) { set(ptr); }
        inline explicit crMatrixd( double const * const ptr ) { set(ptr); }
        inline explicit crMatrixd( const crQuat& quat ) { set(quat); }

        crMatrixd( value_type a00, value_type a01, value_type a02, value_type a03,
                 value_type a10, value_type a11, value_type a12, value_type a13,
                 value_type a20, value_type a21, value_type a22, value_type a23,
                 value_type a30, value_type a31, value_type a32, value_type a33);

        ~crMatrixd() {}

        int compare(const crMatrixd& m) const { return memcmp(m_mat,m.m_mat,sizeof(m_mat)); }
		int compare(const crMatrixd& m) { return memcmp(m_mat,m.m_mat,sizeof(m_mat)); }

        bool operator < (const crMatrixd& m) const { return compare(m)<0; }
        bool operator == (const crMatrixd& m) const { return compare(m)==0; }
        bool operator != (const crMatrixd& m) const { return compare(m)!=0; }
		
		bool operator < (const crMatrixd& m) { return compare(m)<0; }
		bool operator == (const crMatrixd& m) { return compare(m)==0; }
		bool operator != (const crMatrixd& m) { return compare(m)!=0; }

        inline value_type& operator()(int row, int col) { return m_mat[row][col]; }
        inline value_type operator()(int row, int col) const { return m_mat[row][col]; }

        inline bool valid() const { return !isNaN(); }
        inline bool isNaN() const { return CRCore::isNaN(m_mat[0][0]) || CRCore::isNaN(m_mat[0][1]) || CRCore::isNaN(m_mat[0][2]) || CRCore::isNaN(m_mat[0][3]) ||
                                                 CRCore::isNaN(m_mat[1][0]) || CRCore::isNaN(m_mat[1][1]) || CRCore::isNaN(m_mat[1][2]) || CRCore::isNaN(m_mat[1][3]) ||
                                                 CRCore::isNaN(m_mat[2][0]) || CRCore::isNaN(m_mat[2][1]) || CRCore::isNaN(m_mat[2][2]) || CRCore::isNaN(m_mat[2][3]) ||
                                                 CRCore::isNaN(m_mat[3][0]) || CRCore::isNaN(m_mat[3][1]) || CRCore::isNaN(m_mat[3][2]) || CRCore::isNaN(m_mat[3][3]); }

        inline crMatrixd& operator = (const crMatrixd& rhs)
        {
            if( &rhs == this ) return *this;
            set(rhs.ptr());
            return *this;
        }
        
        inline crMatrixd& operator = (const crMatrixf& other);

        inline void set(const crMatrixd& rhs) { set(rhs.ptr()); }

        void set(const crMatrixf& rhs);

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
          * Note, if matrix is not an perspective matrix then invalid values will be returned.*/
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
		inline bool invert( const crMatrixd& rhs)
		{
			bool is_4x3 = (rhs.m_mat[0][3]==0.0f && rhs.m_mat[1][3]==0.0f &&  rhs.m_mat[2][3]==0.0f && rhs.m_mat[3][3]==1.0f);
			return is_4x3 ? invert_4x3(rhs) :  invert_4x4(rhs);
		}

		/** 4x3 matrix invert, not right hand column is assumed to be 0,0,0,1. */
		bool invert_4x3( const crMatrixd& rhs);

		/** full 4x4 matrix invert. */
		bool invert_4x4( const crMatrixd& rhs);

		/** ortho-normalize the 3x3 rotation & scale matrix */ 
		void orthoNormalize(const crMatrixd& rhs); 

        //basic utility functions to create new matrices
	    inline static crMatrixd identity( void );
        inline static crMatrixd scale( const crVector3d& sv);
        inline static crMatrixd scale( value_type sx, value_type sy, value_type sz);
        inline static crMatrixd translate( const crVector3d& dv);
        inline static crMatrixd translate( value_type x, value_type y, value_type z);
		inline static crMatrixd rotate( const crVector3f& from, const crVector3f& to);
        inline static crMatrixd rotate( const crVector3d& from, const crVector3d& to);
        inline static crMatrixd rotate( value_type angle, value_type x, value_type y, value_type z);
		inline static crMatrixd rotate( value_type angle, const crVector3f& axis);
        inline static crMatrixd rotate( value_type angle, const crVector3d& axis);
		inline static crMatrixd rotate( value_type angle1, const crVector3f& axis1, 
										value_type angle2, const crVector3f& axis2,
										value_type angle3, const crVector3f& axis3);
        inline static crMatrixd rotate( value_type angle1, const crVector3d& axis1, 
                                     value_type angle2, const crVector3d& axis2,
                                     value_type angle3, const crVector3d& axis3);
        inline static crMatrixd rotate( const crQuat& quat);
        inline static crMatrixd inverse( const crMatrixd& matrix);
        
        /** Create a orthographic projection. See glOrtho for further details.*/
        inline static crMatrixd ortho(double left, double right,
                                   double bottom, double top,
                                   double zNear, double zFar);

        /** Create a 2D orthographic projection. See glOrtho for further details.*/
        inline static crMatrixd ortho2D(double left, double right,
                                     double bottom, double top);

        /** Create a perspective projection. See glFrustum for further details.*/
        inline static crMatrixd frustum(double left, double right,
                                     double bottom, double top,
                                     double zNear, double zFar);

        /** Create a symmetrical perspective projection, See gluPerspective for further details.
          * Aspect ratio is defined as width/height.*/
        inline static crMatrixd perspective(double fovy,double aspectRatio,
                                         double zNear, double zFar);

        /** Create the position and orientation as per a camera, using the same convention as gluLookAt. */
        inline static crMatrixd lookAt(const crVector3d& eye,const crVector3d& center,const crVector3d& up);

		inline static crMatrixd pegToFarPlane(const crMatrixd &matrix)
		{
			crMatrixd mat;

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
        inline crVector3d getTrans() const { return crVector3d(m_mat[3][0],m_mat[3][1],m_mat[3][2]); } 
        
        inline crVector3d getScale() const { return crVector3d(m_mat[0][0],m_mat[1][1],m_mat[2][2]); }
        
    	/** apply apply an 3x3 transform of v*M[0..2,0..2]  */
    	inline static crVector3d transform3x3(const crVector3d& v,const crMatrixd& m);
    	/** apply apply an 3x3 transform of M[0..2,0..2]*v  */
    	inline static crVector3d transform3x3(const crMatrixd& m,const crVector3d& v);


        // basic crMatrixd multiplication, our workhorse methods.
        void mult( const crMatrixd&, const crMatrixd& );
        void preMult( const crMatrixd& );
        void postMult( const crMatrixd& );

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

        inline void operator *= ( const crMatrixd& other ) 
        {    if( this == &other ) {
                crMatrixd temp(other);
                postMult( temp );
            }
            else postMult( other ); 
        }

        inline crMatrixd operator * ( const crMatrixd &m ) const
	{
	    CRCore::crMatrixd r;
            r.mult(*this,m);
	    return  r;
	}

	virtual crBase* cloneType() const { return new crMatrixd(); } 
	virtual crBase* clone(const crCopyOp&) const { return new crMatrixd(*this); }
	virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crMatrixd*>(obj)!=NULL; }
	virtual const char* libraryName() const { return "CRCore"; }
	virtual const char* className() const { return "crMatrixd"; }

	operator const value_type *() const { return (const value_type *) m_mat; }

    protected:
        value_type m_mat[4][4];

};

class RefMatrixd : public crMatrixd
{
    public:
    
        RefMatrixd():crMatrixd() {}
        RefMatrixd( const crMatrixd& other) : crMatrixd(other) {}
        RefMatrixd( const crMatrixf& other) : crMatrixd(other) {}
        RefMatrixd( const RefMatrixd& other) : /*crBase(other),*/ crMatrixd(other) {}
        explicit RefMatrixd( crMatrixd::value_type const * const def ):crMatrixd(def) {}
        RefMatrixd( crMatrixd::value_type a00, crMatrixd::value_type a01, crMatrixd::value_type a02, crMatrixd::value_type a03,
            crMatrixd::value_type a10, crMatrixd::value_type a11, crMatrixd::value_type a12, crMatrixd::value_type a13,
            crMatrixd::value_type a20, crMatrixd::value_type a21, crMatrixd::value_type a22, crMatrixd::value_type a23,
            crMatrixd::value_type a30, crMatrixd::value_type a31, crMatrixd::value_type a32, crMatrixd::value_type a33):
            crMatrixd(a00, a01, a02, a03,
                   a10, a11, a12, a13,
                   a20, a21, a22, a23,
                   a30, a31, a32, a33) {}

        virtual crBase* cloneType() const { return new RefMatrixd(); } 
        virtual crBase* clone(const crCopyOp&) const { return new RefMatrixd(*this); }
        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const RefMatrixd*>(obj)!=NULL; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "Matrix"; }
        
        
    protected:
    
        virtual ~RefMatrixd() {}
};


//static utility methods
inline crMatrixd crMatrixd::identity(void)
{
    crMatrixd m;
    m.makeIdentity();
    return m;
}

inline crMatrixd crMatrixd::scale(value_type sx, value_type sy, value_type sz)
{
    crMatrixd m;
    m.makeScale(sx,sy,sz);
    return m;
}

inline crMatrixd crMatrixd::scale(const crVector3d& v )
{
    return scale(v.x(), v.y(), v.z() );
}

inline crMatrixd crMatrixd::translate(value_type tx, value_type ty, value_type tz)
{
    crMatrixd m;
    m.makeTranslate(tx,ty,tz);
    return m;
}

inline crMatrixd crMatrixd::translate(const crVector3d& v )
{
    return translate(v.x(), v.y(), v.z() );
}

inline crMatrixd crMatrixd::rotate( const crQuat& q )
{
    return crMatrixd(q);
}
inline crMatrixd crMatrixd::rotate(value_type angle, value_type x, value_type y, value_type z )
{
    crMatrixd m;
    m.makeRotate(angle,x,y,z);
    return m;
}
inline crMatrixd crMatrixd::rotate(value_type angle, const crVector3f& axis )
{
	crMatrixd m;
	m.makeRotate(angle,axis);
	return m;
}
inline crMatrixd crMatrixd::rotate(value_type angle, const crVector3d& axis )
{
    crMatrixd m;
    m.makeRotate(angle,axis);
    return m;
}
inline crMatrixd crMatrixd::rotate( value_type angle1, const crVector3f& axis1, 
								   value_type angle2, const crVector3f& axis2,
								   value_type angle3, const crVector3f& axis3)
{
	crMatrixd m;
	m.makeRotate(angle1,axis1,angle2,axis2,angle3,axis3);
	return m;
}
inline crMatrixd crMatrixd::rotate( value_type angle1, const crVector3d& axis1, 
                              value_type angle2, const crVector3d& axis2,
                              value_type angle3, const crVector3d& axis3)
{
    crMatrixd m;
    m.makeRotate(angle1,axis1,angle2,axis2,angle3,axis3);
    return m;
}
inline crMatrixd crMatrixd::rotate(const crVector3f& from, const crVector3f& to )
{
	crMatrixd m;
	m.makeRotate(from,to);
	return m;
}
inline crMatrixd crMatrixd::rotate(const crVector3d& from, const crVector3d& to )
{
    crMatrixd m;
    m.makeRotate(from,to);
    return m;
}

inline crMatrixd crMatrixd::inverse( const crMatrixd& matrix)
{
    crMatrixd m;
    m.invert(matrix);
    return m;
}

inline crMatrixd crMatrixd::ortho(double left, double right,
                            double bottom, double top,
                            double zNear, double zFar)
{
    crMatrixd m;
    m.makeOrtho(left,right,bottom,top,zNear,zFar);
    return m;
}

inline crMatrixd crMatrixd::ortho2D(double left, double right,
                              double bottom, double top)
{
    crMatrixd m;
    m.makeOrtho2D(left,right,bottom,top);
    return m;
}

inline crMatrixd crMatrixd::frustum(double left, double right,
                              double bottom, double top,
                              double zNear, double zFar)
{
    crMatrixd m;
    m.makeFrustum(left,right,bottom,top,zNear,zFar);
    return m;
}

inline crMatrixd crMatrixd::perspective(double fovy,double aspectRatio,
                                  double zNear, double zFar)
{
    crMatrixd m;
    m.makePerspective(fovy,aspectRatio,zNear,zFar);
    return m;
}

inline crMatrixd crMatrixd::lookAt(const crVector3d& eye,const crVector3d& center,const crVector3d& up)
{
    crMatrixd m;
    m.makeLookAt(eye,center,up);
    return m;
}

inline crVector3d crMatrixd::postMult( const crVector3d& v ) const
{
    value_type d = 1.0f/(m_mat[3][0]*v.x()+m_mat[3][1]*v.y()+m_mat[3][2]*v.z()+m_mat[3][3]) ;
    return crVector3d( (m_mat[0][0]*v.x() + m_mat[0][1]*v.y() + m_mat[0][2]*v.z() + m_mat[0][3])*d,
        (m_mat[1][0]*v.x() + m_mat[1][1]*v.y() + m_mat[1][2]*v.z() + m_mat[1][3])*d,
        (m_mat[2][0]*v.x() + m_mat[2][1]*v.y() + m_mat[2][2]*v.z() + m_mat[2][3])*d) ;
}

inline crVector3d crMatrixd::preMult( const crVector3d& v ) const
{
    value_type d = 1.0f/(m_mat[0][3]*v.x()+m_mat[1][3]*v.y()+m_mat[2][3]*v.z()+m_mat[3][3]) ;
    return crVector3d( (m_mat[0][0]*v.x() + m_mat[1][0]*v.y() + m_mat[2][0]*v.z() + m_mat[3][0])*d,
        (m_mat[0][1]*v.x() + m_mat[1][1]*v.y() + m_mat[2][1]*v.z() + m_mat[3][1])*d,
        (m_mat[0][2]*v.x() + m_mat[1][2]*v.y() + m_mat[2][2]*v.z() + m_mat[3][2])*d);
}

inline crVector4d crMatrixd::postMult( const crVector4d& v ) const
{
    return crVector4d( (m_mat[0][0]*v.x() + m_mat[0][1]*v.y() + m_mat[0][2]*v.z() + m_mat[0][3]*v.w()),
        (m_mat[1][0]*v.x() + m_mat[1][1]*v.y() + m_mat[1][2]*v.z() + m_mat[1][3]*v.w()),
        (m_mat[2][0]*v.x() + m_mat[2][1]*v.y() + m_mat[2][2]*v.z() + m_mat[2][3]*v.w()),
        (m_mat[3][0]*v.x() + m_mat[3][1]*v.y() + m_mat[3][2]*v.z() + m_mat[3][3]*v.w())) ;
}

inline crVector4d crMatrixd::preMult( const crVector4d& v ) const
{
    return crVector4d( (m_mat[0][0]*v.x() + m_mat[1][0]*v.y() + m_mat[2][0]*v.z() + m_mat[3][0]*v.w()),
        (m_mat[0][1]*v.x() + m_mat[1][1]*v.y() + m_mat[2][1]*v.z() + m_mat[3][1]*v.w()),
        (m_mat[0][2]*v.x() + m_mat[1][2]*v.y() + m_mat[2][2]*v.z() + m_mat[3][2]*v.w()),
        (m_mat[0][3]*v.x() + m_mat[1][3]*v.y() + m_mat[2][3]*v.z() + m_mat[3][3]*v.w()));
}
inline crVector3d crMatrixd::transform3x3(const crVector3d& v,const crMatrixd& m)
{
    return crVector3d( (m.m_mat[0][0]*v.x() + m.m_mat[1][0]*v.y() + m.m_mat[2][0]*v.z()),
                 (m.m_mat[0][1]*v.x() + m.m_mat[1][1]*v.y() + m.m_mat[2][1]*v.z()),
                 (m.m_mat[0][2]*v.x() + m.m_mat[1][2]*v.y() + m.m_mat[2][2]*v.z()));
}

inline crVector3d crMatrixd::transform3x3(const crMatrixd& m,const crVector3d& v)
{
    return crVector3d( (m.m_mat[0][0]*v.x() + m.m_mat[0][1]*v.y() + m.m_mat[0][2]*v.z()),
                 (m.m_mat[1][0]*v.x() + m.m_mat[1][1]*v.y() + m.m_mat[1][2]*v.z()),
                 (m.m_mat[2][0]*v.x() + m.m_mat[2][1]*v.y() + m.m_mat[2][2]*v.z()) ) ;
}

inline void crMatrixd::preMultTranslate( const crVector3d& v )
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

inline void crMatrixd::preMultTranslate( const crVector3f& v )
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

inline void crMatrixd::postMultTranslate( const crVector3d& v )
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

inline void crMatrixd::postMultTranslate( const crVector3f& v )
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

inline void crMatrixd::preMultScale( const crVector3d& v )
{
	m_mat[0][0] *= v[0]; m_mat[0][1] *= v[0]; m_mat[0][2] *= v[0]; m_mat[0][3] *= v[0];
	m_mat[1][0] *= v[1]; m_mat[1][1] *= v[1]; m_mat[1][2] *= v[1]; m_mat[1][3] *= v[1];
	m_mat[2][0] *= v[2]; m_mat[2][1] *= v[2]; m_mat[2][2] *= v[2]; m_mat[2][3] *= v[2];
}

inline void crMatrixd::preMultScale( const crVector3f& v )
{
	m_mat[0][0] *= v[0]; m_mat[0][1] *= v[0]; m_mat[0][2] *= v[0]; m_mat[0][3] *= v[0];
	m_mat[1][0] *= v[1]; m_mat[1][1] *= v[1]; m_mat[1][2] *= v[1]; m_mat[1][3] *= v[1];
	m_mat[2][0] *= v[2]; m_mat[2][1] *= v[2]; m_mat[2][2] *= v[2]; m_mat[2][3] *= v[2];
}

inline void crMatrixd::postMultScale( const crVector3d& v )
{
	m_mat[0][0] *= v[0]; m_mat[1][0] *= v[0]; m_mat[2][0] *= v[0]; m_mat[3][0] *= v[0];
	m_mat[0][1] *= v[1]; m_mat[1][1] *= v[1]; m_mat[2][1] *= v[1]; m_mat[3][1] *= v[1];
	m_mat[0][2] *= v[2]; m_mat[1][2] *= v[2]; m_mat[2][2] *= v[2]; m_mat[3][2] *= v[2];
}

inline void crMatrixd::postMultScale( const crVector3f& v )
{
	m_mat[0][0] *= v[0]; m_mat[1][0] *= v[0]; m_mat[2][0] *= v[0]; m_mat[3][0] *= v[0];
	m_mat[0][1] *= v[1]; m_mat[1][1] *= v[1]; m_mat[2][1] *= v[1]; m_mat[3][1] *= v[1];
	m_mat[0][2] *= v[2]; m_mat[1][2] *= v[2]; m_mat[2][2] *= v[2]; m_mat[3][2] *= v[2];
}

inline void crMatrixd::preMultRotate( const crQuat& q )
{
	if (q.zeroRotation())
		return;
	crMatrixd r;
	r.setRotate(q);
	preMult(r);
}

inline void crMatrixd::postMultRotate( const crQuat& q )
{
	if (q.zeroRotation())
		return;
	crMatrixd r;
	r.setRotate(q);
	postMult(r);
}

inline crVector3d operator* (const crVector3d& v, const crMatrixd& m )
{
	return m.preMult(v);
}
inline crVector4d operator* (const crVector4d& v, const crMatrixd& m )
{
	return m.preMult(v);
}

inline crVector3d crMatrixd::operator* (const crVector3d& v) const
{
	return postMult(v);
}
inline crVector4d crMatrixd::operator* (const crVector4d& v) const
{
	return postMult(v);
}

inline std::ostream& operator<< (std::ostream& os, const crMatrixd& m )
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
