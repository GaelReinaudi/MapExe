#ifndef GUNDERLAYITEM_H
#define GUNDERLAYITEM_H

#include <QObject>
#include <QBrush>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QGraphicsPixmapItem>
class GAgentWrapitem;

class GUnderlayItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
	Q_PROPERTY(double highlight WRITE SetHighlightLevel);

public:
	GUnderlayItem(GAgentWrapitem* parentContainerItem);
	virtual ~GUnderlayItem() {}

public slots:
	void SetHighlightLevel(double newVal) {
		m_Highlight = newVal;
		QColor newCol = brush().color();
		newCol.setAlphaF(m_Highlight);
		setBrush(newCol);
		//update();
	}

	void UpdateSize();
	void ShineIn();
	void ShineOut();

private:
	QGraphicsPixmapItem* m_pDropShadowItem;

	double m_Highlight;
	QPropertyAnimation* m_Animation;
};

#endif // GUNDERLAYITEM_H
