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

#ifndef CRTEXT_STYLE
#define CRTEXT_STYLE 1

#include <CRCore/crBase.h>
#include <CRCore/crVector2.h>
#include <CRText/crExport.h>

#include <iosfwd>
#include <vector>

namespace CRText
{

class CRTEXT_EXPORT crBevel : public CRCore::crBase
{
    public:

        crBevel();
        crBevel(const crBevel& bevel, const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRText, crBevel)

        bool operator == (const crBevel& rhs) const
        {
            if (m_thickness != rhs.m_thickness) return false;
            return m_vertices==rhs.m_vertices;
        }

        void setBevelThickness(float thickness) { m_thickness = thickness; }
        float getBevelThickness() const { return m_thickness; }

        void flatBevel(float width=0.25f);

        void roundedBevel(float width=0.5f, unsigned int numSteps=10);

        void roundedBevel2(float width=0.5f, unsigned int numSteps=10);

        typedef std::vector<CRCore::crVector2> Vertices;

        void setVertices(const Vertices& vertices) { m_vertices = vertices; }
        Vertices& getVertices() { return m_vertices; }
        const Vertices& getVertices() const { return m_vertices; }

        //void print(std::ostream& fout);

    protected:

        float           m_thickness;
        Vertices        m_vertices;
};


class CRTEXT_EXPORT crStyle : public CRCore::crBase
{
    public:

        crStyle();
        crStyle(const crStyle& style, const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRText, crStyle)

        /// default Layout implementation used if no other is specified on TextNode
        static CRCore::ref_ptr<crStyle>& getDefaultStyle();

        bool operator == (const crStyle& style) const;

        /// NULL is no bevel
        void setBevel(crBevel* bevel) { m_bevel = bevel; }
        const crBevel* getBevel() const { return m_bevel.get(); }


        /// 1 is the default width of the text
        void setWidthRatio(float widthRatio) { m_widthRatio = widthRatio; }
        float getWidthRatio() const { return m_widthRatio; }

        /// 0 is 2D text
        void setThicknessRatio(float thicknessRatio) { m_thicknessRatio = thicknessRatio; }
        float getThicknessRatio() const { return m_thicknessRatio; }

        /// 0 is off
        void setOutlineRatio(float outlineRatio) { m_outlineRatio = outlineRatio; }
        float getOutlineRatio() const { return m_outlineRatio; }

        /// 1.0 is default number of samples
        void setSampleDensity(float sd) { m_sampleDensity = sd; }
        float getSampleDensity() const { return m_sampleDensity; }

    protected:

        CRCore::ref_ptr<crBevel>     m_bevel;

        float                   m_widthRatio;
        float                   m_thicknessRatio;
        float                   m_outlineRatio;
        float                   m_sampleDensity;
};

}

#endif
