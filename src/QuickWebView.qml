import QtQuick
import QtQuick.Controls
import QuickWebView

WindowContainer {
    id: control
    property var url
    property bool debug: false
    signal pageFinished(var url)
    Component.onCompleted: {
        web_impl.init(control.debug)
        if(control.url){
            web_impl.navigate(control.url)
        }
    }
    window: QuickWebViewImpl{
        id: web_impl
        onPageFinished:
            (url)=>{
                control.pageFinished(url)
            }
    }
    onUrlChanged: {
        web_impl.navigate(control.url)
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
