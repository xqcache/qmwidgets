#include "qmanalogclock.h"
#include "stdafx.h"
#include <QDateTime>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPen>
#include <QTimer>

class QmAnalogClockPrivate {
private:
    QDateTime dateTime;

    QColor borderColor;
    QColor dialColor;
    QColor secondHandColor;
    QColor minuteHandColor;
    QColor hourHandColor;
    QColor scaleColor;
    QColor scaleTextColor;
    QColor handCenterColor;

    QTimer* refreshTimer;

    bool showBorder;
    bool showScaleText;
    bool showSecondHand;

    friend class QmAnalogClock;
};

QmAnalogClock::QmAnalogClock(QWidget* parent)
    : QFrame(parent)
    , d(new QmAnalogClockPrivate)
{

    d->borderColor = QColor(0, 0, 0);
    d->dialColor = QColor(255, 255, 255);
    d->secondHandColor = QColor(255, 0, 0, 200);
    d->handCenterColor = QColor(255, 0, 0);
    d->minuteHandColor = Qt::black;
    d->hourHandColor = Qt::black;
    d->scaleColor = Qt::black;
    d->scaleTextColor = Qt::black;
    d->refreshTimer = new QTimer(this);
    d->showBorder = true;
    d->showScaleText = true;
    d->showSecondHand = true;

    connect(d->refreshTimer, &QTimer::timeout, [this]() {
        d->dateTime.setSecsSinceEpoch(d->dateTime.toSecsSinceEpoch() + 1);
        update();
    });

    d->refreshTimer->start(1000);
    d->dateTime = QDateTime::currentDateTime();

    QGraphicsDropShadowEffect* pEffect = new QGraphicsDropShadowEffect(this);
    pEffect->setOffset(5, 5);
    pEffect->setBlurRadius(8);
    pEffect->setColor(Qt::lightGray);
    setGraphicsEffect(pEffect);
}

QmAnalogClock::~QmAnalogClock()
{
    delete d;
}

void QmAnalogClock::setBorderVisible(bool visible)
{
    d->showBorder = visible;
    update();
}

bool QmAnalogClock::isBorderVisible()
{
    return d->showBorder;
}

void QmAnalogClock::setScaleTextVisible(bool visible)
{
    d->showScaleText = visible;
    update();
}

bool QmAnalogClock::isScaleTextVisible()
{
    return d->showScaleText;
}

void QmAnalogClock::setSecondHandVisible(bool visible)
{
    d->showSecondHand = visible;
    update();
}

bool QmAnalogClock::isSecondHandVisible()
{
    return d->showSecondHand;
}

void QmAnalogClock::setBorderColor(const QColor& color)
{
    d->borderColor = color;
    update();
}

QColor QmAnalogClock::borderColor()
{
    return d->borderColor;
}

void QmAnalogClock::setDialColor(const QColor& color)
{
    d->dialColor = color;
    update();
}

QColor QmAnalogClock::dialColor()
{
    return d->dialColor;
}

void QmAnalogClock::setSecondHandColor(const QColor& color)
{
    d->secondHandColor = color;
    update();
}

QColor QmAnalogClock::secondHandColor()
{
    return d->secondHandColor;
}

void QmAnalogClock::setMinuteHandColor(const QColor& color)
{
    d->minuteHandColor = color;
    update();
}

QColor QmAnalogClock::minuteHandColor()
{
    return d->minuteHandColor;
}

void QmAnalogClock::setHourHandColor(const QColor& color)
{
    d->hourHandColor = color;
    update();
}

QColor QmAnalogClock::hourHandColor()
{
    return d->hourHandColor;
}

void QmAnalogClock::setScaleColor(const QColor& color)
{
    d->scaleColor = color;
    update();
}

QColor QmAnalogClock::scaleColor()
{
    return d->scaleColor;
}

void QmAnalogClock::setScaleTextColor(const QColor& color)
{
    d->scaleTextColor = color;
    update();
}

QColor QmAnalogClock::scaleTextColor()
{
    return d->scaleTextColor;
}

void QmAnalogClock::setHandCenterColor(const QColor& color)
{
    d->handCenterColor = color;
    update();
}

QColor QmAnalogClock::handCenterColor()
{
    return d->handCenterColor;
}

void QmAnalogClock::setDateTime(const QDateTime& dateTime)
{
    d->dateTime = dateTime;
    update();
}

QDateTime QmAnalogClock::dateTime() const
{
    return d->dateTime;
}

void QmAnalogClock::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::NoBrush);

    QRectF r = rect();
    QPointF c = r.center();

    qreal padding = 10;
    qreal baseLength = qMin(width(), height());

    qreal dialRadius = baseLength * 0.95 / 2;

    p.save();
    if (d->showBorder) {
        QPen dialPen(d->borderColor);
        dialPen.setWidthF(baseLength * 0.01);
        p.setPen(dialPen);
    }
    p.setBrush(d->dialColor);
    p.drawEllipse(c, dialRadius, dialRadius);
    p.restore();

    // ���ƿ̶���
    p.save();
    qreal majorScaleLen = baseLength * 0.06;
    qreal minorScaleLen = majorScaleLen * 0.3;
    QPen majorScalePen(d->scaleColor);
    majorScalePen.setCapStyle(Qt::RoundCap);
    majorScalePen.setWidthF(baseLength * 0.005);
    QPen minorScalePen(majorScalePen);
    minorScalePen.setWidthF(majorScalePen.widthF() * 0.8);
    p.translate(c);
    qreal scalePadding = majorScaleLen * 0.2;
    QLineF majorLine = QLineF(dialRadius - majorScaleLen - scalePadding, 0, dialRadius - scalePadding, 0);
    QLineF minorLine = QLineF(dialRadius - minorScaleLen - scalePadding, 0, dialRadius - scalePadding, 0);

    for (int i = 0; i < 12; i++) {
        p.setPen(majorScalePen);
        p.drawLine(majorLine);
        for (int j = 0; j < 4; j++) {
            p.setPen(minorScalePen);
            p.rotate(6);
            p.drawLine(minorLine);
        }
        p.rotate(6);
    }
    p.restore();

    if (d->showScaleText) {
        p.save();
        p.translate(c);
        p.setPen(d->scaleTextColor);
        QFontMetricsF fontMetrics = p.fontMetrics();
        QRectF scaleTextRect = fontMetrics.boundingRect("99");
        qreal scaleTextRadius = majorLine.x1() - scaleTextRect.width();
        qreal rotateAngle = -60;
        qreal scaleTextHalfHeight = scaleTextRect.height() / 2;
        qreal scaleTextHalfWidth = scaleTextRect.width() / 2;
        for (int i = 1; i <= 12; i++) {
            qreal cx = scaleTextRadius * qCos(qDegreesToRadians(rotateAngle));
            qreal cy = scaleTextRadius * qSin(qDegreesToRadians(rotateAngle));

            scaleTextRect.moveLeft(cx - scaleTextHalfWidth);
            scaleTextRect.moveTop(cy - scaleTextHalfHeight);

            p.drawText(scaleTextRect, Qt::AlignCenter, QString::number(i));

            rotateAngle += 30;
        }
        p.restore();
    }

    // ����ʱ��
    p.save();
    int hour = d->dateTime.time().hour();
    int minute = d->dateTime.time().minute();
    int second = d->dateTime.time().second();

    qreal hourHandLen = dialRadius * 0.55;
    qreal hourHandWith = dialRadius * 0.04;
    QLineF hourLine = QLineF(-hourHandLen * 0.2, 0, hourHandLen * 0.8, 0);
    QPen hourHandPen(d->hourHandColor);
    hourHandPen.setCapStyle(Qt::RoundCap);
    hourHandPen.setWidthF(hourHandWith);
    p.translate(c);
    p.rotate(-90 + hour * 30 + minute / 2.0);
    p.setPen(hourHandPen);
    p.drawLine(hourLine);
    p.restore();

    // ���Ʒ���
    p.save();
    qreal minuteHandLen = dialRadius * 0.85;
    qreal minuteHandWith = dialRadius * 0.03;
    QLineF minuteLine = QLineF(-minuteHandLen * 0.2, 0, minuteHandLen * 0.8, 0);
    QPen minuteHandPen(d->minuteHandColor);
    minuteHandPen.setCapStyle(Qt::RoundCap);
    minuteHandPen.setWidthF(minuteHandWith);
    p.translate(c);
    p.rotate(-90 + minute * 6 + second / 10.0);
    p.setPen(minuteHandPen);
    p.drawLine(minuteLine);
    p.restore();

    if (d->showSecondHand) {
        // ��������
        p.save();
        qreal secondHandLen = dialRadius;
        qreal secondHandWith = dialRadius * 0.01;
        QLineF secondLine = QLineF(-secondHandLen * 0.2, 0, secondHandLen * 0.8, 0);
        QPen secondHandPen(d->secondHandColor);
        secondHandPen.setCapStyle(Qt::RoundCap);
        secondHandPen.setWidthF(secondHandWith);
        p.translate(c);
        p.rotate(-90 + second * 6);
        p.setPen(secondHandPen);
        p.drawLine(secondLine);
    }

    p.setPen(d->handCenterColor);
    p.setBrush(d->handCenterColor);
    p.drawEllipse(QPointF(0, 0), dialRadius * 0.04, dialRadius * 0.04);
    p.restore();
}
