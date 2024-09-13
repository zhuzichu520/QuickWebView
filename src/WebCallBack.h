#pragma once

#include <QObject>

class WebCallBack {
public:
  WebCallBack();
  std::function<void(bool)> initialized;
  std::function<void(QString)> pageFinished;
};
