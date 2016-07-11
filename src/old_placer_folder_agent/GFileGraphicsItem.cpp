#include "GFileGraphicsItem.h"
#include "GFileAgent.h"
#include <QFont>
#include <QFontMetrics>

GFileGraphicsItem::GFileGraphicsItem(GFileAgent* pAgent, QGraphicsItem *parentItem /*= 0*/)
    : QGraphicsRectItem(parentItem)
	, GHasAgent(pAgent)
    , m_FileNameItem(this)
{
// 	setFlag(QGraphicsItem::ItemStacksBehindParent);

	//! get informed of the changes to the file info
	connect(pAgent, SIGNAL(FileInfoChanged(QFileInfo)), this, SLOT(Event_FileInfoUpdated(QFileInfo)));
	Event_FileInfoUpdated(pAgent->Info());

	setAcceptHoverEvents(true);
    setRect(TypicalScreen.adjusted(25,25,-25,-25));


    QFont fileNameFont;
    fileNameFont.setFamily("Gill Sans");
    QString fileNameStr(pAgent->Info().fileName());

    QFontMetrics fm(fileNameFont);
    int fileNameWidth=fm.width(fileNameStr);
    int currentFontSize = fileNameFont.pointSize();
    int correctFontSize = currentFontSize*boundingRect().width()/fileNameWidth;
    fileNameFont.setPointSize(correctFontSize);
    m_FileNameItem.setPlainText(fileNameStr);
    m_FileNameItem.setFont(fileNameFont);
    //m_FileNameItem.setDefaultTextColor("#5C5C5C");
    m_FileNameItem.setDefaultTextColor("white");
}

GFileGraphicsItem::~GFileGraphicsItem()
{
}

void GFileGraphicsItem::Event_FileInfoUpdated( QFileInfo fileInfo )
{
	QString extFile = fileInfo.suffix().toLower();
	if(extFile == "jpg" || extFile == "png") {
		QPixmap pixImage(fileInfo.filePath());
        pixImage = pixImage.scaled(TypicalScreenSize.width() - 50, TypicalScreenSize.height() - 50, Qt::KeepAspectRatio);
        QGraphicsPixmapItem* pixImageItem = new QGraphicsPixmapItem(pixImage, this);
        pixImageItem->setOffset(25,25);

		qDebug() << fileInfo.filePath();
	}
}
