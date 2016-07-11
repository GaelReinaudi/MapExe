#ifndef GGROUPAGENT_H
#define GGROUPAGENT_H

#include "agent.h"
#include "GGroupItem.h"
#include "GGroupWrapitem.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GGroupAgent is a 
/*!
*/
class GGroupAgent : public GContentAgent<GGroupAgent, GGroupItem>
{
	Q_OBJECT

public:
	GGroupAgent(QObject *parent, QString uniqueIdentifierName = "");
	~GGroupAgent();

	//! Re-implemented
	GAgentWrapitem* ProvideNewAgentWrappingItem(QGraphicsItem* pParentItem) {
		GGroupWrapitem* pAgentItemContainer = new GGroupWrapitem(this, pParentItem);
		return pAgentItemContainer;
	}

	//! Implemented
	virtual bool InjectInitializationData(const QVariantList & theDataList);
	//! Implemented
	virtual QVariantList ProvideInitializationData();

	void InsertAgent( GAgentWrapitem* pAgWrap ) {
		m_SubAgentsIDs.insert(pAgWrap->UniqueSystemID(), 0 /*Future use*/);
	}

public:
	// Ids of the agents in this group and pointer to.......
	QMap<QString, QVariant> m_SubAgentsIDs;

};
/////////////////////////////////////////////////////////////////////
//! \brief The docccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
/*!
*/
class GGroupAgentCreationFilter : public QObject, public GAgentCreationFilter
{
	Q_OBJECT

public:
	GGroupAgentCreationFilter(QObject *parent = 0): QObject(parent) {}
	~GGroupAgentCreationFilter() {}

	virtual double MightCreateAgent(const QMimeData* pMime) const {
		if(pMime->hasUrls()) {
			QList<QUrl> urlList = pMime->urls();
			foreach(QUrl theUrl, urlList) {
				if(theUrl.isValid()) {
					if(theUrl.isLocalFile()) {
						QString theExtension = QFileInfo(theUrl.toLocalFile()).suffix().toLower();
						if(theExtension == "png" || theExtension == "jpg" || theExtension == "jpeg")
							if(urlList.count() >= 2)
								return 8.0;
					}
				}
			}
		}
		return 0.0;
	}
	virtual QVariantList ExtractInitializationData(const QMimeData* pMime) const {
		QList<QUrl> urlList = pMime->urls();
		QVariantList varList;
		foreach(QUrl theUrl, urlList) {
			if(theUrl.isValid()) {
				if(theUrl.isLocalFile()) {
					QString theExtension = QFileInfo(theUrl.toLocalFile()).suffix().toLower();
					if(theExtension == "png" || theExtension == "jpg" || theExtension == "jpeg")
						varList << theUrl.toLocalFile();
				}
			}
		}
		return varList;
	}

	virtual QString GetClassName() const {return "GGroupAgent";}

private:
	
};

#endif // GGROUPAGENT_H
