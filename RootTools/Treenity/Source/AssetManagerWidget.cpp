#include <RootTools/Treenity/Include/AssetManagerWidget.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootTools/Treenity/Include/Log.h>
#include <QDesktopServices>
extern RootEngine::GameSharedContext g_engineContext;

AssetManagerWidget::AssetManagerWidget( QWidget* p_parent /*= 0*/ )
	: QWidget(p_parent)
{
	ui.setupUi(this);

	connect(ui.lineEdit_assetSearch,			SIGNAL(textEdited(const QString &)),			this,				SLOT(SearchLineChanged(const QString&)));
	connect(ui.treeView_assetFileBrowser,		SIGNAL(clicked(const QModelIndex&)),			this,				SLOT(FolderSelected(const QModelIndex&)));
	connect(ui.listView_fileBrowser,			SIGNAL(doubleClicked(const QModelIndex&)),		this,				SLOT(FileSelected(const QModelIndex&)));
	connect(ui.pushButton_back,					SIGNAL(clicked()),								this,				SLOT(NavigateBack()));
	connect(ui.horizontalSlider_icon,			SIGNAL(valueChanged(int)),						this,				SLOT(IconSizeChanged(int)));

	//Create folder model for tree view
	m_assetFolderModel = new QFileSystemModel();
	m_assetFolderModel->setRootPath(QString::fromStdString(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/"));
	m_assetFolderModel->setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);

	//Set up tree view and add folder file model
	ui.treeView_assetFileBrowser->setModel(m_assetFolderModel);
	ui.treeView_assetFileBrowser->setColumnWidth(0, 150);
	ui.treeView_assetFileBrowser->hideColumn(1);
	ui.treeView_assetFileBrowser->hideColumn(2);
	ui.treeView_assetFileBrowser->hideColumn(3);
	ui.treeView_assetFileBrowser->setRootIndex(m_assetFolderModel->index(QString::fromStdString(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/")));

	//Set up list widget
	m_assetFileModel = new QFileSystemModel();
	m_assetFileModel->setRootPath(QString::fromStdString(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/"));
	//m_assetFileModel->setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);

	ui.listView_fileBrowser->setModel(m_assetFileModel);
	ui.listView_fileBrowser->setRootIndex(m_assetFileModel->index(QString::fromStdString(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/")));
}

AssetManagerWidget::~AssetManagerWidget()
{
	//Clean up
	delete m_assetFileModel;
	delete m_assetFolderModel;
}

/*		Blue print of Asset browser
________________________________________________________
|				|_______________Search bar______________|
|				|										|
|				|										|
|				|										|
|   Tree view	|			List view					|
|				|										|
|				|										|
|               |										|
|               |										|
________________________________________________________

*/
//When typing in the search box
void AssetManagerWidget::SearchLineChanged( const QString& p_val )
{
	//Set new filter when editing search bar. The filter will look for the input string in the file names and filter on that
	QString searchValue = "*" + p_val + "*.*";
	QStringList filters;
	filters << searchValue;
	m_assetFileModel->setNameFilters(filters);
	m_assetFileModel->setNameFilterDisables(false);
}

//Double click in tree view
void AssetManagerWidget::FolderSelected( const QModelIndex& p_val )
{
	QFileInfo fileInfo = m_assetFolderModel->fileInfo(p_val);
	m_assetFileModel->setRootPath(fileInfo.filePath());
	ui.listView_fileBrowser->setRootIndex(m_assetFileModel->index(fileInfo.filePath()));
	//Clear search bar when selecting a new folder. 2014-05-08: Only local search is available.
	ui.lineEdit_assetSearch->clear();
	SearchLineChanged("");
}

//Double click in list view
void AssetManagerWidget::FileSelected( const QModelIndex& p_val )
{
	QFileInfo fileInfo = m_assetFileModel->fileInfo(p_val);
	if(fileInfo.isDir()) //Doubleclicked a folder! Navigate YO!
	{
		m_assetFileModel->setRootPath(fileInfo.filePath());
		ui.listView_fileBrowser->setRootIndex(m_assetFileModel->index(fileInfo.filePath()));

		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, fileInfo.filePath().toStdString().c_str());
	}
	else if(fileInfo.suffix().compare("particle") == 0) //Doubleclicked a .particle-file. Opens up the particle editor!
	{
		//Start the Particle editor from the same folder. Alternative is to remove this if-statement and let Qt open the particle editor.
		std::string particleFileInfo = fileInfo.filePath().toStdString();
		//Enclose .exe-file path in quotes and enclose argument-path in quotes and then enclose the whole thing in quotes! Windows Magic right here :D
		std::string particleEditorPath = "\"\"" + g_engineContext.m_resourceManager->GetWorkingDirectory() + "ParticleEditor.exe" + "\" \"" + particleFileInfo + "\"\"";
		system(particleEditorPath.c_str());
		Log::Write("Particle editor started!");
	}
	else
	{
		//This opens a file with the default program. If no program is selected it will prompt the user to find a program. Very nice... very nice indeed!
		QString temp = "file:///" + fileInfo.filePath();
		QDesktopServices::openUrl(QUrl(temp, QUrl::TolerantMode)); //Don't know if QDesktopServices is included in our external libs. If this is causing compile problems, comment it out!
	}

}

//Navigate back in list view
void AssetManagerWidget::NavigateBack()
{
	
	QDir temp(m_assetFileModel->rootPath());
	//Check if we back up too much
	if(temp.dirName() == "Assets")
		return;

	if(!temp.cdUp())
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Navigating back is not really a good idea any more...");
	}
	else
	{
		//New backed-up path!
		QString newPath = temp.absolutePath();

		m_assetFileModel->setRootPath(newPath);
		ui.listView_fileBrowser->setRootIndex(m_assetFileModel->index(newPath));
	}
}

//Navigate forward in list view
void AssetManagerWidget::NavigateForward()
{

}

void AssetManagerWidget::IconSizeChanged( int p_val )
{
	//In slider is all the way to the left we switch to list mode!
	if(p_val == ui.horizontalSlider_icon->minimum())
	{
		ui.listView_fileBrowser->setViewMode(QListView::ListMode);
		ui.listView_fileBrowser->setIconSize(QSize(15, 15));
		ui.listView_fileBrowser->setGridSize(QSize(15, 15));	
	}
	else
	{
		ui.listView_fileBrowser->setViewMode(QListView::IconMode);
		ui.listView_fileBrowser->setIconSize(QSize(p_val, p_val));	
		ui.listView_fileBrowser->setGridSize(QSize(p_val + 10, p_val + 15));	
	}
	
}

