#pragma once

#include "qmimageslider.h"
#include "qmwidgets_global.h"

class QmAutoReturnSliderPrivate;

class QMWIDGETS_EXPORT QmAutoReturnSlider : public QmImageSlider {
    Q_OBJECT
public:
    explicit QmAutoReturnSlider(QWidget* parent = nullptr);
    ~QmAutoReturnSlider() noexcept override;

    void setAnimated(bool on);
    void setMiddleValue(int value);

    void setAutoRepeat(bool repeat);
    void setAutoRepeatDelay(int ms);
    void setAutoRepeatInterval(int ms);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onSliderReleased();

private:
    QmAutoReturnSliderPrivate* d_ { nullptr };
};