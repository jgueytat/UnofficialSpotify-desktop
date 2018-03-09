import QtQuick 2.0


Item {
    id: page

    anchors.fill: parent

    ListView {
        id: listView
        anchors.fill: parent

        model: spotifyConnectModel
        delegate: Text { text: qsTr("Name") + " " + name }

        Connections {
            target: spotifyConnectModel
            onError: console.log(errorString)
        }
    }
}
