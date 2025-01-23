#include "qmautoreturnslider.h"
#include <QPropertyAnimation>

class QmAutoReturnSliderPrivate {
public:
    QmAutoReturnSliderPrivate(QmAutoReturnSlider* parent)
        : q_(parent)
    {
    }

private:
    friend class QmAutoReturnSlider;
    QmAutoReturnSlider* q_ { nullptr };
    bool animated_ { false };
    int middle_value_ { 0 };
};

QmAutoReturnSlider::QmAutoReturnSlider(QWidget* parent)
    : QmImageSlider(parent)
    , d_(new QmAutoReturnSliderPrivate(this))
{
    connect(this, &QmImageSlider::sliderReleased, this, &QmAutoReturnSlider::onSliderReleased);
}

QmAutoReturnSlider::~QmAutoReturnSlider() noexcept
{
    delete d_;
}

void QmAutoReturnSlider::setAnimated(bool on)
{
    d_->animated_ = on;
}

void QmAutoReturnSlider::setMiddleValue(int value)
{
    d_->middle_value_ = value;
    QSignalBlocker blocker(this);
    setValue(value);
}

void QmAutoReturnSlider::keyPressEvent(QKeyEvent* event)
{
    event->ignore();
}

void QmAutoReturnSlider::mouseMoveEvent(QMouseEvent* event)
{
    QSignalBlocker blocker(this);
    QmImageSlider::mouseMoveEvent(event);
}

void QmAutoReturnSlider::mouseReleaseEvent(QMouseEvent* event)
{
    emit valueChanged(value());
    QmImageSlider::mouseReleaseEvent(event);
}

void QmAutoReturnSlider::mousePressEvent(QMouseEvent* event)
{
    {
        QSignalBlocker blocker(this);
        QmImageSlider::mousePressEvent(event);
    }
}

void QmAutoReturnSlider::onSliderReleased()
{
    if (d_->animated_) {
        QSignalBlocker* blocker = new QSignalBlocker(this);
        QPropertyAnimation* anim = new QPropertyAnimation(this, "value", this);
        anim->setEasingCurve(QEasingCurve::OutBack);
        anim->setStartValue(value());
        anim->setEndValue(d_->middle_value_);
        anim->setDuration(300);
        connect(anim, &QPropertyAnimation::finished, this, [blocker, this, anim] { delete blocker; });
        anim->start(QPropertyAnimation::DeleteWhenStopped);
    } else {
        QSignalBlocker blocker(this);
        setValue(d_->middle_value_);
    }
}