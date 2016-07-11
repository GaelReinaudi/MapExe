#ifndef GFILEAGENT_H
#define GFILEAGENT_H

#include "GUriAgent.h"

class GFileAgent : public GUriAgent
{
//    Q_OBJECT
public:
    GFileAgent(QObject* parent);
    ~GFileAgent();

protected:
	//! Re-implemented 
	void PopulateAgentItem(GAgentTabletItem* pTabletItem);

private:
    
};

#endif // GFILEAGENT_H
