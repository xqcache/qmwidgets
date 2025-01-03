#include "qmthermometer.h"
#include "stdafx.h"
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QWheelEvent>
#include <QtMath>

class QmThermometerPrivate {
private:
    QColor bgColor;
    QColor scaleColor;
    QColor valueTextColor;
    QColor outerColor;
    QColor barColor;
    double value;
    double maxValue;
    double minValue;

    int majorScaleCount;
    int minorScaleCount;

    bool interactive;
    double singleStep;

    QString prefix;
    QString suffix;
    int decimals;

    bool enable_shadow_ { false };

    friend class QmThermometer;
};

QmThermometer::QmThermometer(QWidget* parent)
    : QFrame(parent)
    , d(new QmThermometerPrivate)
{
    setMinimumWidth(qMax(fontMetrics().boundingRect(QString::number(d->maxValue)).width(),
        fontMetrics().boundingRect(QString::number(d->minValue)).width()));

    d->bgColor = QColor(255, 255, 255);
    d->scaleColor = QColor(205, 58, 48);
    d->outerColor = QColor(253, 72, 64);
    d->barColor = QColor(209, 56, 40);
    d->valueTextColor = QColor(255, 255, 255);
    d->interactive = true;
    d->singleStep = 5;
    d->majorScaleCount = 5;
    d->minorScaleCount = 5;
    d->value = -40;
    d->decimals = 1;
    d->minValue = -40;
    d->maxValue = 100;
    d->prefix = QString("");
    d->suffix = QString("");

    setFocusPolicy(Qt::StrongFocus);
    setShadowEnabled(d->enable_shadow_);
}

QmThermometer::~QmThermometer()
{
    delete d;
}

void QmThermometer::setBgColor(const QColor& color)
{
    d->bgColor = color;
    update();
}

QColor QmThermometer::bgColor() const
{
    return d->bgColor;
}

void QmThermometer::setBarColor(const QColor& color)
{
    d->barColor = color;
    update();
}

QColor QmThermometer::barColor() const
{
    return d->barColor;
}

void QmThermometer::setScaleColor(const QColor& color)
{
    d->scaleColor = color;
    update();
}

QColor QmThermometer::scaleColor() const
{
    return d->scaleColor;
}

void QmThermometer::setValueTextColor(const QColor& color)
{
    d->valueTextColor = color;
    update();
}

QColor QmThermometer::valueTextColor() const
{
    return d->valueTextColor;
}

void QmThermometer::setDecimals(int decimals)
{
    d->decimals = decimals;
    update();
}

int QmThermometer::decimals()
{
    return d->decimals;
}

void QmThermometer::setPrefix(const QString& prefix)
{
    d->prefix = prefix;
    update();
}

QString QmThermometer::prefix() const
{
    return d->prefix;
}

void QmThermometer::setSuffix(const QString& suffix)
{
    d->suffix = suffix;
    update();
}

QString QmThermometer::suffix() const
{
    return d->suffix;
}

void QmThermometer::setMajorScaleCount(int count)
{
    d->majorScaleCount = count;
    update();
}

int QmThermometer::majorScaleCount()
{
    return d->majorScaleCount;
}

void QmThermometer::setMinorScaleCount(int count)
{
    d->minorScaleCount = count;
    update();
}

int QmThermometer::minorScaleCount()
{
    return d->minorScaleCount;
}

void QmThermometer::setValue(double value)
{
    if (!qFuzzyIsNull(d->value - value)) {
        if (value > d->maxValue) {
            d->value = d->maxValue;
            return;
        } else if (value < d->minValue) {
            d->value = d->minValue;
            return;
        }
        d->value = value;
        emit valueChanged(d->value);
        update();
    }
}

double QmThermometer::value()
{
    return d->value;
}

void QmThermometer::setMaxValue(double maxValue)
{
    d->maxValue = maxValue;
    if (d->maxValue < d->minValue) {
        d->maxValue = d->minValue;
    }
    if (d->value > d->maxValue) {
        d->value = d->maxValue;
    }

    update();
}

double QmThermometer::maxValue()
{
    return d->maxValue;
}

void QmThermometer::setMinValue(double minValue)
{
    d->minValue = minValue;
    if (d->minValue > d->maxValue) {
        d->minValue = d->maxValue;
    }
    if (d->value < d->minValue) {
        d->value = d->minValue;
    }
    update();
}

double QmThermometer::minValue()
{
    return d->minValue;
}

void QmThermometer::setRange(double minValue, double maxValue)
{
    setMinValue(minValue);
    setMaxValue(maxValue);
}

void QmThermometer::setSingleStep(double stepValue)
{
    d->singleStep = stepValue;
}

double QmThermometer::singleStep()
{
    return d->singleStep;
}

void QmThermometer::setInteractive(bool yes)
{
    d->interactive = yes;
}

void QmThermometer::wheelEvent(QWheelEvent* event)
{
    if (d->interactive) {
        if (event->angleDelta().y() > 0) {
            setValue(d->value + d->singleStep);
        } else {
            setValue(d->value - d->singleStep);
        }
    } else {
        QWidget::wheelEvent(event);
    }
}

void QmThermometer::keyPressEvent(QKeyEvent* event)
{
    if (d->interactive) {
        if (event->key() == Qt::Key_Up) {
            setValue(d->value + 1);
        } else if (event->key() == Qt::Key_Down) {
            setValue(d->value - 1);
        } else if (event->key() == Qt::Key_PageUp) {
            setValue(d->value + d->singleStep);
        } else if (event->key() == Qt::Key_PageDown) {
            setValue(d->value - d->singleStep);
        }
    } else {
        QWidget::keyPressEvent(event);
    }
}

bool QmThermometer::isShadowEnabled() const
{
    return d->enable_shadow_;
}

void QmThermometer::setShadowEnabled(bool enabled)
{
    auto* effect = graphicsEffect();

    if (enabled) {
        if (!effect) {
            QGraphicsDropShadowEffect* pEffect = new QGraphicsDropShadowEffect(this);
            pEffect->setOffset(5, 5);
            pEffect->setBlurRadius(8);
            pEffect->setColor(Qt::lightGray);
            setGraphicsEffect(pEffect);
        }
    } else {
        setGraphicsEffect(nullptr);
    }
    d->enable_shadow_ = enabled;
}

QSize QmThermometer::sizeHint() const
{
    return minimumSizeHint();
}

QSize QmThermometer::minimumSizeHint() const
{
    QSize min_val_size = fontMetrics().boundingRect(QString::number(d->minValue)).size();
    QSize max_val_size = fontMetrics().boundingRect(QString::number(d->maxValue)).size();

    return QSize(qMax(min_val_size.width(), max_val_size.width()), qMax(min_val_size.height(), max_val_size.height()));
}

void QmThermometer::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);

    QRectF r = rect();
    QPointF c = r.center();

    // p.save();
    // p.setBrush(d->bgColor);
    // p.drawRect(r);
    // p.restore();

    p.save();
    qreal padding = 10;

    qreal baseLength = (MIN(r.width(), r.height()) - 2 * padding) * 0.7;
    qreal outerBottomDiameter = baseLength;
    qreal outerTopDiameter = outerBottomDiameter / 2;

    if (outerBottomDiameter > r.width() - 2 * padding)
        outerBottomDiameter = r.width() - 2 * padding;
    QPen outerPen(d->outerColor);
    outerPen.setWidthF(outerBottomDiameter * 0.05);
    outerPen.setCapStyle(Qt::RoundCap);
    p.setPen(outerPen);
    p.setBrush(d->bgColor);
    QPainterPath outerPath;
    qreal x = c.x() - outerTopDiameter / 2;
    QRectF outerTopRect = QRectF(x, padding, outerTopDiameter, outerTopDiameter);
    QRectF outerBottomRect
        = QRectF(c.x() - outerBottomDiameter / 2, r.height() - padding - outerBottomDiameter, outerBottomDiameter, outerBottomDiameter);
    outerPath.moveTo(x, padding + outerTopDiameter / 2);
    outerPath.arcTo(outerBottomRect, 120, 300);
    outerPath.arcTo(outerTopRect, 0, 180);
    p.drawPath(outerPath);
    p.restore();

    p.save();
    p.setBrush(d->barColor);
    qreal innerBottomPadding = baseLength * 0.1;
    qreal innerTopPadding = innerBottomPadding / (baseLength / outerTopDiameter);
    QPainterPath innerPath;
    QRectF innerBottomRect = outerBottomRect.adjusted(innerBottomPadding, innerBottomPadding, -innerBottomPadding, -innerBottomPadding);
    QRectF innerTopRect = outerTopRect.adjusted(innerTopPadding, innerTopPadding, -innerTopPadding, -innerTopPadding);
    qreal barTop = padding + innerTopPadding + innerTopRect.height() / 2;
    qreal barBottom = innerBottomRect.top();
    qreal barLength = (d->maxValue - d->value) * ((barBottom - barTop) / (d->maxValue - d->minValue));
    // qreal innerTopChordLen = qSin(qDegreesToRadians(22.5)) * innerBottomRect.width() / 2;
    qreal innerTopChordLen = 0.1913415 * innerBottomRect.width();

    innerPath.moveTo(c.x() - innerTopChordLen, barTop + barLength);
    innerPath.arcTo(innerBottomRect, 112.5, 315);
    innerPath.lineTo(c.x() + innerTopChordLen, barTop + barLength);
    p.drawPath(innerPath);
    p.restore();

    p.save();
    p.setPen(d->valueTextColor);
    p.drawText(innerBottomRect, Qt::AlignCenter, d->prefix + QString::number(d->value, 'f', d->decimals) + d->suffix);
    p.restore();

    p.save();
    QPen majorScalePen(d->scaleColor);
    majorScalePen.setCapStyle(Qt::RoundCap);
    majorScalePen.setWidthF(baseLength * 0.05);

    QPen minorScalePen(majorScalePen);
    minorScalePen.setWidthF(baseLength * 0.03);

    qreal majorScaleDelta = (barBottom - barTop) / d->majorScaleCount;
    qreal minorScaleDelta = majorScaleDelta / d->minorScaleCount;
    qreal majorScaleLen = baseLength * 0.25;
    qreal minorScaleLen = baseLength * 0.15;
    qreal scaleLeft = outerTopRect.right() + baseLength * 0.15;
    QLineF baseMajorScaleLine = QLineF(scaleLeft, barTop, scaleLeft + majorScaleLen, barTop);
    QLineF baseMinorScaleLine = QLineF(scaleLeft, barTop, scaleLeft + minorScaleLen, barTop);

    p.setPen(majorScalePen);
    p.drawLine(baseMajorScaleLine);
    baseMajorScaleLine.translate(0, majorScaleDelta);
    for (int i = 0; i < d->majorScaleCount; i++) {
        p.setPen(majorScalePen);
        p.drawLine(baseMajorScaleLine);
        baseMajorScaleLine.translate(0, majorScaleDelta);

        p.setPen(minorScalePen);
        for (int j = 0; j < d->minorScaleCount; j++) {
            p.drawLine(baseMinorScaleLine);
            baseMinorScaleLine.translate(0, minorScaleDelta);
        }
    }

    p.restore();
}
