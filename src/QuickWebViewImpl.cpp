#include "QuickWebViewImpl.h"


#include <QQuickWindow>

QuickWebViewImpl::QuickWebViewImpl(QWindow *parent)
    : QWindow{parent}
{
    setFlag(Qt::FramelessWindowHint,true);
    m_webview = reinterpret_cast<WEBVIEW *>(new webview::webview(false, reinterpret_cast<void*>(winId())));
}

void QuickWebViewImpl::resizeEvent(QResizeEvent *ev){
    if(m_webview){
        m_webview->resize_widget();
    }
}

void QuickWebViewImpl::navigate(const QString& url){
    m_webview->navigate(url.toStdString());
}
