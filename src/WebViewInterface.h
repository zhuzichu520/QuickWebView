#pragma once

#include "WebCallBack.h"
#include <QWindow>
#include <QJSValue>

class WebViewInterface {
public:
    virtual ~WebViewInterface() = default;
    virtual void init(bool debug, QWindow *window, WebCallBack *callBack) = 0;
    virtual void navigate(const QString &url) = 0;
    virtual void resizeWebView() = 0;
    virtual void bind(const QString &name, QJSValue func) = 0;
    virtual void runJavaScript(const QString &js) = 0;
    virtual void loadHtml(const QString &html) = 0;
    virtual void unbind(const QString &name) = 0;
};
