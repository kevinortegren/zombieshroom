#pragma once

#include <QWidget>
#include <QFileSystemModel>

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

private slots:
	void SearchLineChanged(const QString& p_val);
	void FolderSelected(const QModelIndex& p_val);
	void FileSelected(const QModelIndex& p_val);
	void NavigateBack();
	void NavigateForward();
};