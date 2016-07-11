#include "GItemManager.h"
#include "Agent/GAgentWrapitem.h"
#include "SceneView/GMapGraphicsView.h"
#include "SceneView/GMapGraphicsScene.h"
#include "../../../LabExe/src/ToolBox/GSerializable.h"

#include <QPointer>
#include <QDesktopServices>
#include <QStyleOptionGraphicsItem>

GItemManager::GItemManager(QObject *parent)
	: QThread(parent)
	, m_pCurrentView(0)
	, m_pCurrentScene(0)
	, m_CurrentLevel(17)
{
	// set the size of the caches
	for(int level = 0; level < NUM_SCREENSHOT_LEVELS ; level++) {
		m_Screens[level].setMaxCost(1000);
	}

	QString cacheLoc = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
	QString strProgramName = QCoreApplication::applicationName();
	m_CacheDirectoryPath = cacheLoc + "/" + strProgramName + "ScreenShots/%1";

	qDebug() << "Cache location: " << m_CacheDirectoryPath;

	QDir theDir;
	// load the screenshots
	for(int level = 0; level < NUM_SCREENSHOT_LEVELS ; level++) {
		QString dirPath = m_CacheDirectoryPath.arg(level);
		theDir.setPath(dirPath);
		// for each entry that is a file
		foreach(QFileInfo finfo, theDir.entryInfoList(QDir::Files)) {
			QString theKey = finfo.completeBaseName();
			QString filePath = finfo.absoluteFilePath();
			QPixmap* pTheImage = new QPixmap(filePath);
			// insert it in the cache
			if(!pTheImage->isNull())
				m_Screens[level].insert(theKey, pTheImage, 100 / (1 << level));
		}
	}
}

GItemManager::~GItemManager()
{
	QDir theDir;
	// makes the directory if not
	for(int i = 0; i < NUM_SCREENSHOT_LEVELS; i++) {
		// ex: E:/BasaltScreenShots/0
		QString dirPath = m_CacheDirectoryPath.arg(i);
		theDir.mkpath(dirPath);
		theDir.setPath(dirPath);
		// empties the directory if anything in it
		foreach(QFileInfo finfo, theDir.entryInfoList()) {
			theDir.remove(finfo.fileName());
		}
	}
	for(int i = 0; i < NUM_SCREENSHOT_LEVELS; i++) {
		// ex: E:/BasaltScreenShots/0/%1
		QString filePath = m_CacheDirectoryPath.arg(i) + "/%1";
		foreach(const QString & k, m_Screens[i].keys()) {
			// ex: E:/BasaltScreenShots/0/GPictureAgent-c1eff0.130131.093354
			QString path = filePath.arg(k);
//			m_Screens[i].object(k)->save(path + ".JPG");
			m_Screens[i].object(k)->save(path + ".PNG");
		}
	}
}

QPixmap GItemManager::PixDig( int i ) const
{
	static bool madePixmaps = false;
	static QPixmap m_PixDigits[10];
#if defined(Q_OS_MAC)
	static QString imagesURI = "../../../../images/";
#else
	static QString imagesURI = "../images/";
#endif

	// if the static pixmap where made, then return the desired one.
	if(madePixmaps) {
		return m_PixDigits[i];
	}

	// we load some pixmap of digits that might help us debug the program before we 
	// get the whole screen-shotting mechanics working
	for(int j = 0; j < 10 ; j++) {
		static QString folder = imagesURI + "digits/";
		static QString fileDigName = "GN-4_digit_%1.jpg";
		m_PixDigits[j] = QPixmap(folder  + fileDigName.arg(j));
	}
	madePixmaps = true;
	// and of course return the desired pixmap
	return PixDig(i);
}

QPixmap GItemManager::PixDig( GAgentWrapitem* pAgIt ) const
{
	int levelIt = Level(pAgIt);
	levelIt = qBound(0, levelIt, 9);
	return PixDig(levelIt);
}

void GItemManager::ChangeItemLevel(GAgentWrapitem* pAgItem, int newLevel)
{
	QMutexLocker locker(&mutex);

	int prevLevel = Level(pAgItem);
	int itemWidth = pAgItem->boundingRect().width();
	itemWidth += 2;
	newLevel = qMin(newLevel, newLevel + int(1.44269504 * qLn(1600.0 / itemWidth)));
	m_Items_Level[pAgItem] = newLevel;

	// if getting closer
	if(newLevel < prevLevel) {
		QMetaObject::invokeMethod(pAgItem, "GettingCloser"
			, Qt::BlockingQueuedConnection
			, Q_ARG(int, newLevel)
			);
	} // if getting further
	else if(newLevel > prevLevel) {
		QMetaObject::invokeMethod(pAgItem, "GettingFurther"
// 			, Qt::BlockingQueuedConnection
			, Q_ARG(int, newLevel)
			);
	}
//	m_Screens_2.insert(pAgItem->UniqueSystemID(), QPixmap());
}

QList<GAgentWrapitem*> GItemManager::GetCurrentWrapItemsIntersectRect()
{
	QList<GAgentWrapitem*> theWraps;
	// let's define the rectangle to consider for enumerating the items inside
	int minSize = qMin(m_CurrentRect.height(), m_CurrentRect.width());
	minSize /= 16;
	QRect visibleRect = m_CurrentRect.adjusted(minSize, minSize, -minSize, -minSize);

	// get items in the rect
	const QList<QGraphicsItem*> itemsVisible = m_pCurrentView->items(visibleRect, Qt::IntersectsItemBoundingRect);
	foreach(QGraphicsItem* pIt, itemsVisible) {
		GAgentWrapitem* pWrapIt = 0;
		pWrapIt = qgraphicsitem_cast<GAgentWrapitem*>(pIt);
		if(!pWrapIt)
			continue;
		// if not a top level item, we don't do shit... for now
		if(pWrapIt->parentItem() != m_pCurrentScene->RootGraphicsItem())
			continue;
// 		int itemWidth = pWrapIt->boundingRect().width();
// 		itemWidth++;
		theWraps << pWrapIt;
	}
	return theWraps;
}

void GItemManager::StartUpdateDistanceItems(GMapGraphicsView* pView, char newLevel, const QRect & inRect)
{
	if(isRunning())
		return;
	m_pCurrentView = pView;
	m_pCurrentScene = qobject_cast<GMapGraphicsScene*>(pView->scene());
	m_CurrentLevel = newLevel;
	m_CurrentRect = inRect;
	m_CurrentListOfWrapItems = GetCurrentWrapItemsIntersectRect();
	start();
}

void GItemManager::run()
{
	// then we do the lengthy thing on the actual wrapitems
	foreach(GAgentWrapitem* pWrap, m_CurrentListOfWrapItems) {
		ItemManagerInstance()->ChangeItemLevel(pWrap, m_CurrentLevel);
	}
}

void GItemManager::ScreenShotContent(GAgentWrapitem* pAgIt, QSharedPointer<QGraphicsItem> sharedContentItem)
{
	QSize contentSize = sharedContentItem ? sharedContentItem.data()->boundingRect().size().toSize() : QSize(100, 2000);
	if(!contentSize.isValid() || contentSize.isNull()) {
		contentSize = pAgIt->boundingRect().size().toSize();
	}
	if(!contentSize.isValid() || contentSize.isNull()) {
		return;
	}
	
	// TEST to have screenshot never bigger than a typical screen size
	if(qMax(contentSize.width(), contentSize.height()) > 1600)
		contentSize.scale(QSize(1600, 1600), Qt::KeepAspectRatio);//ByExpanding);

	QPixmap* pScreenShotFullScale = new QPixmap(contentSize);

 	pScreenShotFullScale->fill(Qt::transparent);

	if(sharedContentItem) {
		QPainter painter(pScreenShotFullScale);
		QStyleOptionGraphicsItem styleOption;
//		sharedContentItem.data()->paint(&painter, &styleOption);
	// TEST to have screenshot never bigger than a typical screen size
//		sharedContentItem.data()->scene()->render(&painter, QRect(), sharedContentItem.data()->mapToScene(sharedContentItem.data()->boundingRect()).boundingRect());
		sharedContentItem.data()->scene()->render(&painter, QRect(QPoint(0,0), contentSize), sharedContentItem.data()->mapToScene(pAgIt->boundingRect()).boundingRect());
// 		painter.end();
	}
	else {
		pScreenShotFullScale->fill(QColor(255, 0, 0).rgb());
		qDebug() << "oups" << "ShowScreenShot()";
	}

	if(pScreenShotFullScale->isNull())
		return;

	// full size screen shot in the cache 1
	m_Screens[1].insert(pAgIt->m_AgentTypeID
		// new pixmap scaled down
		, pScreenShotFullScale
		// cost
		, 50);
	// sizes divided by powers of 2
	for(int level = 2; level < NUM_SCREENSHOT_LEVELS ; level++) {
		m_Screens[level].insert(pAgIt->m_AgentTypeID
			// new pixmap scaled down
			, new QPixmap(pScreenShotFullScale->scaled(contentSize / (1 << (level - 1))))
			// cost
			, 100 / (1 << level));
	}
}

QPixmap GItemManager::Pix( GAgentWrapitem* pAgIt ) const
{
	int levelIt = Level(pAgIt);
	levelIt = qMax(0, levelIt);
	levelIt = qMin(levelIt, NUM_SCREENSHOT_LEVELS - 1);
	int levelInitial = levelIt;
	// which pixamp to return?
	QPixmap* pPixRet = 0;
	// take the biggest available screenshot (starting at our current level).
	while(levelIt < NUM_SCREENSHOT_LEVELS) {
		pPixRet = m_Screens[levelIt][pAgIt->m_AgentTypeID];
		if(pPixRet)
			return *pPixRet;
		levelIt++;
	}
	return QPixmap();
//	return PixDig(levelInitial);
}