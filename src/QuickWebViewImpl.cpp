#include "QuickWebViewImpl.h"

#include <QQuickWindow>
#include <QFile>
#include <QResource>
#include <QJSEngine>
#include <QThreadPool>
#include <QTimer>
QuickWebViewImpl::QuickWebViewImpl(QWindow *parent)
    : QWindow{parent}
{
    setFlag(Qt::FramelessWindowHint,true);
    m_webview = reinterpret_cast<WEBVIEW *>(new webview::webview(true, reinterpret_cast<void*>(winId())));
    m_webview->dispatch([this]{ Q_EMIT dispatch(); });
}

void QuickWebViewImpl::resizeEvent(QResizeEvent *ev){
    if(!m_webview){
        return;
    }
#ifdef Q_OS_WIN
    m_webview->resize_widget();
#endif
}


void QuickWebViewImpl::setNavigateListener(QJSValue func){
    if(!m_webview){
        return;
    }
    auto sharedFunc = std::make_shared<QJSValue>(func);
    m_webview->set_navigate_listener([sharedFunc,this](const std::string &uri, void *usercontext){
        QTimer::singleShot(50,this,[sharedFunc,uri]{
            QJSValueList param;
            param<<QString::fromStdString(uri);
            sharedFunc->call(param);
        });
    },nullptr);
}

void QuickWebViewImpl::runJavaScript(const QString& js){
    if(!m_webview){
        return;
    }
    if(js.startsWith("appdir:")){
        QString file = QGuiApplication::applicationDirPath()+js.mid(7);
        m_webview->eval(file.toStdString());
    }else{
        m_webview->eval(js.toStdString());
    }
}

void QuickWebViewImpl::navigate(const QString& url){
    if(!m_webview){
        return;
    }
    if(url.startsWith("appdir:")){
        QString file = QGuiApplication::applicationDirPath()+url.mid(7);
        m_webview->navigate(file.toStdString());
    }else{
        m_webview->navigate(url.toStdString());
    }
}

void QuickWebViewImpl::bind(const QString& name,QJSValue func){
    if(!m_webview){
        return;
    }
    auto sharedFunc = std::make_shared<QJSValue>(func);
    m_webview->bind(name.toStdString(), [&,sharedFunc](const std::string &req) -> std::string {
        auto jsEngine = qjsEngine(this);
        QJSValueList param;
        auto data = jsEngine->evaluate(QString("(%1)").arg(QString::fromStdString(req))).toVariant().toList();
        for (const auto& value : data) {
            param << jsEngine->toScriptValue(value);
        }
        auto result = sharedFunc->call(param).toString().toStdString();
        if(result != "undefined"){
            return result;
        }
        return {};
    });
}

void QuickWebViewImpl::unbind(const QString& name){
    if(!m_webview){
        return;
    }
    m_webview->unbind(name.toStdString());
}

void QuickWebViewImpl::loadHtml(const QString& html){
    if(!m_webview){
        return;
    }
    m_webview->set_html(html.toStdString());
}

QuickWebViewImpl::~QuickWebViewImpl(){
    delete m_webview;
}

QString QuickWebViewImpl::readFile(const QString &fileName) {
    QString content;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        content = stream.readAll();
    }
    return content;
}
