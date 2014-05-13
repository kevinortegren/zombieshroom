#pragma once

#include <QWidget>
#include <QFileSystemModel>
#include <QMenu>

#include <RootTools/Treenity/GeneratedFiles/ui_AssetManager.h>

class AssetManagerWidget : public QWidget
{
	Q_OBJECT
public:
	AssetManagerWidget(QWidget* p_parent = 0);
	~AssetManagerWidget();
private:
	Ui::AssetManagerUi ui;

	QFileSystemModel* m_assetFolderModel;
	QFileSystemModel* m_assetFileModel;

	QMenu* m_fileContextMenu;
	QMenu* m_offsideContextMenu;

	QStringList m_currentFilter;

	void SetFolderSpecificFilters(const QString& p_folderName);

private slots:
	void SearchLineChanged(const QString& p_val);
	void FolderSelected(const QModelIndex& p_val);
	void FileSelected(const QModelIndex& p_val);
	void NavigateBack();
	void NavigateForward();
	void IconSizeChanged(int p_val);
	void TreeListContextMenu(const QPoint& p_val);
	void FolderLoaded(const QString & p_path);
	void CollapseAll();
	void ExpandAll();
};