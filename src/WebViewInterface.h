#pragma once

#include "WebCallBack.h"
#include <QWindow>

class WebViewInterface {
public:
    virtual ~WebViewInterface() = default;
    virtual void init(bool debug, QWindow *window, WebCallBack *callBack) = 0;
};
