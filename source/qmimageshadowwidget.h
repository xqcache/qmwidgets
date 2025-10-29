#pragma once

#include "qmwidgets_global.h"
#include <QWidget>

struct QmImageShadowWidgetPrivate;

class QMWIDGETS_EXPORT QmImageShadowWidget : public QWidget {
    Q_OBJECT

public:
    explicit QmImageShadowWidget(QWidget* parent = nullptr);
    ~QmImageShadowWidget() noexcept override;

    void setShadowPicture(const QString& path, const QRect& content_geometry, int blur_radius = 10);
    void setShadowEnabled(bool enabled);
    void reset();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QmImageShadowWidgetPrivate* d_ { nullptr };
    Q_DISABLE_COPY(QmImageShadowWidget)
};