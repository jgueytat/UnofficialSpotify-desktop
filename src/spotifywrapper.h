#ifndef SPOTIFYWRAPPER_H
#define SPOTIFYWRAPPER_H

#include <QtCore>
#include <QtNetwork>

#include <QOAuth2AuthorizationCodeFlow.h>

class SpotifyWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString token MEMBER m_token NOTIFY tokenChanged)
    Q_PROPERTY(QString spotifyPlayer MEMBER m_spotifyPlayerFilePath CONSTANT)
public:
    SpotifyWrapper(const QString &clientIdentifier, const QString &clientIdentifierSharedKey, QObject *parent = nullptr);

    QNetworkReply *requestConnect();

    bool isPermanent() const;
    void setPermanent(bool value);

signals:
    void authenticated();
    void tokenChanged(const QString &token);

public slots:
    void grant();
    void tokenChangedSlot(const QString &token);

private:
    QOAuth2AuthorizationCodeFlow oauth2;
    bool permanent = false;
    QString m_token;
    QString m_spotifyPlayerFilePath;
};

#endif // SPOTIFYWRAPPER_H
