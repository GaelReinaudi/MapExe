#include "GFileAgent.h"
#include "GFileGraphicsItem.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GFileAgent, QObject)

GFileAgent::GFileAgent(QObject* parent)
	: GUriAgent(parent)
{
}

GFileAgent::~GFileAgent()
{

}

void GFileAgent::PopulateAgentItem(GAgentTabletItem* pTabletItem)
{
	GFileGraphicsItem* pFileIt = new GFileGraphicsItem(this, pTabletItem);
}

