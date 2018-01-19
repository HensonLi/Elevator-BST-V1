#include "webview.h"

WebView::WebView(QWidget *parent) :
    QWebView(parent)
{
    netwkAcssMng = 0;
    /*netwkAcssMng = new QNetworkAccessManager;
    QNetworkProxy proxy;
    proxy = QNetworkProxy::HttpProxy;
    proxy.setHostName(tr("10.200.0.1"));    //���������
    proxy.setPort(8080);
    netwkAcssMng->setProxy(proxy);
    page()->setNetworkAccessManager(netwkAcssMng);
    */
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    //��ʾFlash������
    setAttribute(Qt::WA_DeleteOnClose);
    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebSettings::PluginsEnabled, true);//��Ҫ
    settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    settings()->setAttribute(QWebSettings::JavaEnabled, true);
    //����򲻿�ҳ���еĶ�����������
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

void WebView::SetUrl(QString pUrl)
{
    load(QUrl(pUrl));
    show();
}

void WebView::ShowHtml(QString pHtmlFile)
{
    QFile file(pHtmlFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        IDE_TRACE();
        return;
    }
    QString content;
    QTextStream stream(&file);
    content = stream.readAll();
    setHtml(content);
    show();
}

QVariant WebView::EvaluateJS(QString pJs)
{
    QWebFrame *frame = page()->mainFrame();
    if(frame)
        return frame->evaluateJavaScript(pJs);
    return QVariant();
}
