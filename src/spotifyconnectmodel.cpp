// Unofficial Spotify
#include "spotifyconnectmodel.h"


SpotifyConnectModel::SpotifyConnectModel(SpotifyWrapper * spotifyWrapper, QObject *parent) :
    QAbstractListModel(parent),
    m_pSpotifyWrapper(spotifyWrapper)
{
    Q_ASSERT(m_pSpotifyWrapper);
    m_pSpotifyWrapper->grant();
    connect(m_pSpotifyWrapper, &SpotifyWrapper::authenticated, this, &SpotifyConnectModel::requestConnect);
}

int SpotifyConnectModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_aConnect.size();
}

QHash<int, QByteArray> SpotifyConnectModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[IdRole] = "id";
    roles[IsActiveRole] = "active";
    roles[IsRestrictedRole] = "restricted";
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[VolumePercentRole] = "volumepercent";

    return roles;
}

QVariant SpotifyConnectModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_aConnect.count())
            return QVariant();

    const QJsonObject & l_oConnect = m_aConnect[index.row()];

    if (role == IdRole) {
        Q_ASSERT(!l_oConnect["id"].isUndefined());
        Q_ASSERT(l_oConnect["id"].isString());

        return l_oConnect["id"].toString();
    } else if (role == IsActiveRole) {
        Q_ASSERT(!l_oConnect["is_active"].isUndefined());
        Q_ASSERT(l_oConnect["is_active"].isBool());

        return l_oConnect["is_active"].toBool();
    } else if (role == IsRestrictedRole) {
        Q_ASSERT(!l_oConnect["is_restricted"].isUndefined());
        Q_ASSERT(l_oConnect["is_restricted"].isBool());

        return l_oConnect["is_restricted"].toBool();
    } else if (role == NameRole) {
        Q_ASSERT(!l_oConnect["name"].isUndefined());
        Q_ASSERT(l_oConnect["name"].isString());

        return l_oConnect["name"].toString();
    } else if (role == TypeRole) {
        Q_ASSERT(!l_oConnect["type"].isUndefined());
        Q_ASSERT(l_oConnect["type"].isString());

        return l_oConnect["type"].toString();
    } else if (role == VolumePercentRole) {
        Q_ASSERT(!l_oConnect["volume_percent"].isUndefined());
        Q_ASSERT(l_oConnect["volume_percent"].isDouble());

        return l_oConnect["volume_percent"].toDouble();
    }

    return QVariant();
}

void SpotifyConnectModel::requestConnect()
{
    qDebug() << "[SpotifyConnectModel::requestConnect] : Request launched";
    auto reply = m_pSpotifyWrapper->requestConnect();

    connect(reply, &QNetworkReply::finished, [=]() {
        qDebug() << "[SpotifyConnectModel::requestConnect] : Reply received";

        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit error(reply->errorString());
            return;
        }
        const auto json = reply->readAll();
        const auto document = QJsonDocument::fromJson(json);

        qDebug() << "[SpotifyConnectModel::requestConnect] : Content received";
        qDebug() << document;

        Q_ASSERT(document.isObject());
        const auto rootObject = document.object();

        Q_ASSERT(!rootObject["devices"].isUndefined());
        Q_ASSERT(rootObject["devices"].isArray());
        QJsonArray l_devices = rootObject["devices"].toArray();

        if (l_devices.isEmpty()) {
            emit error(QString("No decive detected"));
            return;
        }

        beginResetModel();
        m_aConnect.clear();
        for (const QJsonValue& l_device : l_devices) {
            Q_ASSERT(l_device.isObject());
            m_aConnect.append(l_device.toObject());
        }
        endResetModel();
    });
}
