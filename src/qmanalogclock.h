#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QmAnalogClockPrivate;

class QMWIDGETS_EXPORT QmAnalogClock : public QFrame {
    Q_OBJECT
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime)
    Q_PROPERTY(bool showBorder READ isBorderVisible WRITE setBorderVisible)
    Q_PROPERTY(bool showScaleText READ isScaleTextVisible WRITE setScaleTextVisible)
    Q_PROPERTY(bool showSecondHand READ isSecondHandVisible WRITE setSecondHandVisible)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(QColor dialColor READ dialColor WRITE setDialColor)
    Q_PROPERTY(QColor secondHandColor READ secondHandColor WRITE setSecondHandColor)
    Q_PROPERTY(QColor minuteHandColor READ minuteHandColor WRITE setMinuteHandColor)
    Q_PROPERTY(QColor hourHandColor READ hourHandColor WRITE setHourHandColor)
    Q_PROPERTY(QColor scaleColor READ scaleColor WRITE setScaleColor)
    Q_PROPERTY(QColor scaleTextColor READ scaleTextColor WRITE setScaleTextColor)
    Q_PROPERTY(QColor handCenterColor READ handCenterColor WRITE setHandCenterColor)

public:
    QmAnalogClock(QWidget* parent = Q_NULLPTR);
    ~QmAnalogClock();

    void setBorderVisible(bool visible);
    bool isBorderVisible();

    void setScaleTextVisible(bool visible);
    bool isScaleTextVisible();

    void setSecondHandVisible(bool visible);
    bool isSecondHandVisible();

    void setBorderColor(const QColor& color);
    QColor borderColor();

    void setDialColor(const QColor& color);
    QColor dialColor();

    void setSecondHandColor(const QColor& color);
    QColor secondHandColor();

    void setMinuteHandColor(const QColor& color);
    QColor minuteHandColor();

    void setHourHandColor(const QColor& color);
    QColor hourHandColor();

    void setScaleColor(const QColor& color);
    QColor scaleColor();

    void setScaleTextColor(const QColor& color);
    QColor scaleTextColor();

    void setHandCenterColor(const QColor& color);
    QColor handCenterColor();

    void setDateTime(const QDateTime& dateTime);
    QDateTime dateTime() const;

protected:
    void paintEvent(QPaintEvent* event);

private:
    QmAnalogClockPrivate* d;
};
