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
#include <PieMenu.h>
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/EditorInterface.h>
#include <Utility/ECS/Include/World.h>

class Canvas3D : public QWidget
{
	Q_OBJECT
		
public:
	Canvas3D(QWidget* p_parent = 0);
	~Canvas3D();

	void CreateOpenGLContext();
	void SetEngineInterface(EngineInterface* p_engineInterface);
	void SetEditorInterface(EditorInterface* p_editorInterface);

protected:
	void wheelEvent(QWheelEvent* event);
	void keyPressEvent( QKeyEvent *k );
	void keyReleaseEvent( QKeyEvent *k );
	void resizeEvent( QResizeEvent * event );
	//void enterEvent( QEvent * event );
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent *event);
	
private:
	std::shared_ptr<SDL_Window> m_window;
	std::shared_ptr<PieMenu> m_pieMenu;
	EngineInterface* m_engineInterface;
	EditorInterface* m_editorInterface;
	ECS::World* m_world;

	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	
private slots:
	void pieMenuClickTest();
	void PieMenuAddComponent();
};