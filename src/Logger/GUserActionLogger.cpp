#include "GUserActionLogger.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>

#include <QWebPage>
#include <QWebFrame>

GUserActionLogger::GUserActionLogger(QString strAction)
	: QThread()
	, m_strAction(strAction)
{
	QUrl urlPageTest("http://www.fodograbj.com/1.php");
	m_pPage = new QWebPage();
	m_pPage->mainFrame()->load(urlPageTest);
	connect(m_pPage, SIGNAL(loadFinished(bool)), this, SLOT(ProcessPageLoaded()));

}

GUserActionLogger::~GUserActionLogger()
{
}

void GUserActionLogger::ProcessPageLoaded()
{
	m_pPage->deleteLater();
}

void GUserActionLogger::run()
{
// 	manager = new QNetworkAccessManager(0);
// 	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ProcessNetworkReply(QNetworkReply*)));
// 
// 	QUrl urlPageTest("http://www.fodograbj.com/1.php");
// 	QNetworkRequest request;
// 	request.setUrl(urlPageTest);
// 	request.setRawHeader(m_strAction.toUtf8(), "MyOwnBrowser 1.0");
// 	QNetworkReply *reply = manager->head(request);


}

void GUserActionLogger::ProcessNetworkReply( QNetworkReply* theReply )
{
	theReply;
}