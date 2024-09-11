import QtQuick
import QtQuick.Controls
import QuickWebView

WindowContainer {
    id: control
    property bool debug: false
    Component.onCompleted: {
        web_impl.init(control.debug)
    }
    window: QuickWebViewImpl{
        id: web_impl
    }
    function navigate(url){
        web_impl.navigate(url)
    }
    function runJavaScript(js){
        web_impl.runJavaScript(js)
    }
    function loadHtml(html){
        web_impl.loadHtml(html)
    }
    function bind(name,func){
        web_impl.bind(name,func)
    }
    function unbind(name){
        web_impl.unbind(name)
    }
    function setNavigateListener(func){
        web_impl.setNavigateListener(func)
    }
}
