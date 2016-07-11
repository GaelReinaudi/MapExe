#ifndef GAGENTCREATIONFILTER_H
#define GAGENTCREATIONFILTER_H

#include <QMimeData>
class GAgent;

/////////////////////////////////////////////////////////////////////
//! \brief The docccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
/*!
*/
class GAgentCreationFilter
{
public:
	GAgentCreationFilter() {}
	~GAgentCreationFilter() {}

	virtual double MightCreateAgent(const QMimeData* pMime) const = 0;
	virtual QVariantList ExtractInitializationData(const QMimeData* pMime) const = 0;

	virtual QString GetClassName() const {return "";}

private:
	
};

#endif // GAGENTCREATIONFILTER_H
