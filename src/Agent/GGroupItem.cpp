#include "GGroupItem.h"
#include "GGroupAgent.h"
#include "GAgentManager.h"
#include "SceneView/GMapGraphicsScene.h"

GGroupItem::GGroupItem(GGroupAgent* pAgent, GAgentWrapitem* parentItem)
	: GContentGraphicsItem<QGraphicsItemGroup, GGroupAgent>(pAgent, parentItem)
	, QObject(0)
{
	GGroupAgent* pGroup = qobject_cast<GGroupAgent*> (pAgent);
	if(!pGroup)
		return;
	QList<QString> allPaths = pGroup->m_SubAgentsIDs.keys();
	int numPages = pGroup->m_SubAgentsIDs.count();
	int widthGrid = qRound(qSqrt(numPages));

	for(int ithPage = 0; ithPage < numPages ; ithPage++) {
		QString uid = pGroup->m_SubAgentsIDs.value(allPaths[ithPage]).toString();
		GAgent* pPage = AgentManagerInstance()->GetAgent(uid);
		int i = ithPage % widthGrid;
		int j = ithPage / widthGrid;
		double hPos = i * 2100.0;
		double vPos = j * 2100.0;
		QGraphicsItem* pIt = pPage->ProvideNewAgentWrappingItem(qobject_cast<GMapGraphicsScene*>(parentItem->scene()));
		pIt->setPos(parentItem->pos().x() + hPos, parentItem->pos().y() + vPos);
//		QGraphicsItem* pIt = pPage->ProvideNewAgentWrappingItem(parentItem);
// 		pIt->setPos(hPos, vPos);
		parentItem->deleteLater();
		pAgent->deleteLater();
	}
//	parentItem->SetRect(parentItem->childrenBoundingRect());
}

GGroupItem::~GGroupItem()
{

}

void GGroupItem::Event_DroppedAgent( GAgentWrapitem* pAgWrap )
{
	addToGroup(pAgWrap);
	Agent()->InsertAgent(pAgWrap);
}
