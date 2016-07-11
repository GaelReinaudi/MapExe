#include "GFileSystemModel.h"
#include "GFolderAgent.h"
#include "GFileAgent.h"
#include <QTimer>

GFileSystemModel::GFileSystemModel(GAgent *parent /*= 0*/)
	: QFileSystemModel(parent)
    , m_InitialCrawlDepth(2)
{
	connect(this, SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(MakeAgentsInIndex(QModelIndex, int, int)), Qt::UniqueConnection);

	// very bad and dirty
	QTimer::singleShot(4000, this, SLOT(ExploreMore()));

//	setResolveSymlinks(true);
}

GFileSystemModel::~GFileSystemModel()
{

}

GAgent* GFileSystemModel::AgentFromIndex( const QModelIndex & index )
{
	// is there any pointer to an agent that matches the index
	GAgent* pAgent = m_IndexAgent.value(index.internalId());
	// if so, return it
	if(pAgent)
		return pAgent;

	// if not, lets see if it matches the root dirPath
	QString pathItem = filePath(index);
	// if so, we return the Agent used in the constructor of this class
	if(pathItem == rootPath())
		pAgent = qobject_cast<GAgent*>(QObject::parent());
	if(pAgent)
		m_IndexAgent.insert(QPersistentModelIndex(index).internalId(), pAgent);

	return pAgent;
}

/////////////////////////////////////////////////////////////////////
/*!
If start and end are not provided, all the children are explored.
\param:  const QModelIndex & parent : the index to create the Agent for
\param:  int start : first row of the children to consider
\param:  int end : last row (included) of the children to consider
*////////////////////////////////////////////////////////////////////
void GFileSystemModel::MakeAgentsInIndex(const QModelIndex & parent, int start /*= 0*/, int end /*= -1*/)
{
// 	QString parentPath = filePath(parent);
// 	qDebug() << "MakeAgentsInIndex" << parentPath;
	// the parent index should have an agent
	GAgent* pParAgent = AgentFromIndex(parent);
	if(!pParAgent)
		return;
	if(end == -1)
		end += rowCount(parent);

	int indexAdded = 0;
	// make the agents
	for(int irow = start; irow <= end; irow++) {
		QModelIndex childIndex = parent.child(irow, 0);
		// if not valid or if it already exist as an agent, let's not make it.
		if(!childIndex.isValid() || m_IndexAgent.contains(childIndex.internalId()))
			continue;
		// otherwise we make it
		GUriAgent* pAgent = 0;
		if(isDir(childIndex)) {
			pAgent = new GFolderAgent(pParAgent);
			// should we explore deeper
			if(DepthFromRoot(filePath(childIndex)) <= m_InitialCrawlDepth) {
				m_IndexesToExplore.append(childIndex);
			}
		}
		else {
            pAgent = new GFileAgent(pParAgent);
		}
		pAgent->SetInfo(fileInfo(childIndex));

		// set the the pointer to the agent in our hash. 
		m_IndexAgent.insert(QPersistentModelIndex(childIndex).internalId(), pAgent);
		indexAdded++;

// 		// Did the index have already some children. If so, we want to create the corresponding agent as well.
// 		bool recursiveChildMaking = hasChildren(childIndex);
// 		if(recursiveChildMaking) {
// 			int numChildren = rowCount(childIndex);
// 			if(numChildren > 0)
// 				MakeAgentsInIndex(childIndex, 0, numChildren - 1);
// 		}

	}
// 	qDebug() << "Made" << indexAdded << "agents in" << filePath(parent);
	DepthFromRoot(filePath(parent));
}

int GFileSystemModel::DepthFromRoot( QString dirPath )
{
	QDir theRoot = rootDirectory();
	QDir dirForPath = QDir(dirPath);
	QString relPath = dirForPath.relativeFilePath(theRoot.path());

	return relPath.count("../");
}

void GFileSystemModel::ExploreMore()
{
	if(m_IndexesToExplore.isEmpty())
		return;
	QModelIndex index = m_IndexesToExplore.takeFirst();
	fetchMore(index);
	// call that function again latter to go through the list
	QTimer::singleShot(400, this, SLOT(ExploreMore()));
}

void GFileSystemModel::OpenFolder( QFileInfo folderInfo )
{
    Explore(folderInfo.filePath());
}

void GFileSystemModel::Explore( QString path )
{
	m_InitialCrawlDepth = 0;
	QModelIndex folderIndex = index(path);
	if(!folderIndex.isValid())
		return;

	m_IndexesToExplore.append(folderIndex);
	ExploreMore();
	qDebug() << path;
}
