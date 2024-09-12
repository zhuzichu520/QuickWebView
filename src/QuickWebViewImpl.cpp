#include "QuickWebViewImpl.h"

#include <QFile>
#include <QJSEngine>
#include <QQuickWindow>
#include <QResource>
#include <QThreadPool>
#include <QTimer>

QuickWebViewImpl::QuickWebViewImpl(QWindow *parent) : QWindow{parent} {
  setFlag(Qt::FramelessWindowHint, true);
}

void QuickWebViewImpl::resizeEvent(QResizeEvent *ev) { resizeWebView(); }

void QuickWebViewImpl::init(bool debug) {
  m_webview = reinterpret_cast<WEBVIEW *>(
      new webview::webview(debug, reinterpret_cast<void *>(winId())));
  m_webview->resize_widget();
  m_webview->set_navigate_listener(
      [this](const std::string &uri, void *usercontext) {
        QTimer::singleShot(50, this, [this, uri] {
          Q_EMIT pageFinished(QString::fromStdString(uri));
        });
      },
      nullptr);
}

void QuickWebViewImpl::runJavaScript(const QString &js) {
  if (!m_webview) {
    return;
  }
  if (js.startsWith("appdir:")) {
    QString file = QGuiApplication::applicationDirPath() + js.mid(7);
    m_webview->eval(file.toStdString());
  } else {
    m_webview->eval(js.toStdString());
  }
}

void QuickWebViewImpl::resizeWebView() {
  if (!m_webview) {
    return;
  }
#ifdef Q_OS_WIN
  m_webview->resize_widget();
#endif
}

void QuickWebViewImpl::navigate(const QString &url) {
  if (!m_webview) {
    return;
  }
  if (url.startsWith("appdir:")) {
    QString file = QGuiApplication::applicationDirPath() + url.mid(7);
    m_webview->navigate(file.toStdString());
  } else {
    m_webview->navigate(url.toStdString());
  }
}

void QuickWebViewImpl::bind(const QString &name, QJSValue func) {
  if (!m_webview) {
    return;
  }
  auto sharedFunc = std::make_shared<QJSValue>(func);
  m_webview->bind(
      name.toStdString(),
      [&, sharedFunc](const std::string &req) -> std::string {
        auto jsEngine = qjsEngine(this);
        QJSValueList param;
        auto data =
            jsEngine->evaluate(QString("(%1)").arg(QString::fromStdString(req)))
                .toVariant()
                .toList();
        for (const auto &value : data) {
          param << jsEngine->toScriptValue(value);
        }
        auto result = sharedFunc->call(param).toString().toStdString();
        if (result != "undefined") {
          return result;
        }
        return {};
      });
}

void QuickWebViewImpl::unbind(const QString &name) {
  if (!m_webview) {
    return;
  }
  m_webview->unbind(name.toStdString());
}

void QuickWebViewImpl::loadHtml(const QString &html) {
  if (!m_webview) {
    return;
  }
  m_webview->set_html(html.toStdString());
}

QuickWebViewImpl::~QuickWebViewImpl() {
  delete m_webview;
  m_webview = nullptr;
}
