#pragma once

#include <QFile>
#include <QObject>
#include <QSqlDatabase>
#include <QTimer>

#include "src/ScreenshotsModel.h"

class ScreenCaptureController : public QObject {
    Q_OBJECT
public:
    explicit ScreenCaptureController(QObject* parent = nullptr);

    ~ScreenCaptureController();

    std::shared_ptr<ScreenshotsModel> screenshotsModel() const;

public slots:
    void startCapture();
    void stopCapture();

signals:
    void imageSourceChanged();

private:
    void loadScreenshotesFromDatabase();

    QSqlDatabase _db;
    std::shared_ptr<QTimer> _captureTimer;
    std::shared_ptr<ScreenshotsModel> _screenshotsModel;
    bool _isCaptureEnabled = false;
};
