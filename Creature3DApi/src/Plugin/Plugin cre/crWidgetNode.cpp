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
#include <Plugin cre/crException.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crWidgetNode.h>
#include <Plugin cre/crMatrixTransform.h>
using namespace cre;

//////////////////////////////////////////////////////////////////////////
//
//crWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crWidgetNode::write(crDataOutputStream* out){
    out->writeInt(CRWIDGETNODE);
    CRCore::crMatrixTransform* node = dynamic_cast<CRCore::crMatrixTransform*>(this);
    if(node){
		CRCore::crVector3 pos = node->getTrans();
		pos[2] = 0.0f;
		node->setPosition(pos);
        static_cast<cre::crMatrixTransform*>(node)->write(out);
		//out->writeBool(m_enable);
		//out->writeBool(m_canFocus);
		//out->writeBool(m_canCaptureMouse);
		//out->writeBool(m_canCaptureInput);
		//out->writeBool(m_matrixEnable);
    }
    else
        throw crException("crWidgetNode::write(): Could not cast this crWidgetNode to an crMatrixTransform.");
}

void crWidgetNode::read(crDataInputStream* in){
    int id = in->peekInt();
    if(id == CRWIDGETNODE){
        id = in->readInt();

        CRCore::crMatrixTransform* node = dynamic_cast<CRCore::crMatrixTransform*>(this);
        if(node){
            ((cre::crMatrixTransform*)(node))->read(in);
        }
        else
            throw crException("crWidgetNode::read(): Could not cast this crWidgetNode to an crMatrixTransform.");

		//m_enable = in->readBool();
		//m_canFocus = in->readBool();
		//m_canCaptureMouse = in->readBool();
		//m_canCaptureInput = in->readBool();
		//m_matrixEnable = in->readBool();
    }
    else{
        throw crException("crWidgetNode::read(): Expected identification");
    }
}
//////////////////////////////////////////////////////////////////////////
//
//crCanvasNode
//
//////////////////////////////////////////////////////////////////////////
void crCanvasNode::write(crDataOutputStream* out){
	out->writeInt(CRCANVASNODE);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);

	}
	else
		throw crException("crCanvasNode::write(): Could not cast this crCanvasNode to an crWidgetNode.");
}

void crCanvasNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRCANVASNODE){
		id = in->readInt();

		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
		}
		else
			throw crException("crCanvasNode::read(): Could not cast this crCanvasNode to an crWidgetNode.");


	}
	else{
		throw crException("crCanvasNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crButtonWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crButtonWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRBUTTONWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
	}
	else
		throw crException("crButtonWidgetNode::write(): Could not cast this crButtonWidgetNode to an crWidgetNode.");
}

void crButtonWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRBUTTONWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
		}
		else
			throw crException("crButtonWidgetNode::read(): Could not cast this crButtonWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crButtonWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crScrollBarWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crScrollBarWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRSCROLLBARWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);

		//out->writeVec2(m_range);
		//out->writeFloat(m_lineValue);
		//out->writeFloat(m_pageValue);
		//out->writeUChar(m_scrollBarType);
	}
	else
		throw crException("crScrollBarWidgetNode::write(): Could not cast this crScrollBarWidgetNode to an crWidgetNode.");
}

void crScrollBarWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRSCROLLBARWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
			
			//m_range = in->readVec2();
			//m_lineValue = in->readFloat();
			//m_pageValue = in->readFloat();
			//m_scrollBarType = (crScrollBarWidgetNode::ScrollBarType)(in->readUChar());
		}
		else
			throw crException("crScrollBarWidgetNode::read(): Could not cast this crScrollBarWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crScrollBarWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crStaticTextWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crStaticTextWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRSTATICTEXTWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
		//out->writeFloat(m_characterSize);
		//out->writeString(m_font);
		//out->writeVec4(m_color);
		//out->writeUChar(m_alignment);
		//out->writeString(m_vScrollBarName);
		//out->writeVec4(m_rectOffset);
	}
	else
		throw crException("crStaticTextWidgetNode::write(): Could not cast this crStaticTextWidgetNode to an crWidgetNode.");
}

void crStaticTextWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRSTATICTEXTWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
			
			//m_characterSize = in->readFloat();
			//m_font = in->readString();
			//m_color = in->readVec4();
			//m_alignment = in->readUChar();
			//m_vScrollBarName = in->readString();
			//m_rectOffset = in->readVec4();
		}
		else
			throw crException("crStaticTextWidgetNode::read(): Could not cast this crStaticTextWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crStaticTextWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crHypertextWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crHypertextWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRHYPERTEXTWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
		//out->writeFloat(m_characterSize);
		//out->writeString(m_font);
		//out->writeVec4(m_color);
		//out->writeUChar(m_alignment);
		//out->writeString(m_vScrollBarName);
		//out->writeVec4(m_rectOffset);
	}
	else
		throw crException("crHypertextWidgetNode::write(): Could not cast this crHypertextWidgetNode to an crWidgetNode.");
}

void crHypertextWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRHYPERTEXTWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);

			//m_characterSize = in->readFloat();
			//m_font = in->readString();
			//m_color = in->readVec4();
			//m_alignment = in->readUChar();
			//m_vScrollBarName = in->readString();
			//m_rectOffset = in->readVec4();
		}
		else
			throw crException("crHypertextWidgetNode::read(): Could not cast this crHypertextWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crHypertextWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crEditWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crEditWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CREDITWIDGET);
	CRUI::crStaticTextWidgetNode* node = dynamic_cast<CRUI::crStaticTextWidgetNode*>(this);
	if(node){
		static_cast<cre::crStaticTextWidgetNode*>(node)->write(out);
		//out->writeBool(m_multiLine);
		//out->writeBool(m_password);
	}
	else
		throw crException("crEditWidgetNode::write(): Could not cast this crEditWidgetNode to an crWidgetNode.");
}

void crEditWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREDITWIDGET){
		id = in->readInt();
		CRUI::crStaticTextWidgetNode* node = dynamic_cast<CRUI::crStaticTextWidgetNode*>(this);
		if(node){
			((cre::crStaticTextWidgetNode*)(node))->read(in);

			//m_multiLine = in->readBool();
			//m_password = in->readBool();
		}
		else
			throw crException("crEditWidgetNode::read(): Could not cast this crEditWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crEditWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crListBoxWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crListBoxWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRELISTBOXWIDGET);
	CRUI::crStaticTextWidgetNode* node = dynamic_cast<CRUI::crStaticTextWidgetNode*>(this);
	if(node){
		static_cast<cre::crStaticTextWidgetNode*>(node)->write(out);
		//out->writeInt(m_dataList.size());
		//for (DataList::iterator itr = m_dataList.begin();
		//	 itr != m_dataList.end();
		//	 ++itr)
		//{
		//	out->writeString(*itr);
		//}
	}
	else
		throw crException("crEditWidgetNode::write(): Could not cast this crEditWidgetNode to an crWidgetNode.");
}

void crListBoxWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRELISTBOXWIDGET){
		id = in->readInt();
		CRUI::crStaticTextWidgetNode* node = dynamic_cast<CRUI::crStaticTextWidgetNode*>(this);
		if(node){
			((cre::crStaticTextWidgetNode*)(node))->read(in);

			//int count = in->readInt();
			//for(int i = 0; i<count; ++i)
			//{
			//	addData(in->readString());
			//}
		}
		else
			throw crException("crEditWidgetNode::read(): Could not cast this crEditWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crEditWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crImageBoxWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crImageBoxWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CREIMAGEBOXWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
		//out->writeString(m_imageFile);
		//out->writeVec4(m_rectOffset);
		//out->writeBool(m_canDragImage);
	}
	else
		throw crException("crImageBoxWidgetNode::write(): Could not cast this crImageBoxWidgetNode to an crWidgetNode.");
}

void crImageBoxWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREIMAGEBOXWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
			//m_imageFile = in->readString();
			//m_rectOffset = in->readVec4();
			//m_canDragImage = in->readBool();
		}
		else
			throw crException("crImageBoxWidgetNode::read(): Could not cast this crImageBoxWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crImageBoxWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crCheckBoxWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crCheckBoxWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRECHECKBOXWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
	}
	else
		throw crException("crCheckBoxWidgetNode::write(): Could not cast this crCheckBoxWidgetNode to an crWidgetNode.");
}

void crCheckBoxWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRECHECKBOXWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
		}
		else
			throw crException("crCheckBoxWidgetNode::read(): Could not cast this crCheckBoxWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crCheckBoxWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crRadioWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crRadioWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRERADIOWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
	}
	else
		throw crException("crRadioWidgetNode::write(): Could not cast this crRadioWidgetNode to an crWidgetNode.");
}

void crRadioWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRERADIOWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
		}
		else
			throw crException("crRadioWidgetNode::read(): Could not cast this crRadioWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crRadioWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crRadioGroupWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crRadioGroupWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRERADIOGROUPWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
		//out->writeInt(m_radioGroup.size());
		//for( RadioGroup::iterator itr = m_radioGroup.begin();
		//	itr != m_radioGroup.end();
		//	++itr )
		//{
		//	out->writeString(itr->first);
		//}
	}
	else
		throw crException("crRadioGroupWidgetNode::write(): Could not cast this crRadioGroupWidgetNode to an crWidgetNode.");
}

void crRadioGroupWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRERADIOGROUPWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
			//int count = in->readInt();
			//std::string str;
			//for (int i = 0; i<count; i++)
			//{
			//	str = in->readString();
			//	m_radioGroup.push_back(RadioNamePair(str,0));
			//}
		}
		else
			throw crException("crRadioGroupWidgetNode::read(): Could not cast this crRadioGroupWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crRadioGroupWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crProgressWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crProgressWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CREPROGRESSWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
		
	}
	else
		throw crException("crProgressWidgetNode::write(): Could not cast this crProgressWidgetNode to an crWidgetNode.");
}

void crProgressWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREPROGRESSWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);

		}
		else
			throw crException("crProgressWidgetNode::read(): Could not cast this crProgressWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crProgressWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crSliderWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crSliderWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRESLIDERWIDGET);
	CRUI::crProgressWidgetNode* node = dynamic_cast<CRUI::crProgressWidgetNode*>(this);
	if(node){
		static_cast<cre::crProgressWidgetNode*>(node)->write(out);

	}
	else
		throw crException("crSliderWidgetNode::write(): Could not cast this crSliderWidgetNode to an crWidgetNode.");
}

void crSliderWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRESLIDERWIDGET){
		id = in->readInt();
		CRUI::crProgressWidgetNode* node = dynamic_cast<CRUI::crProgressWidgetNode*>(this);
		if(node){
			((cre::crProgressWidgetNode*)(node))->read(in);

		}
		else
			throw crException("crSliderWidgetNode::read(): Could not cast this crSliderWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crSliderWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crComboBoxWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crComboBoxWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRECOMBOBOXWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
		//out->writeString(m_comboTextBoxName);
		//out->writeString(m_comboButtonName);
		//out->writeString(m_comboListBoxName);
	}
	else
		throw crException("crComboBoxWidgetNode::write(): Could not cast this crComboBoxWidgetNode to an crWidgetNode.");
}

void crComboBoxWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRECOMBOBOXWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
			//m_comboTextBoxName = in->readString();
			//m_comboButtonName = in->readString();
			//m_comboListBoxName = in->readString();
		}
		else
			throw crException("crComboBoxWidgetNode::read(): Could not cast this crComboBoxWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crComboBoxWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crChooseColorWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crChooseColorWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRECHOOSECOLORWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
	}
	else
		throw crException("crChooseColorWidgetNode::write(): Could not cast this crChooseColorWidgetNode to an crWidgetNode.");
}

void crChooseColorWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRECHOOSECOLORWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
		}
		else
			throw crException("crChooseColorWidgetNode::read(): Could not cast this crChooseColorWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crChooseColorWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crTableWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crTableWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRETABLEWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
		//out->writeInt(m_listGroup.size());
		//for( ListGroup::iterator itr = m_listGroup.begin();
		//	itr != m_listGroup.end();
		//	++itr )
		//{
		//	out->writeString(itr->first);
		//}
	}
	else
		throw crException("crTableWidgetNode::write(): Could not cast this crTableWidgetNode to an crWidgetNode.");
}

void crTableWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRETABLEWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
			//int count = in->readInt();
			//std::string str;
			//for (int i = 0; i<count; i++)
			//{
			//	str = in->readString();
			//	m_listGroup.push_back(ListNamePair(str,0));
			//}
		}
		else
			throw crException("crTableWidgetNode::read(): Could not cast this crTableWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crTableWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crListControlWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crListControlWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRELISTCONTROLWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
	}
	else
		throw crException("crListControlWidgetNode::write(): Could not cast this crListControlWidgetNode to an crWidgetNode.");
}

void crListControlWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRELISTCONTROLWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
		}
		else
			throw crException("crListControlWidgetNode::read(): Could not cast this crListControlWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crListControlWidgetNode::read(): Expected identification");
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crScissorWidgetNode
//
//////////////////////////////////////////////////////////////////////////
void crScissorWidgetNode::write(crDataOutputStream* out){
	out->writeInt(CRESCISSORWIDGET);
	CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
	if(node){
		static_cast<cre::crWidgetNode*>(node)->write(out);
	}
	else
		throw crException("crScissorWidgetNode::write(): Could not cast this crScissorWidgetNode to an crWidgetNode.");
}

void crScissorWidgetNode::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRESCISSORWIDGET){
		id = in->readInt();
		CRCore::crWidgetNode* node = dynamic_cast<CRCore::crWidgetNode*>(this);
		if(node){
			((cre::crWidgetNode*)(node))->read(in);
		}
		else
			throw crException("crScissorWidgetNode::read(): Could not cast this crScissorWidgetNode to an crWidgetNode.");
	}
	else{
		throw crException("crScissorWidgetNode::read(): Expected identification");
	}
}