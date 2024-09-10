import QtQuick
import QtQuick.Controls
import QuickWebView

WindowContainer {
    window: QuickWebViewImpl{
        id: web_impl
    }
    function navigate(url){
        web_impl.navigate(url)
    }
}
