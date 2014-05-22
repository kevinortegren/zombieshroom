#pragma once

#include <qwidget.h>
#include <RootTools/Treenity/GeneratedFiles/ui_TextureDropWidget.h>
#include <RootTools/Treenity/Include/EngineInterface.h>

class TextureDrop : public QWidget
{
	Q_OBJECT
public:
	TextureDrop(QWidget* p_parent = 0);
	virtual ~TextureDrop();

	void SetEngineInterface(EngineInterface* p_engineInterface);
	void SetName(const QString& p_textureName);
	void SetTexture(Render::TextureInterface* p_texture);
	void Clear();

private:
	QPixmap GetPixmap(Render::TextureInterface* p_texture);

	EngineInterface* m_engineInterface;
	Ui::Form ui;

private slots:
	void TextureDropped(const QString& p_textureName);

signals:
	void textureChanged(Render::TextureInterface* p_textureInterface);

};