#include "MacosWebView.h"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>


MacosWebView::MacosWebView() {
    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_userDataFolder = QDir(appDataLocation).filePath("WebView2UserData");
    QDir().mkpath(m_userDataFolder);
}

MacosWebView::~MacosWebView() {

}

void MacosWebView::init(bool debug, QWindow *window, WebCallBack *callBack) {

}

void MacosWebView::navigate(const QString &url) {
    m_webView->Navigate(reinterpret_cast<LPCWSTR>(url.utf16()));
}

void MacosWebView::bind(const QString &name, QJSValue func) {
    m_bindings.insert(name, func);
    auto jsTemp = QString(R"(window.%1 = function(data){
window.chrome.webview.postMessage({
    method: '%1',
    data: data
});
};)");
    runJavaScript(jsTemp.arg(name));
}

void MacosWebView::runJavaScript(const QString &js) {

}

void MacosWebView::resizeWebView() {

}
