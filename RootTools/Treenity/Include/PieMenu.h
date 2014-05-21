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
	bool isHovered() const;
	void setHovered(bool state);
public slots:
	void hide();
protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
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
	bool m_hovered;
};

class PieMenu : public QWidget
{
	Q_OBJECT
public:
	PieMenu(QWidget* parent);
	~PieMenu();

	bool canSee() const;
public slots:
	void showMenu();
	void closeMenu();
protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
private:
	int angleToIndex(qreal angle) const;
	void setHovered(int index, bool state);

	QList<PiePiece*> m_pieces;
	int m_radius;
	int m_hoveredIndex;
	QPoint m_center;
	QGraphicsScene* m_scene;
	QGraphicsView* m_view;
	QGraphicsPixmapItem* m_background;
};