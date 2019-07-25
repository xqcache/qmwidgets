#include "stdafx.h"
#include "qmcirclelight.h"
#include <QLinearGradient>
#include <QtMath>

#define MIN(a, b)		(((a) > (b)) ? (b) : (a))

class QmCircleLightPrivate {
private:
	QColor textColor;
	QColor borderOutColorStart;
	QColor borderOutColorEnd;
	QColor borderInColorStart;
	QColor borderInColorEnd;
	QColor lightBgColor;
	QColor darkBgColor;
	QColor overlayColor;
	
	QString text;

	friend class QmCircleLight;
};

QmCircleLight::QmCircleLight(QWidget *parent)
	: QFrame(parent), d(new QmCircleLightPrivate)
{
	d->textColor = QColor(255, 255, 255);
	d->borderOutColorStart = QColor(255, 255, 255);
	d->borderOutColorEnd = QColor(166, 166, 166);
	d->borderInColorStart = QColor(166, 166, 166);
	d->borderInColorEnd = QColor(255, 255, 255);
	d->lightBgColor = QColor(100, 184, 255);
	d->darkBgColor = QColor(100, 100, 100);
	d->overlayColor = QColor(255, 255, 255, 30);
	d->text = QString("");
}

QmCircleLight::~QmCircleLight()
{
	delete d;
}

void QmCircleLight::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	p.setRenderHint(QPainter::Antialiasing);
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::NoBrush);

	QRectF r = rect();
	QPointF c = r.center();
	qreal baseLength = MIN(width(), height());
	qreal borderOutRadius = baseLength * 0.9 / 2;
	qreal borderInRadius = borderOutRadius * 0.9;

	p.save();
	QLinearGradient borderOutGradient = QLinearGradient(c.x(), r.top(), c.x(), r.bottom());
	borderOutGradient.setColorAt(0, d->borderOutColorStart);
	borderOutGradient.setColorAt(1, d->borderOutColorEnd);
	p.setBrush(borderOutGradient);
	p.drawEllipse(c, borderOutRadius, borderOutRadius);

	QLinearGradient borderInGradient = QLinearGradient(c.x(), r.top(), c.x(), r.bottom());
	borderInGradient.setColorAt(0, d->borderInColorStart);
	borderInGradient.setColorAt(1, d->borderInColorEnd);
	p.setBrush(borderInGradient);
	p.drawEllipse(c, borderInRadius, borderInRadius);
	p.restore();

	p.save();
	qreal lightRadius = borderInRadius * 0.9;
	p.setBrush(d->lightBgColor);
	p.drawEllipse(c, lightRadius, lightRadius);
	p.restore();

	p.save();
	p.setPen(Qt::red);
	p.setBrush(d->overlayColor);
	QPainterPath overlayPath;

	QRectF overlayOutRect = QRectF(c.x() - lightRadius, c.y() - lightRadius, 2 * lightRadius, 2 * lightRadius);
	QRectF overlayInRect = overlayOutRect.translated(lightRadius / 4, lightRadius / 4);
	// overlay Œ¥ªÊ÷∆ÕÍ≥…
	overlayPath.moveTo(c.x() + lightRadius * qCos(qDegreesToRadians(30.0)), c.y() - lightRadius * qSin(qDegreesToRadians(30.0)));
	overlayPath.arcTo(overlayOutRect, 30, 180);
	overlayPath.arcTo(r, 90, 90);
	//overlayPath.lineTo(20, 100);

	p.drawPath(overlayPath);
	p.restore();
}
