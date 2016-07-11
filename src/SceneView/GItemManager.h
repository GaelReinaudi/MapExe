#ifndef GSCREENSHOTMANAGER_H
#define GSCREENSHOTMANAGER_H

#include <QThread>
#include <QCache>
#include <QMap>
#include <QHash>
#include <QPixmap>
#include <QImage>
#include <QMutex>
class QGraphicsItem;
class GSerializable;
class GAgentWrapitem;
class GMapGraphicsView;
class GMapGraphicsScene;

#define NUM_SCREENSHOT_LEVELS 6

class GItemManager : public QThread
{
	Q_OBJECT

public:
	GItemManager(QObject *parent);
	~GItemManager();

	void ChangeItemLevel(GAgentWrapitem* pAgItem, int newLevel);
	// Returns the level of the item or a large value if it was never there
	int Level(GAgentWrapitem* pAgItem) const {
		return m_Items_Level.value(pAgItem, 100);
	}

	// test. Returns a pixmap representing a digit
	QPixmap PixDig(int i) const;
	// test. Returns a pixmap representing a digit corresponding to the aoom level of the item
	QPixmap PixDig(GAgentWrapitem* pAgIt) const;
	// test. Returns an appropriate pixmap for the pAgIt
	QPixmap Pix(GAgentWrapitem* pAgIt) const;

	//! Test go over items multi-threaded
	void StartUpdateDistanceItems(GMapGraphicsView* pView, char newLevel, const QRect & inRect);
	//! Gets all the items in the rect that are wrapitems in the current view + rect
	QList<GAgentWrapitem*> GetCurrentWrapItemsIntersectRect();
	void ScreenShotContent(GAgentWrapitem* pAgIt, QSharedPointer<QGraphicsItem> sharedContentItem);

protected:
	void run();

private:
	QHash<GAgentWrapitem*, int> m_Items_Level;
	QCache<QString, QPixmap> m_Screens[NUM_SCREENSHOT_LEVELS];

	char m_CurrentLevel;
	GMapGraphicsView* m_pCurrentView;
	GMapGraphicsScene* m_pCurrentScene;
	QRect m_CurrentRect;
	QList<GAgentWrapitem*> m_CurrentListOfWrapItems;
	QMutex mutex;
	// path of the cache directory
	QString m_CacheDirectoryPath;

};

#endif // GSCREENSHOTMANAGER_H
