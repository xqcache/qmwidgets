#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QmThermometerPrivate;

class QMWIDGETS_EXPORT QmThermometer : public QFrame
{
	Q_OBJECT
	Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
	Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue)
	Q_PROPERTY(double minValue READ minValue WRITE setMinValue)
	Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep)
	Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
	Q_PROPERTY(int majorScaleCount READ majorScaleCount WRITE setMajorScaleCount)
	Q_PROPERTY(int minorScaleCount READ minorScaleCount WRITE setMinorScaleCount)
	Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor)
	Q_PROPERTY(QColor barColor READ barColor WRITE setBarColor)
	Q_PROPERTY(QColor scaleColor READ scaleColor WRITE setScaleColor)
	Q_PROPERTY(QColor valueTextColor READ valueTextColor WRITE setValueTextColor)
	
public:
	QmThermometer(QWidget *parent);
	~QmThermometer();

	void setBgColor(const QColor &color);
	QColor bgColor() const;

	void setBarColor(const QColor &color);
	QColor barColor() const;

	void setScaleColor(const QColor &color);
	QColor scaleColor() const;

	void setValueTextColor(const QColor &color);
	QColor valueTextColor() const;

	void setDecimals(int decimals);
	int decimals();

	void setPrefix(const QString &prefix);
	QString prefix() const;

	void setSuffix(const QString &suffix);
	QString suffix() const;

	void setMajorScaleCount(int count);
	int majorScaleCount();

	void setMinorScaleCount(int count);
	int minorScaleCount();
	
	double value();

	void setMaxValue(double maxValue);
	double maxValue();

	void setMinValue(double minValue);
	double minValue();

	void setRange(double minValue, double maxValue);

	void setSingleStep(double stepValue);
	double singleStep();

	// 设置是否可交互
	void setInteractive(bool yes);

Q_SIGNALS:
	void valueChanged(double value);

public Q_SLOTS:
	void setValue(double value);

protected:
	void paintEvent(QPaintEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	//void mousePressEvent(QMouseEvent *event);

private:
	QmThermometerPrivate *d;
};
