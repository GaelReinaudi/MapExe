#ifndef GAgentPLUGIN_H
#define GAgentPLUGIN_H
#include "labexegraphicsmap_global.h"

#include "GAgentCreationFilter.h"
#include <QObject>
#include <QtPlugin>

class GAgent;

/////////////////////////////////////////////////////////////////////
//! \brief The GAgentPlugin class defines an interface that has to be used when writing an Agent plugin.
/*!
This is how a user can use his own Agent into the LabExe.
See the Qt help about writing plugins to extend applications.

A GAgentPlugin is not the Agent itself. It is actually a factory that will get called by the GAgentManager 
and that will be able to produced an object (the Agent) from a class derived from GAgent.
*/
class LABEXEGRAPHICSMAP_EXPORT GAgentPlugin : public QObject, public GAgentCreationFilter
{
	Q_OBJECT

public:
	GAgentPlugin(QObject *parent);
	virtual ~GAgentPlugin();

private:
	// This is a function that is defined only through the macro G_REGISTER_PLUGIN_PROG_AGENT
	// It will register the Agent class in the Agent manager.
	virtual void RegisterAgentClassInAgentManager() const = 0;

	friend class GAgentManager;
};

Q_DECLARE_INTERFACE(GAgentPlugin, "Gael.GAgentPlugin/1.0")


/*! \def G_REGISTER_PLUGIN_PROG_AGENT(ClassName)
\brief This is the Macro that makes your derived class from GProgAgent available to the Agent factory. 
You must put it in the GAgentPlugin-derived class header file, in the private section of the class, bellow Q_OBJECT and Q_INTERFACES.

It declares and defines an inline static creator function, CreateAgent(QString uniqueIdentifierName, QObject *parent), 
that will be used to register in the Agent manager.
*/
#define G_REGISTER_PLUGIN_PROG_AGENT(ClassName)																	\
G_1_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName)																		\
void RegisterAgentClassInAgentManager() const {																	\
G_2_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName)																		\
}																													

/*! \def G_REGISTER_2_PLUGIN_PROG_AGENT(ClassName, ClassName2)
\brief same as G_REGISTER_PLUGIN_PROG_AGENT, but for 2 Agents
*/
#define G_REGISTER_2_PLUGIN_PROG_AGENT(ClassName, ClassName2)														\
G_1_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName)																		\
G_1_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName2)																	\
void RegisterAgentClassInAgentManager() const {																	\
G_2_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName)																		\
G_2_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName2)																	\
}																													

/*! \def G_REGISTER_3_PLUGIN_PROG_AGENT(ClassName, ClassName2, ClassName3)
\brief same as G_REGISTER_PLUGIN_PROG_AGENT, but for 3 Agents
*/
#define G_REGISTER_3_PLUGIN_PROG_AGENT(ClassName, ClassName2, ClassName3)											\
G_1_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName)																		\
G_1_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName2)																	\
G_1_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName3)																	\
void RegisterAgentClassInAgentManager() const {																	\
G_2_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName)																		\
G_2_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName2)																	\
G_2_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName3)																	\
}																													

#define G_1_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName)																\
static GAgent* CreatorUsedForTheAgentManager ## ClassName (QString uniqueIdentifierName, QObject *parent) {		\
	static int nProgDevCreated ## ClassName = 0;																	\
	nProgDevCreated ## ClassName ++;																				\
	GAgent* pNDev = 0;																								\
	pNDev = new ClassName(parent, uniqueIdentifierName);															\
	QString strName(pNDev->metaObject()->className());																\
	strName += QString("-%1").arg(nProgDevCreated ## ClassName);													\
	pNDev->setObjectName(strName);																							\
	return pNDev;																									\
}																													\
QString GetClassName() const {return # ClassName ;}


#define G_2_PART_REGISTER_PLUGIN_PROG_AGENT(ClassName)																\
	RegisterProgAgentInManager( # ClassName , CreatorUsedForTheAgentManager ## ClassName );	


#endif // GAgentPLUGIN_H
