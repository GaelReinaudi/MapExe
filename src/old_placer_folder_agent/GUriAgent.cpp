#include "GUriAgent.h"
#include <QGraphicsEllipseItem>

GUriAgent::GUriAgent(QObject *parent)
	: GAgent(parent)
{

}

GUriAgent::~GUriAgent()
{

}

void GUriAgent::SetInfo( QFileInfo val )
{
	m_Info = val; 
	emit FileInfoChanged(m_Info);
}