#ifndef GUSERACTIONLOGER_H
#define GUSERACTIONLOGER_H

#include <QRunnable>
#include <QThread>
#include <QString>
#include <QNetworkAccessManager>
#include <QWebPage>

/////////////////////////////////////////////////////////////////////
//! \brief The docccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
/*!
*/
class GUserActionLogger : public QThread
{
	Q_OBJECT

public:
	GUserActionLogger(QString strAction);
	~GUserActionLogger();

	//! Implemented
	void run();

protected slots:
	void ProcessNetworkReply(QNetworkReply* theReply);
	void ProcessPageLoaded();

private:
	QString m_strAction;
	QNetworkAccessManager* manager;
	QWebPage* m_pPage;
};

#endif // GUSERACTIONLOGER_H
