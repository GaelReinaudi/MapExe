#include "GFolderAgent.h"
#include "GFolderGraphicsItem.h"
#include "GRandomPlacerAgent.h"
#include "GGrasshopperPlacerAgent.h"
#include "GGridPlacerAgent.h"
#include "GForcePlacerAgent.h"
#include "GFireworksPlacerAgent.h"

#include "GFileSystemModel.h"

G_REGISTER_PROG_AGENT_CLASS(GFolderAgent)
G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GFolderAgent, QObject)

GFolderAgent::GFolderAgent(QObject *parent)
	: GUriAgent(parent)
	, m_pFileSystem(0)
{
	new GGridPlacerAgent(this);

	static bool firstpass = true;
	if(firstpass) {
		firstpass = false;
		qsrand(5554);
	}
    switch(3 + 0*qrand() % 2) {
	case 0:
		new GGridPlacerAgent(this);
		break;
	case 1:
		new GGrasshopperPlacerAgent(this);
		break;
	case 2:
		new GForcePlacerAgent(this);
		break;
	case 3:
		new GFireworksPlacerAgent(this);
		break;
	}

	// this defines the m_pFileSystem at creation time, when the parent is given.
	// so that when/if the agent is un-parented, this model will still be valid
	FileSystemModel();
}

GFolderAgent::~GFolderAgent()
{

}

void GFolderAgent::PopulateAgentItem(GAgentTabletItem* pTabletItem)
{
	GFolderGraphicsItem* pFoldIt = new GFolderGraphicsItem(this, pTabletItem);
}

void GFolderAgent::Event_DoubleClicked(GAgentTabletItem *pOnWhichItem)
{
    FileSystemModel()->OpenFolder(Info());
}

GFileSystemModel* GFolderAgent::FileSystemModel()
{
	m_pFileSystem = qobject_cast<GFileSystemModel*>(m_pFileSystem);
	if(m_pFileSystem)
		return m_pFileSystem;

	GFolderAgent* pParFolderAgent = qobject_cast<GFolderAgent*>(ParentAgent());
	if(pParFolderAgent)
		return m_pFileSystem = pParFolderAgent->FileSystemModel();
	
	m_pFileSystem = new GFileSystemModel(this);
	m_pFileSystem->setRootPath(Info().path());
	return m_pFileSystem;
}