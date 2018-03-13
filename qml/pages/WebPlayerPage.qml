import QtQuick 2.0
import QtWebEngine 1.5

Item {
    id: page
    anchors.fill: parent

    WebEngineView {
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
