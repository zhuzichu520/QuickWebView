import QtQuick
import QtQuick.Controls
import QuickWebView

WindowContainer {
    id: control
    property var url
    property bool debug: false
    signal pageFinished(var url)
    readonly property bool initialized: d.isInitialized
    Component.onCompleted: {
        web_impl.init(control.debug)
    }
    window: QuickWebViewImpl{
        id: web_impl
        onPageFinished:(url)=>{
                           control.pageFinished(url)
                       }
        onInitialized:(success)=>{
                          d.isInitialized =success
                          if(success){
                              if(control.url){
                                  web_impl.navigate(control.url)
                              }
                          }
                      }
    }
    QtObject{
        id: d
        property bool isInitialized: false
    }
    onUrlChanged: {
        if(d.isInitialized){
            web_impl.navigate(control.url)
        }
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
