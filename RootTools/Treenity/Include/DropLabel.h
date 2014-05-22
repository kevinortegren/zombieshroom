#pragma once

#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDir>

class DropLabel : public QLabel
{
	Q_OBJECT
public:
	DropLabel(QWidget* p_parent = 0);
	~DropLabel();
	void AddDropFilter(const QString& p_filterSuffix);
protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);
private:
	QStringList m_filter;
signals:
	void dropped(const QString& p_path);

};