#ifndef SPOTIFYCONNECTMODEL_H
#define SPOTIFYCONNECTMODEL_H

#include "spotifywrapper.h"

#include <QAbstractListModel>

class SpotifyConnectModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ConnectRoles {
            IdRole = Qt::UserRole + 1,
            IsActiveRole,
            IsRestrictedRole,
            NameRole,
            TypeRole,
            VolumePercentRole
        };

    SpotifyConnectModel(SpotifyWrapper * const spotifyWrapper, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

signals:
    void error(const QString &errorString);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    SpotifyWrapper * m_pSpotifyWrapper;
    QList<QJsonObject> m_aConnect;
    void requestConnect();
};

#endif // SPOTIFYCONNECTMODEL_H
