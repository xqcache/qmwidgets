#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QMWIDGETS_EXPORT QmGaugeCar : public QFrame
{
	Q_OBJECT
		Q_PROPERTY(QColor outeCircleColor READ outeCircleColor WRITE setOuteCircleColor)
		Q_PROPERTY(QColor innerCircleColor READ innerCircleColor WRITE setInnerCircleColor)
		Q_PROPERTY(QColor pieColorStart READ pieColorStart WRITE setPieColorStart)
		Q_PROPERTY(QColor pieColorMid READ pieColorMid WRITE setPieColorMid)
		Q_PROPERTY(QColor pieColorEnd READ pieColorEnd WRITE setPieColorEnd)
		Q_PROPERTY(QColor coverCircleColor READ coverCircleColor WRITE setCoverCircleColor)
		Q_PROPERTY(QColor scaleColor READ scaleColor WRITE setScaleColor)
		Q_PROPERTY(QColor pointerColor READ pointerColor WRITE setPointerColor)
		Q_PROPERTY(qreal value READ value WRITE setValue)
		Q_PROPERTY(qreal minValue READ minValue WRITE setMinValue)
		Q_PROPERTY(qreal maxValue READ maxValue WRITE setMaxValue)
		Q_PROPERTY(int scaleAngleStart READ scaleAngleStart WRITE setScaleAngleStart)
		Q_PROPERTY(int scaleAngleEnd READ scaleAngleEnd WRITE setScaleAngleEnd)
		Q_PROPERTY(int majorScaleCount READ majorScaleCount WRITE setMajorScaleCount)
		Q_PROPERTY(int minorScaleCount READ minorScaleCount WRITE setMinorScaleCount)
		Q_PROPERTY(int decimalCount READ decimalCount WRITE setDecimalCount)
		Q_PROPERTY(QString unitText READ unitText WRITE setUnitText)
		Q_PROPERTY(bool showUnitText READ showUnitText WRITE setShowUnitText)

public:
	explicit QmGaugeCar(QWidget *parent = nullptr);

	QColor outeCircleColor() const;
	void setOuteCircleColor(const QColor &outeCircleColor);

	QColor innerCircleColor() const;
	void setInnerCircleColor(const QColor &innerCircleColor);

	QColor pieColorStart() const;
	void setPieColorStart(const QColor &pieColorStart);

	QColor pieColorMid() const;
	void setPieColorMid(const QColor &pieColorMid);

	QColor pieColorEnd() const;
	void setPieColorEnd(const QColor &pieColorEnd);

	QColor coverCircleColor() const;
	void setCoverCircleColor(const QColor &coverCircleColor);

	QColor scaleColor() const;
	void setScaleColor(const QColor &scaleColor);

	QColor pointerColor() const;
	void setPointerColor(const QColor &pointerColor);

	qreal value() const;

	qreal minValue() const;
	void setMinValue(const qreal &minValue);

	qreal maxValue() const;
	void setMaxValue(const qreal &maxValue);

	QColor valueBgColor() const;
	void setValueBgColor(const QColor &valueBgColor);

	QColor valueColor() const;
	void setValueColor(const QColor &valueColor);

	int scaleAngleStart() const;
	void setScaleAngleStart(int scaleAngleStart);

	int scaleAngleEnd() const;
	void setScaleAngleEnd(int scaleAngleEnd);

	int majorScaleCount() const;
	void setMajorScaleCount(int majorScaleCount);

	int minorScaleCount() const;
	void setMinorScaleCount(int minorScaleCount);

	int decimalCount() const;
	void setDecimalCount(int decimalCount);

	void setRange(qreal min, qreal max);

	QString unitText() const;
	void setUnitText(const QString &unitText);

	bool showUnitText() const;
	void setShowUnitText(bool showUnitText);

public slots:
	void setValue(const qreal &value);

signals:
	void valueChanged(qreal value);

protected:
	void paintEvent(QPaintEvent *event);
	void wheelEvent(QWheelEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);


private:
	QColor m_outeCircleColor;
	QColor m_innerCircleColor;
	QColor m_pieColorStart;
	QColor m_pieColorMid;
	QColor m_pieColorEnd;
	QColor m_coverCircleColor;
	QColor m_scaleColor;
	QColor m_pointerColor;
	QColor m_valueBgColor;
	QColor m_valueColor;

	qreal m_value;
	qreal m_minValue;
	qreal m_maxValue;

	int m_scaleAngleStart;
	int m_scaleAngleEnd;
	// 刻度盘
	int m_majorScaleCount;
	int m_minorScaleCount;
	// 小数位个数
	int m_decimalCount;

	QString m_unitText;
	bool m_showUnitText;


	qreal m_baseRadius;
	qreal m_outerCircleRadius;
	qreal m_innerCircleRadius;
	qreal m_coverCircleRadius;
	qreal m_pieRadius;
	qreal m_pointerLength;
	qreal m_scaleRadius;

	QPoint m_lastCursorPos;

	QPointF m_centerPos;

	int m_width;
	int m_height;
};