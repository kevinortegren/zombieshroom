#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QBoxLayout>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtCore/QStringList>
#include <QtGui/QMouseEvent>
#include <Qtgui/QPixmap>
#include <Qtgui/QPainter>

#include "GeneratedFiles/ui_AbilityEditor.h"

class AbilityEditor : public QMainWindow
{
	Q_OBJECT

public:
	AbilityEditor(QWidget *parent = 0);
	~AbilityEditor();
	void Init();

	QStringList m_compNames;


private slots:
		void UpdatePropertyBrowser();
private:
	Ui::AbilityEditorClass ui;
	QtTreePropertyBrowser* m_propBrows;
	QLayout* m_mainLayout;
	QPainter* m_painter;
	QPixmap* m_pixmap;
	AbilityEditorNameSpace::OnCreate* m_onCreate;
	AbilityEditorNameSpace::OnCollide* m_onCollide;
	AbilityEditorNameSpace::OnDestroy* m_onDestroy;
	bool event(QEvent* event);
	bool eventFilter(QObject * p_object, QEvent * p_event);
	//void mousePressEvent(QMouseEvent* event);

};
