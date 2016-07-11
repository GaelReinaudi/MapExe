#ifndef GURIAGENT_H
#define GURIAGENT_H

#include "Agent/GAgent.h"
#include <QFileInfo>

/////////////////////////////////////////////////////////////////////
//! \brief The GUriAgent class is a GAgent class that takes care of representing and manipulating a uri.
/*!
*/
class GUriAgent : public GAgent
{
	Q_OBJECT

public:
	GUriAgent(QObject *parent);
	~GUriAgent();

	//! the file info
	QFileInfo Info() const { return m_Info; }
	//! Sets the file info
	void SetInfo(QFileInfo val);

signals:
	//! emitted when SetInfo() was called to update the file info
	void FileInfoChanged(QFileInfo fileInfo);

private:
	QFileInfo m_Info;
};

#endif // GURIAGENT_H
