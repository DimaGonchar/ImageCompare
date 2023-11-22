#include <QBuffer>
#include <QCryptographicHash>
#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>
#include <QSqlError>
#include <QSqlQuery>

#include "src/ScreenCaptureController.h"

ScreenCaptureController::ScreenCaptureController(QObject *parent)
    : QObject(parent) {
    _captureTimer = std::make_shared<QTimer>(this);
    _captureTimer->setSingleShot(false);

    loadScreenshotesFromDatabase();
}

ScreenCaptureController::~ScreenCaptureController() {
    if (_db.isOpen()) {
        _db.close();
    }
}

void ScreenCaptureController::startCapture() {
    if (!_isCaptureEnabled) {
        _isCaptureEnabled = true;

        connect(_captureTimer.get(), &QTimer::timeout, this, [&]() {
            if (QScreen *screen = QGuiApplication::primaryScreen()) {
                QPixmap screenshot = screen->grabWindow();
                QBuffer screenshotBuffer;
                if (screenshotBuffer.open(QIODevice::ReadWrite)) {
                    screenshot.save(&screenshotBuffer, "PNG");
                    QByteArray dataToSave = screenshotBuffer.buffer().toBase64();
                    QByteArray hash = QCryptographicHash::hash(dataToSave, QCryptographicHash::Md5).toHex();

                    QSqlDatabase::database().transaction();
                    if (_db.open()) {
                        QSqlQuery query(_db);
                        query.prepare("INSERT INTO screenshotes (image, hash, percent) VALUES (:image, "
                                      ":hash, :percent)");
                        query.bindValue(":image", dataToSave);
                        query.bindValue(":hash", QString::fromLatin1(hash));
                        query.bindValue(":percent", 11); // random number for percent

                        const bool transactionSuccess = query.exec();

                        _db.commit();
                        _db.close();

                        QString msg(transactionSuccess ? "Taked new screenshot "
                                                       : "Failed save screenshot to database. Reason:"
                                            + query.lastError().text());
                        qDebug() << msg;
                    }

                    _screenshotsModel->insertScreenshotDetailsToFront(ScreenshotDetails(dataToSave, 11));
                }
            } else {
                qWarning() << "Failed to get the primary screen";
            }
        });

        _captureTimer->start(60000);
    }
}

void ScreenCaptureController::stopCapture() {
    if (_isCaptureEnabled) {
        _isCaptureEnabled = false;

        _captureTimer->stop();
    }
}

void ScreenCaptureController::loadScreenshotesFromDatabase() {
    _screenshotsModel = std::make_shared<ScreenshotsModel>(this);

    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName("Screenshotes");

    if (!_db.open()) {
        qDebug() << "Failed open database - > Error:" + _db.lastError().text();
        return;
    }

    QSqlQuery query(_db);
    query.exec("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='screenshotes'");

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count == 1) {
            qDebug() << "Table 'screenshotes' exists";

            QVector<ScreenshotDetails> screenshotes;

            query.exec("SELECT * from screenshotes");
            while (query.next()) {
                ScreenshotDetails screenshot(query.value("image").toByteArray(),
                                             query.value("percent").toDouble());
                screenshotes.push_front(screenshot);
            }

            _screenshotsModel->initModel(screenshotes);

        } else {
            qDebug() << "Table 'screenshotes' does not exist";

            const bool isTableCreated = query.exec("CREATE TABLE IF NOT EXISTS screenshotes ("
                                                   "image BLOB NOT NULL,"
                                                   "hash TEXT NOT NULL,"
                                                   "percent INTEGER DEFAULT 0);");

            isTableCreated
                ? qDebug() << "Table 'screenshotes' in database Screenshotes created"
                : qDebug() << "Failed create table 'screenshotes'.Reason:" + query.lastError().text();
        }
    }

    _db.close();
}

std::shared_ptr<ScreenshotsModel> ScreenCaptureController::screenshotsModel() const {
    return _screenshotsModel;
}
