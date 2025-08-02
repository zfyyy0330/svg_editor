#include "httpRequestHelper.h"
#include <QMessageBox>
#include <QFile>
#include <QSslConfiguration>
#include <QSslCertificate>

HttpRequestHelper::HttpRequestHelper(QObject *parent) : QObject(parent){
    manager = new QNetworkAccessManager(this);
}

void HttpRequestHelper::sendGetRequest(const QUrl &url){
    QNetworkRequest request(url);
    qDebug()<< QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, &HttpRequestHelper::handleResponse);
}

void HttpRequestHelper::handleResponse(){
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply){
        if (reply->error() == QNetworkReply::NoError){
            int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (status_code >= 400){
                QMessageBox::warning(nullptr, tr("网络错误"), tr("返回请求状态码：" + status_code));
            }
            else {
                QByteArray data = reply->readAll();
                emit responseReceived(QString::fromUtf8(data));
            }
        }
        else{
            QMessageBox::warning(nullptr, tr("网络错误"), reply->errorString());
        }

        reply->deleteLater();
    }
}
