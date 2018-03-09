// Qt
#include <QtGui>

// Qt backport
#include <QOAuthHttpServerReplyHandler.h>

// Unofficial Spotify
#include "spotifywrapper.h"


const QUrl connectUrl("https://api.spotify.com/v1/me/player/devices");


SpotifyWrapper::SpotifyWrapper(const QString &clientIdentifier, const QString &clientIdentifierSharedKey, QObject *parent) :
    QObject(parent)
{
    m_spotifyPlayerFilePath = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).filePath("spotify-player.html");

    auto replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    oauth2.setReplyHandler(replyHandler);
    oauth2.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    oauth2.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    // Connect scope: "user-read-playback-state"
    // Web Playback scope: "streaming", "user-read-birthdate", "user-read-email", "user-read-private"
    oauth2.setScope("user-read-playback-state streaming user-read-birthdate user-read-email user-read-private");

    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged, [=](
            QAbstractOAuth::Status status) {
        if (status == QAbstractOAuth::Status::Granted)
            emit authenticated();
    });
    oauth2.setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QVariantMap *parameters) {
        if (stage == QAbstractOAuth::Stage::RequestingAuthorization && isPermanent())
            parameters->insert("duration", "permanent");
    });
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    oauth2.setClientIdentifier(clientIdentifier);
    oauth2.setClientIdentifierSharedKey(clientIdentifierSharedKey);

    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::tokenChanged,
            this, &SpotifyWrapper::tokenChangedSlot);
}

QNetworkReply *SpotifyWrapper::requestConnect()
{
    return oauth2.get(connectUrl);
}

bool SpotifyWrapper::isPermanent() const
{
    return permanent;
}

void SpotifyWrapper::setPermanent(bool value)
{
    permanent = value;
}

void SpotifyWrapper::grant()
{
    oauth2.grant();
}

void SpotifyWrapper::tokenChangedSlot(const QString &token)
{
    m_token = token;

    QFile l_spotifyPlayerFileIn("://html/spotify-player.html.in");

    Q_ASSERT(l_spotifyPlayerFileIn.open(QIODevice::ReadOnly|QIODevice::Text));
    QByteArray l_spotifyPlayerContent = l_spotifyPlayerFileIn.readAll();
    l_spotifyPlayerFileIn.close();

    l_spotifyPlayerContent.replace(QString("TOKEN"), token.toLatin1());

    QFile l_spotifyPlayerFile(m_spotifyPlayerFilePath);
    Q_ASSERT(l_spotifyPlayerFile.open(QIODevice::WriteOnly|QIODevice::Text));
    l_spotifyPlayerFile.write(l_spotifyPlayerContent);
    l_spotifyPlayerFile.close();

    emit tokenChanged(m_token);
}
