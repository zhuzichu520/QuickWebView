#include "WindowsWebView.h"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <WebView2EnvironmentOptions.h>

using namespace Microsoft::WRL;

WindowsWebView::WindowsWebView() {
    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_userDataFolder = QDir(appDataLocation).filePath("WebView2UserData");
    QDir().mkpath(m_userDataFolder);
}

WindowsWebView::~WindowsWebView() {
    if (m_webViewController) {
        m_webViewController->Close();
        m_webViewController = nullptr;
    }
    m_webView = nullptr;
    if (m_childWindow) {
        delete m_childWindow;
        m_childWindow = nullptr;
    }
    if (m_childWindowHandle) {
        ::DestroyWindow(m_childWindowHandle);
        m_childWindowHandle = nullptr;
    }
}

void WindowsWebView::init(bool debug, QWindow *window, WebCallBack *callBack) {
    m_window = window;
    m_callBack = callBack;
    m_childWindowHandle =
        ::CreateWindowEx(0, L"Static", L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
                         reinterpret_cast<HWND>(m_window->winId()), NULL, nullptr, nullptr);
    m_childWindow = QWindow::fromWinId(reinterpret_cast<WId>(m_childWindowHandle));
    m_childWindow->setParent(window);
    m_childWindow->setFlags(Qt::WindowType::Widget);
    auto options = Make<CoreWebView2EnvironmentOptions>();
    options->put_AllowSingleSignOnUsingOSPrimaryAccount(FALSE);
    std::wstring userDataFolderW = m_userDataFolder.toStdWString();
    LPCWSTR userDataFolderLPCW = userDataFolderW.c_str();
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, userDataFolderLPCW, options.Get(),
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this, debug](HRESULT result, ICoreWebView2Environment *env) -> HRESULT {
                if (SUCCEEDED(result)) {
                    env->CreateCoreWebView2Controller(
                        m_childWindowHandle,
                        Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [this, debug](HRESULT result,
                                          ICoreWebView2Controller *controller) -> HRESULT {
                                if (SUCCEEDED(result)) {
                                    m_webViewController = controller;
                                    m_webViewController->AddRef();
                                    m_webViewController->get_CoreWebView2(&m_webView);
                                    m_webView->AddRef();
                                    ICoreWebView2Settings *settings;
                                    m_webView->get_Settings(&settings);
                                    settings->put_AreDevToolsEnabled(debug ? TRUE : FALSE);
                                    settings->put_AreDefaultContextMenusEnabled(TRUE);
                                    m_webViewController->put_IsVisible(TRUE);
                                    if (m_callBack) {
                                        m_callBack->initialized(true);
                                    }
                                    ::EventRegistrationToken token;
                                    m_webView->add_WebMessageReceived(
                                        Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                            [this](ICoreWebView2 *sender,
                                                   ICoreWebView2WebMessageReceivedEventArgs *args)
                                                -> HRESULT {
                                                PWSTR message;
                                                args->get_WebMessageAsJson(&message);
                                                QString jsonString =
                                                    QString::fromStdWString(std::wstring(message));
                                                CoTaskMemFree(message);
                                                QJsonDocument doc =
                                                    QJsonDocument::fromJson(jsonString.toUtf8());
                                                auto obj = doc.object();
                                                auto method = obj.value("method").toString();
                                                auto data = obj.value("data").toString();
                                                QJSValueList param;
                                                param << data;
                                                m_bindings.value(method).call(param);
                                                return S_OK;
                                            })
                                            .Get(),
                                        &token);
                                    m_webView->add_NavigationCompleted(
                                        Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                            [this](ICoreWebView2 *sender,
                                                   ICoreWebView2NavigationCompletedEventArgs *args)
                                                -> HRESULT {
                                                PWSTR wide_uri = nullptr;
                                                auto hr = sender->get_Source(&wide_uri);
                                                if (SUCCEEDED(hr)) {
                                                    if (m_callBack) {
                                                        m_callBack->pageFinished(
                                                            QString::fromStdWString(wide_uri));
                                                    }
                                                }
                                                CoTaskMemFree(wide_uri);
                                                return S_OK;
                                            })
                                            .Get(),
                                        &token);

                                } else {
                                    if (m_callBack) {
                                        m_callBack->initialized(false);
                                    }
                                }
                                return S_OK;
                            })
                            .Get());
                } else {
                    if (m_callBack) {
                        m_callBack->initialized(false);
                    }
                }
                return S_OK;
            })
            .Get());
    Q_UNUSED(hr);
}

void WindowsWebView::navigate(const QString &url) {
    m_webView->Navigate(reinterpret_cast<LPCWSTR>(url.utf16()));
}

void WindowsWebView::bind(const QString &name, QJSValue func) {
    m_bindings.insert(name, func);
    auto jsTemp = QString(R"(window.%1 = function(data){
window.chrome.webview.postMessage({
    method: '%1',
    data: data
});
};)");
    runJavaScript(jsTemp.arg(name));
}

void WindowsWebView::unbind(const QString &name) {
    m_bindings.remove(name);
}

void WindowsWebView::runJavaScript(const QString &js) {
    m_webView->ExecuteScript(js.toStdWString().c_str(), nullptr);
}

void WindowsWebView::resizeWebView() {
    if (m_webViewController && m_webView) {
        int width = m_window->width();
        int height = m_window->height();
        qreal devicePixelRatio = m_window->devicePixelRatio();
        RECT bounds;
        bounds.left = 0;
        bounds.top = 0;
        bounds.right = width * devicePixelRatio;
        bounds.bottom = height * devicePixelRatio;
        m_webViewController->put_Bounds(bounds);
        if (m_childWindow) {
            m_childWindow->setGeometry(0, 0, width, height);
        }
    }
}

void WindowsWebView::loadHtml(const QString &html) {
    m_webView->NavigateToString(html.toStdWString().c_str());
}
