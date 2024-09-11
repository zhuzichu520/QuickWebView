import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QuickWebView

ApplicationWindow {
    id: window
    width: 1280
    height: 900
    visible: true
    title: qsTr("QuickWebView")
    property int count: 0

    component Loading:  WindowContainer {
        window: Window{
            BusyIndicator {
                anchors.centerIn: parent
                width: 80
                height: 80
            }
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        Rectangle {
            id: layout_left
            implicitWidth: 180
            SplitView.maximumWidth: 300
            SplitView.minimumWidth: 160
            color: "lightgray"
            WindowContainer {
                anchors.fill: parent
                window: ApplicationWindow{
                    Label{
                        text: qsTr("Native Area")
                        font.pixelSize: 20
                        anchors{
                            top: parent.top
                            topMargin: 20
                            horizontalCenter: parent.horizontalCenter
                        }
                    }
                    ScrollView{
                        anchors{
                            fill: parent
                            topMargin: 60
                        }
                        ColumnLayout{
                            width: layout_left.width
                            spacing: 12
                            Button{
                                Layout.fillWidth: true
                                Layout.leftMargin: 10
                                Layout.rightMargin: 10
                                text: "调用Web方法"
                                onClicked: {
                                    webview.runJavaScript("increment();")
                                }
                            }
                            Rectangle{
                                width: 60
                                height: 60
                                color: "#4CAF50"
                                Layout.alignment: Qt.AlignHCenter
                                Label{
                                    color: "#FFFFFF"
                                    anchors.centerIn: parent
                                    font.bold: true
                                    font.pixelSize: 20
                                    text: window.count
                                }
                            }
                            TextField{
                                id: text_field_send
                                Layout.fillWidth: true
                                Layout.leftMargin: 10
                                Layout.rightMargin: 10
                                Layout.topMargin: 32
                                placeholderText: "输入要发送的数据"
                            }
                            Button{
                                Layout.fillWidth: true
                                Layout.leftMargin: 10
                                Layout.rightMargin: 10
                                text: "发送"
                                onClicked: {
                                    var data = text_field_send.text
                                    webview.runJavaScript(`onReceive("${data}");`)
                                }
                            }
                            TextField{
                                id: text_field_receive
                                Layout.fillWidth: true
                                Layout.leftMargin: 10
                                Layout.rightMargin: 10
                                placeholderText: "接收web过来的数据"
                            }
                        }
                    }
                }
            }
        }
        Item {
            implicitWidth: 240
            SplitView.maximumWidth: 300
            SplitView.minimumWidth: 160
            QuickWebView{
                id: webview
                anchors.fill: parent
                Component.onCompleted: {
                    webview.navigate("appdir:/webres/test.html")
                    webview.setNavigateListener(function(url){
                        loading.visible =false
                        webview.bind("increment",function(req){
                            window.count = window.count + 1
                            if(window.count>100){
                                window.count = 0
                            }
                        })
                        webview.bind("onReceive",function(req){
                            text_field_receive.text = req
                        })
                    })
                }
            }
            Loading {
                id: loading
                anchors.fill: webview
            }
        }
        Item{
            SplitView.minimumWidth: 50
            SplitView.fillWidth: true
            WindowContainer {
                id: layout_controller
                anchors{
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    margins: 10
                }
                height: 30
                window: ApplicationWindow{
                    RowLayout{
                        width: parent.width
                        TextField{
                            id: text_field_url
                            text: "https://www.bing.com/"
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                        }
                        Button{
                            highlighted: true
                            text: qsTr("Go")
                            Layout.preferredWidth: 80
                            Layout.alignment: Qt.AlignVCenter
                            onClicked: {
                                webview_2.navigate(text_field_url.text)
                            }
                        }
                    }
                }
            }
            QuickWebView{
                id: webview_2
                anchors{
                    left: parent.left
                    right: parent.right
                    top: layout_controller.bottom
                    bottom: parent.bottom
                    margins: 10
                }
                Component.onCompleted: {
                    webview_2.navigate(text_field_url.text)
                    webview_2.setNavigateListener(function(url){
                        loading_2.visible =false
                    })
                }
            }
            Loading {
                id: loading_2
                anchors.fill: webview_2
            }
        }
    }
}
