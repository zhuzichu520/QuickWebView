#include "QuickWebViewImpl.h"

#include <QFile>
#include <QJSEngine>
#include <QQuickWindow>
#include <QResource>
#include <QThreadPool>
#include <QTimer>


QuickWebViewImpl::QuickWebViewImpl(QWindow *parent) : QWindow{parent} {
    setFlag(Qt::FramelessWindowHint, true);
}

void QuickWebViewImpl::resizeEvent(QResizeEvent *ev) {
    resizeWebView();
}

void QuickWebViewImpl::init(bool debug) {
    m_callBack = new WebCallBack();
    m_callBack->initialized = ([this](bool success) {
        resizeWebView();
        Q_EMIT this->initialized(success);
    });
    m_callBack->pageFinished = ([this](const QString &url) { Q_EMIT pageFinished(url); });
    m_webView.init(debug, this, m_callBack);
}

void QuickWebViewImpl::runJavaScript(const QString &js) {
    m_webView.runJavaScript(js);
}

void QuickWebViewImpl::resizeWebView() {
    m_webView.resizeWebView();
}

void QuickWebViewImpl::navigate(const QString &url) {
    if (url.startsWith("appdir:")) {
        QString file = "file://" + QGuiApplication::applicationDirPath() + url.mid(7);
        m_webView.navigate(file);
    } else {
        m_webView.navigate(url);
    }
}

void QuickWebViewImpl::bind(const QString &name, QJSValue func) {
    m_webView.bind(name, func);
}

void QuickWebViewImpl::unbind(const QString &name) {
}

void QuickWebViewImpl::loadHtml(const QString &html) {
    m_webView.loadHtml(html);
}

QuickWebViewImpl::~QuickWebViewImpl() {
}

QString QuickWebViewImpl::readFile(const QString &fileName) {
    QString content;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        content = stream.readAll();
    }
    return content;
}
