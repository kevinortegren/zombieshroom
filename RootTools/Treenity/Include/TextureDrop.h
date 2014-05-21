#pragma once

#include <qwidget.h>
#include <RootTools/Treenity/GeneratedFiles/ui_TextureDropWidget.h>

class TextureDrop : public QWidget
{
	Q_OBJECT
public:
	TextureDrop(QWidget* p_parent = 0);
	virtual ~TextureDrop();
private:
	Ui::Form ui;
};