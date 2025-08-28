#pragma once

#include "qmwidgets_global.h"
#include <QWidget>

class QMWIDGETS_EXPORT QmGradientSeparator : public QWidget {
    Q_OBJECT
public:
    explicit QmGradientSeparator(QWidget* parent);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setLineWidth(int width);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int line_width_ { 2 };
};