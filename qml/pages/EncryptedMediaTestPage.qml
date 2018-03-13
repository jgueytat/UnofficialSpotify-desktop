import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtWebEngine 1.5

Item {
    id: page
    anchors.fill: parent

    WebEngineView {
        id: webView
        anchors.fill: parent

        settings.pluginsEnabled: true

        url: "https://bitmovin.com/mpeg-dash-hls-drm-test-player/"
    }
}
