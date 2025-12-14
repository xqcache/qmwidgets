#pragma once

#include "qmwidgets_global.h"
#include <QWidget>

class QMWIDGETS_EXPORT QmShadowFrame : public QWidget {
    Q_OBJECT
public:
    explicit QmShadowFrame(QWidget* parent = nullptr);

    void setShadowMargin(int m);
    void setShadowEnabled(bool enabled);

    void setCornerRadius(qreal r);
    qreal cornerRadius() const;

    int shadowMargin() const;
    QMargins shadowMargins() const;

    std::function<void(QPainter*)> onPaint;

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void paintShadow(QPainter& p);

private:
    qreal radius_ = 8.0;
    int margin_ = 20; // 给阴影留的“内边距”
    bool shadow_enabled_ { true };
};
