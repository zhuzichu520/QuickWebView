#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QQuickWindow>
#include "webview/webview.h"

#ifdef Q_OS_MACOS
    using WEBVIEW = webview::webview;
#endif

#ifdef Q_OS_LINUX
    using WEBVIEW = webview::webview;
#endif

#ifdef Q_OS_WIN
    using WEBVIEW = webview::detail::win32_edge_engine;
#endif


class QuickWebViewImpl : public QWindow
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit QuickWebViewImpl(QWindow *parent = nullptr);
    ~QuickWebViewImpl();
    Q_INVOKABLE void navigate(const QString& url);
    Q_INVOKABLE void loadHtml(const QString& html);
    Q_INVOKABLE void runJavaScript(const QString& js);
    Q_INVOKABLE void bind(const QString& name,QJSValue func);
    Q_INVOKABLE void unbind(const QString& name);
    Q_INVOKABLE void setNavigateListener(QJSValue func);
    Q_SIGNAL void dispatch();
protected:
    void resizeEvent(QResizeEvent *ev) override;
private:
    QString readFile(const QString &fileName);
private:
    WEBVIEW* m_webview = nullptr;
    long count = 0;
};
