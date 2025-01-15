#pragma once

#include "qmwidgets_global.h"
#include <QSlider>

class QmAutoReturnSliderPrivate;

class QMWIDGETS_EXPORT QmAutoReturnSlider : public QSlider {
    Q_OBJECT
public:
    explicit QmAutoReturnSlider(QWidget* parent = nullptr);
    ~QmAutoReturnSlider() noexcept override;

    void setAnimated(bool on);
    void setMiddleValue(int value);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onSliderReleased();

private:
    QmAutoReturnSliderPrivate* d_ { nullptr };
};