#include "qmswitch.h"
#include "stdafx.h"
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

class QmSwitchPrivate {
private:
    QColor checkedBgColor;
    QColor uncheckedBgColor;
    QColor checkedHandleColor;
    QColor uncheckedHandleColor;
    bool checked;
    QPointF handlePos;

    int padding;
    int rectRadius;

    QString checked_text;
    QString unchecked_text;

    friend class QmSwitch;
};

QmSwitch::QmSwitch(QWidget* parent)
    : QFrame(parent)
    , d(new QmSwitchPrivate)
{
    d->checkedBgColor = QColor("#039BE5");
    d->uncheckedBgColor = QColor("#cfd8dc");
    d->checkedHandleColor = QColor("#eeeeee");
    d->uncheckedHandleColor = QColor("#f5f5f5");
    d->rectRadius = 8;
    d->checked = false;
    d->handlePos = QPointF(0, 0);
    d->padding = 2;

    // QGraphicsDropShadowEffect* pEffect = new QGraphicsDropShadowEffect(this);
    // pEffect->setOffset(5, 5);
    // pEffect->setBlurRadius(18);
    // pEffect->setColor(Qt::black);
    // setGraphicsEffect(pEffect);
}

QmSwitch::~QmSwitch()
{
    delete d;
}

QSize QmSwitch::sizeHint() const
{
    return minimumSizeHint();
}

QSize QmSwitch::minimumSizeHint() const
{
    return { fontMetrics().height() * 2, fontMetrics().height() };
}

void QmSwitch::setCheckedBgColor(const QColor& color)
{
    d->checkedBgColor = color;
    update();
}

QColor QmSwitch::checkedBgColor() const
{
    return d->checkedBgColor;
}

void QmSwitch::setUncheckedBgColor(const QColor& color)
{
    d->uncheckedBgColor = color;
    update();
}

QColor QmSwitch::uncheckedBgColor() const
{
    return d->uncheckedBgColor;
}

void QmSwitch::setCheckedHandleColor(const QColor& color)
{
    d->checkedHandleColor = color;
    update();
}

QColor QmSwitch::checkedHandleColor() const
{
    return d->checkedHandleColor;
}

void QmSwitch::setUncheckedHandleColor(const QColor& color)
{
    d->uncheckedHandleColor = color;
    update();
}

QColor QmSwitch::uncheckedHandleColor() const
{
    return d->uncheckedHandleColor;
}

void QmSwitch::setCheckedText(const QString& text)
{
    d->checked_text = text;
    update();
}

void QmSwitch::setUncheckedText(const QString& text)
{
    d->unchecked_text = text;
    update();
}

void QmSwitch::setRectRadius(int rectRadius)
{
    d->rectRadius = rectRadius;
    update();
}

int QmSwitch::rectRadius()
{
    return d->rectRadius;
}

void QmSwitch::setPadding(int padding)
{
    d->padding = padding;
    update();
}

int QmSwitch::padding()
{
    return d->padding;
}

void QmSwitch::_setHandlePos(const QPointF& pos)
{
    d->handlePos = pos;
    update();
}

QPointF QmSwitch::_handlePos() const
{
    return d->handlePos;
}

void QmSwitch::setChecked(bool checked)
{
    d->checked = checked;
    updateHandlePos();
    update();
}

bool QmSwitch::isChecked()
{
    return d->checked;
}

void QmSwitch::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);
    initPainter(&p);

    QRectF r = rect();
    QColor handleColor;
    p.save();
    p.setPen(Qt::NoPen);
    if (d->checked) {
        p.setBrush(d->checkedBgColor);
        handleColor = d->checkedHandleColor;
    } else {
        p.setBrush(d->uncheckedBgColor);
        handleColor = d->uncheckedHandleColor;
    }
    p.drawRoundedRect(r, d->rectRadius, d->rectRadius);

    QRectF handleRect = QRectF(d->handlePos, QSizeF(r.width() / 2, r.height())).adjusted(d->padding, d->padding, -d->padding, -d->padding);
    p.setBrush(handleColor);
    p.drawRoundedRect(handleRect, d->rectRadius - d->padding, d->rectRadius - d->padding);
    p.restore();

    auto text = d->checked ? d->checked_text : d->unchecked_text;
    if (!text.isEmpty()) {
        p.drawText(handleRect, Qt::AlignCenter, text);
    }
}

void QmSwitch::mousePressEvent(QMouseEvent* event)
{
    d->checked = !d->checked;

    QPointF leftPos = QPointF(0, d->handlePos.y());
    QPointF rightPos = QPointF(width() / 2, d->handlePos.y());

    QPropertyAnimation* pAnimation = new QPropertyAnimation(this, "handlePos");
    pAnimation->setEasingCurve(QEasingCurve::OutQuart);
    pAnimation->setDuration(200);
    if (d->checked) {
        pAnimation->setStartValue(leftPos);
        pAnimation->setEndValue(rightPos);
    } else {
        pAnimation->setStartValue(rightPos);
        pAnimation->setEndValue(leftPos);
    }
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(pAnimation, &QPropertyAnimation::finished, this, [this] { emit toggled(d->checked); });
}

void QmSwitch::resizeEvent(QResizeEvent* event)
{
    updateHandlePos();
}

void QmSwitch::updateHandlePos()
{
    if (d->checked) {
        d->handlePos.setX(width() / 2);
    } else {
        d->handlePos.setX(0);
    }
}
