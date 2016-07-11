#ifndef GFILESYSTEMMODEL_H
#define GFILESYSTEMMODEL_H

#include <QFileSystemModel>
class GAgent;

/////////////////////////////////////////////////////////////////////
//! \brief The GFileSystemModel class specializes the QFileSystemModel in order to be convenient to use in relation with an underlying GAgent structure.
/*!
more...
*/
class GFileSystemModel : public QFileSystemModel
{
 	Q_OBJECT

public:
	GFileSystemModel(GAgent *parent = 0);
	~GFileSystemModel();

	//! Returns the agent that corresponds to a given index in the model
	GAgent* AgentFromIndex(const QModelIndex & index);

public slots:
	//! Makes the agent corresponding to the children of the index if they don't exist already in the Hash. 
	void MakeAgentsInIndex(const QModelIndex & parent, int start = 0, int end = -1);
	//! slot version of fetchMore that explores from the list m_IndexesToExplore
	void ExploreMore();
	//! Appends the path to the list of folder to explore, m_IndexesToExplore, and then calls ExploreMore().
	void Explore(QString path);
	//! Opens the folder corresponding to the QFileInfo
	void OpenFolder(QFileInfo folderInfo);


private:
	//! Returns the depth level of a path compared to the root folder. Exploring the root, is level 0, exploring the subfolders of the root is level 1,...
	int DepthFromRoot(QString dirPath);

private:
	//! Hash holding the parity index <-> GAgent
	QHash<qint64, GAgent*> m_IndexAgent;
	//! test for exploring folders at startup
	int m_InitialCrawlDepth;
	//! A list of indexes to explore that could help do this in a separate thread
	QList<QModelIndex> m_IndexesToExplore;
};

#endif // GFILESYSTEMMODEL_H
