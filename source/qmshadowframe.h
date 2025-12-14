#pragma once

#include <QWidget>

class QmShadowFrame : public QWidget {
    Q_OBJECT
public:
    explicit QmShadowFrame(QWidget* parent = nullptr);

    void setCornerRadius(qreal r)
    {
        radius_ = r;
        update();
    }
    void setShadowMargin(int m)
    {
        margin_ = m;
        updateGeometry();
        update();
    }
    int shadowMargin() const
    {
        return margin_;
    }

    void setShadowEnabled(bool enabled);

    qreal cornerRadius() const;

    QMargins shadowMargins() const;

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void paintShadow(QPainter& p);

private:
    qreal radius_ = 8.0;
    int margin_ = 20; // 给阴影留的“内边距”
    bool shadow_enabled_ { true };
};
