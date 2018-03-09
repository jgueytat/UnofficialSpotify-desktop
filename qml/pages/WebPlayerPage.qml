import QtQuick 2.0
import QtWebView 1.1

Item {
    id: page

    anchors.fill: parent

    WebView {
        id: webView
        anchors.fill: parent

        Connections {
            target: spotifyWrapper
            onTokenChanged:  {
                webView.url = spotifyWrapper.spotifyPlayer;
                webView.reload();
            }
        }
    }
}
