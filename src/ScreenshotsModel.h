#pragma once

#include "src/ScreenshotDetails.h"
#include <QAbstractItemModel>

class ScreenshotsModel : public QAbstractItemModel {
    Q_OBJECT

public:
    enum Roles {
        Screenshot,
        SimilarityPercentage
    };

    explicit ScreenshotsModel(QObject *parent = nullptr);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    void initModel(const QVector<ScreenshotDetails> &screenshotsDetails);

public slots:
    void insertScreenshotDetailsToFront(const ScreenshotDetails &details);

private:
    void initRoles();

    QHash<int, QByteArray> _roles;
    QVector<ScreenshotDetails> _screenshots;
};
