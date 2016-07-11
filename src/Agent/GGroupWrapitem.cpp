#include "GGroupWrapitem.h"
#include "GGroupItem.h"
#include "GGroupAgent.h"

GGroupWrapitem::GGroupWrapitem(GAgent* pAgent, QGraphicsItem* pParentItem)
    : GAgentWrapitem(pAgent, pParentItem)
{
}

GGroupWrapitem::~GGroupWrapitem()
{

}

void GGroupWrapitem::Event_DroppedAgent( GAgentWrapitem* pAgWrap )
{
// 	GGroupItem* theGroupItem = qgraphicsitem_cast<GGroupItem*>(ContentItem());
// 	if(!theGroupItem)
// 		return;
// 	theGroupItem->addToGroup(pAgWrap);
}

void GGroupWrapitem::PopulateSettings( QSettings& inQsettings )
{
	GAgentWrapitem::PopulateSettings(inQsettings);

// 	// top level wrapitems in the group
// 	QList<GAgentWrapitem*> listChildWrapItems;
// 	foreach(QGraphicsItem* pAgIt, ContentItem()->childItems()) {
// 		GAgentWrapitem* pWrapItem = qgraphicsitem_cast<GAgentWrapitem*>(pAgIt);
// 		if(pWrapItem)
// 			listChildWrapItems.append(pWrapItem);
// 	}
// 	// save the list of top item made above
// 	GSerializable::SaveListAndItems<GAgentWrapitem>(inQsettings, "Items", listChildWrapItems);
}

void GGroupWrapitem::InterpretSettings( QSettings& fromQsettings )
{
	GAgentWrapitem::InterpretSettings(fromQsettings);
}
