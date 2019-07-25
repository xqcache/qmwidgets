#include "stdafx.h"
#include "qmimageanalogclock.h"
#include <QDateTime>
#include <QPainter>
#include <QPixmap>


class QmImageAnalogClockPrivate {

private:
	QPixmap dialPixmap;
	QPixmap minuteHandPixmap;
	QPixmap hourHandPixmap;
	QPixmap secondHandPixmap;

	QDateTime dateTime;

	double hourHandAngleOffset;
	double minuteHandAngleOffset;
	double secondHandAngleOffset;

	QTimer* refreshTimer;

	friend class QmImageAnalogClock;
};

QmImageAnalogClock::QmImageAnalogClock(QWidget *parent)
	: QFrame(parent), d(new QmImageAnalogClockPrivate)
{

	d->dialPixmap = QPixmap(":/icons/analog_clock_dial.png");
	d->hourHandPixmap = QPixmap(":/icons/analog_clock_hour_hand.png");
	d->minuteHandPixmap = QPixmap(":/icons/analog_clock_minute_hand.png");
	d->secondHandPixmap = QPixmap(":/icons/analog_clock_second_hand.png");

	d->hourHandAngleOffset = 45;
	d->minuteHandAngleOffset = -45;
	d->secondHandAngleOffset = 170;

	d->refreshTimer = new QTimer(this);
	connect(d->refreshTimer, &QTimer::timeout, [=]() {
		d->dateTime.setSecsSinceEpoch(d->dateTime.toSecsSinceEpoch() + 1);
		update(); });
	d->refreshTimer->start(1000);

	d->dateTime = QDateTime::currentDateTime();
}

QmImageAnalogClock::~QmImageAnalogClock()
{
	delete d;
}

void QmImageAnalogClock::setDialPixmap(const QPixmap& pixmap)
{
	d->dialPixmap = pixmap;
	update();
}

void QmImageAnalogClock::setHourHandPixmap(const QPixmap& pixmap)
{
	d->hourHandPixmap = pixmap;
	update();
}

void QmImageAnalogClock::setMinuteHandPixmap(const QPixmap& pixmap)
{
	d->minuteHandPixmap = pixmap;
	update();
}

void QmImageAnalogClock::setSecondHandPixmap(const QPixmap& pixmap)
{
	d->secondHandPixmap = pixmap;
	update();
}

void QmImageAnalogClock::setDateTime(const QDateTime& dateTime)
{
	d->dateTime = dateTime;
	update();
}

QDateTime QmImageAnalogClock::dateTime() const
{
	return d->dateTime;
}

void QmImageAnalogClock::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	QRectF r;
	r.setSize(QSizeF(MIN(width(), height()), MIN(width(), height())));
	r.moveLeft((width() - r.width()) / 2);
	r.moveTop((height() - r.height()) / 2);
	QPointF c = r.center();

	p.setRenderHint(QPainter::Antialiasing);
	p.setRenderHint(QPainter::SmoothPixmapTransform);
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::NoBrush);

	p.drawPixmap(r, d->dialPixmap, d->dialPixmap.rect());

	QTime currentTime = d->dateTime.time();

	int hour = currentTime.hour();
	int minute = currentTime.minute();
	int second = currentTime.second();

	p.save();
	p.translate(c);
	p.rotate(d->hourHandAngleOffset + hour * 30.0 + minute / 2.0);
	p.translate(-c);
	p.drawPixmap(r, d->hourHandPixmap, d->hourHandPixmap.rect());
	p.restore();

	p.save();
	p.translate(c);
	p.rotate(d->minuteHandAngleOffset + minute * 6.0 + second / 10.0);
	p.translate(-c);
	p.drawPixmap(r, d->minuteHandPixmap, d->minuteHandPixmap.rect());
	p.restore();

	p.save();
	p.translate(c);
	p.rotate(d->secondHandAngleOffset + second * 6.0);
	p.translate(-c);
	p.drawPixmap(r, d->secondHandPixmap, d->secondHandPixmap.rect());
	p.restore();
}

