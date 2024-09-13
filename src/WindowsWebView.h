#pragma once

#include <QSharedPointer>
#include <QWindow>
#include <WebView2.h>
#include <qjsvalue.h>
#include <windows.h>
#include <QMap>
#include <wrl.h>

#include "WebCallBack.h"
#include "WebViewInterface.h"

using namespace Microsoft::WRL;

class WindowsWebView : public WebViewInterface {

public:
    WindowsWebView();
    ~WindowsWebView();
    void init(bool debug, QWindow *window, WebCallBack *callBack) override;
    void navigate(const QString &url);
    void resizeWebView();
    void bind(const QString &name, QJSValue func);
    void runJavaScript(const QString &js);

private:
    HRESULT OnEnvironmentCreated(HRESULT result, ICoreWebView2Environment *env, bool debug);

private:
    ComPtr<ICoreWebView2Controller> m_webViewController;
    ComPtr<ICoreWebView2> m_webView;
    QWindow *m_childWindow;
    QWindow *m_window;
    WebCallBack *m_callBack;
    HWND m_childWindowHandle;
    QString m_userDataFolder;
    QMap<QString, QJSValue> m_bindings;
};
