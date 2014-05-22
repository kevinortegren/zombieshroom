#include <RootTools/Treenity/Include/CustomFileIconProvider.h>
#include <RootTools/Treenity/Include/Utils.h>
#include <QPixmapCache>

CustomFileIconProvider::CustomFileIconProvider()
	: QFileIconProvider()
{
	QPixmapCache::setCacheLimit(131072*4);
}

CustomFileIconProvider::~CustomFileIconProvider()
{

}

QIcon CustomFileIconProvider::icon( IconType type ) const
{
	return QFileIconProvider::icon(type);
}

QIcon CustomFileIconProvider::icon( const QFileInfo & fileInfo ) const
{
	if(fileInfo.isFile())
	{
		if(fileInfo.suffix() == "world")
		{
			//Level file selected
			QString image = fileInfo.dir().path() + "/" + fileInfo.completeBaseName() + ".png";
			QFileInfo file(image);
			if(file.exists())
			{
				return LoadIcon(image);
			}
			else
			{
				//If .png is not available, check if .jpg is
				QString image = fileInfo.dir().path() + "/" + fileInfo.completeBaseName() + ".jpg";
				QFileInfo fileJPG(image);
				if(fileJPG.exists())
				{
					return LoadIcon(image);
				}
				else
				{
					QString imgPath("Resources/nopreview.png");
					return LoadIcon(imgPath);
				}
			}

		}
		else if(fileInfo.suffix() == "png" || fileInfo.suffix() == "jpg" || fileInfo.suffix() == "dds")
		{
			//Load preview image on a separate thread to avoid loading-locks (Works like a charm!)
			return LoadIcon(fileInfo.filePath());
		}
		else if(fileInfo.suffix() == "js")
		{
			QString imgPath("Resources/javascripticon.png");
			return LoadIcon(imgPath);
		}
		else
		{
			QString imgPath("Resources/genericicon.png");
			return LoadIcon(imgPath);
		}
	}

	return QFileIconProvider::icon(fileInfo);
}

QString CustomFileIconProvider::type( const QFileInfo & info ) const
{
	return QFileIconProvider::type(info);
}

QIcon CustomFileIconProvider::LoadIcon(const QString p_filepath ) const
{
	int limit = QPixmapCache::cacheLimit();
	QPixmap pm;
	if (!QPixmapCache::find(p_filepath, &pm))
	{
		QImageReader reader(p_filepath);

		if(!reader.canRead())
		{
			if (!QPixmapCache::find("Resources/genericicon.png", &pm)) {
				pm.load("Resources/genericicon.png");
				QPixmapCache::insert("Resources/genericicon.png", pm);
			}
			QIcon newIcon(pm);
			return newIcon;
		}

		QImage image = reader.read();
		if(image.isNull())
		{
			if (!QPixmapCache::find("Resources/genericicon.png", &pm)) {
				pm.load("Resources/genericicon.png");
				QPixmapCache::insert("Resources/genericicon.png", pm);
			}
			QIcon newIcon(pm);
			return newIcon;
		}

		pm = QPixmap::fromImage(image.scaled(QSize(200,200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		QPixmapCache::insert(p_filepath, pm);

		QIcon newIcon(pm);
		return newIcon;
	}

	QIcon newIcon(pm);
	return newIcon;
}