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

	void Init(Render::TextureSemantic::TextureSemantic p_texSem, const QString& p_textureName);

	void SetEngineInterface(EngineInterface* p_engineInterface);

	const QString& GetName();
	Render::TextureSemantic::TextureSemantic GetTextureSemantic();

	void SetTexture(Render::TextureInterface* p_texture);
	void Clear();

private:
	QPixmap GetPixmap(Render::TextureInterface* p_texture);
	QString m_name;
	EngineInterface* m_engineInterface;
	Ui::Form ui;

	//This describes which texture is associated with this texture drop
	Render::TextureSemantic::TextureSemantic m_textureSem;

private slots:
	void TextureDropped(const QString& p_textureName);

signals:
	void textureChanged(Render::TextureInterface* p_textureInterface, Render::TextureSemantic::TextureSemantic p_textureSem);

};