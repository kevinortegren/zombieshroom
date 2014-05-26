#include <PieMenu.h>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>
#include <QGuiApplication>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <qmath.h>
#include <iostream>

PiePiece::PiePiece(int index, int radius, QPoint position, PieMenu* parent)
	: m_startAngle((360 / 8) * index), m_radius(radius), m_center(position), m_parent(parent), m_hovered(false)
{
	setParent((QObject*)m_parent);

	QPointF thereVec = angleToVector(2 * M_PI * m_startAngle / 360.0);
	QPointF backVec = angleToVector(2 * M_PI * ((int)m_startAngle + 45) / 360.0);
	QRectF smallRect(-m_radius / 2, -m_radius / 2, m_radius, m_radius);

	m_shapePath.moveTo(QPointF(0, 0));
	m_shapePath.moveTo(thereVec * m_radius * 0.5);
	m_shapePath.arcTo(boundingRect(), m_startAngle, 360 / 8);
	m_shapePath.lineTo(backVec * m_radius * 0.5);
	m_shapePath.arcTo(smallRect, m_startAngle + 45, -360 / 8);
	m_shapePath.closeSubpath();

	m_pixmap = QPixmap("Resources/resizeButton.png");

	connect(this, SIGNAL(clicked()), m_parent, SLOT(closeMenu()));
}

QPainterPath PiePiece::shape() const 
{
	return QPainterPath(m_shapePath);
}

QRectF PiePiece::boundingRect() const
{
	return QRectF(-m_radius, -m_radius, m_radius * 2, m_radius * 2);
}

void PiePiece::updatePosition()
{
	m_center = QCursor::pos();

	/*m_shapePath = QPainterPath(m_center);

	m_shapePath.arcTo(boundingRect(), m_startAngle, 360 / 8);
	m_shapePath.closeSubpath();*/
}

void PiePiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	bool down = option->state & QStyle::State_Sunken;
	QLinearGradient grad(0, m_radius / 16, 0, (m_radius / 16) + 4);
	
	grad.setColorAt(down ? 1 : 0, QColor(80, 80, 80));
	grad.setColorAt(down ? 0 : 1, QColor(50, 50, 50));
	if (m_hovered) 
	{
		grad.setColorAt(down ? 1 : 0, QColor(90, 90, 90));
		grad.setColorAt(down ? 0 : 1, QColor(60, 60, 60));
	}
	else
	{
		
	}
	painter->setPen(QColor(20, 20, 20));
	painter->setBrush(grad);
	//painter->drawEllipse(boundingRect());
	painter->drawPath(m_shapePath);
	painter->fillPath(m_shapePath, painter->brush());

	/*
	grad.setColorAt(down ? 1 : 0, Qt::darkGray);
	grad.setColorAt(down ? 0 : 1, Qt::lightGray);
	painter->setPen(Qt::NoPen);
	painter->setBrush(grad);
	if (down)
	{
		painter->translate(1, 1);
	}*/
	//painter->drawEllipse(boundingRect().adjusted(3, 3, -3, -3));

	//painter->drawPixmap(-m_Pixmap.width() / 2, -m_Pixmap.height() / 2, m_Pixmap);
}

void PiePiece::hide()
{
	QGraphicsItem::hide();
}

void PiePiece::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	event->ignore();
}

void PiePiece::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{ 
	event->ignore();
}

void PiePiece::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
	event->ignore();
}

void PiePiece::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	event->ignore();
}

void PiePiece::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	event->ignore();
}

void PiePiece::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	event->ignore();
}

void PiePiece::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	event->ignore();
}

bool PiePiece::isHovered() const
{
	return m_hovered;
}

void PiePiece::setHovered( bool state )
{
	m_hovered = state;
	update();
}



PieMenu::PieMenu(QWidget* parent)
	: QWidget(parent)
	, m_radius(150)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	setStyleSheet("background: transparent; background-color: none");

	m_scene = new QGraphicsScene(this);
	m_view = new QGraphicsView(m_scene, this);
	m_view->setStyleSheet("background: transparent");
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_view->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
	m_view->setAttribute(Qt::WA_AlwaysShowToolTips);
	m_view->setAttribute(Qt::WA_TranslucentBackground);
	m_view->hide();

	for (int i = 0; i < 8; ++i)
	{
		PiePiece* piece = new PiePiece(i, m_radius, QCursor::pos(), this);
		piece->setAcceptedMouseButtons(Qt::NoButton);
		m_pieces.push_back(piece);
		m_scene->addItem(piece);
	}

	//m_background = new QGraphicsPixmapItem();
	//m_scene->addItem(m_background);
	
}

PieMenu::~PieMenu()
{
	delete m_scene;
	delete m_view;
}

bool PieMenu::canSee() const
{
	return m_view->isVisible();
}

void PieMenu::showMenu()
{
	QScreen* screen = QGuiApplication::primaryScreen();
	m_center = QCursor::pos();
	
	int bgRadius = m_radius + 20;

	QPixmap bg(screen->grabWindow(qApp->desktop()->winId(), m_center.x() - bgRadius, m_center.y() - bgRadius, bgRadius * 2, bgRadius * 2));

	//m_background->setPixmap(bg);
	//m_background->setPos(-bg.width() / 2, -bg.height() / 2);
	//m_background->setZValue(-1);
	

	QRect bgRect(-bgRadius, -bgRadius, bgRadius * 2, bgRadius * 2);
	m_view->setSceneRect(bgRect);
	m_view->setGeometry(m_center.x() - bgRadius, m_center.y() - bgRadius, bgRadius * 2, bgRadius * 2);

	for (int i = 0; i < m_pieces.size(); ++i)
	{
		m_pieces[i]->updatePosition();
		m_pieces[i]->setAcceptHoverEvents(true);
		m_pieces[i]->setAcceptedMouseButtons(Qt::RightButton);
		m_pieces[i]->setEnabled(true);
	}

	m_view->show();
	m_view->setEnabled(true);
	setEnabled(true);
	grabMouse();
}

void PieMenu::closeMenu()
{
	for (int i = 0; i < m_pieces.size(); ++i)
	{
		m_pieces[i]->setAcceptHoverEvents(false);
		m_pieces[i]->setAcceptedMouseButtons(Qt::NoButton);
		m_pieces[i]->setHovered(false);
		m_hoveredIndex = -1;
	}

	m_view->hide();
}

void PieMenu::mouseMoveEvent( QMouseEvent * event )
{
	QPointF vec = QCursor::pos() - m_center;
	qreal ang = qAtan2(-vec.y(), vec.x());

	if (QPointF::dotProduct(vec, vec) > m_radius * m_radius * 0.5 * 0.5)
		setHovered(angleToIndex(ang), true);
	else
		setHovered(angleToIndex(ang), false);

	QWidget::mouseMoveEvent(event);
}

void PieMenu::mouseReleaseEvent( QMouseEvent * event )
{
	releaseMouse();

	closeMenu();

	

	/*std::cout << "vec: " << vec.x() << " " << vec.y() << std::endl;
	
	
	std::cout << "Index of pie piece: " << angleToIndex(ang) << std::endl;
	std::cout << "Angle is: " << (int)(360 * ang / (2 * M_PI)) % 360 << " degrees" << std::endl;*/

	QWidget::mouseReleaseEvent(event);
}

int PieMenu::angleToIndex( qreal angle ) const
{
	while (angle < 0)
		angle += (2 * M_PI);
	while (angle > 2 * M_PI)
		angle -= (2 * M_PI);

	qreal test = M_PI_4;
	int index = 0;
	while (angle >= test)
	{
		test += M_PI_4;
		index++;
	}

	return index;
}

void PieMenu::setHovered( int index, bool state )
{
	if (m_hoveredIndex >= 0 && m_hoveredIndex < 8)
		m_pieces[m_hoveredIndex]->setHovered(false);
	m_hoveredIndex = index;
	m_pieces[m_hoveredIndex]->setHovered(state);
	if (!state)
		m_hoveredIndex = -1;
}

QPointF angleToVector( qreal radians )
{
	QPointF vec;
	vec.setX(qCos(radians));
	vec.setY(-qSin(radians));

	return vec;
}
