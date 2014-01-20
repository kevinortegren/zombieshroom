#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QBoxLayout>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtCore/QStringList>
#include <QtGui/QMouseEvent>
#include <Qtgui/QPixmap>
#include <Qtgui/QPainter>
#include "ScriptGenerator.h"
#include "Exporter.h"
#include "Importer.h"

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
		void ChangedTab();
		void GenerateScript();
		void SaveAs();
		void Save();
		void Load();
private:
	Ui::AbilityEditorClass ui;
	QtTreePropertyBrowser* m_propBrows;
	QtVariantPropertyManager* m_propMan;
	QLayout* m_mainLayout;
	QPainter* m_painter;
	QPixmap* m_pixmap;
	AbilityEditorNameSpace::OnCreate* m_onCreate;
	AbilityEditorNameSpace::OnCollide* m_onCollide;
	AbilityEditorNameSpace::OnDestroy* m_onDestroy;
	AbilityEditorNameSpace::ScriptGenerator* m_scriptGenerator;
	AbilityEditorNameSpace::Exporter* m_exporter;
	AbilityEditorNameSpace::Importer* m_importer;
	unsigned int m_LastSelectedTab;
	QString m_currentSavePath;
	bool event(QEvent* event);
	QTreeWidgetItem* m_LastSelectedItem;
	//void mousePressEvent(QMouseEvent* event);

};
