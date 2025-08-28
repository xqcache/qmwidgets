#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QmImageAnalogClockPrivate;

class QMWIDGETS_EXPORT QmImageAnalogClock : public QFrame {
    Q_OBJECT
    Q_PROPERTY(QPixmap dialPixmap READ dialPixmap WRITE setDialPixmap)
    Q_PROPERTY(QPixmap hourHandPixmap READ hourHandPixmap WRITE setHourHandPixmap)
    Q_PROPERTY(QPixmap minuteHandPixmap READ minuteHandPixmap WRITE setMinuteHandPixmap)
    Q_PROPERTY(QPixmap secondHandPixmap READ secondHandPixmap WRITE setSecondHandPixmap)
    Q_PROPERTY(double hourHandAngleOffset READ hourHandAngleOffset WRITE setHourHandAngleOffset)
    Q_PROPERTY(double minuteHandAngleOffset READ minuteHandAngleOffset WRITE setMinuteHandAngleOffset)
    Q_PROPERTY(double secondHandAngleOffset READ secondHandAngleOffset WRITE setSecondHandAngleOffset)
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime)
public:
    QmImageAnalogClock(QWidget* parent);
    ~QmImageAnalogClock();

    void setDialPixmap(const QPixmap& pixmap);
    QPixmap dialPixmap() const;

    void setHourHandPixmap(const QPixmap& pixmap);
    QPixmap hourHandPixmap() const;

    void setMinuteHandPixmap(const QPixmap& pixmap);
    QPixmap minuteHandPixmap() const;

    void setSecondHandPixmap(const QPixmap& pixmap);
    QPixmap secondHandPixmap() const;

    QDateTime dateTime() const;

    void setHourHandAngleOffset(double hourHandAngleOffset);
    double hourHandAngleOffset();

    void setMinuteHandAngleOffset(double minuteHandAngleOffset);
    double minuteHandAngleOffset();

    void setSecondHandAngleOffset(double secondHandAngleOffset);
    double secondHandAngleOffset();

public Q_SLOTS:
    void setDateTime(const QDateTime& dateTime);

protected:
    void paintEvent(QPaintEvent* event);

private:
    QmImageAnalogClockPrivate* d;
};
