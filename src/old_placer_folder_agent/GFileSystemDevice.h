#ifndef GFILESYSTEMDEVICE_H
#define GFILESYSTEMDEVICE_H

#include "device.h"
#include "param.h"

#include "Agent/GAgent.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GFileSystemDevice class defines an interface that displays the file system of the computer.
/*!
*/
class GFileSystemDevice : public GProgDevice, public GHasAgent
{
	Q_OBJECT

public:
	GFileSystemDevice(QObject *parent, QString uniqueIdentifierName = "");

	~GFileSystemDevice();

	//! Re-implemented
	void PopulateSettings( QSettings& inQsettings );
	//! Re-implemented
	void ReadFromSettings( QSettings& fromQsettings );

public slots:
	//! Changes the root path
	void ChangeRootDirectory(QString newPath);

protected:
	//! Re-implemented
	void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget);

private:
	GParamString m_Path;
	//! The QFileSystemModel object that actually explores the file system
	QFileSystemModel* m_pFileSystemModel;
};

#endif // GFILESYSTEMDEVICE_H
