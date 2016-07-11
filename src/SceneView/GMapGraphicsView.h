#ifndef GMAPGRAPHICSVIEW_H
#define GMAPGRAPHICSVIEW_H
#include "../labexegraphicsmap_global.h"

#include <QGraphicsView>
#include <QWheelEvent>
#include <QElapsedTimer>
#include <QDebug>
#include <math.h>
#include <QRubberBand>

class GMapGraphicsScene;
class GAgentWrapitem;

/////////////////////////////////////////////////////////////////////
//! \brief The GMapGraphicsScene class inherits QGraphicsView to provide a convenient interaction with the agent based structure.
/*!
*/
class LABEXEGRAPHICSMAP_EXPORT GMapGraphicsView : public QGraphicsView
{
	Q_OBJECT
	Q_PROPERTY(QPointF center READ GetCenter WRITE SetCenter);

public:
	GMapGraphicsView(QWidget *parent);
	~GMapGraphicsView();

	//! Zooms the view by a given factor
	void ZoomBy(qreal scaleFactor);
	//! The current centerpoint in the view
	QPointF GetCenter() { return mapToScene(rect()).boundingRect().center();}
// 	//! Returns the map scene
// 	GMapGraphicsScene* MapScene() {return qobject_cast<GMapGraphicsScene*>(scene());}

public slots:
	//! Sets the center of the view on a given point of the scene
	void SetCenter(const QPointF & newCenter);
	//! Sets the zoom/scale of the view
	void SetZoom(double newZoom);

protected:
	//! Re-implemented
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
	//! Re-implemented
	virtual void wheelEvent(QWheelEvent* event);
	//! Re-implemented
	void mouseDoubleClickEvent(QMouseEvent* event);
// 	//! Re-implemented
    void drawBackground (QPainter * painter, const QRectF & rect);
	//! Re-implemented
	void keyPressEvent(QKeyEvent *event);

protected slots:
	//! updates the size of the view when the scene rect has changed. We make the view rect bigger so that it is easier to zoom on the edges.
	void UpdateFromSceneRect(const QRectF & newSceneRect);

signals:
	//! emitted when the zooming factor changed
	void ZoomChanged(double newScale);
	//! emitted when the zooming factor changed
	void LogDistanceChanged(double newLog);
	//! emitted when the zooming factor changed
	void DistanceLevelChanged(int newDistanceLevel);

private:
	//! updates the position of the view so that the panning works
	void UpdatePanningView();
	//! Updates the zooming values of the items in the view
	void UpdateAgentsZooming();
	//! Tries to move to an item on the "direction" of the item currently on the screen, if any. return true if does move
	bool TryMove(int direction, Qt::KeyboardModifiers keyModif = Qt::NoModifier);
	//! Returns the wrap item at the point specified. 0 if none
	GAgentWrapitem* GetWrapItemAt( QPointF atPoint );
	//! Uses a QAnimation to move the center of the scene
	void SmoothMoveCenter(QPointF newCenter, int AnimationLength_ms = 500);

private:
    //! background image default (large) size
    QPixmap m_BackgroundImage0;
    //! background image that is created from the above, being scaled to the viewport size
    QPixmap m_BackgroundImage;

	//! Holds the point that is being dragged in the view, used for panning
	QPointF m_HeldPointAtPanning;
	//! From panning the view
	QPoint m_LastPanPoint;
    QPoint m_posPress;
    //QRubberBand m_RubberBand;
	//! Accumulates small wheel scrolling before actaully zooming the view in order to prevent to often an update
	int m_CumulDeltaMouse;
};


/////////////////////////////////////////////////////////////////////
//! \brief The FPSTracker should help tracking the time spent on painting the view for our optimization purposes.
/*!
*/
class FPSTracker : public QObject
{
	Q_OBJECT

public:
	FPSTracker(QObject* pParent) : QObject(pParent)
	{
		m_timer.start();
		m_frames = 0;
		m_LastMeasuredFps = 0;
	}

	bool eventFilter(QObject *object, QEvent *e) {
		if(e->type() == QEvent::Paint) {
			++m_frames;
			qint64 msSinceLast = m_timer.elapsed();
			if(msSinceLast > 100) {
				double oldFps = m_LastMeasuredFps;
				m_LastMeasuredFps = double(m_frames * 1000) / double(msSinceLast);
//				qDebug() << QString("Average ms pr frame: %1, %2 frames in %3 ms").arg(msSinceLast / (qreal) m_frames).arg(m_frames).arg(m_timer.elapsed());
				emit FpsUpdated(QString("fps: %1").arg(0.5 * (m_LastMeasuredFps + oldFps), 0, 'f', 1));
				m_timer.start();
				m_frames = 0;
			}
		}
		return QObject::eventFilter(object, e);
	}

signals:
	void FpsUpdated(QString newFpsMessage);

private:
	QElapsedTimer m_timer;
	bool m_started;
	int m_frames;
	double m_LastMeasuredFps;
};

#endif // GMAPGRAPHICSVIEW_H
