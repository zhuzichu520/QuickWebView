#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QQuickWindow>
#include "webview/webview.h"

#ifdef Q_OS_MACOS
#endif

#ifdef Q_OS_LINUX
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
protected:
    void resizeEvent(QResizeEvent *ev) override;
private:
    WEBVIEW* m_webview = nullptr;
};
