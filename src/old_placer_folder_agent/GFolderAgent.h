#ifndef GFOLDERAGENT_H
#define GFOLDERAGENT_H

#include "GUriAgent.h"
class GFileSystemModel;

/////////////////////////////////////////////////////////////////////
//! \brief The GFolderAgent class is a GAgent class that takes care of representing and manipulating a folder.
/*!
*/
class GFolderAgent : public GUriAgent
{
 	Q_OBJECT

public:
	GFolderAgent(QObject *parent);
	~GFolderAgent();

protected:
	//! Re-implemented 
	void PopulateAgentItem(GAgentTabletItem* pTabletItem);

public:
	//! Can be reimplemented by a subclass in order to point to a specific GFileSystemModel object.
    virtual GFileSystemModel* FileSystemModel();

protected:
	//! Re-implemented in order to open the folder
	virtual void Event_DoubleClicked(GAgentTabletItem *pOnWhichItem);

private:
	//! variable that points to a file system model that can be used to open and explore the folder.
	GFileSystemModel* m_pFileSystem;
	
};

#endif // GFOLDERAGENT_H
