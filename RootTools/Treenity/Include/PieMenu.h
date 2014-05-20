#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

class PieMenu;

class PiePiece : public QGraphicsObject
{
	Q_OBJECT
public:
	PiePiece(int index, int radius, QPoint position, PieMenu* parent);
	QPainterPath shape() const;
	QRectF boundingRect() const;
	void updatePosition();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*);
public slots:
	void hide();
protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
signals:
	void clicked();
private:
	qreal m_startAngle;
	int m_radius;
	QPoint m_center;
	QPainterPath m_shapePath;
	QPixmap m_pixmap;
	PieMenu* m_parent;
};

class PieMenu : public QWidget
{
	Q_OBJECT
public:
	PieMenu(QWidget* parent);
	~PieMenu();
public slots:
	void showMenu();
	void closeMenu();
private:
	QList<PiePiece*> m_pieces;
	int m_radius;
	QPoint m_center;
	QGraphicsScene* m_scene;
	QGraphicsView* m_view;
	QGraphicsPixmapItem* m_background;
};