#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFileSystemModel>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtCore/QStringList>
#include <QtGui/QMouseEvent>
#include <Qtgui/QPixmap>
#include <Qtgui/QPainter>
#include "ScriptGenerator.h"
#include "Exporter.h"
#include "Importer.h"
//#include "Entity.h"

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
		//void ChangedTab();
		void GenerateScript();
		void SaveAs();
		void Save();
		void Load();
		void FileViewDrag(const QModelIndex& p_modelIndex);
		//void AddNewEntity();
		void BrowseName();
		void ChangeAbilityName();
		void SetCooldown(double p_cooldown);
private:
	Ui::AbilityEditorClass ui;
	QtTreePropertyBrowser* m_propBrows;
	QtVariantPropertyManager* m_propMan;
	QLayout* m_mainLayout;
	QPainter* m_painter;
	QPixmap* m_pixmap;
	AbilityEditorNameSpace::Entity* m_entity;
	AbilityEditorNameSpace::OnCreate* m_onCreate;
	AbilityEditorNameSpace::OnCollide* m_onCollide;
	AbilityEditorNameSpace::OnDestroy* m_onDestroy;
	AbilityEditorNameSpace::ScriptGenerator* m_scriptGenerator;
	AbilityEditorNameSpace::Exporter* m_exporter;
	AbilityEditorNameSpace::Importer* m_importer;
	unsigned int m_LastSelectedTab;
	QString m_currentSavePath;
	bool event(QEvent* event);
	QListWidgetItem* m_LastSelectedItem;
	QFileSystemModel* m_fileViewModel;

	std::vector<AbilityEditorNameSpace::Condition*> m_conditions;

	//void mousePressEvent(QMouseEvent* event);

};
