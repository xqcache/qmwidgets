#include "qmimageslider.h"
#include "qmninepatchpixmap.h"
#include <QPainter>
#include <memory>

class QmImageSliderPrivate {
public:
    QmImageSliderPrivate(QmImageSlider* q);

private:
    friend class QmImageSlider;
    QmImageSlider* q_ { nullptr };

    std::unique_ptr<QmNinePatchPixmap> box_pixmap_;
    std::unique_ptr<QmNinePatchPixmap> handle_pixmap_;

    int int_value_ { 0 };
    float value_ { 0.0f };
    float minimum_ { 0.0f };
    float maximum_ { 100.0f };
    QMarginsF interactive_paddings_ { 0, 0, 0, 0 };

    QPoint clicked_pos_;
    bool clicked_ { false };

    bool aspect_ratio_ { false };
};

QmImageSliderPrivate::QmImageSliderPrivate(QmImageSlider* q)
    : q_(q)
{
    box_pixmap_ = std::make_unique<QmNinePatchPixmap>(":/qmwidgets/icons/autoreturnslider/slider_box", QRect(17, 24, 63, 253));
    handle_pixmap_ = std::make_unique<QmNinePatchPixmap>(":/qmwidgets/icons/autoreturnslider/slider_handle", QRect(24, 24, 50, 28));
    interactive_paddings_ = { 9, 12, 9, 10 };
}

QmImageSlider::QmImageSlider(QWidget* parent)
    : QFrame(parent)
    , d_(new QmImageSliderPrivate(this))
{
    setMinimumSize(qMax(d_->box_pixmap_->minimumSize().width(), d_->handle_pixmap_->minimumSize().width()),
        qMax(d_->box_pixmap_->minimumSize().height(), d_->handle_pixmap_->minimumSize().height()));
}

QmImageSlider::~QmImageSlider() noexcept
{
    delete d_;
}

QSize QmImageSlider::sizeHint() const
{
    return d_->box_pixmap_->size();
}

QSize QmImageSlider::minimumSizeHint() const
{
    return QSize(qMax(d_->box_pixmap_->minimumSize().width(), d_->handle_pixmap_->minimumSize().width()),
        qMax(d_->box_pixmap_->minimumSize().height(), d_->handle_pixmap_->minimumSize().height()));
}

void QmImageSlider::setBoxPixmap(QmNinePatchPixmap* pixmap)
{
    if (!pixmap) {
        return;
    }
    d_->box_pixmap_.reset(pixmap);
}

void QmImageSlider::setHandlePixmap(QmNinePatchPixmap* pixmap)
{
    if (!pixmap) {
        return;
    }
    d_->handle_pixmap_.reset(pixmap);
}

void QmImageSlider::setInteractivePadding(const QMarginsF& paddings)
{
    d_->interactive_paddings_ = paddings;
}

void QmImageSlider::setScaleAspectRatio(bool enable)
{
    d_->aspect_ratio_ = enable;
    update();
}

void QmImageSlider::setValue(float value)
{
    auto new_value = value;
    new_value = qMax(new_value, d_->minimum_);
    new_value = qMin(new_value, d_->maximum_);
    if (!qFuzzyCompare(new_value, d_->value_)) {
        d_->value_ = new_value;
        emit valueChanged(new_value);
        update();
        if (d_->int_value_ != static_cast<int>(new_value)) {
            d_->int_value_ = static_cast<int>(new_value);
            emit valueChanged(d_->int_value_);
        }
    }
}

void QmImageSlider::setRange(float minimum, float maximum)
{
    setMinimum(minimum);
    setMaximum(maximum);
}

void QmImageSlider::setMinimum(float v)
{
    d_->minimum_ = v;
    if (d_->value_ < d_->minimum_) {
        if (!qFuzzyCompare(d_->value_, d_->minimum_)) {
            d_->value_ = d_->minimum_;
            emit valueChanged(d_->value_);
            update();
            if (d_->int_value_ != static_cast<int>(d_->minimum_)) {
                d_->int_value_ = static_cast<int>(d_->minimum_);
                emit valueChanged(d_->int_value_);
            }
        }
    }
}

void QmImageSlider::setMaximum(float v)
{
    d_->maximum_ = v;
    if (d_->value_ > d_->maximum_) {
        if (!qFuzzyCompare(d_->value_, d_->maximum_)) {
            d_->value_ = d_->maximum_;
            emit valueChanged(d_->value_);
            update();
            if (d_->int_value_ != static_cast<int>(d_->maximum_)) {
                d_->int_value_ = static_cast<int>(d_->maximum_);
                emit valueChanged(d_->int_value_);
            }
        }
    }
}

float QmImageSlider::value() const
{
    return d_->value_;
}

float QmImageSlider::minimum() const
{
    return d_->minimum_;
}

float QmImageSlider::maximum() const
{
    return d_->maximum_;
}

void QmImageSlider::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    d_->box_pixmap_->draw(boxRect().toRect(), &painter);
    d_->handle_pixmap_->draw(handleRect().toRect(), &painter);
}

QRectF QmImageSlider::boxRect() const
{
    if (!d_->aspect_ratio_) {
        return rect();
    }
    QRectF box_rect = d_->box_pixmap_->rect();

    qreal ratio = 1;
    if (box_rect.width() > box_rect.height()) {
        ratio = 1.0 * width() / box_rect.width();
    } else {
        ratio = 1.0 * height() / box_rect.height();
    }

    QSizeF min_size = minimumSizeHint();
    box_rect.setWidth(qMin<qreal>(width(), qMax(box_rect.width() * ratio, min_size.width())));
    box_rect.setHeight(qMin<qreal>(height(), qMax(box_rect.height() * ratio, min_size.height())));
    box_rect.moveCenter(this->rect().center());

    return box_rect;
}

QRectF QmImageSlider::handleRect() const
{
    const QRectF& rect = boxRect();
    const QRectF& box_rect = d_->box_pixmap_->rect();
    const QRectF& box_inner = d_->box_pixmap_->innerRect();
    const QRectF& handle_rect = d_->handle_pixmap_->rect();
    const QRectF& handle_inner = d_->handle_pixmap_->innerRect();
    qreal box_fixed_h = box_rect.height() - box_inner.height();

    qreal box_inner_h_ratio = (rect.height() - box_fixed_h) / box_inner.height();

    // 缩放计算缩放平移后的inner_rect大小及位置
    qreal handle_inner_height = handle_inner.height() * box_inner_h_ratio;
    qreal handle_fixed_width = handle_rect.width() - handle_inner.width();

    qreal value_ratio = 1.0 - (d_->value_ - d_->minimum_) / (d_->maximum_ - d_->minimum_);
    qreal value_height = box_inner.top() + (box_inner.height() * box_inner_h_ratio - handle_inner_height) * value_ratio;

    QRectF handle_target = handle_inner;
    handle_target.setWidth(rect.width() - handle_fixed_width);
    handle_target.setHeight(handle_inner_height);
    handle_target.moveTop(value_height);

    // 将inenr_rect转换为handle_rect
    handle_target.moveLeft(handle_rect.left());
    handle_target.setWidth(handle_target.width() + handle_fixed_width);
    handle_target.setTop(handle_target.top() - handle_inner.top());
    handle_target.setBottom(handle_target.bottom() + (handle_rect.bottom() - handle_inner.bottom()));

    handle_target.moveLeft(this->rect().center().x() - handle_target.width() / 2.0);

    return handle_target;
}

QRectF QmImageSlider::handleInteractiveRect() const
{
    QRectF result = handleRect();
    const QRectF& handle_rect = d_->handle_pixmap_->rect();
    const QRectF& handle_inner = d_->handle_pixmap_->innerRect();

    result.setLeft(result.left() + handle_inner.left());
    result.setTop(result.top() + handle_inner.top());
    result.setRight(result.right() - (handle_rect.right() - handle_inner.right()));
    result.setBottom(result.bottom() - (handle_rect.bottom() - handle_inner.bottom()));

    return result.marginsAdded(d_->interactive_paddings_);
}

void QmImageSlider::mousePressEvent(QMouseEvent* event)
{
    if (handleInteractiveRect().contains(event->pos())) {
        d_->clicked_pos_ = event->pos();
        d_->clicked_ = true;
        emit sliderPressed();
    }
}

void QmImageSlider::mouseReleaseEvent(QMouseEvent* event)
{
    if (d_->clicked_) {
        d_->clicked_ = false;
        emit sliderReleased();
    }
}

void QmImageSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (!d_->clicked_) {
        return;
    }
    qreal sliding_offset = event->pos().y() - d_->clicked_pos_.y();

    const QRectF& box_rect = d_->box_pixmap_->rect();
    const QRectF& box_inner = d_->box_pixmap_->innerRect();
    const QRectF& handle_inner = d_->handle_pixmap_->innerRect();

    qreal box_inner_h_ratio = (height() - (box_rect.height() - box_inner.height())) / box_inner.height();
    qreal value_height = (box_inner.height() - handle_inner.height()) * box_inner_h_ratio;
    qreal value_pixels = value_height / (d_->maximum_ - d_->minimum_);

    qreal target_value = d_->value_ - sliding_offset / value_pixels;
    setValue(target_value);
    if (target_value >= d_->minimum_ && target_value <= d_->maximum_) {
        d_->clicked_pos_ = event->pos();
    }
}

int QmImageSlider::intValue() const
{
    return d_->int_value_;
}

bool QmImageSlider::isPressed() const
{
    return d_->clicked_;
}