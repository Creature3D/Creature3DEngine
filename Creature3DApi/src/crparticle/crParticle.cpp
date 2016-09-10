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
#include <CRParticle/crParticle.h>
#include <CRParticle/crLinearInterpolator.h>
#include <CRParticle/crParticleSystem.h>

#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMatrix.h>
#include <crgl/gl.h>
#include <CRCore/crNotify.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crBrain.h>
namespace
{

    const float cosPI3 = cosf(CRCore::PI / 3.0f);
    const float sinPI3 = sinf(CRCore::PI / 3.0f);
    const float hex_texcoord_x1 = 0.5f + 0.5f * cosPI3;
    const float hex_texcoord_x2 = 0.5f - 0.5f * cosPI3;
    const float hex_texcoord_y1 = 0.5f + 0.5f * sinPI3;
    const float hex_texcoord_y2 = 0.5f - 0.5f * sinPI3;

}

CRParticle::crParticle::crParticle()
:   m_shape(QUAD),
    m_sr(CRCore::crVector3(0.2f, 0.2f, 0.2f), CRCore::crVector3(0.2f, 0.2f, 0.2f)),
    m_ar(1, 0),
    m_cr(CRCore::crVector4(1, 1, 1, 1), CRCore::crVector4(1, 1, 1, 1)),
    m_si(new crLinearInterpolator), 
    m_ai(new crLinearInterpolator), 
    m_ci(new crLinearInterpolator),
    m_alive(true),
    m_mustdie(false),
    m_lifeTime(2),
    m_radius(0.2f),
    m_mass(0.1f),
    m_massinv(10.0f),
    m_prev_pos(0, 0, 0),
    m_position(0, 0, 0),
    m_velocity(0, 0, 0),
    m_prev_angle(0, 0, 0),
    m_angle(0, 0, 0),
    m_angul_arvel(0, 0, 0),
    m_t0(0),
    m_current_size(0,0,0),
    m_current_alpha(0),
    m_s_tile(1.0f),
    m_t_tile(1.0f),
    m_num_tile(1),
    m_cur_tile(-1),
    m_s_coord(0.0f),
    m_t_coord(0.0f),
    m_previousParticle(INVALID_INDEX),
    m_nextParticle(INVALID_INDEX),
	m_idr(0,0),
	m_idi(new crLinearInterpolator),
	m_current_id(0),
	m_alignedMode(AXIAL_None)
	//m_tiler(0,0),
	//m_tilei(NULL)
{
	swapBuffers();
}
void CRParticle::crParticle::setStaticAngle(const CRCore::crVector3& angle)
{
	m_staticAngle = angle;
	if (m_staticAngle.x() > CRCore::PI*2) m_staticAngle.x() -= CRCore::PI*2;
	if (m_staticAngle.x() < -CRCore::PI*2) m_staticAngle.x() += CRCore::PI*2;
	if (m_staticAngle.y() > CRCore::PI*2) m_staticAngle.y() -= CRCore::PI*2;
	if (m_staticAngle.y() < -CRCore::PI*2) m_staticAngle.y() += CRCore::PI*2;
	if (m_staticAngle.z() > CRCore::PI*2) m_staticAngle.z() -= CRCore::PI*2;
	if (m_staticAngle.z() < -CRCore::PI*2) m_staticAngle.z() += CRCore::PI*2;
}
CRCore::crVector3& CRParticle::crParticle::getStaticAngle()
{
	return m_staticAngle;
}
void CRParticle::crParticle::setAlignedMode(AxisAligned mode)
{
	m_alignedMode = mode;
}
CRParticle::crParticle::AxisAligned CRParticle::crParticle::getAlignedMode()
{
	return m_alignedMode;
}
bool CRParticle::crParticle::update(double dt)
{
    // this method should return false when the particle dies;
    // so, if we were instructed to die, do it now and return.
    if (m_mustdie) {
        m_alive = false;
        return false;
    }

    double x = 0;    

    // if we don't live forever, compute our normalized age.
    if (m_lifeTime > 0) {
        x = m_t0 / m_lifeTime;
    }

    m_t0 += dt;

    // if our age is over the lifetime limit, then die and return.
    if (x > 1) {
        m_alive = false;
        return false;
    }
    
    // compute the current values for size, alpha and color.
	//int currentTile = 0;
	if (dt == m_t0) 
	{
		if(!m_si.valid()) m_current_size = m_sr.get_random();
		if(!m_ai.valid()) m_current_alpha = m_ar.get_random();
		if(!m_ci.valid()) m_current_color = m_cr.get_random();
		if(!m_idi.valid()) m_current_id = m_idr.get_random();
		//if(!m_tilei.valid()) 
		//{
		//	m_cur_tile = m_tiler.get_random();
		//	m_s_coord = m_s_tile * fmod(m_cur_tile , 1.0 / m_s_tile);
		//	m_t_coord = 1.0 - m_t_tile * (static_cast<int>(m_cur_tile * m_t_tile) + 1);
		//}
	}
	else if (m_lifeTime > 0) 
	{
       if(m_si.valid()) m_current_size = m_si.get()->interpolate(x, m_sr);
       if(m_ai.valid()) m_current_alpha = m_ai.get()->interpolate(x, m_ar);
       if(m_ci.valid()) m_current_color = m_ci.get()->interpolate(x, m_cr);
	   if(m_idi.valid()) m_current_id = m_idi.get()->interpolate(x, m_idr);
	   //if(m_tilei.valid()) currentTile = m_tilei.get()->interpolate(x, m_tiler);
    }
	
	////Compute the current texture tile based on our normalized age
	int currentTile = static_cast<int>(x * m_num_tile);
	//If the current texture tile is different from previous, then compute new texture coords
	if(/*m_tilei.valid() && */currentTile != m_cur_tile)
	{
		m_cur_tile = currentTile;
		m_s_coord = m_s_tile * fmod(m_cur_tile , 1.0 / m_s_tile);
		m_t_coord = 1.0 - m_t_tile * (static_cast<int>(m_cur_tile * m_s_tile) + 1);
		// CRCore::notify(CRCore::NOTICE)<<this<<" setting tex coords "<<m_s_coord<<" "<<m_t_coord<<std::endl;
	}

    // update position
    m_prev_pos = m_position;
    m_position += m_velocity * dt;

    // update angle
    m_prev_angle = m_angle;
    m_angle += m_angul_arvel * dt;

    if (m_angle.x() > CRCore::PI*2) m_angle.x() -= CRCore::PI*2;
    if (m_angle.x() < -CRCore::PI*2) m_angle.x() += CRCore::PI*2;
    if (m_angle.y() > CRCore::PI*2) m_angle.y() -= CRCore::PI*2;
    if (m_angle.y() < -CRCore::PI*2) m_angle.y() += CRCore::PI*2;
    if (m_angle.z() > CRCore::PI*2) m_angle.z() -= CRCore::PI*2;
    if (m_angle.z() < -CRCore::PI*2) m_angle.z() += CRCore::PI*2;

    return true;
}

void CRParticle::crParticle::render(CRCore::crState& state, const CRCore::crObject* object, float scale) const
{
	//glColor4f(  m_current_color_buf.x(), 
	//            m_current_color_buf.y(), 
	//            m_current_color_buf.z(), 
	//            m_current_color_buf.w() * m_current_alpha_buf);
	const CRCore::crDrawable *drawable = object->getDrawable(m_current_id_buf);
	if(!drawable)
		return;
	drawable->setColor(CRCore::crVector4( m_current_color_buf.x(), 
										  m_current_color_buf.y(), 
										  m_current_color_buf.z(), 
										  m_current_color_buf.w() * m_current_alpha_buf));
	CRCore::crMatrix R;
	CRCore::crVector3 angle = m_angle_buf + m_staticAngle;
	R.makeRotate(
		angle.x(), CRCore::crVector3(1, 0, 0), 
		angle.y(), CRCore::crVector3(0, 1, 0), 
		angle.z(), CRCore::crVector3(0, 0, 1));
	CRCore::crVector3 s = m_current_size_buf * scale;
	R.postMultScale(s);
	//CRCore::crMatrix mat = R * CRCore::crMatrix::scale(s,s,s);
    R.setTrans(m_position_buf);
    
	glPushMatrix();
	glMultMatrix(R.ptr());
	drawable->draw(state);
    glPopMatrix();
}

void CRParticle::crParticle::render(CRCore::crState& state, const CRCore::crVector3& xpos, const CRCore::crVector3& px, const CRCore::crVector3& py, float scale) const
{
	//glColor4f(  m_current_color_buf.x(), 
	//            m_current_color_buf.y(), 
	//            m_current_color_buf.z(), 
	//            m_current_color_buf.w() * m_current_alpha_buf);
	state.applyColor(CRCore::crVector4(m_current_color_buf.x(), 
		m_current_color_buf.y(), 
		m_current_color_buf.z(), 
		m_current_color_buf.w() * m_current_alpha_buf));

	CRCore::crMatrix R;
	CRCore::crVector3 angle = m_angle_buf + m_staticAngle;
	R.makeRotate(
		angle.x(), CRCore::crVector3(1, 0, 0), 
		angle.y(), CRCore::crVector3(0, 1, 0), 
		angle.z(), CRCore::crVector3(0, 0, 1));
	//CRCore::crVector3 p1 = (px * m_current_size_buf * scale);
	//CRCore::crVector3 p2(py * m_current_size_buf * scale);
	CRCore::crVector3 p1 = px * scale;
	p1[0] *= m_current_size_buf[0];
	p1[1] *= m_current_size_buf[1];
	p1[2] *= m_current_size_buf[2];
	CRCore::crVector3 p2 = py * scale;
	p2[0] *= m_current_size_buf[0];
	p2[1] *= m_current_size_buf[1];
	p2[2] *= m_current_size_buf[2];

	switch (m_shape)
	{
	case POINT:        
		glVertex3f(xpos.x(), xpos.y(), xpos.z()); 
		break;

	case QUAD:
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf);
		glVertex3fv((xpos-(p1+p2)*R).ptr());
		glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf);
		glVertex3fv((xpos+(p1-p2)*R).ptr());
		glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf+m_t_tile_buf);
		glVertex3fv((xpos+(p1+p2)*R).ptr());
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf+m_t_tile_buf);
		glVertex3fv((xpos-(p1-p2)*R).ptr());
		break;
	case QUAD2:
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf);
		glVertex3fv((xpos-p1*R).ptr());
		glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf);
		glVertex3fv((xpos+p1*R).ptr());
		glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf+m_t_tile_buf);
		glVertex3fv((xpos+(p1+p2*2.0f)*R).ptr());
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf+m_t_tile_buf);
		glVertex3fv((xpos-(p1-p2*2.0f)*R).ptr());
		break;
	case QUAD_TRIANGLESTRIP:
		glPushMatrix();
		glTranslatef(xpos.x(), xpos.y(), xpos.z());
		glMultMatrix(R.ptr());
		// we must glBegin() and glEnd() here, because each particle is a single strip
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf+m_t_tile_buf);
		glVertex3fv((p1+p2).ptr());      
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf+m_t_tile_buf);
		glVertex3fv((-p1+p2).ptr());        
		glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf);
		glVertex3fv((p1-p2).ptr());
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf);
		glVertex3fv((-p1-p2).ptr());
		glEnd();
		glPopMatrix();
		break;

	case HEXAGON:
		glPushMatrix();
		glTranslatef(xpos.x(), xpos.y(), xpos.z());
		glMultMatrix(R.ptr());        
		// we must glBegin() and glEnd() here, because each particle is a single fan
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(m_s_coord_buf + m_s_tile_buf * 0.5f, m_t_coord_buf + m_t_tile_buf * 0.5f);
		glVertex3f(0,0,0);
		glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x1, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y1);
		glVertex3fv((p1*cosPI3+p2*sinPI3).ptr());
		glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x2, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y1);
		glVertex3fv((-p1*cosPI3+p2*sinPI3).ptr());
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf + m_t_tile_buf * 0.5f);
		glVertex3fv((-p1).ptr());
		glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x2, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y2);
		glVertex3fv((-p1*cosPI3-p2*sinPI3).ptr());
		glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x1, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y2);
		glVertex3fv((p1*cosPI3-p2*sinPI3).ptr());
		glTexCoord2f(m_s_coord_buf + m_s_tile_buf, m_t_coord_buf + m_t_tile_buf * 0.5f);
		glVertex3fv((p1).ptr());
		glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x1, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y1);
		glVertex3fv((p1*cosPI3+p2*sinPI3).ptr());
		glEnd();
		glPopMatrix();
		break;

	case LINE:
		{
			// Get the normalized direction of the particle, to be used in the 
			// calculation of one of the linesegment endpoints.
			float vl = m_velocity_buf.length();
			if (vl != 0) 
			{
				//CRCore::crVector3 v = m_velocity_buf * m_current_size_buf * scale / vl;
				CRCore::crVector3 v = m_velocity_buf * scale / vl;
				v[0] *= m_current_size_buf[0];
				v[1] *= m_current_size_buf[1];
				v[2] *= m_current_size_buf[2];

				glTexCoord1f(0);
				glVertex3f(xpos.x(), xpos.y(), xpos.z());
				glTexCoord1f(1);
				glVertex3f(xpos.x() + v.x(), xpos.y() + v.y(), xpos.z() + v.z());
			}
		}
		break;

	default:
		CRCore::notify(CRCore::WARN) << "Invalid shape for particles\n";
	}
}
void CRParticle::crParticle::render(CRCore::crState& state, const CRCore::crMatrix& parentmat, const CRCore::crVector3& xpos, const CRCore::crVector3& px, const CRCore::crVector3& py, float scale) const
{
    //glColor4f(  m_current_color_buf.x(), 
    //            m_current_color_buf.y(), 
    //            m_current_color_buf.z(), 
    //            m_current_color_buf.w() * m_current_alpha_buf);
	state.applyColor(CRCore::crVector4(m_current_color_buf.x(), 
		            m_current_color_buf.y(), 
		            m_current_color_buf.z(), 
		            m_current_color_buf.w() * m_current_alpha_buf));

    CRCore::crMatrix R;
	CRCore::crVector3 angle = m_angle_buf + m_staticAngle;
    R.makeRotate(
        angle.x(), CRCore::crVector3(1, 0, 0), 
        angle.y(), CRCore::crVector3(0, 1, 0), 
        angle.z(), CRCore::crVector3(0, 0, 1));
	if(m_alignedMode != AXIAL_None)
	{
		CRCore::crMatrix matrix;
		CRCore::crVector3 eye_local = CRCore::crBrain::getInstance()->getCameraPosition();
		//CRCore::notify(CRCore::ALWAYS)<<"crParticle eye_local = "<<eye_local<<"mode = "<<m_alignedMode<<std::endl;
		CRCore::crVector3 pos = parentmat.preMult(xpos);
		CRCore::crVector3 ev(eye_local-pos);
		switch (m_alignedMode)
		{
		case CRParticle::crParticle::AXIAL_ROT_X_AXIS:
			{
				ev.x() = 0.0f;
				float ev_length = ev.length();
				if (ev_length>0.0f)
				{
					float inv = 1.0f/ev_length;
					float s = -ev.z()*inv;
					float c = -ev.y()*inv;
					matrix(1,1) = c;
					matrix(2,1) = -s;
					matrix(1,2) = s;
					matrix(2,2) = c;
				}
			}
			break;
		case CRParticle::crParticle::AXIAL_ROT_Y_AXIS:
			{
				ev.y() = 0.0f;
				float ev_length = ev.length();
				if (ev_length>0.0f)
				{
					float inv = 1.0f/ev_length;
					float s = -ev.z()*inv;
					float c = ev.x()*inv;
					matrix(0,0) = c;
					matrix(2,0) = s;
					matrix(0,2) = -s;
					matrix(2,2) = c;
				}
			}
			break;
		case CRParticle::crParticle::AXIAL_ROT_Z_AXIS:
			{
				ev.z() = 0.0f;
				float ev_length = ev.length();
				if (ev_length>0.0f)
				{
					float inv = 1.0f/ev_length;
					float s = ev.x()*inv;
					float c = -ev.y()*inv;
					matrix(0,0) = c;
					matrix(1,0) = -s;
					matrix(0,1) = s;
					matrix(1,1) = c;
				}
			}
			break;
		case CRParticle::crParticle::POINT_ROT_EYE:
			{
				CRCore::crVector3 normal(0,0,1);
				float ev_len = ev.length();
				if (ev_len != 0.0f)
				{
					ev /= ev_len;

					CRCore::crVector3 cp(ev^normal);
					float dot = ev*normal;

					float cp_len = cp.length();
					if (cp_len != 0.0f)
					{
						cp /= cp_len;

						float rotation_cp = acosf(dot);
						matrix.makeRotate(-CRCore::inRadians(rotation_cp),cp[0],cp[1],cp[2]);
					}
				}
			}
			break;
		case CRParticle::crParticle::POINT_ROT_WORLD_Z_AXIS:
			{
				CRCore::crVector2   about_z( -ev.y(), ev.x() );
				if( about_z.length() == 0.0f ) about_z.x() = 1.0f;
				about_z.normalize();
				float  xy_distance = sqrt( ev.x()*ev.x() + ev.y()*ev.y() );
				CRCore::crVector2   from_xy( xy_distance, -ev.z() );
				if( from_xy.length() == 0.0f ) from_xy.x() = 1.0f;
				from_xy.normalize();

				matrix(0,0) =  about_z.x();
				matrix(0,1) =  about_z.y();
				matrix(1,0) = -about_z.y()*from_xy.x();
				matrix(1,1) =  about_z.x()*from_xy.x();
				matrix(1,2) =  from_xy.y();
				matrix(2,0) =  about_z.y()*from_xy.y();
				matrix(2,1) = -about_z.x()*from_xy.y();
				matrix(2,2) =  from_xy.x();
			}
			break;
		}
		//matrix.setTrans(xpos);
		R.postMult(matrix);
	}
	//CRCore::crVector3 p1 = (px * m_current_size_buf * scale);
    //CRCore::crVector3 p2(py * m_current_size_buf * scale);
	CRCore::crVector3 p1 = px * scale;
	p1[0] *= m_current_size_buf[0];
	p1[1] *= m_current_size_buf[1];
	p1[2] *= m_current_size_buf[2];
	CRCore::crVector3 p2 = py * scale;
	p2[0] *= m_current_size_buf[0];
	p2[1] *= m_current_size_buf[1];
	p2[2] *= m_current_size_buf[2];

    switch (m_shape)
    {
    case POINT:        
        glVertex3f(xpos.x(), xpos.y(), xpos.z()); 
        break;

    case QUAD:
        glTexCoord2f(m_s_coord_buf, m_t_coord_buf);
        glVertex3fv((xpos-(p1+p2)*R).ptr());
        glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf);
        glVertex3fv((xpos+(p1-p2)*R).ptr());
        glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf+m_t_tile_buf);
        glVertex3fv((xpos+(p1+p2)*R).ptr());
        glTexCoord2f(m_s_coord_buf, m_t_coord_buf+m_t_tile_buf);
        glVertex3fv((xpos-(p1-p2)*R).ptr());
        break;
	case QUAD2:
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf);
		glVertex3fv((xpos-p1*R).ptr());
		glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf);
		glVertex3fv((xpos+p1*R).ptr());
		glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf+m_t_tile_buf);
		glVertex3fv((xpos+(p1+p2*2.0f)*R).ptr());
		glTexCoord2f(m_s_coord_buf, m_t_coord_buf+m_t_tile_buf);
		glVertex3fv((xpos-(p1-p2*2.0f)*R).ptr());
		break;
    case QUAD_TRIANGLESTRIP:
        glPushMatrix();
        glTranslatef(xpos.x(), xpos.y(), xpos.z());
        glMultMatrix(R.ptr());
        // we must glBegin() and glEnd() here, because each particle is a single strip
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf+m_t_tile_buf);
        glVertex3fv((p1+p2).ptr());      
        glTexCoord2f(m_s_coord_buf, m_t_coord_buf+m_t_tile_buf);
        glVertex3fv((-p1+p2).ptr());        
        glTexCoord2f(m_s_coord_buf+m_s_tile_buf, m_t_coord_buf);
        glVertex3fv((p1-p2).ptr());
        glTexCoord2f(m_s_coord_buf, m_t_coord_buf);
        glVertex3fv((-p1-p2).ptr());
        glEnd();
        glPopMatrix();
        break;

    case HEXAGON:
        glPushMatrix();
        glTranslatef(xpos.x(), xpos.y(), xpos.z());
        glMultMatrix(R.ptr());        
        // we must glBegin() and glEnd() here, because each particle is a single fan
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(m_s_coord_buf + m_s_tile_buf * 0.5f, m_t_coord_buf + m_t_tile_buf * 0.5f);
        glVertex3f(0,0,0);
        glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x1, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y1);
        glVertex3fv((p1*cosPI3+p2*sinPI3).ptr());
        glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x2, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y1);
        glVertex3fv((-p1*cosPI3+p2*sinPI3).ptr());
        glTexCoord2f(m_s_coord_buf, m_t_coord_buf + m_t_tile_buf * 0.5f);
        glVertex3fv((-p1).ptr());
        glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x2, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y2);
        glVertex3fv((-p1*cosPI3-p2*sinPI3).ptr());
        glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x1, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y2);
        glVertex3fv((p1*cosPI3-p2*sinPI3).ptr());
        glTexCoord2f(m_s_coord_buf + m_s_tile_buf, m_t_coord_buf + m_t_tile_buf * 0.5f);
        glVertex3fv((p1).ptr());
        glTexCoord2f(m_s_coord_buf + m_s_tile_buf * hex_texcoord_x1, m_t_coord_buf + m_t_tile_buf * hex_texcoord_y1);
        glVertex3fv((p1*cosPI3+p2*sinPI3).ptr());
        glEnd();
        glPopMatrix();
        break;

    case LINE:
        {
            // Get the normalized direction of the particle, to be used in the 
            // calculation of one of the linesegment endpoints.
            float vl = m_velocity_buf.length();
            if (vl != 0) 
			{
                //CRCore::crVector3 v = m_velocity_buf * m_current_size_buf * scale / vl;
				CRCore::crVector3 v = m_velocity_buf * scale / vl;
				v[0] *= m_current_size_buf[0];
				v[1] *= m_current_size_buf[1];
				v[2] *= m_current_size_buf[2];

                glTexCoord1f(0);
                glVertex3f(xpos.x(), xpos.y(), xpos.z());
                glTexCoord1f(1);
                glVertex3f(xpos.x() + v.x(), xpos.y() + v.y(), xpos.z() + v.z());
            }
        }
        break;

    default:
        CRCore::notify(CRCore::WARN) << "Invalid shape for particles\n";
    }
}

void CRParticle::crParticle::setUpTexCoordsAsPartOfConnectedParticleSystem(crParticleSystem* ps)
{
    if (getPreviousParticle()!=crParticle::INVALID_INDEX)
    {
        update(0.0);

        crParticle* previousParticle = ps->getParticle(getPreviousParticle());
        const CRCore::crVector3& previousPosition = previousParticle->getPosition();
        const CRCore::crVector3& newPosition = getPosition();
        float distance = (newPosition-previousPosition).length();
        float s_coord_delta = 0.5f*distance/getCurrentSize().length();
        float s_coord = previousParticle->m_s_coord + s_coord_delta;

        setTextureTile(1,1,0);
        m_cur_tile = 0;
        m_s_coord = s_coord;
        m_t_coord = 0.0f;
    }
}
