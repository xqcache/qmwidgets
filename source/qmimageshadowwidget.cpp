#include "qmimageshadowwidget.h"
#include "qmninepatchpixmap.h"

struct QmImageShadowWidgetPrivate {
    bool shadow_enabled { false };
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
    d_->shadow_enabled = !d_->pixmap->isNull();
    setContentsMargins(blur_radius, blur_radius, blur_radius, blur_radius);
}

void QmImageShadowWidget::reset()
{
    d_->pixmap.reset();
    d_->shadow_enabled = false;
    setContentsMargins(0, 0, 0, 0);
}

void QmImageShadowWidget::setShadowEnabled(bool enabled)
{
    d_->shadow_enabled = enabled;
    setAttribute(Qt::WA_StyledBackground, !enabled);
    update();
}

void QmImageShadowWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if (!d_->shadow_enabled || !d_->pixmap) {
        QStyleOption opt;
        opt.initFrom(this);
        initPainter(&painter);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter);
        return;
    }
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    d_->pixmap->draw(rect(), &painter);
}