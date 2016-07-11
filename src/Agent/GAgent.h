#ifndef GAGENT_H
#define GAGENT_H
#include "labexegraphicsmap_global.h"

#include "../../../LabExe/src/ToolBox/GSerializable.h"
#include "../../../LabExe/src/ToolBox/GObjectFactory.h"

#include <QThread>
class QGraphicsItem;
class GMapGraphicsScene;
class GAgentTabletItem;
class GAgentWrapitem;
class GAgentManager;
class GItemManager;
class GStemCellAgent;
class QGraphicsView;
class GOverlayItem;

//! Returns the Agent manager
LABEXEGRAPHICSMAP_EXPORT GAgentManager* AgentManagerInstance(); 
//! Returns the Item manager
LABEXEGRAPHICSMAP_EXPORT GItemManager* ItemManagerInstance(); 
//! Loads the plugins in the Agent manager
LABEXEGRAPHICSMAP_EXPORT void LoadPluginsInAgentManager(); 
//! Logs a user action on a monitoring website. Does this in a separate thread from QThreadPool::globalInstance()->start()
LABEXEGRAPHICSMAP_EXPORT void LogUserAction(QString strAction); 
//! Makes a Scene that implements all the desired features for a graphics map containing agents
LABEXEGRAPHICSMAP_EXPORT GMapGraphicsScene* MakeGraphicsMap(QObject* parent); 
//! Makes a View for the given scene map that implements all the desired features for a graphics map containing agents
LABEXEGRAPHICSMAP_EXPORT QGraphicsView* MakeGraphicsView(QWidget* parent, GMapGraphicsScene* forScene); 



//typedef GAgentTabletItem GAgentWrapitem;
typedef GAgentWrapitem GAgentWrapitem;


/////////////////////////////////////////////////////////////////////
//! \brief The GAgent class is the base abstract class of any agent in the scene bench and that can provide a graphics item.
/*!
The user interface will be provided by calling the ProvideNewAgentWrappingItem() function. The provided GraphicsItem 
should be in charge of the interaction with the agent (e.g. updates from the agent, or click events to the agent). 

- Agents can parent other agents and for a tree structure. 
- Agents can react at a child being added by re-implementing the QObject::childEvent() method.
- By default a signal ChildAgentAdded() is emitted when an agent becomes a child of this GAgent object.
*/
class LABEXEGRAPHICSMAP_EXPORT GAgent : public QThread, public GSerializable
{
	Q_OBJECT

public:
	GAgent(QObject *parent, QString uniqueIdentifierName = "");
	virtual ~GAgent();

	//! Returns the direct children agent of this agent.
	QList<GAgent*> ChildAgents() const;
	//! Returns the siblings agent of this agent, i.e. the child agent of its parent, except itself.
	QList<GAgent*> SiblingAgents() const;
	//! Returns the Parent agent of this agent. 0 if none.
	GAgent* ParentAgent() const;
	//! Represents the best level of zoom in any existing scene/view
	double BestZoomedIn() const { return m_BestZoomedIn; }
    //! Removes the parent from an agent. This is for removing folders from parent folders to disengage the hierarchy so that folders don't drag child folders.
    void UnParent();

// protected:
	//! Returns a new graphics item that represents the object. 
    virtual GAgentWrapitem* ProvideNewAgentWrappingItem(QGraphicsItem* pParentItem);

public:
	//! For convenience, adds the item as a top item of the scene
	GAgentWrapitem* ProvideNewAgentWrappingItem(GMapGraphicsScene* pScene);

	//! Convenient function that tells if this agent is a GStemCellAgent. If so, it returns the pointer to itself.
	virtual GStemCellAgent* isStemCell() {return 0;}
	//! Called in order to put some content in an agent item
	virtual QGraphicsItem* NewContentItem(GAgentWrapitem* pParentItem) = 0;

	//! Must be implemented to provide the agent with initialization data. Must return true if, and only if, the data where accepted by the agent.
	virtual bool InjectInitializationData(const QVariantList & theDataList) = 0;
	//! Must be implemented to provide sufficient data that will be saved and used latter to InjectInitializationData() in a newly created agent.
	virtual QVariantList ProvideInitializationData() = 0;

	//! for menus population
	virtual void PopulateOverlay(GOverlayItem* pOlIt);

public:
	//! Re-implemented 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented 
	virtual void InterpretSettings(QSettings& fromQsettings);

public slots:
	//! removes from the system and deleteLater() the agent
	void Destroy();

protected:

	//! Called by a graphics item when it is double-clicked. pOnWhichItem is the graphics item that actually got clicked. Default does nothing.
	virtual void Event_DoubleClicked(GAgentWrapitem* pOnWhichItem) {}
	//! Called when a view changed the level of zoom on this agent
	virtual void Event_ViewZoomChanged(double zoomVal) {m_PreviousBestZoomedIn = m_BestZoomedIn; m_BestZoomedIn = zoomVal;}

protected:
	//! Re-implemented. Checks if the child is a GAgent and send the appropriate signal (e.g. to the graphics items).
	void childEvent(QChildEvent* pChildEvent);
	//! Re-implemented to add the new ID in the AgentManagerInstance(). It doesn't remove the previous ID.
	void Event_UniqueSystemIDChanged();

signals:
	//! Emitted when a GAgent child is added to this GAgent object
	void ChildAgentAdded(GAgent* pAgent);
	//! Emitted when the agent wants to unload the displayed content
	void UnloadContentRequested();
	//! Emitted when the agent wants to load the displayed content
	void LoadContentRequested();
	//! Emitted when the agent was un-parented from its parent object.
    void UnParented();

private:
	//! adds this agent in the agent manager. If it was already added with another name, the old name is not removed. Both names will give this device when calling GDeviceManager::GetAgent();
	void AddToAgentManager() const;
protected:
	//! Represents the best level of zoom in any existing scene/view
	double m_BestZoomedIn;
	double m_PreviousBestZoomedIn;
};

Q_DECLARE_METATYPE(GAgent*)

/////////////////////////////////////////////////////////////////////
//! \brief GHasAgent is a class that defines something that is linked to a GAgent object.
/*!
It eases the communication between object in the program and the corresponding agent
*/
class LABEXEGRAPHICSMAP_EXPORT GHasAgent
{

public:
	GHasAgent(GAgent* pAgent) {m_pAgent = pAgent;}
	virtual ~GHasAgent() {}

	//! Returns the agent that is provided in the constructor.
	GAgent* TheAgent() const { return m_pAgent; }

protected:
	//! The agent that is provided in the constructor.
	GAgent* m_pAgent;

};

//! Registers the prog device into the device manager.
LABEXEGRAPHICSMAP_EXPORT bool RegisterProgAgentInManager(const QString & className, GAgent*(deviceCreator)(QString, QObject*)); 

//! Registers a filter that will be compared to dropped and propose an action.
LABEXEGRAPHICSMAP_EXPORT bool RegisterMimeFilterInManager(const QString & className, GAgent*(deviceCreator)(QString, QObject*)); 

/*! \def G_REGISTER_PROG_AGENT_CLASS(ClassName)
\brief This is the Macro that makes your derived class from GProgAgent available to the Agent factory. 
You must put it in your derived class cpp file.If your Agent is defined in a plugin, 
you should use the G_REGISTER_PLUGIN_PROG_AGENT instead (in the .h file).

It is this that allows you to de-serialize derived Agent using the name of the corresponding class. 
Take a look here: 
- "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
*/
#define G_REGISTER_PROG_AGENT_CLASS(ClassName)														\
	namespace																						\
{ 																									\
	int	nProgAgCreated ## ClassName = 0;																		\
	/*! Definition of a function that returns a new instance of a Agent !*/						\
	GAgent* CreatorUsedForTheAgentManager ## ClassName(QString uniqueIdentifierName, QObject *parent)							\
	{																								\
		nProgAgCreated ## ClassName++;																			\
		GAgent* pNAg = 0;																			\
		pNAg = new ClassName(parent, uniqueIdentifierName);						\
/*		pNAg->ChangeUniqueSystemID(uniqueIdentifierName);*/	\
		QString strName(pNAg->metaObject()->className());											\
		strName += QString("-%1").arg(nProgAgCreated ## ClassName);												\
		pNAg->setObjectName(strName);																		\
		return pNAg;																				\
	}																								\
	const bool regPrDe = RegisterProgAgentInManager( # ClassName , CreatorUsedForTheAgentManager ## ClassName);		\
}  



#endif // GAGENT_H
