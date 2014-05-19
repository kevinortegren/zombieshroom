#pragma once

#include <QFileIconProvider>
#include <QIcon>
#include <QImageReader>
#include <QImage>

class CustomFileIconProvider : public QFileIconProvider
{
public:
	CustomFileIconProvider();
	~CustomFileIconProvider();

	QIcon icon(IconType type) const;
	QIcon icon(const QFileInfo & fileInfo) const;

	QString type(const QFileInfo & info) const;

private:

	QIcon LoadIcon(const QString p_filepath ) const;
};