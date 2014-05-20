#include <PieMenu.h>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>
#include <QGuiApplication>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

PiePiece::PiePiece(int index, int radius, QPoint position, PieMenu* parent)
	: m_startAngle((360 / 8) * index), m_radius(radius), m_center(position), m_parent(parent)
{
	m_shapePath.moveTo(QPointF(0, 0));
	m_shapePath.arcTo(boundingRect(), m_startAngle, 360 / 8);
	m_shapePath.closeSubpath();

	m_pixmap = QPixmap("Resources/resizeButton.png");

	connect(this, SIGNAL(clicked()), m_parent, SLOT(closeMenu()));
}

QPainterPath PiePiece::shape() const 
{
	return m_shapePath;
}

QRectF PiePiece::boundingRect() const
{
	return QRectF(QPointF(-m_radius, -m_radius), QPointF(m_radius, m_radius));
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
	QLinearGradient grad(-m_radius / 2, -m_radius / 2, m_radius / 2, m_radius / 2);
	grad.setColorAt(down ? 1 : 0, Qt::white);
	grad.setColorAt(down ? 0 : 1, Qt::darkGray);
	if (isSelected()) 
		painter->setPen(Qt::darkGray);
	else
		painter->setPen(Qt::lightGray);
	painter->setBrush(grad);
	//painter->drawEllipse(boundingRect());
	painter->drawPath(m_shapePath);
	painter->fillPath(m_shapePath, painter->brush());


	grad.setColorAt(down ? 1 : 0, Qt::darkGray);
	grad.setColorAt(down ? 0 : 1, Qt::lightGray);
	painter->setPen(Qt::NoPen);
	painter->setBrush(grad);
	if (down)
	{
		painter->translate(1, 1);
	}
	//painter->drawEllipse(boundingRect().adjusted(3, 3, -3, -3));

	//painter->drawPixmap(-m_Pixmap.width() / 2, -m_Pixmap.height() / 2, m_Pixmap);
}

void PiePiece::hide()
{
	QGraphicsItem::hide();
}

void PiePiece::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	setSelected(true);
	update();
}

void PiePiece::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	setSelected(false);
	update();
}

void PiePiece::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	emit clicked();
	update();
}

PieMenu::PieMenu(QWidget* parent)
	: QWidget(parent)
	, m_radius(100)
{
	m_scene = new QGraphicsScene(this);
	m_view = new QGraphicsView(m_scene, this);
	m_view->setStyleSheet("border: none;");
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_view->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
	m_view->setAttribute(Qt::WA_AlwaysShowToolTips);
	m_view->hide();

	for (int i = 0; i < 8; ++i)
	{
		PiePiece* piece = new PiePiece(i, m_radius, QCursor::pos(), this);
		piece->setAcceptedMouseButtons(Qt::NoButton);
		m_pieces.push_back(piece);
		m_scene->addItem(piece);
	}

	m_background = new QGraphicsPixmapItem();
	m_scene->addItem(m_background);
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

	m_background->setPixmap(bg);
	m_background->setPos(-bg.width() / 2, -bg.height() / 2);
	m_background->setZValue(-1);

	QRect bgRect(-bgRadius, -bgRadius, bgRadius * 2, bgRadius * 2);
	m_view->setSceneRect(bgRect);
	m_view->setGeometry(m_center.x() - bgRadius, m_center.y() - bgRadius, bgRadius * 2, bgRadius * 2);

	for (int i = 0; i < m_pieces.size(); ++i)
	{
		m_pieces[i]->updatePosition();
		m_pieces[i]->setAcceptHoverEvents(true);
		m_pieces[i]->setAcceptedMouseButtons(Qt::RightButton);
	}

	m_view->show();
}

void PieMenu::closeMenu()
{
	for (int i = 0; i < m_pieces.size(); ++i)
	{
		m_pieces[i]->setAcceptHoverEvents(false);
		m_pieces[i]->setAcceptedMouseButtons(Qt::NoButton);
	}

	m_view->hide();
}
