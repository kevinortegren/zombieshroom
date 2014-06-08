#include <RootTools/Treenity/Include/TextureDrop.h>
#include <QPixmapCache>
#include <QImageReader>

TextureDrop::TextureDrop(QWidget* p_parent)
	: QWidget(p_parent)
{
	ui.setupUi(this);
	ui.label_2->AddDropFilter("dds");

	connect(ui.label_2, SIGNAL(dropped(const QString&)), this, SLOT(TextureDropped(const QString&)));
}

TextureDrop::~TextureDrop()
{}

void TextureDrop::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;
}

void TextureDrop::SetTexture(Render::TextureInterface* p_texture)
{
	if(p_texture != nullptr)
	{
		QPixmap pm = GetPixmap(p_texture);			
		ui.label_2->setPixmap(pm.scaled(ui.label_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
}

void TextureDrop::Clear()
{
	ui.label_2->setPixmap(QPixmap());
	ui.label_2->setText("No Texture");

}

QPixmap TextureDrop::GetPixmap(Render::TextureInterface* p_texture)
{
	std::string fullTexturePath = m_engineInterface->GetContentPath() + "Assets\\Textures\\" +  m_engineInterface->GetTextureName(p_texture) + ".dds";
	std::replace( fullTexturePath.begin(), fullTexturePath.end(), '\\', '/');

	int limit = QPixmapCache::cacheLimit();
	QPixmap pm;
	if (!QPixmapCache::find(QString::fromStdString(fullTexturePath), &pm))
	{
		QImageReader reader(QString::fromStdString(fullTexturePath));

		if(!reader.canRead())
		{
			return pm;
		}

		QImage image = reader.read();
		if(image.isNull())
		{
			return pm;
		}

		pm = QPixmap::fromImage(image);

		QPixmapCache::insert(QString::fromStdString(fullTexturePath), pm);
	}

	return pm;
}

void TextureDrop::TextureDropped(const QString& p_textureName)
{
	Render::TextureInterface* texture = m_engineInterface->GetTexture(p_textureName.toStdString());

	SetTexture(texture);

	emit textureChanged(texture, m_textureSem);
}

const QString& TextureDrop::GetName()
{
	return m_name;
}

Render::TextureSemantic::TextureSemantic TextureDrop::GetTextureSemantic()
{
	return m_textureSem;
}

void TextureDrop::Init( Render::TextureSemantic::TextureSemantic p_texSem, const QString& p_textureName )
{
	m_textureSem = p_texSem;
	ui.label->setText(p_textureName);
	m_name = p_textureName;
}
