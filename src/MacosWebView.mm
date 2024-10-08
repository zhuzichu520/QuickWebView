#include "MacosWebView.h"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#import <WebKit/WebKit.h>

@interface WebViewDelegate : NSObject <WKNavigationDelegate> {
    MacosWebView *qWebView;
}
- (WebViewDelegate *)initWithWebView:(MacosWebView *)webViewPrivate;
- (void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation;
@end

@implementation WebViewDelegate

- (WebViewDelegate *)initWithWebView:(MacosWebView *)webViewPrivate {
    if ((self = [super init])) {
        Q_ASSERT(webViewPrivate);
        qWebView = webViewPrivate;
    }
    return self;
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation {
    NSURL *currentURL = webView.URL; // Get the current URL of the WKWebView
    NSString *urlString = [currentURL absoluteString];
    if (qWebView->m_callBack) {
        qWebView->m_callBack->pageFinished(QString::fromUtf8([urlString UTF8String]));
    }
    NSString *title = webView.title;
    if (qWebView->m_callBack) {
        qWebView->m_callBack->titleChanged(QString::fromUtf8([title UTF8String]));
    }
}

@end

@interface MessageHandler : NSObject <WKScriptMessageHandler> {
    MacosWebView *qWebView;
}
- (MessageHandler *)initWithWebView:(MacosWebView *)webViewPrivate;
@end

@implementation MessageHandler

- (MessageHandler *)initWithWebView:(MacosWebView *)webViewPrivate {
    if ((self = [super init])) {
        Q_ASSERT(webViewPrivate);
        qWebView = webViewPrivate;
    }
    return self;
}

- (void)userContentController:(WKUserContentController *)userContentController
      didReceiveScriptMessage:(WKScriptMessage *)message {
    if ([message.name isEqualToString:@"postMessageHandler"]) {
        NSDictionary *body = (NSDictionary *) message.body;
        NSString *method = body[@"method"];
        id data = body[@"data"];
        if (qWebView->m_bindings.contains(QString::fromUtf8([method UTF8String]))) {
            QJSValue func = qWebView->m_bindings.value(QString::fromUtf8([method UTF8String]));
            QJSValueList args;
            args << QJSValue(QString::fromUtf8([data description].UTF8String));
            func.call(args);
        } else {
            qDebug() << "No binding found for method:" << method;
        }
    }
}

@end

MacosWebView::MacosWebView() {
    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_userDataFolder = QDir(appDataLocation).filePath("WebView2UserData");
    QDir().mkpath(m_userDataFolder);
}

MacosWebView::~MacosWebView() {
    if (m_wkWebView) {
        [m_wkWebView removeFromSuperview];
        [m_wkWebView release];
        m_wkWebView = nil;
    }
    if (m_childWindow) {
        m_childWindow->setParent(nullptr);
        delete m_childWindow;
        m_childWindow = nullptr;
    }
}

void MacosWebView::init(bool debug, QWindow *window, WebCallBack *callBack) {
    this->m_window = window;
    this->m_callBack = callBack;
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
    WKUserContentController *userContentController = [[WKUserContentController alloc] init];
    [userContentController addScriptMessageHandler:[[MessageHandler alloc] initWithWebView:this]
                                              name:@"postMessageHandler"];
    configuration.userContentController = userContentController;
    WKWebView *webView =
        [[WKWebView alloc] initWithFrame:NSMakeRect(0, 0, window->width(), window->height())
                           configuration:configuration];
    WKPreferences *preferences = webView.configuration.preferences;
    [preferences setValue:@(debug) forKey:@"developerExtrasEnabled"];
    WKWebsiteDataStore *dataStore = [WKWebsiteDataStore defaultDataStore];
    NSSet *websiteDataTypes = [WKWebsiteDataStore allWebsiteDataTypes];
    NSDate *dateFrom = [NSDate dateWithTimeIntervalSince1970:0];
    [dataStore removeDataOfTypes:websiteDataTypes
                   modifiedSince:dateFrom
               completionHandler:^{
                   NSLog(@"All website data has been cleared");
               }];
    m_wkWebView = webView;
    m_wkWebView.navigationDelegate = [[WebViewDelegate alloc] initWithWebView:this];
    m_childWindow = QWindow::fromWinId(WId(m_wkWebView));
    if (m_childWindow) {
        m_childWindow->setParent(window);
        m_childWindow->setFlags(Qt::WindowType::Widget);
    }
    if (m_callBack) {
        m_callBack->initialized(true);
    }
}

void MacosWebView::navigate(const QString &url) {
    auto uri = QUrl(url);
    NSURL *nsurl = QUrl(url).toNSURL();
    if (url.startsWith("/")) {
        [m_wkWebView loadFileURL:nsurl allowingReadAccessToURL:nsurl];
    } else {
        [m_wkWebView loadRequest:[NSURLRequest requestWithURL:nsurl]];
    }
}

void MacosWebView::bind(const QString &name, QJSValue func) {
    m_bindings.insert(name, func);
    auto jsTemp = QString(R"(window.%1 = function(data) {
window.webkit.messageHandlers.postMessageHandler.postMessage({
        method: '%1',
        data: data
    });
};)");
    runJavaScript(jsTemp.arg(name));
}

void MacosWebView::unbind(const QString &name) {
    m_bindings.remove(name);
}

void MacosWebView::runJavaScript(const QString &js) {
    NSString *javaScriptString = [NSString stringWithUTF8String:js.toUtf8().constData()];
    [m_wkWebView evaluateJavaScript:javaScriptString completionHandler:nil];
}

void MacosWebView::loadHtml(const QString &html) {
    NSString *htmlString = [NSString stringWithUTF8String:html.toUtf8().constData()];
    [m_wkWebView loadHTMLString:htmlString baseURL:nil];
}

void MacosWebView::resizeWebView() {
    int width = m_window->width();
    int height = m_window->height();
    if (m_wkWebView) {
        [m_wkWebView setFrame:NSMakeRect(0, 0, width, height)];
    }
    if (m_childWindow) {
        m_childWindow->setGeometry(0, 0, width, height);
    }
}
