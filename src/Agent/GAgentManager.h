#ifndef GAgentManager_H
#define GAgentManager_H
#include "labexegraphicsmap_global.h"

// to prevent static error linking when defining agents in a library. see loki's doc.
#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT 

#include "loki/Factory.h"
#include "loki/Singleton.h"
#include <QString>
#include <QHash>
#include <QtGlobal>
#include <QObjectCleanupHandler>
#include "../../../LabExe/src/ToolBox/GSerializable.h"
#include "../../../LabExe/src/ToolBox/GObjectFactory.h"
class QMimeData;

class QObject;
class GAgent;
class GAgentCreationFilter;
class GStemCellAgent;
class GAgentShelf;
class GAgentPlugin;
class GAgentManager;

// A custom FactoryError that doesn't throw an exception when OnUnknownType() of GAgent
template <typename IdentifierType, class AbstractProduct>
struct MyAgentFactoryError
{
	struct Exception : public std::exception
	{
		const char* what() const throw() { return "Unknown Type"; }
	};

	static AbstractProduct* OnUnknownType(IdentifierType s)
	{
		QString mess("Type \"%1\" is unknown.");
			mess = mess.arg(QString(s));
			mess += "\r\n Making a stem cell agent instead.";
        qDebug(mess.toUtf8());
		return 0;
	}
};

class _GAgentManager_ToSingletonize_DO_NOT_USE_AS_IS 
	: public Loki::Factory<	  GAgent
							, QString
							, Loki::Typelist<QString, Loki::Typelist< QObject*, Loki::NullType > > 
							, MyAgentFactoryError>
{
public:
	_GAgentManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
	~_GAgentManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
};

/////////////////////////////////////////////////////////////////////
//! \brief The agent manager is an object factory for the abstract class GAgent.
/*!
In order for a Agent to be instantiated by the GAgentManager, it must:
- inherit GAgent
- have a constructor taking as argument: (QString uniqueID, QObject pParentObject).
- have the macro G_REGISTER_AGENT_CLASS at the top of its corresponding .cpp file

If you need to learn what is and what is the use to an object factory, you should look for "factory pattern" on Google,
or better, read "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
It is also a singleton: there is only one instance of agent manager in the program.

In order to use this singleton agent manager you have to use the (typical) syntax:
ex:\code
AgentManagerInstance()->SomeMemberFunction();
\endcode

The Agent manager also store all the pointer to all the created GAgent's (created by himself or not).
It is easy to obtain any GAgent* by using its uniqueIdentifierName:
ex:\code
GAgent* pag = GetAgentFromAgentManager(uniqueIdentifierName);
\endcode
*/
class LABEXEGRAPHICSMAP_EXPORT GAgentManager
	: public QObject 
	, public _GAgentManager_ToSingletonize_DO_NOT_USE_AS_IS
	, public GSerializable
{
	Q_OBJECT

public:
	GAgentManager();
	~GAgentManager();

	//! Returns a list of all the type registered in the factory.
	QList<QString> RegisteredAgentTypes() const;
	//! Returns a list of all the agents that were Add()`ed to the hash.
	QList<GAgent*> AllAgents() const {return m_Agent_ID.keys();}
	//! Returns true if the agent manager contains the uniqueIdentifierName of an agent
	bool Contains(const QString & uniqueIdentifierName) const {return m_ID_Agent.contains(uniqueIdentifierName);}
	//! Returns the Agent whose uniqueIdentifierName matches, 0 otherwise.
	GAgent* GetAgent(const QString & uniqueIdentifierName) const {return m_ID_Agent.value(uniqueIdentifierName, 0);}
	//! Overload provided for convenience. It takes a const QMimeData* from e.g. a DropEvent. 0 if no match.
	GAgent* GetAgent(const QMimeData* theMimeData) const;
	//! Returns the agent with the strTypeID, or tries to generate one given the Type contained in strTypeID.
	GAgent* GetOrGenerateAgent(const QString & strTypeID) const;

	//! Adds the pair <uniqueId, pointer to agent*> to the hash of agents.
	void Add(GAgent* pAgent);
	//! Removes the agent and its uniqueID from the hash.
	int Remove(GAgent* pAgent);

	//! Loads the GAgentPlugin's from the directory AgentPlugins in the application directory. Those will later be used to add Agents in the agent shelf.
	void LoadAgentPlugins();
	//! To be called when the app exist so that we can clean up before the main c++ event loop exits
	void CleanUp();

	//! Creates a new Agent from the provided class name, with a provided unique uniqueIdentifierName. if no parent, sets the parent to be the agent manager.
	GAgent* CreateNewProgAgent(const QString & className, const QString & uniqueIdentifierName, QObject *parent = 0) const;
	//! Creates a new GProgAgent using the class name provided and parent.
	GAgent* CreateNewProgAgent(const QString & className, QObject *parent) const;
	//! Overloaded function that Creates a new GProgAgent using the class name provided and sets the parent to be the agent manager. The ID will be produced by the agent
	GAgent* CreateNewProgAgent(const QString & className) const;
	//! Makes a new Stem Cell Agent, that holds some info about what agent it could become in the future
	GStemCellAgent* CreateNewStemCellAgent(const QString & className, const QString & uniqueIdentifierName = "", QObject *pParent = 0) const;

public:
	//! Re-implemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	void InterpretSettings(QSettings& fromQsettings);

// 	//! Adds a Agent to the Agent shelf. Will create the shelf object if it doesn't exist
// 	void AddAgentToShelf(GAgent* pAgent);
// 	//! Returns the pointer to the hardware shelf.
// 	GAgentShelf* HardwareAgentShelf() const { MakeTheAgentShelf(); return m_pHardAgentShelf; }

protected slots:
	//! clears the agents from the agent manager
	void Clear();

signals:
	//! Emitted when an agent is added to the manager
	void AddedAgent(GAgent* pAgent);
	//! Emitted when an agent is removed from the manager
	void RemovedAgent(GAgent* pAgent);

private:
	//! Hash of the ID to agent
	QHash<QString, GAgent*> m_ID_Agent;
	//! Hash of the agent to ID
	QHash<GAgent*, QString> m_Agent_ID;

public:
// 	//! Registers the GProgAgent`s class name. Called by the macro G_REGISTER_PROG_AGENT_CLASS.
// 	bool RegisterProgAgent(QString ProgAgentsClassName) { m_RegisteredProgAgent.insert(ProgAgentsClassName, "not-used-yet"); return true;}
// 	//! GProgAgent`s class name that have been registered with the macro G_REGISTER_PROG_AGENT_CLASS.
// 	QList<QString> RegisteredProgAgents() const { return m_RegisteredProgAgent.keys(); }

	//! intended to help cleanup when creating Agents with parent = 0
	mutable QObjectCleanupHandler* m_pCleanUpAgents;
	//! List of registered agent creation filters that will be used to provide the apropriate agent when dropping or transmitting data
	QList<GAgentCreationFilter*> m_CreationFilters;
};

typedef Loki::SingletonHolder<GAgentManager> GAgentManagerSing;

#endif // GAgentManager_H

