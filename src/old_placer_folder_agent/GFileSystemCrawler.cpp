#include "GFileSystemCrawler.h"
#include "GFileSystemModel.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GFileSystemCrawler, QObject)

GFileSystemCrawler::GFileSystemCrawler( QObject *parent, GFileSystemModel* pModelToUse /*= 0*/)
	: GFolderAgent(parent)
	, m_pSystemModel(pModelToUse)
{
	if(!m_pSystemModel)
		m_pSystemModel = GFolderAgent::FileSystemModel();
	if(!m_pSystemModel)
		m_pSystemModel = new GFileSystemModel(this);

	connect(m_pSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT(EventDirectoryLoaded(QString)));
}

GFileSystemCrawler::~GFileSystemCrawler()
{

}

void GFileSystemCrawler::EventDirectoryLoaded( QString path)
{
// 	qDebug() << path;
// 	// making the agents of the content of the index after it has been loaded
// 	QModelIndex indexDirectory = SystemModel()->index(path);
// 	m_pSystemModel->MakeAgentsInIndex(indexDirectory);
}
















