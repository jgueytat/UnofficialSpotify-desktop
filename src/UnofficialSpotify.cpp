// Qt
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// Unofficial Spotify
#include "clientidsecret/spotifysecret.h"
#include "spotifywrapper.h"
#include "spotifyconnectmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // C++ objects
    SpotifyWrapper m_SpotifyWrapper(SPOTIFY_CLIENT_ID, SPOTIFY_CLIENT_SECRET, &app);
    SpotifyConnectModel m_SpotifyConnectModel(&m_SpotifyWrapper, &app);

    // Connect C++ to QML
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("spotifyWrapper", &m_SpotifyWrapper);
    engine.rootContext()->setContextProperty("spotifyConnectModel", &m_SpotifyConnectModel);

    engine.load(QUrl(QStringLiteral("qrc:/UnofficialSpotify.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
