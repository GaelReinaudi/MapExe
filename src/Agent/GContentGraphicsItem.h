#ifndef GContentGraphicsItem_H
#define GContentGraphicsItem_H

#include "GAgentWrapitem.h"

/////////////////////////////////////////////////////////////////////
//! \brief Base class just to group things in the doxygen class hierarchy
class ContentGraphicsItem {
};

/////////////////////////////////////////////////////////////////////
//! \brief The GContentGraphicsItem docccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
/*!
*/
template <typename DerivedItem, typename AgentType> 
class GContentGraphicsItem : public DerivedItem, public ContentGraphicsItem
{
public:
    GContentGraphicsItem(AgentType* pAgent, GAgentWrapitem* parentItem = 0);

	virtual ~GContentGraphicsItem();
//    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget = 0);

	//! return a pointer to the agent that manages this graphics item. The type returned matches the corresponding AgentType provided during the specialisation of the template class
	AgentType* Agent() const {return m_pAgent;}

	//! Re-implemented in order to inform when the content size changed.
	virtual QRectF boundingRect() const {
		QRectF oldRect = m_LastMonitoredBoundingRect;
		m_LastMonitoredBoundingRect = DerivedItem::boundingRect();
		if(oldRect != m_LastMonitoredBoundingRect)
			Event_BoundingRectChanged(m_LastMonitoredBoundingRect);
		return m_LastMonitoredBoundingRect;
	}

protected:
	//! Called when the bounding rect of the DerivedItem item changed
	virtual void Event_BoundingRectChanged(QRectF & theNewRect) const {
		GAgentWrapitem* pParIt = static_cast<GAgentWrapitem*>(DerivedItem::parentItem());
		if(pParIt) {
			pParIt->Event_ContentRectChanged(theNewRect);
		}
	}
    void mousePressEvent(QGraphicsSceneMouseEvent* event) {
        QGraphicsItem::mousePressEvent(event);
        //setZValue(1);
    }

public:
	//! holds a pixmap screen-shot of the item at scale 1
	QPixmap m_ScreenShotFullScale;
	//! the graphics item holding the pixmap
	QGraphicsPixmapItem* m_pScreenShotItem;

private:
	//! pointer to the managing agent
	AgentType* m_pAgent;
	//! memory of what is the bounding rect so that we can react if it changes
	mutable QRectF m_LastMonitoredBoundingRect;
	
};

template <typename DerivedItem, typename AgentType>
GContentGraphicsItem<DerivedItem, AgentType>::GContentGraphicsItem(AgentType* pAgent, GAgentWrapitem* parentItem /*= 0*/)
	: DerivedItem(parentItem)
	, m_pAgent(pAgent)
	, m_pScreenShotItem(0)
{
	Q_ASSERT(m_pAgent != 0);
    //QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable);
}

template <typename DerivedItem, typename AgentType>
GContentGraphicsItem<DerivedItem, AgentType>::~GContentGraphicsItem()
{

}

//template <typename DerivedItem, typename AgentType>
//void GContentGraphicsItem<DerivedItem, AgentType>::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget /*= 0*/) {
//    QStyleOptionGraphicsItem myoption = (*option);
//    myoption.state &= !QStyle::State_Selected;
//    QGraphicsItem::paint(painter, &myoption, widget);
//}

#endif // GContentGraphicsItem_H
