#include "GOverlayItem.h"
#include "Agent/GAgentWrapitem.h"

GOverlayItem::GOverlayItem(GAgentWrapitem* parentContainerItem)
    : QGraphicsRectItem(parentContainerItem)
    //, QGraphicsLinearLayout(Qt::Horizontal, 0)
    , QObject(dynamic_cast<QObject*>(parentContainerItem))
    //, m_pLinearLayout(new QGraphicsLinearLayout())
    //, m_pLayoutWidget(new QGraphicsWidget(this))
{
#if defined(Q_OS_MAC)
	static QString imagesURI = "../../../../images/";
#else
	static QString imagesURI = "../images/";
#endif
    //static QPixmap overPixMap(imagesURI + "smokeOverlay.png");
    //if(overPixMap.isNull())
        //return;
    //QGraphicsWidget* m_pLayoutWidget = new QGraphicsWidget(this);
    //QGraphicsPixmapItem* m_pOverlayCanvasItem = new QGraphicsPixmapItem(overPixMap, this);

    //double parentItemWidth = parentContainerItem->boundingRect().width();

    //double heightScale = parentItemHeight/m_pOverlayCanvasItem->boundingRect().height()/20;
    //double widthScale = parentItemWidth/m_pOverlayCanvasItem->boundingRect().height();
    //m_pOverlayCanvasItem->setTransform(QTransform(widthScale, 0, 0, 0, heightScale, 0, 0, 0, 1));

    //GCloseButtonItem* m_pCloseButtonItem = new GCloseButtonItem(this);
    //m_pCloseButtonItem->setScale(parentItemHeight/m_pCloseButtonItem->boundingRect().height()/25);

    //m_pCloseButtonItem->setPos(50,0);

    //m_pLinearLayout->addItem(qobject_cast<QGraphicsItem*>(m_pCloseButtonItem));
    //m_pLayoutWidget->setLayout(m_pLinearLayout);
    //m_pLayoutWidget->setPos(0,0);
    setPos(0,0);
    parentItemHeight = parentContainerItem->boundingRect().height();
    buttonHeightMax = parentItemHeight/10;
    marginWidth = buttonHeightMax/6;
}

void GOverlayItem::addOverlayButton(QGraphicsItem* pOvBut, int pos)
{
    pOvBut->setScale(buttonHeightMax/pOvBut->boundingRect().height());
    pOvBut->setPos((1 + pos)*marginWidth + pos*buttonHeightMax, marginWidth);
    //numCurrentButtons += 1;
    //qDebug() << numCurrentButtons;
}
