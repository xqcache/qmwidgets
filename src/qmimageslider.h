#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QmNinePatchPixmap;
class QmImageSliderPrivate;

class QMWIDGETS_EXPORT QmImageSlider : public QFrame {
    Q_OBJECT
    Q_PROPERTY(float value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(float minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(float maximum READ maximum WRITE setMaximum)
public:
    explicit QmImageSlider(QWidget* parent = nullptr);
    ~QmImageSlider() noexcept override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setValue(float value);
    void setRange(float minimum, float maximum);
    void setMinimum(float v);
    void setMaximum(float v);

    void setBoxPixmap(QmNinePatchPixmap* pixmap);
    void setHandlePixmap(QmNinePatchPixmap* pixmap);
    void setInteractivePadding(const QMarginsF& paddings);
    void setScaleAspectRatio(bool enable);

    float value() const;
    float minimum() const;
    float maximum() const;

signals:
    void valueChanged(float value);
    void sliderReleased();
    void sliderPressed();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    bool isPressed() const;

private:
    QRectF boxRect() const;
    QRectF handleRect() const;
    QRectF handleInteractiveRect() const;

private:
    QmImageSliderPrivate* d_ { nullptr };
};