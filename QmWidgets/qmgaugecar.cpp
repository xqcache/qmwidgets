#include "stdafx.h"
#include "qmgaugecar.h"

#include <QWheelEvent>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QtMath>

QmGaugeCar::QmGaugeCar(QWidget *parent) :
	QFrame(parent)
{
	m_value = 0;
	m_maxValue = 100;
	m_minValue = 0;
	m_scaleAngleStart = 45;
	m_scaleAngleEnd = 45;
	m_majorScaleCount = 5;
	m_minorScaleCount = 10;
	m_decimalCount = 0;

	m_unitText = QString("km/h");
	m_showUnitText = true;

	m_outeCircleColor = QColor(80, 80, 80);
	m_innerCircleColor = QColor(60, 60, 60);
	m_pieColorStart = QColor(24, 189, 155);
	m_pieColorMid = QColor(218, 218, 0);
	m_pieColorEnd = QColor(255, 107, 107);
	m_coverCircleColor = QColor(100, 100, 100);
	m_scaleColor = QColor(255, 255, 255);
	m_pointerColor = QColor(255, 107, 107, 200);
	m_valueBgColor = QColor(255, 255, 255);
	m_valueColor = QColor(0, 0, 0);

	QGraphicsDropShadowEffect *pEffect = new QGraphicsDropShadowEffect(this);
	pEffect->setOffset(5, 5);
	pEffect->setBlurRadius(8);
	pEffect->setColor(Qt::lightGray);
	setGraphicsEffect(pEffect);
}

void QmGaugeCar::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)

		qreal ox = m_centerPos.x();
	qreal oy = m_centerPos.y();
	qreal scaleOpenAngle = 360 - m_scaleAngleStart - m_scaleAngleEnd;

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(Qt::NoPen);

	// 绘制外圆
	p.save();
	p.setBrush(m_outeCircleColor);
	p.drawEllipse(m_centerPos, m_outerCircleRadius, m_outerCircleRadius);
	p.restore();

	// 绘制内圆
	p.save();
	p.setBrush(m_innerCircleColor);
	p.drawEllipse(m_centerPos, m_innerCircleRadius, m_innerCircleRadius);
	p.restore();

	// 绘制pie
	p.save();
	int pieStartAngle = (270 - m_scaleAngleStart) * 16;
	int pieSpanAngle = static_cast<int>(-scaleOpenAngle * 16);
	QRectF pieRect(ox - m_pieRadius, oy - m_pieRadius, m_pieRadius * 2, m_pieRadius * 2);
	p.setBrush(m_pieColorStart);
	p.drawPie(pieRect, pieStartAngle, static_cast<int>(pieSpanAngle * 0.7));
	p.setBrush(m_pieColorMid);
	p.drawPie(pieRect, pieStartAngle + static_cast<int>(pieSpanAngle * 0.7), static_cast<int>(pieSpanAngle * 0.15));
	p.setBrush(m_pieColorEnd);
	p.drawPie(pieRect, pieStartAngle + static_cast<int>(pieSpanAngle * 0.85), static_cast<int>(pieSpanAngle * 0.15));
	p.restore();

	// 绘制圆环覆盖圆
	p.save();
	p.setBrush(m_coverCircleColor);
	p.drawEllipse(m_centerPos, m_coverCircleRadius, m_coverCircleRadius);
	p.restore();

	// 绘制刻度
	p.save();
	QPen scalePen(m_scaleColor);
	scalePen.setCapStyle(Qt::RoundCap);
	p.setPen(scalePen);
	qreal majorScaleLength = m_scaleRadius * 0.17;
	qreal minorScaleLength = m_scaleRadius * 0.12;
	qreal minorScaleDeltaAngle = scaleOpenAngle / (m_majorScaleCount * m_minorScaleCount);
	qreal majorScaleDeltaAngle = scaleOpenAngle / m_majorScaleCount;
	qreal majorScalePenWidth = majorScaleLength * 0.1;
	qreal minorScalePenWidth = majorScaleLength * 0.08;

	p.translate(m_centerPos);
	p.rotate(90 + m_scaleAngleStart);

	for (int i = 0; i <= m_majorScaleCount; i++) {
		scalePen.setWidthF(majorScalePenWidth);
		p.setPen(scalePen);
		p.drawLine(QPointF(m_scaleRadius, 0), QPointF(m_scaleRadius - majorScaleLength, 0));
		for (int j = 0; j < m_minorScaleCount && i < m_majorScaleCount; j++) {
			scalePen.setWidthF(minorScalePenWidth);
			p.setPen(scalePen);
			p.drawLine(QPointF(m_scaleRadius, 0), QPointF(m_scaleRadius - minorScaleLength, 0));
			p.rotate(minorScaleDeltaAngle);
		}
	}
	p.restore();

	// 绘制刻度值
	p.save();
	p.translate(m_centerPos);
	QFont scaleFont = p.font();
	scaleFont.setPointSizeF(m_baseRadius * 0.08);
	p.setFont(scaleFont);
	p.setPen(scalePen);
	QFontMetricsF fontMetrics(scaleFont);
	QRectF textRectF;

	for (int i = 0; i <= m_majorScaleCount; i++) {
		QString text = QString::number(m_minValue + i * (m_maxValue - m_minValue) / m_majorScaleCount, 'f', m_decimalCount);
		qreal textWidth = fontMetrics.horizontalAdvance(text);
		qreal textHeight = fontMetrics.height();
		qreal omega = qDegreesToRadians(i * majorScaleDeltaAngle + 90 + m_scaleAngleStart);
		textRectF.setX(1.2 * m_scaleRadius * qCos(omega) - textWidth / 2);
		textRectF.setY(1.2 * m_scaleRadius * qSin(omega) - textHeight / 2);
		textRectF.setWidth(textWidth);
		textRectF.setHeight(textHeight);

		p.drawText(textRectF, Qt::AlignCenter, text);
	}
	p.restore();

	qreal valueTextBgWidth = fontMetrics.horizontalAdvance(QString::number(m_maxValue));

	// 绘制指针
	p.save();
	int pointerColorAlpha = m_pointerColor.alpha();
	m_pointerColor.setAlpha(255);
	p.setBrush(m_pointerColor);
	p.drawEllipse(m_centerPos, valueTextBgWidth * 1.2, valueTextBgWidth * 1.2);
	m_pointerColor.setAlpha(pointerColorAlpha);
	p.setBrush(m_pointerColor);
	p.translate(m_centerPos);
	p.rotate(90 + m_scaleAngleStart + m_value * scaleOpenAngle / (m_maxValue - m_minValue));
	QPolygonF pointerPolygon(4);
	pointerPolygon << QPointF(0, 0) << QPointF(0, -m_pointerLength * 0.05) << QPointF(m_pointerLength, 0) << QPointF(0, m_pointerLength * 0.05);
	p.drawPolygon(pointerPolygon);
	p.restore();

	QString valueText = QString::number(m_value, 'f', m_decimalCount);
	QSizeF valueTextSize = fontMetrics.boundingRect(valueText).size();
	// 绘制数值
	p.save();
	p.setBrush(m_valueBgColor);
	p.drawEllipse(m_centerPos, valueTextBgWidth, valueTextBgWidth);
	p.setPen(m_valueColor);
	p.setFont(scaleFont);
	p.translate(ox, oy);
	p.drawText(QRectF(-valueTextSize.width() / 2, -valueTextSize.height() / 2, valueTextSize.width() + 2, valueTextSize.height()), Qt::AlignCenter, valueText);
	p.restore();

	if (m_showUnitText) {
		p.save();
		QFont unitTextFont = p.font();
		unitTextFont.setPointSizeF(m_outerCircleRadius * 0.12);
		p.setFont(unitTextFont);
		p.setPen(m_scaleColor);

		fontMetrics = QFontMetricsF(unitTextFont);
		QRectF unitTextRect = fontMetrics.boundingRect(m_unitText);

		unitTextRect.moveLeft(ox - unitTextRect.width() / 2);
		unitTextRect.moveTop(oy + m_innerCircleRadius * 0.62);

		p.drawText(unitTextRect, Qt::AlignCenter, m_unitText);

		p.restore();
	}

}

void QmGaugeCar::resizeEvent(QResizeEvent *event)
{
	m_width = event->size().width();
	m_height = event->size().height();

	m_baseRadius = qMin(m_width, m_height) / 2;

	m_centerPos = rect().center();
	m_outerCircleRadius = m_baseRadius * 0.99;
	m_innerCircleRadius = m_baseRadius * 0.9;
	m_coverCircleRadius = m_baseRadius * 0.45;
	m_pieRadius = m_baseRadius * 0.52;
	m_scaleRadius = m_baseRadius * 0.65;
	m_pointerLength = m_baseRadius * 0.76;

}

void QmGaugeCar::mousePressEvent(QMouseEvent *event)
{
	m_lastCursorPos = event->pos();
}

void QmGaugeCar::mouseMoveEvent(QMouseEvent *event)
{
	setValue(m_value + (m_lastCursorPos.y() - event->y()) * (m_maxValue - m_minValue) / m_outerCircleRadius);
	m_lastCursorPos = event->pos();
	emit valueChanged(m_value);
}

bool QmGaugeCar::showUnitText() const
{
	return m_showUnitText;
}

void QmGaugeCar::setShowUnitText(bool showUnitText)
{
	m_showUnitText = showUnitText;
	update();
}

QString QmGaugeCar::unitText() const
{
	return m_unitText;
}

void QmGaugeCar::setUnitText(const QString &unitText)
{
	m_unitText = unitText;
}

int QmGaugeCar::decimalCount() const
{
	return m_decimalCount;
}

void QmGaugeCar::setDecimalCount(int decimalCount)
{
	if (decimalCount < 0)
		return;
	m_decimalCount = decimalCount;
	update();
}

void QmGaugeCar::setRange(qreal min, qreal max)
{
	setMinValue(min);
	setMaxValue(max);
}

int QmGaugeCar::minorScaleCount() const
{
	return m_minorScaleCount;
}

void QmGaugeCar::setMinorScaleCount(int minorScaleCount)
{
	if (minorScaleCount < 0)
		return;
	m_minorScaleCount = minorScaleCount;
	update();
}

int QmGaugeCar::majorScaleCount() const
{
	return m_majorScaleCount;
}

void QmGaugeCar::setMajorScaleCount(int majorScaleCount)
{
	if (majorScaleCount < 0)
		return;
	m_majorScaleCount = majorScaleCount;
	update();
}

int QmGaugeCar::scaleAngleEnd() const
{
	return m_scaleAngleEnd;
}

void QmGaugeCar::setScaleAngleEnd(int scaleAngleEnd)
{
	m_scaleAngleEnd = scaleAngleEnd;
	update();
}

int QmGaugeCar::scaleAngleStart() const
{
	return m_scaleAngleStart;
}

void QmGaugeCar::setScaleAngleStart(int scaleAngleStart)
{
	m_scaleAngleStart = scaleAngleStart;
	update();
}

QColor QmGaugeCar::outeCircleColor() const
{
	return m_outeCircleColor;
}

void QmGaugeCar::setOuteCircleColor(const QColor &outeCircleColor)
{
	m_outeCircleColor = outeCircleColor;
}

QColor QmGaugeCar::innerCircleColor() const
{
	return m_innerCircleColor;
}

void QmGaugeCar::setInnerCircleColor(const QColor &innerCircleColor)
{
	m_innerCircleColor = innerCircleColor;
}

QColor QmGaugeCar::pieColorStart() const
{
	return m_pieColorStart;
}

void QmGaugeCar::setPieColorStart(const QColor &pieColorStart)
{
	m_pieColorStart = pieColorStart;
}

QColor QmGaugeCar::pieColorMid() const
{
	return m_pieColorMid;
}

void QmGaugeCar::setPieColorMid(const QColor &pieColorMid)
{
	m_pieColorMid = pieColorMid;
}

QColor QmGaugeCar::pieColorEnd() const
{
	return m_pieColorEnd;
}

void QmGaugeCar::setPieColorEnd(const QColor &pieColorEnd)
{
	m_pieColorEnd = pieColorEnd;
}

QColor QmGaugeCar::coverCircleColor() const
{
	return m_coverCircleColor;
}

void QmGaugeCar::setCoverCircleColor(const QColor &coverCircleColor)
{
	m_coverCircleColor = coverCircleColor;
}

QColor QmGaugeCar::scaleColor() const
{
	return m_scaleColor;
}

void QmGaugeCar::setScaleColor(const QColor &scaleColor)
{
	m_scaleColor = scaleColor;
}

QColor QmGaugeCar::pointerColor() const
{
	return m_pointerColor;
}

void QmGaugeCar::setPointerColor(const QColor &pointerColor)
{
	m_pointerColor = pointerColor;
}

qreal QmGaugeCar::value() const
{
	return m_value;
}

void QmGaugeCar::setValue(const qreal &value)
{
	m_value = value;
	if (m_value > m_maxValue)
		m_value = m_maxValue;
	else if (m_value < m_minValue)
		m_value = m_minValue;
	update();
}

qreal QmGaugeCar::minValue() const
{
	return m_minValue;
}

void QmGaugeCar::setMinValue(const qreal &minValue)
{
	m_minValue = minValue;

	if (m_minValue >= m_maxValue)
		m_minValue = m_maxValue - 1;

	setValue(m_value);
	update();
}

qreal QmGaugeCar::maxValue() const
{
	return m_maxValue;
}

void QmGaugeCar::setMaxValue(const qreal &maxValue)
{
	m_maxValue = maxValue;

	if (m_maxValue <= m_minValue)
		m_maxValue = m_minValue + 1;

	setValue(m_value);
	update();
}

void QmGaugeCar::wheelEvent(QWheelEvent *event)
{
	if (event->angleDelta().y() > 0) {
		setValue(++m_value);
	}
	else {
		setValue(--m_value);
	}

	emit valueChanged(m_value);
}

QColor QmGaugeCar::valueColor() const
{
	return m_valueColor;
}

void QmGaugeCar::setValueColor(const QColor &valueColor)
{
	m_valueColor = valueColor;
}

QColor QmGaugeCar::valueBgColor() const
{
	return m_valueBgColor;
}

void QmGaugeCar::setValueBgColor(const QColor &valueBgColor)
{
	m_valueBgColor = valueBgColor;
}