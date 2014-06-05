#include "Canvas3D.h"
#include <iostream>

#include <SDL2/SDL_main.h>
#include <SDL2/SDL_syswm.h> 
#include <SDL2/SDL_hints.h>

#include <RootTools/Treenity/Include/KeyHelper.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/InputManager/Include/InputManager.h>

#include <RootSystems/Include/Transform.h>

#include <RootTools/Treenity/Include/Utils.h>


extern RootEngine::GameSharedContext g_engineContext;

Canvas3D::Canvas3D( QWidget* p_parent /*= 0*/ ) : QWidget(p_parent)
{
	//Flicker fix
	setAttribute(Qt::WA_PaintOnScreen);
	setUpdatesEnabled(false);
	setAcceptDrops(true);
	//Standard setup
	//setGeometry(QRect(2, 28, 1173, 899));
	setFocusPolicy(Qt::ClickFocus);
	show();
	setEnabled(true);
	setMouseTracking(true);

		//Create temporary SDL window to set up correct pixel format and SDL_WINDOW_OPENGL flag
	SDL_Window* sdlwindow = SDL_CreateWindow(
		"Root Particle",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width(),
		height(),
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

	std::cout << "Width: " << width() << " Height: " << height() << std::endl;
	std::cout << "G Width: " << geometry().width() << "G Height: " << geometry().height() << std::endl;

	//Format string with adress to temporary SDL window
	//char buffer [10];
	//sprintf_s(buffer, "%p", sdlwindow);
	//Set SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT hint with adresst to temporary window
	//SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, buffer );

	//Get Qt WId from the frame wherer we want to render. Cast to HWND
	HWND qtWindowHandle = (HWND)winId();
	//SDL_CreateWindowFrom takes an already created native window handle and builds an SDL_Window for us
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindowFrom((void*)qtWindowHandle), SDL_DestroyWindow);
	
	SDL_DestroyWindow(sdlwindow);

	if (m_window == nullptr) 
	{
		std::cout << SDL_GetError() << std::endl;
		throw std::runtime_error("Failed to create window");
	}

	m_pieMenu = std::shared_ptr<PieMenu>(new PieMenu(nullptr));
	
	//Connect pie menu buttons to methods
	PiePiece* tempPiece;

	tempPiece = m_pieMenu->addPiece("Resources/greenplus.png", "Create Entity");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton1()));
	tempPiece = m_pieMenu->addPiece("Resources/3dobjects.png", "Add Renderable");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton2()));
	tempPiece = m_pieMenu->addPiece("Resources/physics.png", "Add Physics");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton3()));
	tempPiece = m_pieMenu->addPiece("Resources/script.png", "Add Script");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton4()));
	tempPiece = m_pieMenu->addPiece("Resources/watercollider.png", "Add water collider");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton5()));
	tempPiece = m_pieMenu->addPiece("Resources/particleicon.png", "Add Particle");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton6()));
	tempPiece = m_pieMenu->addPiece("Resources/redminus.png", "Delete Entity");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton7()));
	tempPiece = m_pieMenu->addPiece("Resources/duplicate.png", "Duplicate");
	connect(tempPiece, SIGNAL(clicked()), this, SLOT(PieButton8()));
}

Canvas3D::~Canvas3D()
{

}

void Canvas3D::resizeEvent( QResizeEvent * event)
{
	if(g_engineContext.m_renderer == nullptr)
	{
		SDL_SetWindowSize(m_window.get(), event->size().width(), event->size().height());
	}
	else
	{
		g_engineContext.m_renderer->SetResolution(false, event->size().width(), event->size().height());
	}
	

	//std::cout << "Resize Width: " << event->size().width() << "Resize Height: " << event->size().height() << std::endl;
}

void Canvas3D::CreateOpenGLContext() //Horrible name
{	
	// Setup the SDL context
	g_engineContext.m_renderer->SetupSDLContext(m_window.get());
	g_engineContext.m_inputSys->SetWindow(m_window.get());
}

void Canvas3D::wheelEvent(QWheelEvent* event)
{
	SDL_Event scrollEvent;
	scrollEvent.type = SDL_MOUSEWHEEL;
	scrollEvent.wheel.y = event->delta() / 100;
	SDL_PushEvent(&scrollEvent);
}

void Canvas3D::mousePressEvent( QMouseEvent* event )
{

	if (event->button() == Qt::RightButton && !(event->modifiers() & Qt::AltModifier)) 
	{
		m_pieMenu->showMenu();
	}

	QWidget::mousePressEvent(event);
}

void Canvas3D::mouseReleaseEvent( QMouseEvent *event )
{
	if (m_pieMenu->canSee())
		m_pieMenu->closeMenu();

	QWidget::mouseReleaseEvent(event);
}


void Canvas3D::dragEnterEvent( QDragEnterEvent *event )
{
	QFileInfo fileInfo(event->mimeData()->text());

	if (event->mimeData()->hasText())
	{
		if(fileInfo.suffix() == "world")
		{
			event->acceptProposedAction();
		}
		else
		{
			event->ignore();
		}
		
	}
}

void Canvas3D::keyPressEvent( QKeyEvent *k )
{
	SDL_Event e;
	e.type = SDL_KEYDOWN;
	e.key.keysym.scancode = GetScanCodeFromQtKey( k->key() );
	e.key.repeat = k->isAutoRepeat();
	SDL_PushEvent(&e);

	QWidget::keyPressEvent(k);
}

void Canvas3D::keyReleaseEvent( QKeyEvent *k )
{
	if (!k->isAutoRepeat())
	{
		SDL_Event e;
		e.type = SDL_KEYUP;
		e.key.keysym.scancode = GetScanCodeFromQtKey( k->key() );
		SDL_PushEvent(&e);

		QWidget::keyReleaseEvent(k);
	}
}

void Canvas3D::dropEvent( QDropEvent *event )
{	
	QFileInfo fileInfo(event->mimeData()->text());

	//if statements with logic for dropping specific files
	if(fileInfo.suffix() == "world")
	{
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
		
		//Load new scene!
		m_engineInterface->LoadScene(filePath);
	}
}

void Canvas3D::focusInEvent(QFocusEvent* event)
{
	if (m_engineInterface->GetMode() == EditorMode::GAME)
	{
		//g_engineContext.m_inputSys->LockInput(true);
		g_engineContext.m_inputSys->LockMouseToCenter(true);
	}
}

void Canvas3D::focusOutEvent(QFocusEvent* event)
{
	if (m_engineInterface->GetMode() == EditorMode::GAME)
	{
		//g_engineContext.m_inputSys->LockInput(false);
		g_engineContext.m_inputSys->LockMouseToCenter(false);
	}
}

void Canvas3D::SetEngineInterface( EngineInterface* p_engineInterface )
{
	m_engineInterface = p_engineInterface;
}

void Canvas3D::SetEditorInterface( EditorInterface* p_editorInterface )
{
	m_editorInterface = p_editorInterface;
}

void Canvas3D::PieButton1()
{
	ECS::Entity* entity = m_engineInterface->CreateEntity();
	m_engineInterface->SetPosition(entity, m_engineInterface->GetPosition(m_engineInterface->GetEntityByTag("AimingDevice")));
}

void Canvas3D::PieButton2()
{
	if(m_editorInterface->GetSelection().size() > 0)
	{
		for(ECS::Entity* ent : m_editorInterface->GetSelection())
		{
			m_engineInterface->AddRenderable(ent);
		}
	}
}

void Canvas3D::PieButton3()
{
	if(m_editorInterface->GetSelection().size() > 0)
	{
		for(ECS::Entity* ent : m_editorInterface->GetSelection())
		{
			m_engineInterface->AddPhysics(ent, true);
		}
	}
}

void Canvas3D::PieButton4()
{
	if(m_editorInterface->GetSelection().size() > 0)
	{
		for(ECS::Entity* ent : m_editorInterface->GetSelection())
		{
			m_engineInterface->AddScript(ent);
		}
	}
}

void Canvas3D::PieButton5()
{
	if(m_editorInterface->GetSelection().size() > 0)
	{
		for(ECS::Entity* ent : m_editorInterface->GetSelection())
		{
			m_engineInterface->AddWaterCollider(ent);
		}
	}
}

void Canvas3D::PieButton6()
{
	if(m_editorInterface->GetSelection().size() > 0)
	{
		for(ECS::Entity* ent : m_editorInterface->GetSelection())
		{
			m_engineInterface->AddParticle(ent);
		}
	}
}

void Canvas3D::PieButton7()
{
	if(m_editorInterface->GetSelection().size() > 0)
	{
		for(ECS::Entity* ent : m_editorInterface->GetSelection())
		{
			m_engineInterface->DeleteEntity(ent);
		}
	}
}

void Canvas3D::PieButton8()
{
	if(m_editorInterface->GetSelection().size() > 0)
	{
		for(ECS::Entity* ent : m_editorInterface->GetSelection())
		{
			m_engineInterface->DuplicateEntity(ent);
		}
	}
}

