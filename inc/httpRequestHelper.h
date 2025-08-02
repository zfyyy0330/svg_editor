#ifndef HTTPREQUESTHELPER_H
#define HTTPREQUESTHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class HttpRequestHelper final : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequestHelper(QObject *parent = nullptr);
    virtual ~HttpRequestHelper() = default;
    void sendGetRequest(const QUrl &url);

signals:
    void responseReceived(const QString &response);

private slots:
    void handleResponse();

private:
    QNetworkAccessManager *manager = nullptr;

};

#endif // HTTPREQUESTHELPER_H
