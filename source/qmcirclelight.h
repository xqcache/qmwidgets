#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QmCircleLightPrivate;

class QMWIDGETS_EXPORT QmCircleLight : public QFrame
{
	Q_OBJECT

public:
	QmCircleLight(QWidget *parent);
	~QmCircleLight();

protected:
	void paintEvent(QPaintEvent *event);

private:
	QmCircleLightPrivate *d;
};
