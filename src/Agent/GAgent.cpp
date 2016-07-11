#include "GAgent.h"
#include "GAgentManager.h"												
#include "SceneView/GMapGraphicsScene.h"												
#include "SceneView/GMapGraphicsView.h"												
#include "GAgentWrapitem.h"
#include "GOverlayItem.h"
#include "GCloseButtonItem.h"

#include "Logger/GUserActionLogger.h"

#include <QChildEvent>

GAgentManager* AgentManagerInstance() { 
	return &(GAgentManagerSing::Instance()); 
}

GItemManager* ItemManagerInstance() { 
	return GAgentWrapitem::ItemManager(); 
}

bool RegisterProgAgentInManager( const QString & className, GAgent*(deviceCreator)(QString, QObject*) ) {
	return AgentManagerInstance()->Register(className, deviceCreator);
}

void LoadPluginsInAgentManager() {
	AgentManagerInstance()->LoadAgentPlugins();
}

GMapGraphicsScene* MakeGraphicsMap( QObject* parent ) {
	return new GMapGraphicsScene(parent);
}

QGraphicsView* MakeGraphicsView( QWidget* parent, GMapGraphicsScene* forScene ) {
	GMapGraphicsView* pView = new GMapGraphicsView(parent);
	pView->setScene(forScene);
	QObject::connect(forScene, SIGNAL(sceneRectChanged(QRectF)), pView, SLOT(UpdateFromSceneRect(QRectF)),Qt::QueuedConnection);
	return pView;
}

void LogUserAction( QString strAction ) {
//	GUserActionLogger *log = new GUserActionLogger(strAction);
//	log->start();
}

GAgent::GAgent(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: QThread(parent)
	, GSerializable(uniqueIdentifierName)
	, m_BestZoomedIn(0.0)
	, m_PreviousBestZoomedIn(0.0)
{
	AddToAgentManager();

	GAgent* pParentAgent = ParentAgent();
	if(pParentAgent)
		emit pParentAgent->ChildAgentAdded(this);
}

GAgent::~GAgent()
{
//2012-09-04	AgentManagerInstance()->Remove(this);
}

/////////////////////////////////////////////////////////////////////
/*!
NOTE: most of the time, this thing will not emit. Because the created object that is becoming a child is not finished constructed.
it will only work when setParent is called on the child or other mechanism adds the fully constructed child to this agent.
is the added child a GAgent
*////////////////////////////////////////////////////////////////////
void GAgent::childEvent( QChildEvent* pChildEvent )
{
	GAgent* pChildAgent = qobject_cast<GAgent*>(pChildEvent->child());
	if(pChildEvent->added() && pChildAgent)
		emit ChildAgentAdded(pChildAgent);
}

QList<GAgent*> GAgent::ChildAgents() const
{
	QList<GAgent*> listAgentChildren;
	foreach(QObject* pObj, QObject::children()) {
		GAgent* pAg = qobject_cast<GAgent*>(pObj);
		if(pAg)
			listAgentChildren.append(pAg);
	}
	return listAgentChildren;
}

GAgent* GAgent::ParentAgent() const
{
	return qobject_cast<GAgent*>(QObject::parent());
}

QList<GAgent*> GAgent::SiblingAgents() const
{
	QList<GAgent*> listSibAg;
	GAgent* pParAgent = ParentAgent();
	if(pParAgent) {
		listSibAg = pParAgent->ChildAgents();
		listSibAg.removeOne(const_cast<GAgent *const>(this));
	}
	return listSibAg;
}

// When this function is called needs to be tweaked. Currently, the UnParenting takes place too early, so that as sibling folders get fired,
// they have no knowledge of where previously fired, and now un-parented, siblings have been placed. This causes lots of overlap.
void GAgent::UnParent()
{
// 	GAgent* pParentAgent = ParentAgent();
// 	if(pParentAgent)
// 		emit pParentAgent->ChildAgentRemoved(this);

	setParent(AgentManagerInstance());
	emit UnParented();
}

void GAgent::Event_UniqueSystemIDChanged()
{
	AddToAgentManager();
}

GAgentWrapitem* GAgent::ProvideNewAgentWrappingItem(QGraphicsItem* pParentItem )
{
	GAgentWrapitem* pAgentItemContainer = new GAgentWrapitem(this, pParentItem);
	return pAgentItemContainer;
}

GAgentWrapitem* GAgent::ProvideNewAgentWrappingItem(GMapGraphicsScene* pScene)
{
	GAgentWrapitem* pAgentItemContainer = ProvideNewAgentWrappingItem((QGraphicsItem*)(0));
	if(pScene && pAgentItemContainer)
		pScene->AddItem(pAgentItemContainer);
	return pAgentItemContainer;
}

void GAgent::PopulateSettings( QSettings& inQsettings )
{
	GSerializable::PopulateSettings(inQsettings);
	inQsettings.setValue("zoom", BestZoomedIn());

	QList<GAgent*> listAgents = ChildAgents();
	if(!listAgents.isEmpty())
		GSerializable::SaveListAndItems<GAgent>(inQsettings, "AgentChildren", listAgents);
}

void GAgent::InterpretSettings( QSettings& fromQsettings )
{
	GSerializable::InterpretSettings(fromQsettings);
	m_BestZoomedIn = fromQsettings.value("zoom", 0.0).toDouble();
	m_PreviousBestZoomedIn = m_BestZoomedIn;

	GSerializable::RestoreListOfObjects(fromQsettings, "AgentChildren", this);
}

void GAgent::AddToAgentManager() const
{
	QString theUniqueID = UniqueSystemID();
	// put it in the device manager
	if(AgentManagerInstance()->Contains(theUniqueID))
		qWarning() << "Agent has not been given a unique identifier:" << theUniqueID << "already exists";
	else
		AgentManagerInstance()->Add(const_cast<GAgent*>(this));
}

void GAgent::Destroy()
{
	AgentManagerInstance()->Remove(this);
	deleteLater();
}

void GAgent::PopulateOverlay(GOverlayItem* pOlIt)
{
    GCloseButtonItem* m_pCloseButtonItem = new GCloseButtonItem(pOlIt);
    pOlIt->addOverlayButton(m_pCloseButtonItem, 0);
}
