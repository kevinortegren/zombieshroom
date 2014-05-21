#include <RootTools/Treenity/Include/TextureDrop.h>

TextureDrop::TextureDrop(QWidget* p_parent)
	: QWidget(p_parent)
{
	ui.setupUi(this);
	ui.label_2->AddDropFilter(".dds");
}

TextureDrop::~TextureDrop()
{}
