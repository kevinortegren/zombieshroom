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

#ifndef QPIEMENU_H
#define QPIEMENU_H

#include <QWidget>
#include <QIcon>
#include <QTime>
#include <QGraphicsPixmapItem>
#include <QEasingCurve>

class QTimer;
// class QSignalMapper;

class QPushButton;
class QGraphicsScene;
class QGraphicsView;
class QAnimationGroup;
// class QGraphicsPixmapItem;

class QClickablePixmapGraphicsItem:
	public QObject,
	public QGraphicsPixmapItem
{
	Q_OBJECT
	//Q_INTERFACES(QGraphicsItem)
public:
	QClickablePixmapGraphicsItem();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
	void clicked();
};

class QButtonGraphicsItem:
	public QObject,
	public QGraphicsItem
{
	Q_OBJECT
	//Q_INTERFACES(QGraphicsItem)
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
	QButtonGraphicsItem(const QPixmap &pixmap);
public:
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*);
public:
	void setSize(int size);
public slots:
	void hide();
signals:
	void clicked();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
private:
	QPixmap m_Pixmap;
	int m_Size;
};

class QPieMenu: public QWidget
{
	Q_OBJECT
public:
	QPieMenu(QWidget *parent);
	~QPieMenu();
public:
	void addAction(const QString &text, const QIcon &icon, const QObject *receiver, const char *method);
	void addAction(const QString &text, const QIcon &icon, const QObject *receiver, const char *method, int param);
	void addMenu(const QString &text, const QIcon &icon, QPieMenu *menu);
	void setIconSize(const QSize&);
	void setAnimEnabled(bool enabled);
public slots:
	void showMenu();
	void closeMenu();
	void hideMenu();
	void hideAllPieMenus();
	void setNextMenu(QWidget*);
	void showNextMenu();
	void setEasingCurve(const QEasingCurve&);
private slots:
	void showMenuNow();
	// void doAnim();
	void bgClicked();
	void checkActiveWindow();
signals:
	void hidden();
private:
	static QList<QPieMenu*> s_AllPieMenus;
	QButtonGraphicsItem* addAction(const QString &text, const QIcon &icon);
	void computePlacing();
	void placeEntries(int radius);
private:
	QList<QButtonGraphicsItem*> m_Entries;
	QSize m_IconSize;
	// QTimer *m_AnimTimer;
	QTimer *m_CheckActiveWindowTimer;
	bool m_AnimEnabled;
	int m_AnimDuration;
	QTime m_AnimTime;
	int m_DestRadius;
	int m_Radius;
	int m_MaxSize;
	QPoint m_CenterPos;
	// QSignalMapper *m_Mapper;
	QGraphicsScene *m_Scene;
	QGraphicsView *m_View;
	QAnimationGroup *m_Animation;
	QPieMenu *m_NextMenu;
	QClickablePixmapGraphicsItem *m_BgPixmapItem;
	QEasingCurve m_EasingCurve;
};

#endif // QPIEMENU_H
