#pragma once

#include <QSharedPointer>
#include <QWindow>
#include <qjsvalue.h>
#include <QMap>

#include "WebCallBack.h"
#include "WebViewInterface.h"


class MacosWebView : public WebViewInterface {

public:
    MacosWebView();
    ~MacosWebView();
    void init(bool debug, QWindow *window, WebCallBack *callBack) override;
    void navigate(const QString &url);
    void resizeWebView();
    void bind(const QString &name, QJSValue func);
    void runJavaScript(const QString &js);

private:
    QWindow *m_childWindow;
    QWindow *m_window;
    WebCallBack *m_callBack;
    QString m_userDataFolder;
    QMap<QString, QJSValue> m_bindings;
};
