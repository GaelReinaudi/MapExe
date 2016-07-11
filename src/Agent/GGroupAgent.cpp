#include "GGroupAgent.h"
#include "GGroupItem.h"
#include "GGroupWrapitem.h"
#include "GAgentManager.h"

G_REGISTER_PROG_AGENT_CLASS(GGroupAgent)

GGroupAgent::GGroupAgent(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: GContentAgent<GGroupAgent, GGroupItem>(parent, uniqueIdentifierName)
{

}

GGroupAgent::~GGroupAgent()
{

}

bool GGroupAgent::InjectInitializationData( const QVariantList & theDataList )
{
	if(theDataList.isEmpty())
		return false;

//	m_SubAgentsIDs = theDataList.first().toHash();

	foreach(const QVariant & v, theDataList) {
		QString imagePath = v.toString();
		GAgent* pAg = AgentManagerInstance()->CreateNewProgAgent("GPictureAgent");
		pAg->InjectInitializationData(QVariantList() << imagePath);
 		m_SubAgentsIDs.insert(imagePath, pAg->UniqueSystemID());
	}
	return true;
}

QVariantList GGroupAgent::ProvideInitializationData()
{
	QVariantList listToRet;

	listToRet << m_SubAgentsIDs;
// 	foreach(QString s, m_SubAgentsIDs.keys()) {
// 		// try to put all the initialization of each itemsQVariantList
// 		listToRet << s;
// 	}
	return listToRet;
}