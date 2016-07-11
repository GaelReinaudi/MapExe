#include "GMapGraphicsView.h"
#include "GItemManager.h"
#include "Agent/GAgentWrapitem.h"
#include <QGLWidget>
#include <QDir>
#include <QGraphicsBlurEffect>
#include <QtTest/QtTest>

//! max absolute zoom level
static double G_MAX_ZOOM = 1000000;
//! min zoom in terms of 
static double G_MIN_ZOOM_SCENE_FACTOR = 10.0;
 
GMapGraphicsView::GMapGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
	, m_CumulDeltaMouse(0)
//	, m_RubberBand(QRubberBand::Rectangle, this)
{
     setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
	// try optimization
//	setViewport(new QGLWidget());

	setOptimizationFlags(QGraphicsView::DontSavePainterState);
 	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//	setCacheMode(QGraphicsView::CacheBackground);

// 	// tracking fps
// 	FPSTracker* pFps = new FPSTracker(this);
// 	viewport()->installEventFilter(pFps);

	// remove scrollbars
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setBackgroundBrush(QBrush("#a3a3a3"));
//	// try to load the background image
    m_BackgroundImage0 = QPixmap(":/images/spotlight2560.png");
}

GMapGraphicsView::~GMapGraphicsView()
{

}

void GMapGraphicsView::wheelEvent( QWheelEvent *event )
{
	int deltaMouse = event->delta();
	m_CumulDeltaMouse += deltaMouse;
	// if we haven't accumulated enough scrolling we just keep waiting for it
	if(qAbs(m_CumulDeltaMouse) < 100.0)
		return;
	// else, we send the cumulative to the zooming
	deltaMouse = m_CumulDeltaMouse;
	m_CumulDeltaMouse = 0;

	double scalingStepFactor = pow(2, deltaMouse / 600.0);
	double currentScale = transform().m11();
	// if we zoom in, the zoom is centered on the mouse
	if(deltaMouse > 0) {
		if(currentScale * scalingStepFactor > G_MAX_ZOOM) {
			scalingStepFactor = G_MAX_ZOOM / currentScale;
// 			return;
		}
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	}
	else { // else, we un-zoom from the center of the view
		if(currentScale * scene()->sceneRect().height() * G_MIN_ZOOM_SCENE_FACTOR < TypicalScreen.height()) {
			if(currentScale * scene()->sceneRect().width() * G_MIN_ZOOM_SCENE_FACTOR < TypicalScreen.width()) {
				qDebug() << "Max zoom out:" << currentScale;
				return;
			}
		}
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	}

	// from Qt Elastic Nodes Example
	ZoomBy(scalingStepFactor);
	UpdatePanningView();

	currentScale = transform().m11();
	if(currentScale >= G_MAX_ZOOM) {
		qDebug() << "Max zoom in:" << currentScale << " >= " << G_MAX_ZOOM;
	}
}

void GMapGraphicsView::mouseDoubleClickEvent( QMouseEvent* event )
{
	QGraphicsView::mouseDoubleClickEvent(event);
}

void GMapGraphicsView::drawBackground( QPainter * painter, const QRectF & rect )
{
//	QGraphicsView::drawBackground(painter, rect);
	// try to draw a fixed background centered
	QRect rectViewPort = painter->viewport();
	QRect croppedRectViewPort = rectViewPort;
	if(m_BackgroundImage.isNull()) {
		double bkgdRatio = double(m_BackgroundImage0.height())/double(m_BackgroundImage0.width());
		double vwptRatio = double(rectViewPort.height())/double(rectViewPort.width());
		if(vwptRatio > bkgdRatio) {
			m_BackgroundImage = m_BackgroundImage0.scaledToHeight(rectViewPort.height());
		}
		else {
			m_BackgroundImage = m_BackgroundImage0.scaledToWidth(rectViewPort.width());
		}
	}
	croppedRectViewPort.moveCenter(QPoint(m_BackgroundImage.width() / 2, m_BackgroundImage.height() / 2));
	QPixmap cropedImage = m_BackgroundImage.copy(croppedRectViewPort);
	painter->setTransform(QTransform());
	painter->drawPixmap(rectViewPort, m_BackgroundImage, croppedRectViewPort);
}

void GMapGraphicsView::UpdateFromSceneRect( const QRectF & newSceneRect )
{
	//Get the rectangle of the visible area in scene coords
	QRectF visibleArea = mapToScene(rect()).boundingRect();
	QRectF newViewSceneRect = newSceneRect;

	double deltaV = newViewSceneRect.width();
	double deltaH = newViewSceneRect.height();
	double delta = qMax(deltaH, deltaV) * 0.8;

	// a gradient background
	// could be optimized (if we keep it) by making it a member and only updating its center and radius
//    QRadialGradient gradient(newSceneRect.center(), delta);
//    gradient.setColorAt(0.0, QColor(0x2C, 0x0C, 0x2C));
//    gradient.setColorAt(0.7, QColor(0x2C, 0x0C, 0x2C));
//    gradient.setColorAt(1.0, QColor(0x1C, 0x0C, 0x1C));
//    setBackgroundBrush(gradient);

    setBackgroundBrush(QBrush("#a3a3a3"));

//    QRectF canvasRect = newSceneRect;
//    canvasRect.adjust(20,20,-20,-20);
//
//    QGraphicsRectItem* canvasRectItem = new QGraphicsRectItem(canvasRect);
//    QColor canvasColor("#3d3d3d");
//    QBrush canvasBrush(canvasColor);
//    QPen canvasPen(canvasColor);
//    canvasRectItem->setBrush(canvasBrush);
//    canvasRectItem->setPen(canvasPen);
//    scene()->addItem(canvasRectItem);
//    canvasRectItem->setZValue(-1);
//
//    QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect();
//    blurEffect->setBlurRadius(20);
//    QGraphicsRectItem* canvasRectItemShadow = new QGraphicsRectItem(newSceneRect);
//    QRectF shadowRect;
//    shadowRect = canvasRectItemShadow->rect();
//    //shadowRect.adjust(-20,-20,20,20);
//    canvasRectItemShadow->setRect(shadowRect);
//    QColor canvasShadowColor("#ffffff");
//    QBrush canvasShadowBrush(canvasShadowColor);
//    QPen canvasShadowPen(canvasShadowColor);
//    canvasRectItemShadow->setBrush(canvasShadowBrush);
//    canvasRectItemShadow->setPen(canvasShadowPen);
//    canvasRectItemShadow->setGraphicsEffect(blurEffect);
//    scene()->addItem(canvasRectItemShadow);
//    canvasRectItemShadow->setZValue(-2);

	delta *= G_MIN_ZOOM_SCENE_FACTOR;
	newViewSceneRect.adjust(-delta, -delta, delta, delta);

	setSceneRect(newViewSceneRect);
	centerOn(visibleArea.center());
}

void GMapGraphicsView::mousePressEvent( QMouseEvent* event )
{
	QGraphicsView::mousePressEvent(event);
	if(event->button() == Qt::MidButton) {
		//For panning the view
		m_LastPanPoint = event->pos();
		m_HeldPointAtPanning = mapToScene(m_LastPanPoint);
		setCursor(Qt::OpenHandCursor);
	}
    else if (itemAt(event->pos()) == 0) {
        m_posPress = event->pos();
        //m_RubberBand.setGeometry(QRect(m_posPress, QSize()));
        //m_RubberBand.show();
    }
}

void GMapGraphicsView::mouseReleaseEvent( QMouseEvent* event )
{
	if(event->button() == Qt::MidButton) {
		setCursor(Qt::ArrowCursor);
		m_LastPanPoint = QPoint();
	}
    else {
        //m_RubberBand.hide();
    }

	QGraphicsView::mouseReleaseEvent(event);
}

void GMapGraphicsView::mouseMoveEvent( QMouseEvent* event )
{
	if(event->buttons() & Qt::MidButton) {
		if(!m_LastPanPoint.isNull()) {
			m_LastPanPoint = event->pos();

			//Update the center ie. do the pan
			UpdatePanningView();
			setCursor(Qt::ClosedHandCursor);
		}
	}
    else {
        //m_RubberBand.setGeometry(QRect(m_posPress, event->pos()).normalized());
    }

	QGraphicsView::mouseMoveEvent(event);
}

void GMapGraphicsView::resizeEvent( QResizeEvent* event )
{
	//Get the rectangle of the visible area in scene coords
	QRectF visibleArea = mapToScene(rect()).boundingRect();
	centerOn(visibleArea.center());

	QGraphicsView::resizeEvent(event);
}

void GMapGraphicsView::UpdatePanningView()
{
	if(!m_LastPanPoint.isNull()) {
		//Get how much we panned
		QPointF delta = mapToScene(m_LastPanPoint) - m_HeldPointAtPanning;
		QPointF newCenter = GetCenter() - delta;
		centerOn(newCenter);
	}
}

void GMapGraphicsView::SetZoom( double newZoom )
{
	setTransform(QTransform());
	scale(newZoom, newZoom);
	UpdateAgentsZooming();
	emit ZoomChanged(newZoom);
}

void GMapGraphicsView::ZoomBy(qreal scaleFactor)
{
	// from Qt Elastic Nodes Example
	scale(scaleFactor, scaleFactor);
	double newZoom = transform().m11();
	UpdateAgentsZooming();
    emit ZoomChanged(newZoom);
}

void GMapGraphicsView::SetCenter( const QPointF & newCenter )
{
	centerOn(newCenter);
	UpdateAgentsZooming();
}

void GMapGraphicsView::UpdateAgentsZooming()
{
	double theZoom =  transform().m11();
	// this one does qSqrt(vector, vector) in the Qt source code..... yewwww 
	//theZoom =  QStyleOptionGraphicsItem::levelOfDetailFromTransform(transform()); 

	double distance = 1.0 / theZoom;
	double logBase2Distance = qLn(distance) * 1.44269504;
	emit LogDistanceChanged(logBase2Distance);
	int distanceLevel = qRound(logBase2Distance);
	emit DistanceLevelChanged(distanceLevel);

	ItemManagerInstance()->StartUpdateDistanceItems(this, distanceLevel, rect());
}

void GMapGraphicsView::keyPressEvent( QKeyEvent *event )
{
	if(TryMove(event->key(), event->modifiers()))
		return;
	QGraphicsView::keyPressEvent(event);
}

bool GMapGraphicsView::TryMove( int direction, Qt::KeyboardModifiers keyModif /*= Qt::NoModifier*/ )
{
	// find if there is ONE item on the screen
	QRectF currentViewRect = rect();
	UpdateAgentsZooming();	
	QList<GAgentWrapitem*> theItems = ItemManagerInstance()->GetCurrentWrapItemsIntersectRect();
	if(theItems.isEmpty())// || theItems.count()  1)
		return false;
	// get the center of the view
	QPointF currentCenter = GetCenter();
	// get the wrap item at the center
	GAgentWrapitem* curentWrapItem = GetWrapItemAt(currentCenter);
	if(!curentWrapItem)
		return false;

	// move right by step 1/10 of the view until you find another item
	QPointF step;
	switch(direction) {
	case Qt::Key_Right:
		step = QPointF(currentViewRect.width() / 10.0, 0.0);
		break;
	case Qt::Key_Left:
		step = QPointF(-currentViewRect.width() / 10.0, 0.0);
		break;
	case Qt::Key_Up:
		step = QPointF(0.0, -currentViewRect.height() / 10.0);
		break;
	case Qt::Key_Down:
		step = QPointF(0.0, currentViewRect.height() / 10.0);
		break;
	default:
		return false;
	}

	for(int i = 0; i < 100 ; i++) {
		currentCenter += step;
		GAgentWrapitem* nextItem = GetWrapItemAt(currentCenter);
		if(nextItem && nextItem != curentWrapItem) {
			if(nextItem != curentWrapItem->parentItem() && curentWrapItem != nextItem->parentItem()) {
				QPointF itemCenter = nextItem->scenePos() + nextItem->boundingRect().center();
				// depending on the ctr or alt key being pressed we move instantaneously, or super slowly.
				//qDebug() << keyModif & (Qt::ShiftModifier | Qt::AltModifier);
				switch(keyModif & (Qt::ShiftModifier | Qt::AltModifier)) {
					case Qt::ShiftModifier:
						SetCenter(itemCenter);
						break;
					case Qt::AltModifier:
						SmoothMoveCenter(itemCenter, 2500);
						break;
					default:
						SmoothMoveCenter(itemCenter);
				}
				return true;
			}
		}
	}
	return false;
}

void GMapGraphicsView::SmoothMoveCenter( QPointF newCenter, int AnimationLength_ms /*= 500*/ )
{
	QPropertyAnimation* animation = new QPropertyAnimation(this, "center");
	animation->setDuration(AnimationLength_ms);
	animation->setEasingCurve(QEasingCurve::OutQuart);
	animation->setStartValue(GetCenter());

	animation->setEndValue(newCenter);

	animation->start();
}

GAgentWrapitem* GMapGraphicsView::GetWrapItemAt( QPointF atPoint )
{
	GAgentWrapitem* pWrapItem = 0;
	foreach(QGraphicsItem* pIt, scene()->items(atPoint, Qt::IntersectsItemBoundingRect, Qt::DescendingOrder, transform())) {
		pWrapItem = dynamic_cast<GAgentWrapitem*>(pIt);
		if(pWrapItem)
			break;
	}
	return pWrapItem;
}





