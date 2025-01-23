#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QmImageSliderPrivate;

class QMWIDGETS_EXPORT QmImageSlider : public QFrame {
    Q_OBJECT
    Q_PROPERTY(float value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit QmImageSlider(QWidget* parent = nullptr);
    ~QmImageSlider() noexcept override;

    QSize sizeHint() const override;

    void setValue(float value);
    void setRange(float minimum, float maximum);
    void setMinimum(float v);
    void setMaximum(float v);

    float value() const;

signals:
    void valueChanged(float value);
    void sliderReleased();
    void sliderPressed();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QRectF handleRect() const;
    QRectF handleInteractiveRect() const;

private:
    QmImageSliderPrivate* d_ { nullptr };
};