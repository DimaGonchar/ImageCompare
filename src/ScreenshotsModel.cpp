#include "src/ScreenshotsModel.h"

ScreenshotsModel::ScreenshotsModel(QObject *parent)
    : QAbstractItemModel(parent) {
    initRoles();
}

QModelIndex ScreenshotsModel::index(int row, int column, const QModelIndex &parent) const {
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex ScreenshotsModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child)
    return QModelIndex();
}

int ScreenshotsModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : _screenshots.size();
}

int ScreenshotsModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 1;
}

QVariant ScreenshotsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.column() != 0 || index.row() < 0 || index.row() >= _screenshots.size())
        return QVariant();

    auto detail = _screenshots[index.row()];
    QByteArray source = detail.imageSource;

    switch (role) {
    case Screenshot:
        return source.startsWith("data:image/jpeg;base64,") ? source : "data:image/jpeg;base64," + source;
    case SimilarityPercentage:
        return detail.similarityPercentage;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ScreenshotsModel::roleNames() const {
    return _roles;
}

void ScreenshotsModel::initModel(const QVector<ScreenshotDetails> &screenshotsDetails) {
    for (const ScreenshotDetails &details : screenshotsDetails) {
        beginInsertRows(QModelIndex(), _screenshots.size(), _screenshots.size());
        _screenshots.push_back(details);
        endInsertRows();
    }
}

void ScreenshotsModel::insertScreenshotDetailsToFront(const ScreenshotDetails &details) {
    QVector<ScreenshotDetails> oldScreenshotes = _screenshots;
    beginResetModel();
    _screenshots.clear();
    endResetModel();

    beginInsertRows(QModelIndex(), _screenshots.size(), _screenshots.size());
    _screenshots.push_back(details);
    endInsertRows();

    for (const ScreenshotDetails &details : oldScreenshotes) {
        beginInsertRows(QModelIndex(), _screenshots.size(), _screenshots.size());
        _screenshots.push_back(details);
        endInsertRows();
    }
}

void ScreenshotsModel::initRoles() {
    _roles[Screenshot] = "screenshot";
    _roles[SimilarityPercentage] = "similarity_percentage";
}
