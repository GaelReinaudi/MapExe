#include "GFileSystemDevice.h"
#include "GFileSystemCrawler.h"
#include "GFileSystemModel.h"

G_REGISTER_PROG_DEVICE_CLASS(GFileSystemDevice)

GFileSystemDevice::GFileSystemDevice(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: GProgDevice(parent, uniqueIdentifierName)
	, GHasAgent(IsShelvedInstance() ? 0 : new GFileSystemCrawler(this)) //can't do that because of creation order// , m_pFileSystemModel))
	, m_Path("Path", this)
	, m_pFileSystemModel(0)
{
	m_Path = QDir::homePath();
	// is the parent a GDeviceShelf? Because if it is, we might not want to use the crawler agent's QFileSystemModel
	// gets the crawler to use its QFileSystemModel object
	GFileSystemCrawler* pCrawler = qobject_cast<GFileSystemCrawler*>(TheAgent());
	if(pCrawler)
		m_pFileSystemModel = pCrawler->GFileSystemCrawler::FileSystemModel(); // GFileSystemCrawler:: because virtual function in constructor is undefined

	if(IsShelvedInstance()) {
		m_pFileSystemModel = new QFileSystemModel(this);
	}
	else {
	}
	ChangeRootDirectory(m_Path);

// 	connect(&m_Path, SIGNAL(ValueDidChange(QString)), this, SLOT(ChangeRootDirectory(QString)));
}

GFileSystemDevice::~GFileSystemDevice()
{

}

void GFileSystemDevice::PopulateDeviceWidget(GDeviceWidget* theDeviceWidget)
{
	QBoxLayout* pLay = new QHBoxLayout();
	theDeviceWidget->AddSubLayout(pLay);
	QTreeView *tree = new QTreeView(theDeviceWidget);
	tree->setModel(m_pFileSystemModel);
	QModelIndex inRott = m_pFileSystemModel->index(m_Path);
	if(inRott.isValid())
		tree->setRootIndex(inRott);
	pLay->addWidget(tree);
}

void GFileSystemDevice::PopulateSettings( QSettings& inQsettings )
{
	GProgDevice::PopulateSettings(inQsettings);
}

void GFileSystemDevice::ReadFromSettings( QSettings& fromQsettings )
{
	GProgDevice::ReadFromSettings(fromQsettings);
}

void GFileSystemDevice::ChangeRootDirectory(QString newPath)
{
	m_pFileSystemModel->setRootPath(newPath);
}