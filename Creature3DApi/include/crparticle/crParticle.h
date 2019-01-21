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
#ifndef CRPARTICLE_PARTICLE
#define CRPARTICLE_PARTICLE 1

#include <CRParticle/crExport.h>
#include <CRParticle/crInterpolator.h>
#include <CRCore/crRange.h>

#include <CRCore/ref_ptr.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crObject.h>
#include <crgl/gl.h>

namespace CRParticle
{

    // forward declare so we can reference it
    class crParticleSystem;

    /**    Implementation of a <B>particle</B>.
        Objects of this class are particles, they have some graphical properties
        and some physical properties. Particles are created by emitters and then placed
        into crParticle Systems, where they live and get updated at each frame.
        Particles can either live forever (lifeTime < 0), or die after a specified
        time (lifeTime >= 0). For each property which is defined as a crRange of values, a
        "current" value will be evaluated at each frame by interpolating the <I>min</I>
        and <I>max</I> values so that <I>curr_value = min</I> when <I>t == 0</I>, and 
        <I>curr_value = max</I> when <I>t == lifeTime</I>.
        You may customize the interpolator objects to achieve any kind of transition.
        If you want the particle to live forever, set its lifetime to any value <= 0;
        in that case, no interpolation is done to compute real-time properties, and only
        minimum values are used.
    */
	class CRPARTICLE_EXPORT crParticle : public CRCore::Referenced
	{
    public:
        
        enum
        {
            INVALID_INDEX = -1
        };

        /**
         Shape of particles.
         NOTE: the LINE shape should be used in conjunction with FIXED alignment mode (see crParticleSystem).
        */
        enum Shape {
            POINT,            // uses GL_POINTS as primitive
            QUAD,            // uses GL_QUADS as primitive
            QUAD_TRIANGLESTRIP,    // uses GL_TRI_angleSTRIP as primitive, but each particle needs a glBegin/glEnd pair
            HEXAGON,            // may save some filling time, but uses more triangles
            LINE,                // uses GL_LINES to draw line segments that point to the direction of motion
			Object,
			QUAD2,
        };
        
        crParticle();

        /// Get the shape of the particle.
        inline Shape getShape() const;
        
        /// Set the shape of the particle.
        inline void setShape(Shape s);

        /// Get whether the particle is still alive.
        inline bool isAlive() const;
		inline bool isBufAlive() const;

        /// Get the life time of the particle (in seconds).
        inline double getLifeTime() const;
        
        /// Get the age of the particle (in seconds).
        inline double getAge() const;
        
        /// Get the minimum and maximum values for polygon size.
        inline const CRCore::rangev3& getSizeRange() const;
        
        /// Get the minimum and maximum values for alpha.
        inline const CRCore::rangef& getAlphaRange() const;
        
        /// Get the minimum and maximum values for color.
        inline const CRCore::rangev4& getColorRange() const;
        
        /// Get the interpolator for computing the size of polygons.
        inline const crInterpolator* getSizeInterpolator() const;
        
        /// Get the interpolator for computing alpha values.
        inline const crInterpolator* getAlphaInterpolator() const;
        
        /// Get the interpolator for computing color values.
        inline const crInterpolator* getColorInterpolator() const;

        /** Get the physical radius of the particle.
            For built-in operators to work correctly, lengths must be expressed in meters.
        */
        inline float getRadius() const;
        
        /** Get the mass of the particle.
            For built-in operators to work correctly, remember that the mass is expressed in kg.
        */
        inline float getMass() const;
        
        /// Get <CODE>1 / getMass()</CODE>.
        inline float getMassInv() const;
        
        /// Get the position vector.
        inline const CRCore::crVector3& getPosition() const;
		inline const CRCore::crVector3& getBufPosition() const;
        
        /**    Get the velocity vector.
            For built-in operators to work correctly, remember that velocity components are expressed
            in meters per second.
        */
        inline const CRCore::crVector3& getVelocity() const;        
        
        /// Get the previous position (the position before last update).
        inline const CRCore::crVector3& getPreviousPosition() const;

        /// Get the angle vector.
        inline const CRCore::crVector3& getAngle() const;
        
        /// Get the rotational velocity vector.
        inline const CRCore::crVector3& getAngularVelocity() const;
        
        /// Get the previous angle vector.
        inline const CRCore::crVector3& getPreviousAngle() const;
        
        /// Get the current color
        inline const CRCore::crVector4& getCurrentColor() const { return m_current_color; }
		inline const CRCore::crVector4& getBufCurrentColor() const { return m_current_color_buf; }

        /// Get the current alpha
        inline float getCurrentAlpha() const { return m_current_alpha; }
		inline float getBufCurrentAlpha() const { return m_current_alpha_buf; }

        /// Get the s texture coordinate of the bottom left of the particle
        inline const float getSTexCoord() const { return m_s_coord; }
		inline const float getBufSTexCoord() const { return m_s_coord_buf; }

        /// Get the t texture coordinate of the bottom left of the particle
        inline float getTCoord() const { return m_t_coord; }

        /// Get width of texture tile
        inline int getTileS() const;
        
        /// Get height of texture tile
        inline int getTileT() const;
        
        /// Get number of texture tiles
        inline int getNumTiles() const { return m_num_tile; }
        
        /** Kill the particle on next update
            NOTE: after calling this function, the <CODE>isAlive()</CODE> method will still 
            return true until the particle is updated again.
        */
        inline void kill();
        
        /// Set the life time of the particle.
        inline void setLifeTime(double t);
        
        /// Set the minimum and maximum values for polygon size.
        inline void setSizeRange(const CRCore::rangev3& r);
        
        /// Set the minimum and maximum values for alpha.
        inline void setAlphaRange(const CRCore::rangef& r);
        
        /// Set the minimum and maximum values for color.
        inline void setColorRange(const CRCore::rangev4& r);
        
        /// Set the interpolator for computing size values.
        inline void setSizeInterpolator(crInterpolator* ri);
        
        /// Set the interpolator for computing alpha values.        
        inline void setAlphaInterpolator(crInterpolator* ai);
        
        /// Set the interpolator for computing color values.
        inline void setColorInterpolator(crInterpolator* ci);

        /** Set the physical radius of the particle.
            For built-in operators to work correctly, lengths must be expressed in meters.
        */
        inline void setRadius(float r);
        
        /** Set the mass of the particle.
            For built-in operators to work correctly, remember that the mass is expressed in kg.
        */
        inline void setMass(float m);
        
        /// Set the position vector.        
        inline void setPosition(const CRCore::crVector3& p);
        
        /**    Set the velocity vector.
            For built-in operators to work correctly, remember that velocity components are expressed
            in meters per second.
        */
        inline void setVelocity(const CRCore::crVector3& v);
        
        /// Add a vector to the velocity vector.
        inline void addVelocity(const CRCore::crVector3& dv);
        
        /// crTransform position and velocity vectors by a matrix.
        inline void transformPositionVelocity(const CRCore::crMatrix& xform);

        /// crTransform position and velocity vectors by a combination of two matrices
        void transformPositionVelocity(const CRCore::crMatrix& xform1, const CRCore::crMatrix& xform2, float r);

        /// Set the angle vector.
        inline void setAngle(const CRCore::crVector3& a);
        
        /**
          Set the angular velocity vector. 
          Components x, y and z are angles of rotation around the respective axis (in radians).
        */
        inline void setAngularVelocity(const CRCore::crVector3& v);
        
        /// Add a vector to the angular velocity vector.
        inline void addAngularVelocity(const CRCore::crVector3& dv);
        
        /// crTransform angle and angularVelocity vectors by a matrix.
        inline void transformAngleVelocity(const CRCore::crMatrix& xform);
        
        /**    Update the particle (don't call this method manually).
            This method is called automatically by <CODE>crParticleSystem::update()</CODE>; it
            updates the graphical properties of the particle for the current time,
            checks whether the particle is still alive, and then updates its position
            by computing <I>P = P + V * dt</I> (where <I>P</I> is the position and <I>V</I> is the velocity).
        */
        bool update(double dt);

        /// Perform some pre-rendering tasks. Called automatically by particle systems.
        inline void beginRender() const;
        
        /// Render the particle. Called automatically by particle systems.
        void render(CRCore::crState& state, const CRCore::crVector3& xpos, const CRCore::crVector3& px, const CRCore::crVector3& py, float scale = 1.0f) const;
		void render(CRCore::crState& state, const CRCore::crMatrix& parentmat, const CRCore::crVector3& xpos, const CRCore::crVector3& px, const CRCore::crVector3& py, float scale = 1.0f) const;
		void render(CRCore::crState& state, const CRCore::crObject* object, float scale) const;

        /// Perform some post-rendering tasks. Called automatically by particle systems.
        inline void endRender() const;
        
        /// Get the current (interpolated) polygon size. Valid only after the first call to update().
        inline const CRCore::crVector3 &getCurrentSize() const;
		inline const CRCore::crVector3 &getBufCurrentSize() const;
        
        /// Specify how the particle texture is tiled
        inline void setTextureTile(int sTile, int tTile, int numTiles = 0);

        /// Set the previous particle
        inline void setPreviousParticle(int previous) { m_previousParticle = previous; }

        /// Get the previous particle
        inline int getPreviousParticle() const { return m_previousParticle; }
		inline int getBufPreviousParticle() const { return m_previousParticle_buf; }

        /// Set the next particle
        inline void setNextParticle(int next) { m_nextParticle = next; }

        /// Get the const next particle
        inline int getNextParticle() const { return m_nextParticle; }
		inline int getBufNextParticle() const { return m_nextParticle_buf; }

		void setStaticAngle(const CRCore::crVector3& angle);
		CRCore::crVector3& getStaticAngle();

        /// Method for initializing a particles texture coords as part of a connected particle system.
        void setUpTexCoordsAsPartOfConnectedParticleSystem(crParticleSystem* ps);

		inline virtual void swapBuffers()
		{
			m_position_buf = m_position;

		    m_current_color_buf = m_current_color;
			m_current_alpha_buf = m_current_alpha;
			m_angle_buf = m_angle;
			m_current_size_buf = m_current_size;
			m_s_coord_buf = m_s_coord;
			m_t_coord_buf = m_t_coord;
			m_s_tile_buf = m_s_tile;
			m_t_tile_buf = m_t_tile;
			m_velocity_buf = m_velocity;

			m_nextParticle_buf = m_nextParticle;
			m_previousParticle_buf = m_previousParticle;
			m_alive_buf = m_alive;

			m_current_id_buf = m_current_id;
		}

		/// Set the minimum and maximum values for alpha.
		inline void setDrawableIdRange(const CRCore::rangei& r){ m_idr = r; }
		inline void setDrawableIdInterpolator(crInterpolator* i){ m_idi = i; }
		inline const CRCore::rangei& getDrawableIdRange() const{ return m_idr; }
		inline const crInterpolator* getDrawableIdInterpolator() const
		{
			return m_idi.get();
		}
		//inline void setTileInterpolator(crInterpolator* i){ m_tilei = i; }
		//inline const crInterpolator* crParticle::getTileInterpolator() const
		//{
		//	return m_tilei.get();
		//}
		enum AxisAligned
		{
			AXIAL_None,
			AXIAL_ROT_X_AXIS,
			AXIAL_ROT_Y_AXIS,
			AXIAL_ROT_Z_AXIS,
			POINT_ROT_EYE,
			POINT_ROT_WORLD_Z_AXIS,
		};
		void setAlignedMode(AxisAligned mode);
		AxisAligned getAlignedMode();
    protected:
    
        Shape m_shape;

        CRCore::rangev3 m_sr;
        CRCore::rangef m_ar;
        CRCore::rangev4 m_cr;

        CRCore::ref_ptr<crInterpolator> m_si;
        CRCore::ref_ptr<crInterpolator> m_ai;
        CRCore::ref_ptr<crInterpolator> m_ci;

        bool m_alive;
        bool m_mustdie;
        double m_lifeTime;

        float m_radius;
        float m_mass;
        float m_massinv;
        CRCore::crVector3 m_prev_pos;
        CRCore::crVector3 m_position;
        CRCore::crVector3 m_velocity;

        CRCore::crVector3 m_prev_angle;
        CRCore::crVector3 m_angle;
        CRCore::crVector3 m_angul_arvel;

        double m_t0;        

        CRCore::crVector3 m_current_size;
        float m_current_alpha;
        CRCore::crVector4 m_current_color;
        
        float m_s_tile;
        float m_t_tile;
        int m_num_tile;
        int m_cur_tile;
        float m_s_coord;
        float m_t_coord;
        
        // previous and next Particles are only used in ConnectedParticleSystems
        int m_previousParticle;
        int m_nextParticle;

		CRCore::crVector4 m_current_color_buf;
		float m_current_alpha_buf;
		CRCore::crVector3 m_angle_buf;
		CRCore::crVector3 m_current_size_buf;
		float m_s_coord_buf;
		float m_t_coord_buf;
		float m_s_tile_buf;
		float m_t_tile_buf;
		CRCore::crVector3 m_velocity_buf;
		CRCore::crVector3 m_position_buf;
		int m_nextParticle_buf;
		int m_previousParticle_buf;
		bool m_alive_buf;

		CRCore::rangei m_idr;
		CRCore::ref_ptr<crInterpolator> m_idi;
        int m_current_id;
		int m_current_id_buf;
		CRCore::crVector3 m_staticAngle;
		AxisAligned m_alignedMode;
		//CRCore::rangei m_tiler;
		//CRCore::ref_ptr<crInterpolator> m_tilei;
    };

    // INLINE FUNCTIONS

    inline crParticle::Shape crParticle::getShape() const
    {
        return m_shape;
    }

    inline void crParticle::setShape(Shape s)
    {
        m_shape = s;
    }

    inline bool crParticle::isAlive() const
    {
        return m_alive;
    }

	inline bool crParticle::isBufAlive() const
	{
		return m_alive_buf;
	}

    inline double crParticle::getLifeTime() const
    {
        return m_lifeTime;
    }
    
    inline double crParticle::getAge() const
    {
        return m_t0;
    }
    
    inline float crParticle::getRadius() const
    {
        return m_radius;
    }
    
    inline void crParticle::setRadius(float r)
    {
        m_radius = r;
    }

    inline const CRCore::rangev3& crParticle::getSizeRange() const
    {
        return m_sr;
    }

    inline const CRCore::rangef& crParticle::getAlphaRange() const
    {
        return m_ar;
    }

    inline const CRCore::rangev4& crParticle::getColorRange() const
    {
        return m_cr;
    }

    inline const crInterpolator* crParticle::getSizeInterpolator() const
    {
        return m_si.get();
    }

    inline const crInterpolator* crParticle::getAlphaInterpolator() const
    {
        return m_ai.get();
    }

    inline const crInterpolator* crParticle::getColorInterpolator() const
    {
        return m_ci.get();
    }

    inline const CRCore::crVector3& crParticle::getPosition() const
    {
        return m_position;
    }

	inline const CRCore::crVector3& crParticle::getBufPosition() const
	{
		return m_position_buf;
	}

    inline const CRCore::crVector3& crParticle::getVelocity() const
    {
        return m_velocity;
    }
    
    inline const CRCore::crVector3& crParticle::getPreviousPosition() const
    {
        return m_prev_pos;
    }

    inline const CRCore::crVector3& crParticle::getAngle() const
    {
        return m_angle;
    }
    
    inline const CRCore::crVector3& crParticle::getAngularVelocity() const
    {
        return m_angul_arvel;
    }
    
    inline const CRCore::crVector3& crParticle::getPreviousAngle() const
    {
        return m_prev_angle;
    }
    
    inline int crParticle::getTileS() const
    {
        return (m_s_tile>0.0f) ? static_cast<int>(1.0f / m_s_tile) : 1;
    }

    inline int crParticle::getTileT() const
    {
        return (m_t_tile>0.0f) ? static_cast<int>(1.0f / m_t_tile) : 1;
    }
    
    inline void crParticle::kill()
    {
        m_mustdie = true;
    }

    inline void crParticle::setLifeTime(double t)
    {
        m_lifeTime = t;
    }

    inline void crParticle::setSizeRange(const CRCore::rangev3& r)
    {
        m_sr = r;
    }

    inline void crParticle::setAlphaRange(const CRCore::rangef& r)
    {
        m_ar = r;
    }

    inline void crParticle::setColorRange(const CRCore::rangev4& r)
    {
        m_cr = r;
    }

    inline void crParticle::setSizeInterpolator(crInterpolator* ri)
    {
        m_si = ri;
    }

    inline void crParticle::setAlphaInterpolator(crInterpolator* ai)
    {
        m_ai = ai;
    }

    inline void crParticle::setColorInterpolator(crInterpolator* ci)
    {
        m_ci = ci;
    }

    inline void crParticle::setPosition(const CRCore::crVector3& p)
    {
        m_position = p;
    }

    inline void crParticle::setVelocity(const CRCore::crVector3& v)
    {
        m_velocity = v;
    }

    inline void crParticle::addVelocity(const CRCore::crVector3& dv)
    {
        m_velocity += dv;
    }

    inline void crParticle::transformPositionVelocity(const CRCore::crMatrix& xform)
    {
        m_position = xform.preMult(m_position);
        m_velocity = CRCore::crMatrix::transform3x3(m_velocity, xform);
    }
    
    inline void crParticle::transformPositionVelocity(const CRCore::crMatrix& xform1, const CRCore::crMatrix& xform2, float r)
    {
        CRCore::crVector3 position1 = xform1.preMult(m_position);
        CRCore::crVector3 velocity1 = CRCore::crMatrix::transform3x3(m_velocity, xform1);
        CRCore::crVector3 position2 = xform2.preMult(m_position);
        CRCore::crVector3 velocity2 = CRCore::crMatrix::transform3x3(m_velocity, xform2);
        float one_minus_r = 1.0f-r;
        m_position = position1*r + position2*one_minus_r;
        m_velocity = velocity1*r + velocity2*one_minus_r;
    }
    
    inline void crParticle::setAngle(const CRCore::crVector3& a)
    {
        m_angle = a;
    }
    
    inline void crParticle::setAngularVelocity(const CRCore::crVector3& v)
    {
        m_angul_arvel = v;
    }
    
    inline void crParticle::addAngularVelocity(const CRCore::crVector3& dv)
    {
        m_angul_arvel += dv;
    }
    
    inline void crParticle::transformAngleVelocity(const CRCore::crMatrix& xform)
    {
        // this should be optimized!
        
        CRCore::crVector3 a1 = m_angle + m_angul_arvel;
        
        m_angle = xform.preMult(m_angle);
        a1 = xform.preMult(a1);
        
        m_angul_arvel = a1 - m_angle;
    }
            
    inline float crParticle::getMass() const
    {
        return m_mass;
    }
    
    inline float crParticle::getMassInv() const
    {
        return m_massinv;
    }
    
    inline void crParticle::setMass(float m)
    {
        m_mass = m;
        m_massinv = 1 / m;
    }
    
    inline void crParticle::beginRender() const
    {
        switch (m_shape)
        {
        case POINT:
            glBegin(GL_POINTS);
            break;
        case QUAD:
		case QUAD2:
            glBegin(GL_QUADS);
            break;
        case LINE:
            glBegin(GL_LINES);
            break;
        default: ;
        }
    }
    
    inline void crParticle::endRender() const
    {
        switch (m_shape)
        {        
        case POINT:
        case QUAD:
		case QUAD2:
        case LINE:
            glEnd();
            break;
        default: ;
        }
    }
    
	inline const CRCore::crVector3 &crParticle::getCurrentSize() const
    {
        return m_current_size;
    }

	inline const CRCore::crVector3 &crParticle::getBufCurrentSize() const
	{
		return m_current_size_buf;
	}
    
    inline void crParticle::setTextureTile(int sTile, int tTile, int numTiles)
    {
        m_s_tile = (sTile>0) ? 1.0f / static_cast<float>(sTile) : 1.0f;
        m_t_tile = (tTile>0) ? 1.0f / static_cast<float>(tTile) : 1.0f;
        if (numTiles <= 0)
        {
            m_num_tile = sTile * tTile;
        }
        else
        {
            m_num_tile = numTiles;
        }
		//m_tiler.set(0,m_num_tile);
    }


}

#endif

