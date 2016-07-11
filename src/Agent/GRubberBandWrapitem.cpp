#include "GRubberBandWrapitem.h"
#include "Agent/GAgentManager.h"
#include "SceneView/GMapGraphicsScene.h"
#include "GGroupItem.h"

GRubberBandWrapitem::GRubberBandWrapitem(GAgent* pAgent, QGraphicsItem* pParentItem)
    : GAgentWrapitem(pAgent, pParentItem)
	, m_IsFixed(false)
	, m_DidGrow(false)
{
}

void GRubberBandWrapitem::SetFixed()
{
	m_IsFixed = true;
}

void GRubberBandWrapitem::MoveEndPoint( QPointF cornerPos )
{
	if(!m_IsFixed) {
		RubberItem()->setRect(QRectF(QPointF(0,0), cornerPos - pos()));
		m_DidGrow = true;
	}
}

void GRubberBandWrapitem::CreateGroup()
{
    // TEMP for making/testing group
    QList<QGraphicsItem*> itemsInside = collidingItems();
    // making list of agent wrapitems
    QList<GAgentWrapitem*> wrapperInside;
    QList<QGraphicsItem*> wrapperInsideItem;
    foreach(QGraphicsItem* pIt, itemsInside) {
        GAgentWrapitem* pAgWrap = qgraphicsitem_cast<GAgentWrapitem*>(pIt);
        if(pAgWrap) {
            wrapperInside.append(pAgWrap);
            wrapperInsideItem.append(pAgWrap);
        }
    }
    // if empty, exit
    if(wrapperInside.isEmpty())
        return;

    // make dummy group item
    QGraphicsItemGroup* dummyGroup = scene()->createItemGroup(wrapperInsideItem);
    QPointF theGroupPos = dummyGroup->boundingRect().topLeft();
    QSizeF theGroupSize = dummyGroup->boundingRect().size();

    // make group agent
    GAgentWrapitem* pAgentWrapitem = 0;
    GAgent* pAgent = AgentManagerInstance()->CreateNewProgAgent("GGroupAgent");
    if(!pAgent)
        return;
    // make wrapper that will forward the "drop" events to the actual groupitem
    pAgentWrapitem = pAgent->ProvideNewAgentWrappingItem(qobject_cast<GMapGraphicsScene*>(scene()));
    pAgentWrapitem->setPos(theGroupPos);

    // simulate a drop of all the wrapitems in the group wrapitem
	GGroupItem* theGroupItem = qgraphicsitem_cast<GGroupItem*>(pAgentWrapitem->ContentItem());
	if(theGroupItem) {
		foreach(GAgentWrapitem* pAgWrap, wrapperInside) {
			QMetaObject::invokeMethod(
				theGroupItem
				, "Event_DroppedAgent"
				, Qt::QueuedConnection
				, Q_ARG(GAgentWrapitem*, pAgWrap)
				);
		}
	}

	// hide the rubber band, we don't need it anymore
	hide();
}

void GRubberBandWrapitem::keyPressEvent(QKeyEvent* event)
{
	GAgentWrapitem* pAgentWrapitem = 0;
	GAgent* pAgent = AgentManagerInstance()->CreateNewProgAgent("GTextAgent");
	if(!pAgent) 
		return;

	pAgentWrapitem = pAgent->ProvideNewAgentWrappingItem(qobject_cast<GMapGraphicsScene*>(scene()));

	pAgentWrapitem->setPos(pos() + Rect().topLeft());
	pAgentWrapitem->SetPreferredSize(Rect().size());

	scene()->sendEvent(pAgentWrapitem, event);
}

