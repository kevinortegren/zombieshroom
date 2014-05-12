#pragma once

#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDir>
#include <memory>
#include <SDL2/SDL.h>
#include <RootTools/Treenity/Include/EngineInterface.h>

class Canvas3D : public QWidget
{
	Q_OBJECT
		
public:
	Canvas3D(QWidget* p_parent = 0);
	~Canvas3D();

	void CreateOpenGLContext();
	void SetEngineInterface(EngineInterface* p_engineInterface);

protected:
	void wheelEvent(QWheelEvent* event);
	//void keyPressEvent( QKeyEvent *k );
	//void keyReleaseEvent( QKeyEvent *k );
	void resizeEvent( QResizeEvent * event );
	//void enterEvent( QEvent * event );
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	
private:
	std::shared_ptr<SDL_Window> m_window;
	EngineInterface* m_engineInterface;
};