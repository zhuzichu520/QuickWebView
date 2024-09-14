#pragma once

#include "WebCallBack.h"
#include "stdafx.h"
#include <QObject>
#include <QWindow>
#include <QtQml/qqml.h>

#ifdef Q_OS_MACOS
#  include "MacosWebView.h"
using WEBVIEW = MacosWebView;
#endif

#ifdef Q_OS_LINUX
using WEBVIEW = LinuxWebView;
#endif

#ifdef Q_OS_WIN
#  include "WindowsWebView.h"
using WEBVIEW = WindowsWebView;
#endif

class QuickWebViewImpl : public QWindow {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY_AUTO(QString, title)
public:
    explicit QuickWebViewImpl(QWindow *parent = nullptr);
    ~QuickWebViewImpl();
    Q_INVOKABLE void init(bool debug);
    Q_INVOKABLE void navigate(const QString &url);
    Q_INVOKABLE void loadHtml(const QString &html);
    Q_INVOKABLE void runJavaScript(const QString &js);
    Q_INVOKABLE void bind(const QString &name, QJSValue func);
    Q_INVOKABLE void unbind(const QString &name);
    Q_SIGNAL void pageFinished(const QString &url);
    Q_SIGNAL void initialized(bool success);

protected:
    void resizeEvent(QResizeEvent *ev) override;

private:
    void resizeWebView();
    QString readFile(const QString &fileName);

private:
    WEBVIEW m_webView;
    WebCallBack *m_callBack;
};
