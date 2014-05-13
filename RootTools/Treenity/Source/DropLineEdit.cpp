#include <RootTools/Treenity/Include/DropLineEdit.h>
#include <RootTools/Treenity/Include/Utils.h>
DropLineEdit::DropLineEdit( QWidget* p_parent /*= 0*/ )
{
	setAcceptDrops(true);
}

DropLineEdit::~DropLineEdit()
{

}

void DropLineEdit::dragEnterEvent( QDragEnterEvent* event )
{
	if (event->mimeData()->hasText() && event->mimeData()->text().contains("Assets"))
	{
		bool filterExists = false;

		QFileInfo fileInfo(event->mimeData()->text());

		for(QString str : m_filter)
		{
			if(fileInfo.suffix() == str)
			{
				filterExists = true;
				break;
			}
		}

		if(filterExists)
		{
			setStyleSheet("background-color: orange;");
			
			event->acceptProposedAction();
		}
		else
			event->ignore();
	}
}

void DropLineEdit::dropEvent( QDropEvent* event )
{
	QFileInfo fileInfo(event->mimeData()->text());

	//A little sting manuipulation to remove the "file:///"-prefix on the file path if dragged from asset browser
	QString filePath;
	if (fileInfo.filePath().startsWith(QLatin1String("file:///")))
	{
		filePath =  fileInfo.filePath().mid(8);
	}
	else
	{
		filePath =  fileInfo.filePath();	
	}

	QDir dirInfo(filePath);
	//Check if there is a name to begin with
	if(fileInfo.baseName() != 0)
	{
		QStringList dirPaths;
		//Collect all dirs up to Assets
		while(dirInfo.dirName() != "Assets")
		{
			dirPaths << dirInfo.dirName();

			if(!dirInfo.cdUp())
				break;
		}

		//Remove and store the file name without suffix
		dirPaths.pop_front();
		//Remove category folder from path list
		dirPaths.pop_back();

		QString relativePath;

		//Construct the relative path
		while (!dirPaths.isEmpty())
		{
			relativePath += dirPaths.takeLast() + "/";
		}

		//Finally add file name to path
		relativePath += fileInfo.completeBaseName();

		//Set text in QLineEdit and emit signal
		setText(relativePath);
		emit editingFinished();
	}

	setStyleSheet("");

}

void DropLineEdit::AddDropFilter( const QString& p_filterSuffix )
{
	m_filter << p_filterSuffix;
}

void DropLineEdit::dragLeaveEvent( QDragLeaveEvent* event )
{
	setStyleSheet("");
}


