#ifndef GCONTENTAGENT_H
#define GCONTENTAGENT_H

#include "agent.h"
#include "Agent/GAgentWrapitem.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GContentAgent is a 
/*!
*/
template <typename DerivedAgent, typename GraphicsClass>
class GContentAgent : public GAgent
{
public:
	GContentAgent(QObject *pParent, QString uniqueIdentifierName = "")
		: GAgent(pParent, uniqueIdentifierName)
	{
		// forces the unique ID to be made by AssignNewUniqueID() if it is "".
		if(uniqueIdentifierName == "")
			AssignNewUniqueID(DerivedAgent::staticMetaObject.className());
	}
	virtual ~GContentAgent() {}

protected:
	//! Re-implemented to provide the graphics class passed in the template argument.
	virtual QGraphicsItem* NewContentItem(GAgentWrapitem* pParentItem) {
		GraphicsClass* pCont = new GraphicsClass(static_cast<DerivedAgent*>(this), pParentItem);
//		connnnnnect(pCont, SIGNAL(), this, SLOT());
		return pCont;
	}

private:
};

#endif // GCONTENTAGENT_H
