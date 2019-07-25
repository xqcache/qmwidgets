#pragma once

#include <QFrame>
#include "qmwidgets_global.h"

class QmImageAnalogClockPrivate;

class QMWIDGETS_EXPORT QmImageAnalogClock : public QFrame
{
	Q_OBJECT

public:
	QmImageAnalogClock(QWidget *parent);
	~QmImageAnalogClock();

	void setDialPixmap(const QPixmap& pixmap);
	QPixmap dialPixmap() const;

	void setHourHandPixmap(const QPixmap& pixmap);
	QPixmap hourHandPixmap() const;

	void setMinuteHandPixmap(const QPixmap& pixmap);
	QPixmap minuteHandPixmap() const;

	void setSecondHandPixmap(const QPixmap& pixmap);
	QPixmap secondHandPixmap() const;

	void setDateTime(const QDateTime& dateTime);
	QDateTime dateTime() const;

protected:
	void paintEvent(QPaintEvent* event);

private:
	QmImageAnalogClockPrivate* d;
};
