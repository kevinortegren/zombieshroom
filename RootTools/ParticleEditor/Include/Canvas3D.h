#pragma once

#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include <memory>
#include <SDL2/SDL.h>

class Canvas3D : public QWidget
{
	Q_OBJECT
		
public:
	Canvas3D(QWidget* p_parent = 0);
	~Canvas3D();

protected:
	void wheelEvent(QWheelEvent* event);
	void keyPressEvent( QKeyEvent *k );
	void keyReleaseEvent( QKeyEvent *k );

	
private:
	std::shared_ptr<SDL_Window> m_window;

};