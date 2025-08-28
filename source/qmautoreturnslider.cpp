#include "qmautoreturnslider.h"
#include <QPropertyAnimation>
#include <QTimer>

class QmAutoReturnSliderPrivate {
public:
    QmAutoReturnSliderPrivate(QmAutoReturnSlider* parent)
        : q_(parent)
        , repeat_timer_(new QTimer(parent))
    {
    }

private:
    friend class QmAutoReturnSlider;
    QmAutoReturnSlider* q_ { nullptr };
    bool animated_ { false };
    int middle_value_ { 0 };

    int repeat_delay_ { 0 };
    int repeat_interval_ { 500 };
    QTimer* repeat_timer_ { nullptr };
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

void QmAutoReturnSlider::setAutoRepeat(bool on)
{
    if (on) {
        connect(d_->repeat_timer_, &QTimer::timeout, this, [this] { emit valueChanged(value()); });
    } else {
        d_->repeat_timer_->disconnect(this);
    }
}

void QmAutoReturnSlider::setAutoRepeatDelay(int ms)
{
    d_->repeat_delay_ = ms;
}

void QmAutoReturnSlider::setAutoRepeatInterval(int ms)
{
    d_->repeat_timer_->setInterval(ms);
}

void QmAutoReturnSlider::keyPressEvent(QKeyEvent* event)
{
    event->ignore();
}

void QmAutoReturnSlider::mousePressEvent(QMouseEvent* event)
{
    QmImageSlider::mousePressEvent(event);
    if (d_->repeat_timer_->isActive()) {
        d_->repeat_timer_->stop();
    }
    if (d_->repeat_delay_ > 0) {
        QTimer::singleShot(d_->repeat_delay_, this, [this] {
            if (isPressed()) {
                d_->repeat_timer_->start();
            }
        });
    }
}

void QmAutoReturnSlider::onSliderReleased()
{
    d_->repeat_timer_->stop();
    if (d_->animated_) {
        QSignalBlocker* blocker = new QSignalBlocker(this);
        QPropertyAnimation* anim = new QPropertyAnimation(this, "value", this);
        anim->setEasingCurve(QEasingCurve::OutBack);
        anim->setStartValue(value());
        anim->setEndValue(d_->middle_value_);
        anim->setDuration(300);
        connect(anim, &QPropertyAnimation::finished, this, [blocker, this, anim] {
            delete blocker;
            emit valueChanged(value());
        });
        anim->start(QPropertyAnimation::DeleteWhenStopped);
    } else {
        QSignalBlocker blocker(this);
        setValue(d_->middle_value_);
    }
}