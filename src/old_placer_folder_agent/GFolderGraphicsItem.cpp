#include "GFolderGraphicsItem.h"
#include "GFolderAgent.h"

GFolderGraphicsItem::GFolderGraphicsItem(GFolderAgent* pAgent, QGraphicsItem *parentItem /*= 0*/)
    : QGraphicsRectItem(parentItem)
	, GHasAgent(pAgent)
    , m_FolderNameItem(this)
{
// 	setFlag(QGraphicsItem::ItemStacksBehindParent);

    setAcceptHoverEvents(true);
    setRect(TypicalScreen.adjusted(100, 100, -100, -100));

//     QFont folderNameFont;
//     QString folderNameStr(pAgent->Info().filePath());
// 
//     QFontMetrics fm(folderNameFont);
//     int folderNameWidth=fm.width(folderNameStr);
//     int currentFontSize = folderNameFont.pointSize();
//     int correctFontSize = currentFontSize*boundingRect().width()/folderNameWidth;
//     folderNameFont.setPointSize(correctFontSize);
//     m_FolderNameItem.setPlainText(folderNameStr);
//     m_FolderNameItem.setFont(folderNameFont);
}

GFolderGraphicsItem::~GFolderGraphicsItem()
{
}

void GFolderGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget * widget /*= 0*/)
{
	Q_UNUSED(widget);
	QGraphicsRectItem::paint(painter, option, widget);

// 	double approxPercentOfScreen = ApproxPercentOfTypicalScreen(option, painter);
// 
// 	if(approxPercentOfScreen < 1e-5)
// 		return;
// 
// 	// lets compute how transparent the item should be
// 	static double PercentFullyTransparent = 0.15;
// 	static double PercentFullyOpaque = 0.01;
// 	double opacity = 1.0;
// 	if(approxPercentOfScreen > PercentFullyTransparent)
// 		opacity = 0.0;
// 	else if(approxPercentOfScreen > PercentFullyOpaque)
// 		opacity -= (approxPercentOfScreen - PercentFullyOpaque) / (PercentFullyTransparent - PercentFullyOpaque);
// 
// 	ChangeBrushOpacity(opacity);
// 
// 	QPen painterPen = painter->pen();
// 	painter->setPen(Qt::NoPen);
// 	GAgentTabletItem::paint(painter, option, widget);
// 	painter->setPen(painterPen);
}

