#include "qmblinklabel.h"
#include <QTimer>

struct QmBlinkLabelPrivate {
    QString text;
    QColor blink_color = Qt::black;
    QColor normal_color = Qt::black;
    int blink_interval { 500 };
    QTimer* blink_timer { nullptr };
    bool blink_state { false };
};

QmBlinkLabel::QmBlinkLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent)
    , d_(new QmBlinkLabelPrivate)
{
}

QmBlinkLabel::QmBlinkLabel(QWidget* parent)
    : QLabel(parent)
    , d_(new QmBlinkLabelPrivate)
{
}

QmBlinkLabel::~QmBlinkLabel() noexcept
{
    delete d_;
}

void QmBlinkLabel::setText(const QString& text)
{
    d_->text = text;
    if (!d_->blink_timer || !d_->blink_timer->isActive()) {
        QLabel::setText(text);
    }
}

QString QmBlinkLabel::text() const
{
    return d_->text;
}

void QmBlinkLabel::setBlinkColor(const QColor& color)
{
    d_->blink_color = color;
}

QColor QmBlinkLabel::blinkColor() const
{
    return d_->blink_color;
}

void QmBlinkLabel::setBlinkInterval(int ms)
{
    d_->blink_interval = ms;
    if (d_->blink_timer && d_->blink_timer->isActive()) {
        d_->blink_timer->stop();
        d_->blink_timer->start(d_->blink_interval);
    }
}

int QmBlinkLabel::blinkInterval() const
{
    return d_->blink_interval;
}

void QmBlinkLabel::startBlink(int ms)
{
    setBlinkInterval(ms);
    startBlink();
}

void QmBlinkLabel::startBlink()
{
    setStyleSheet(QString("color: %1").arg(d_->blink_color.name()));
    if (!d_->blink_timer) {
        d_->blink_timer = new QTimer(this);
        connect(d_->blink_timer, &QTimer::timeout, this, [this] {
            if (!d_->blink_state) {
                QLabel::setText(d_->text);
            } else {
                QLabel::setText("");
            }
            d_->blink_state = !d_->blink_state;
        });
    }
    d_->blink_timer->start(d_->blink_interval);
}

void QmBlinkLabel::stopBlink()
{
    setStyleSheet(QString("color: %1").arg(d_->normal_color.name()));
    if (d_->blink_timer) {
        d_->blink_timer->stop();
    }
}