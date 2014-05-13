#pragma once

#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDir>

class DropLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	DropLineEdit(QWidget* p_parent = 0);
	~DropLineEdit();
	void AddDropFilter(const QString& p_filterSuffix);
protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);
private:
	QStringList m_filter;
};