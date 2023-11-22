#pragma once

#include <QByteArray>

struct ScreenshotDetails {
    QByteArray imageSource;
    double similarityPercentage = 0.0;

    ScreenshotDetails() {
    }

    ScreenshotDetails(const QByteArray& inImageSource, double inSimilarityPercentage)
        : imageSource(inImageSource)
        , similarityPercentage(inSimilarityPercentage) {
    }
};
