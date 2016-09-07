/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef CRGAME_CREXTERNFILELOADVISITOR_H
#define CRGAME_CREXTERNFILELOADVISITOR_H 1

#include "creature3d.h"
#include <string>

namespace CRGame{

class crExternFileLoadVisitor : public CRCore::crNodeVisitor
{
public:
	crExternFileLoadVisitor(CREncapsulation::crTableIO *scenarioTab = NULL);

	virtual ~crExternFileLoadVisitor();
	void readParticleEffect(CRParticle::crParticleEffect *particle, const std::string &command);
	//CRCore::crNode *readExtEvent(std::string &command,const std::string &nodeName);
	virtual void apply(CRCore::crExternNode& node);
	virtual void apply(CRCore::crSequence& node);
	virtual void apply(CRCore::crGroup& node);
	virtual void apply(CRCore::crObject& node);
	virtual void apply(CRCore::crNode& node);
	virtual void apply(CRCore::crDB& node);
	virtual void apply(CRCore::crMatrixTransform& node);
	virtual void apply(CRCore::crLightSource& node);
	virtual void apply(CRParticle::crParticleEffect& node);
	virtual void apply(CRCore::crCanvasNode& node);
	virtual void apply(CRCore::crWidgetNode& node);
	virtual void apply(CRUI::crScrollBarWidgetNode& node);
	virtual void apply(CRUI::crTextAttrWidgetNode& node);
	virtual void apply(CRUI::crStaticTextWidgetNode& node);
	virtual void apply(CRUI::crEditWidgetNode& node);
	virtual void apply(CRUI::crHypertextWidgetNode& node);
	virtual void apply(CRUI::crListBoxWidgetNode& node);
	virtual void apply(CRUI::crListControlWidgetNode& node);
	virtual void apply(CRUI::crTableWidgetNode& node);
	virtual void apply(CRUI::crRadioGroupWidgetNode& node);
	virtual void apply(CRUI::crRadioWidgetNode& node);
	virtual void apply(CRUI::crComboBoxWidgetNode& node);
	virtual void apply(CRUI::crProgressWidgetNode& node);
	virtual void apply(CRUI::crGroupWidgetNode& node);
protected:
	//static CRCore::crStateSet *getProgramStateSet(const std::string& filename, bool addT, bool addB,CRCore::crStateSet *oldStateset);
	//static void loadCfgFile(const std::string &cfgFile, CRCore::crNode *newNode);
	CRCore::ref_ptr<CRCore::crDB> m_currentdb;
	rcfg::ConfigScript m_pagedLod_cfg_script;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_scenarioTab;
};
}
#endif
