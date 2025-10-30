#pragma once

#include "qmwidgets_global.h"
#include <QWidget>

struct QmImageShadowWidgetPrivate;

class QMWIDGETS_EXPORT QmImageShadowWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QPixmap shadowPixmap READ shadowPixmap WRITE setShadowPixmap)
    Q_PROPERTY(QRect contentGeometry READ contentGeometry WRITE setContentGeometry)
    Q_PROPERTY(int blurRadius READ blurRadius WRITE setBlurRadius)

public:
    explicit QmImageShadowWidget(QWidget* parent = nullptr);
    ~QmImageShadowWidget() noexcept override;

    void setShadowPixmap(const QPixmap& pixmap);
    void setContentGeometry(const QRect& content_geo);

    const QPixmap& shadowPixmap() const;
    QRect contentGeometry() const;

    int blurRadius() const;
    void setBlurRadius(int blur_radius);

    void setShadowPicture(const QString& path, const QRect& content_geometry, int blur_radius = 10);
    void setShadowEnabled(bool enabled);
    void reset();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QmImageShadowWidgetPrivate* d_ { nullptr };
    Q_DISABLE_COPY(QmImageShadowWidget)
};