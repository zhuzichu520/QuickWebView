#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqmlextensionplugin.h>

#ifdef QUICKWEBVIEW_BUILD_STATIC_LIB
Q_IMPORT_QML_PLUGIN(QuickWebViewPlugin)
#endif

int main(int argc, char *argv[]) {
  qputenv("QT_QUICK_CONTROLS_CONF", ":/qt/qml/example/qtquickcontrols2.conf");
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/qt/qml/example/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);
  return app.exec();
}
