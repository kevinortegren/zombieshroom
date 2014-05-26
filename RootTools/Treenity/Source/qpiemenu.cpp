/*

Copyright (c) 2011, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "qpiemenu.h"

#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QTimer>
#include <QDateTime>
#include <QSignalMapper>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>
#include <QStyleOptionGraphicsItem>
#include <QScreen>

#include <math.h>
#include <stdio.h>

//
// QClickablePixmapGraphicsItem
//

QClickablePixmapGraphicsItem::QClickablePixmapGraphicsItem()
{

}

void QClickablePixmapGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit clicked();
	QGraphicsPixmapItem::mousePressEvent(event);
}

//
// QButtonGraphicsItem
//

QButtonGraphicsItem::QButtonGraphicsItem(const QPixmap &pixmap):
	m_Pixmap(pixmap),
	m_Size(48),
	m_Angle(0),
	m_AngleSpan(5760)
{
	//setAcceptsHoverEvents(true);
}

QRectF QButtonGraphicsItem::boundingRect() const
{
	return QRectF(-m_Size / 2, -m_Size / 2, m_Size, m_Size);
}

QPainterPath QButtonGraphicsItem::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());
	return path;
}

void QButtonGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	bool down = option->state & QStyle::State_Sunken;
	QLinearGradient grad(-m_Size / 2, -m_Size / 2, m_Size / 2, m_Size / 2);
	grad.setColorAt(down ? 1 : 0, Qt::white);
	grad.setColorAt(down ? 0 : 1, Qt::darkGray);
	painter->setPen(Qt::darkGray);
	painter->setBrush(grad);
	//painter->drawEllipse(boundingRect());
	painter->drawPie(boundingRect(), m_Angle, m_AngleSpan);
	

	grad.setColorAt(down ? 1 : 0, Qt::darkGray);
	grad.setColorAt(down ? 0 : 1, Qt::lightGray);
	painter->setPen(Qt::NoPen);
	painter->setBrush(grad);
	if (down)
	{
		painter->translate(1, 1);
	}
	//painter->drawEllipse(boundingRect().adjusted(3, 3, -3, -3));

	painter->drawPixmap(-m_Pixmap.width() / 2, -m_Pixmap.height() / 2, m_Pixmap);
}

void QButtonGraphicsItem::setSize(int size)
{
	m_Size = size;
	update();
}

void QButtonGraphicsItem::hide()
{
	QGraphicsItem::hide();
}

void QButtonGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	update();
}

void QButtonGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	emit clicked();
	update();
}

void QButtonGraphicsItem::setAngleSpan( int angleSpan )
{
	m_AngleSpan = angleSpan;
}

void QButtonGraphicsItem::setAngle( int angle )
{
	m_Angle = angle;
}



//
// QPieMenu
//

QList<QPieMenu*> QPieMenu::s_AllPieMenus;

QPieMenu::QPieMenu(QWidget *parent):
	QWidget(parent),
	m_IconSize(32, 32),
	m_AnimEnabled(true),
	m_AnimDuration(500),
	m_EasingCurve(QEasingCurve::InOutBack)
{
	/* if (singleton == 0)
	{
		singleton = new QObject();
	} */
	QPieMenu::s_AllPieMenus.push_back(this);

	hide();
	// m_AnimTimer = new QTimer(this);
	// QObject::connect(m_AnimTimer, SIGNAL(timeout()), this, SLOT(doAnim()));
	// m_Mapper = new QSignalMapper(this);

	m_Scene = new QGraphicsScene(this);
	m_View = new QGraphicsView(m_Scene, this);
	m_View->setStyleSheet("border: none;");
	m_View->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	m_View->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_View->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_View->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint); //  | Qt::WindowStaysOnTopHint);
	m_View->setAttribute(Qt::WA_AlwaysShowToolTips);
	m_View->hide();

	m_Animation = new QParallelAnimationGroup(this);

	m_BgPixmapItem = new QClickablePixmapGraphicsItem();
	QObject::connect(m_BgPixmapItem, SIGNAL(clicked()), this, SLOT(bgClicked()));
	m_Scene->addItem(m_BgPixmapItem);

	m_CheckActiveWindowTimer = new QTimer(this);
	QObject::connect(m_CheckActiveWindowTimer, SIGNAL(timeout()), this, SLOT(checkActiveWindow()));
}

QPieMenu::~QPieMenu()
{
	QPieMenu::s_AllPieMenus.removeAt(QPieMenu::s_AllPieMenus.indexOf(this));
	/* for(QList<QButtonGraphicsItem*>::iterator it = m_Entries.begin(); it != m_Entries.end(); it++)
	{
		delete (*it);
	} */
	delete m_View;
	delete m_Scene;
}

QButtonGraphicsItem* QPieMenu::addAction(const QString &text, const QIcon &icon)
{
	QButtonGraphicsItem *btn = new QButtonGraphicsItem(icon.pixmap(m_IconSize));
	btn->setSize(m_IconSize.width() + 20);
	btn->setTransformOriginPoint(m_IconSize.width() / 2 + 3, m_IconSize.height() / 2 + 3);
	// btn->setStyleSheet("background: transparent;");
	// btn->setIconSize(m_IconSize);
	btn->setToolTip(text);
	// btn->setAttribute(Qt::WA_AlwaysShowToolTips);
	// btn->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	// btn->setMask(QRegion(QRect(QPoint(2, 1), btn->sizeHint() - QSize(4, 2)), QRegion::Ellipse));
	// btn->setMask(icon);
	m_Scene->addItem(btn);
	m_Entries.push_back(btn);
	return btn;
}

void QPieMenu::addAction(const QString &text, const QIcon &icon, const QObject *receiver, const char *method)
{
	QButtonGraphicsItem *btn = addAction(text, icon);
	QObject::connect(btn, SIGNAL(clicked()), receiver, method);
	QObject::connect(btn, SIGNAL(clicked()), this, SLOT(closeMenu()));
}

void QPieMenu::addAction(const QString &text, const QIcon &icon, const QObject *receiver, const char *method, int param)
{
	QButtonGraphicsItem *btn = addAction(text, icon);
	QSignalMapper *mapper = new QSignalMapper(this);
	mapper->setMapping(btn, param);
	QObject::connect(btn, SIGNAL(clicked()), mapper, SLOT(map()));
	QObject::connect(mapper, SIGNAL(mapped(int)), receiver, method);
	QObject::connect(btn, SIGNAL(clicked()), this, SLOT(closeMenu()));
}

void QPieMenu::addMenu(const QString &text, const QIcon &icon, QPieMenu *menu)
{
	QButtonGraphicsItem *btn = addAction(text, icon);
	// QObject::connect(btn, SIGNAL(clicked()), menu, SLOT(showMenu()));
	// QObject::connect(this, SIGNAL(hidden()), menu, SLOT(showNextMenu()));
	QSignalMapper *mapper = new QSignalMapper(this);
	mapper->setMapping(btn, menu);
	QObject::connect(mapper, SIGNAL(mapped(QWidget*)), this, SLOT(setNextMenu(QWidget*)));
	QObject::connect(btn, SIGNAL(clicked()), mapper, SLOT(map()));
	QObject::connect(btn, SIGNAL(clicked()), this, SLOT(closeMenu()));
}

void QPieMenu::setIconSize(const QSize &size)
{
	m_IconSize = size;
}

void QPieMenu::setAnimEnabled(bool enabled)
{
	m_AnimEnabled = enabled;
}

void QPieMenu::computePlacing()
{
	QPoint cursorPos = QCursor::pos();
	int maxSize(0); //  = m_Entries.count() * 40;
	int count = m_Entries.count();
	for (int i = 0; i < count; i++)
	{
		QSize hint = m_Entries[i]->boundingRect().size().toSize();
		int size = hint.width() > hint.height() ? hint.width() : hint.height();
		if (size > maxSize) maxSize = size;
	}
	int radius = 5 + maxSize * count / 4;
	QRect availGeom = qApp->desktop()->availableGeometry(cursorPos);
	// printf("availGeom left=%d right=%d top=%d bottom=%d\n", availGeom.left(), availGeom.right(), availGeom.top(), availGeom.bottom());
	int radiusPlusHalfSize = radius + maxSize / 2 + 10;
	if (cursorPos.x() - availGeom.left() < radiusPlusHalfSize) cursorPos.setX(availGeom.left() + radiusPlusHalfSize); else
	if (availGeom.right() - cursorPos.x() < radiusPlusHalfSize) cursorPos.setX(availGeom.right() - radiusPlusHalfSize);
	if (cursorPos.y() - availGeom.top() < radiusPlusHalfSize) cursorPos.setY(availGeom.top() + radiusPlusHalfSize); else
	if (availGeom.bottom() - cursorPos.y() < radiusPlusHalfSize) cursorPos.setY(availGeom.bottom() - radiusPlusHalfSize);

	m_DestRadius = radius;
	m_MaxSize = maxSize;
	m_CenterPos = cursorPos;
}

void QPieMenu::placeEntries(int radius)
{
	m_Radius = radius;

	m_Animation->stop();
	m_Animation->clear();
	int count = m_Entries.count();
	for (int i = 0; i < count; i++)
	{
		// int x = m_CenterPos.x() +
		int x = (int)(radius * cos(i * 2 * 3.14 / count - 3.14 / 2));
		// int y = m_CenterPos.y() +
		int y = (int)(radius * sin(i * 2 * 3.14 / count - 3.14 / 2));

		int angle = -(i * 5760 / count) + 5760 / 4;
		int spanAngle = 5760 / count;
		m_Entries[i]->setAngle(angle);
		m_Entries[i]->setAngleSpan(spanAngle);

		// QSize hint = m_Entries[i]->sizeHint();
		// int size = hint.width() > hint.height() ? hint.width() : hint.height();
		// m_Entries[i]->setGeometry(x - m_MaxSize / 2, y - m_MaxSize / 2, m_MaxSize, m_MaxSize); //  hint.width() / 2, y - hint.height() / 2, hint.width(), hint.height());
		// m_Entries[i]->show();

		// if (radius == 0 && m_Entries[i] == QObject::sender())
		// {
			// skip
		// } else
		// {
		//QPropertyAnimation *a = new QPropertyAnimation(m_Entries[i], "pos");
		//a->setEasingCurve(m_EasingCurve);
		//if (radius > 0)
		//{
		//	a->setStartValue(QPointF(0, 0));
		//}
		//a->setEndValue(QPointF(x, y));
		////a->setDuration(m_AnimDuration + i * 50);
		//a->setDuration(m_AnimDuration * 0.5);

		//m_Animation->addAnimation(a);
		//// }
	}
	/*m_Animation->start();
	QObject::disconnect(m_Animation, SIGNAL(finished()), 0, 0);
	if (radius == 0)
	{
		QObject::connect(m_Animation, SIGNAL(finished()), m_View, SLOT(hide()));
		QObject::connect(m_Animation, SIGNAL(finished()), m_CheckActiveWindowTimer, SLOT(stop()));
		if (m_NextMenu != 0)
		{
			QObject::connect(m_Animation, SIGNAL(finished()), this, SLOT(showNextMenu()));
		}
		QObject::connect(m_Animation, SIGNAL(finished()), this, SIGNAL(hidden()));
	}*/

	if (radius == 0)
	{

		m_View->hide();
		m_CheckActiveWindowTimer->stop();
		if (m_NextMenu != 0)
		{

			showNextMenu();
		}
		hidden();
	}
}

void QPieMenu::showMenu()
{
	hideAllPieMenus();
	QTimer::singleShot(100, this, SLOT(showMenuNow()));
}

void QPieMenu::showMenuNow()
{
	QScreen* screen = QGuiApplication::primaryScreen();
	computePlacing();
	m_NextMenu = 0;

	int bgRadius = m_DestRadius + m_MaxSize / 2 + 20;
	QPixmap bg(screen->grabWindow(qApp->desktop()->winId(), m_CenterPos.x() - bgRadius, m_CenterPos.y() - bgRadius, bgRadius * 2, bgRadius * 2));
	// m_Scene->clear();
	// m_Scene->findChild
	// QGraphicsPixmapItem *pi = new QGraphicsPixmapItem(bg);
	m_BgPixmapItem->setPixmap(bg);
	// pi->setTransformOriginPoint(bg.width() / 2, bg.height() / 2);
	m_BgPixmapItem->setPos(-bg.width() / 2, -bg.height() / 2);
	m_BgPixmapItem->setZValue(-1);
	// m_Scene->addItem(pi);
	QRect bgRect(-bgRadius, -bgRadius, bgRadius * 2, bgRadius * 2);
	m_View->setSceneRect(bgRect);
	m_View->setGeometry(m_CenterPos.x() - bgRadius, m_CenterPos.y() - bgRadius, bgRadius * 2, bgRadius * 2);
	m_View->show();
	//m_View->activateWindow();

	for (int i = 0; i < m_Entries.count(); ++i)
	{
		m_Entries[i]->setSize(bgRadius);
	}

	placeEntries(m_DestRadius);

	/* int count = m_Entries.count();
	for (int i = 0; i < count; i++)
	{
	} */

	// m_AnimTime.start();
	// m_AnimTimer->start(30);

	//m_CheckActiveWindowTimer->start(100);
}

void QPieMenu::bgClicked()
{
	if (m_Animation->state() == QAbstractAnimation::Running)
	{
		m_Animation->setCurrentTime(m_Animation->totalDuration());
	} else
	{
		closeMenu();
	}
}

void QPieMenu::checkActiveWindow()
{
	if (!m_View->isActiveWindow())
	{
		hideMenu();
	}
}

/* void QPieMenu::doAnim()
{
	int elapsed = m_AnimTime.elapsed();
	int radius = (elapsed >= m_AnimDuration ? m_DestRadius : (elapsed * m_DestRadius / m_AnimDuration));
	placeEntries(radius);
	if (elapsed >= m_AnimDuration)
	{
		m_AnimTimer->stop();
	}
} */

void QPieMenu::closeMenu()
{
	printf("QPieMenu::closeMenu()\n");


	m_View->hide();
	m_CheckActiveWindowTimer->stop();
	if (m_NextMenu != 0)
	{
	showNextMenu();
	}
	hidden();
	// m_AnimTimer->stop();

	// QObject::sender();

	/* for (QList<QButtonGraphicsItem*>::iterator it = m_Entries.begin(); it != m_Entries.end(); it++)
	{
		if ((*it) != QObject::sender()) (*it)->hide();
	}

	if (QObject::sender() != 0)
	{
		QTimer::singleShot(200, (QButtonGraphicsItem*) QObject::sender(), SLOT(hide()));
	} */
}

void QPieMenu::hideMenu()
{
	m_CheckActiveWindowTimer->stop();
	m_View->hide();
	QObject::disconnect(m_Animation, SIGNAL(finished()), 0, 0);
}

void QPieMenu::hideAllPieMenus()
{
	QListIterator<QPieMenu*> it(QPieMenu::s_AllPieMenus);
	while(it.hasNext())
	{
		it.next()->hideMenu();
	}
}

void QPieMenu::setNextMenu(QWidget *menu)
{
	m_NextMenu = qobject_cast<QPieMenu*>(menu);
	printf("m_NextMenu = 0x%08X\n", m_NextMenu);
}

void QPieMenu::showNextMenu()
{
	if (m_NextMenu)
	{
		QTimer::singleShot(100, m_NextMenu, SLOT(showMenu()));
	}
}

void QPieMenu::setEasingCurve(const QEasingCurve &easingCurve)
{
	m_EasingCurve = easingCurve;
}

#ifdef QPIEMENU_TESTING
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QMainWindow *wnd = new QMainWindow();
	QPushButton *btn = new QPushButton("test", wnd);
	wnd->setCentralWidget(btn);
	QPieMenu *menu = new QPieMenu(wnd);
	menu->addAction("", QIcon("icons/new_unimap.png"), menu, SLOT(closeMenu()));
	menu->addAction("", QIcon("icons/new_unimap.png"), menu, SLOT(closeMenu()));
	menu->addAction("", QIcon("icons/new_unimap.png"), menu, SLOT(closeMenu()));
	menu->addAction("", QIcon("icons/new_unimap.png"), menu, SLOT(closeMenu()));
	menu->addAction("", QIcon("icons/new_unimap.png"), menu, SLOT(closeMenu()));
	QObject::connect(btn, SIGNAL(clicked()), menu, SLOT(showMenu()));
	wnd->show();
	return app.exec();
}

#endif
