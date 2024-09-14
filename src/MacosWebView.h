#pragma once

#include <QSharedPointer>
#include <QWindow>
#include <qjsvalue.h>
#include <QMap>

#include "WebCallBack.h"
#include "WebViewInterface.h"
Q_FORWARD_DECLARE_OBJC_CLASS(WKWebView);
Q_FORWARD_DECLARE_OBJC_CLASS(WKNavigation);

class MacosWebView : public WebViewInterface {

public:
    MacosWebView();
    ~MacosWebView();
    void init(bool debug, QWindow *window, WebCallBack *callBack) override;
    void navigate(const QString &url) override;
    void resizeWebView() override;
    void bind(const QString &name, QJSValue func) override;
    void unbind(const QString &name) override;
    void runJavaScript(const QString &js) override;
    void loadHtml(const QString &html) override;

public:
    void onPageFinished(const QString &url);

private:
    QWindow *m_childWindow;
    QWindow *m_window;
    WebCallBack *m_callBack;
    QString m_userDataFolder;
    WKWebView *m_wkWebView;
public:
    WKNavigation *m_wkNavigation;
    QMap<QString, QJSValue> m_bindings;
};
