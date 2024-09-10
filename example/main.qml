import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QuickWebView

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: qsTr("QuickWindow")

    RowLayout{
        id: layout_controller
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 10
        }
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
                webview.navigate(text_field_url.text)
            }
        }
    }

    QuickWebView{
        id: webview
        anchors{
            left: parent.left
            right: parent.right
            top: layout_controller.bottom
            bottom: parent.bottom
            margins: 10
        }
        Component.onCompleted: {
            webview.navigate(text_field_url.text)
        }
    }

}
