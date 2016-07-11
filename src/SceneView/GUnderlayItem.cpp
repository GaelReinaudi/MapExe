#include "GUnderlayItem.h"
#include <QDebug>
#include "Agent/GAgentWrapitem.h"

#if defined(Q_OS_MAC)
QString imagesURI = "../../../../images/";
#else
QString imagesURI = "../images/";
#endif
QSize shadowPixSize;

GUnderlayItem::GUnderlayItem(GAgentWrapitem* parentContainerItem)
    : QGraphicsRectItem(parentContainerItem)
	, m_Animation(0)
{
    setFlag(QGraphicsItem::ItemStacksBehindParent);

	static QPixmap shadowPixMap(imagesURI + "dropShadow.png");
	if(shadowPixMap.isNull()) {
		qDebug() << "Where is dropShadow.png ?";
		return;
	}
	shadowPixSize = shadowPixMap.size();
	m_pDropShadowItem = new QGraphicsPixmapItem(shadowPixMap, this);

	connect(parentContainerItem, SIGNAL(ContentRectChanged(QRectF)), this, SLOT(UpdateSize()));

	QMetaObject::invokeMethod(this, "UpdateSize", Qt::QueuedConnection);

	// no pen
	setPen(Qt::NoPen);
	// brush
	QRadialGradient gradient(rect().center(), rect().width(), QPointF());
	gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
	gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
	QBrush brush(gradient);
	setBrush(brush);

	// or just plain red. Chris is in charge of this one.
	setBrush(Qt::red);

	ShineOut();
}

void GUnderlayItem::UpdateSize()
{
//	QRectF parentRect = static_cast<GAgentWrapitem*>(parentItem())->ContentBoundingRect();
	QRectF parentRect = static_cast<GAgentWrapitem*>(parentItem())->boundingRect();
	double parentItemWidth = parentRect.width();
	double parentItemHeight = parentRect.height();

	double fraction = 0.963;
	double transformWidth = parentItemWidth / (shadowPixSize.width() * fraction);
	double transformHeight = parentItemHeight / (shadowPixSize.height() * fraction);

	double transformWidthOffset = parentItemWidth * (1 - fraction) / 2;
	double transformHeightOffset = parentItemHeight * (1 - fraction) / 2;

	m_pDropShadowItem->setTransform(QTransform(transformWidth, 0, 0, 0, transformHeight, 0, 0, 0, 1));
	m_pDropShadowItem->setPos(-transformWidthOffset, -transformHeightOffset);

	// the shape that should shine when hovering
	double adjustLarge = (parentRect.width() + parentRect.height()) * 0.02;
	QRectF shineRect = parentRect.adjusted(-adjustLarge, -adjustLarge, adjustLarge, adjustLarge);
	setRect(shineRect);
}

void GUnderlayItem::ShineIn()
{
	if(m_Animation)
		delete m_Animation;
	m_Animation = new QPropertyAnimation(this, "highlight");
	m_Animation->setDuration(100);
	m_Animation->setStartValue(0.0);
	m_Animation->setEndValue(1.0);
	m_Animation->start();
}

void GUnderlayItem::ShineOut()
{
	if(m_Animation) {
		m_Animation->stop();
		delete m_Animation;
		m_Animation = 0;
	}
	SetHighlightLevel(0.0);
}




















