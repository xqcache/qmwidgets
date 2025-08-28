#include "qmimageshadowwidget.h"
#include "qmninepatchpixmap.h"

struct QmImageShadowWidgetPrivate {

    std::unique_ptr<QmNinePatchPixmap> pixmap;
};

QmImageShadowWidget::QmImageShadowWidget(QWidget* parent)
    : QWidget(parent)
    , d_(new QmImageShadowWidgetPrivate)
{
}

QmImageShadowWidget::~QmImageShadowWidget() noexcept
{
    delete d_;
}

void QmImageShadowWidget::setShadowPicture(const QString& path, const QRect& content_geometry, int blur_radius)
{
    d_->pixmap = std::make_unique<QmNinePatchPixmap>(path, content_geometry);
    setContentsMargins(blur_radius, blur_radius, blur_radius, blur_radius);
}

void QmImageShadowWidget::paintEvent(QPaintEvent* event)
{
    if (!d_->pixmap) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    d_->pixmap->draw(rect(), &painter);
}