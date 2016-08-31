#ifndef CRE_CRWIDGETNODE
#define CRE_CRWIDGETNODE 1

#include <CRCore/crWidgetNode.h>
#include <CRUI/crWidgetExtend.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crWidgetNode : public CRCore::crWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crCanvasNode : public CRCore::crCanvasNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crButtonWidgetNode : public CRUI::crButtonWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crScrollBarWidgetNode : public CRUI::crScrollBarWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crStaticTextWidgetNode : public CRUI::crStaticTextWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crHypertextWidgetNode : public CRUI::crHypertextWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crEditWidgetNode : public CRUI::crEditWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crListBoxWidgetNode : public CRUI::crListBoxWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crImageBoxWidgetNode : public CRUI::crImageBoxWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crCheckBoxWidgetNode : public CRUI::crCheckBoxWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crRadioWidgetNode : public CRUI::crRadioWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crRadioGroupWidgetNode : public CRUI::crRadioGroupWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crProgressWidgetNode : public CRUI::crProgressWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crSliderWidgetNode : public CRUI::crSliderWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crComboBoxWidgetNode : public CRUI::crComboBoxWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crChooseColorWidgetNode : public CRUI::crChooseColorWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crTableWidgetNode : public CRUI::crTableWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crListControlWidgetNode : public CRUI::crListControlWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crScissorWidgetNode : public CRUI::crScissorWidgetNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
