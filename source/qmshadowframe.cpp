#include "qmshadowframe.h"
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>

namespace {
QPainterPath roundedRectPath(const QRectF& r, qreal radius)
{
    QPainterPath p;
    p.addRoundedRect(r, radius, radius);
    return p;
}
} // namespace

QmShadowFrame::QmShadowFrame(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void QmShadowFrame::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    initPainter(&painter);
    painter.setRenderHint(QPainter::Antialiasing);
    paintShadow(painter);
}

void QmShadowFrame::paintShadow(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF contentRect = QRectF(rect()).adjusted(margin_, margin_, -margin_, -margin_);

    if (shadow_enabled_) {
        // 1) 画阴影（分层“假模糊”）
        // 经验：6~10层足够，alpha 递减，扩散递增
        for (int i = 0; i < margin_; ++i) {
            const qreal t = (i + 1) / 10.0; // 0..1
            const qreal spread = 0.8 + t * margin_; // 扩散半径
            const qreal alpha = 18.0 * (1.0 - t) * (1.0 - t) * (1.0 - t); // 二次衰减更自然

            QRectF sr = contentRect.adjusted(-spread, -spread, spread, spread);
            QPainterPath sp = roundedRectPath(sr, radius_ + spread * 0.55);

            painter.fillPath(sp, QColor(0, 0, 0, int(alpha)));
        }
    }

    // 2) 画内容卡片（背景）
    QPainterPath card = roundedRectPath(shadow_enabled_ ? contentRect : rect(), shadow_enabled_ ? radius_ : 0);
    painter.fillPath(card, palette().brush(QPalette::ColorRole::Window));

    if (shadow_enabled_) {
        // 3) 轻微描边（Win11 很常见的“卡片边”）
        QPen pen(QColor(0, 0, 0, 18));
        pen.setWidthF(1.0);
        painter.setPen(pen);
        painter.drawPath(card);
    }

    if (onPaint) {
        onPaint(&painter);
    }
}

void QmShadowFrame::setShadowEnabled(bool enabled)
{
    shadow_enabled_ = enabled;
    update();
}

void QmShadowFrame::setCornerRadius(qreal r)
{
    radius_ = r;
    update();
}

qreal QmShadowFrame::cornerRadius() const
{
    return radius_;
}

void QmShadowFrame::setShadowMargin(int m)
{
    margin_ = m;
    updateGeometry();
    update();
}

QMargins QmShadowFrame::shadowMargins() const
{
    return QMargins(margin_, margin_, margin_, margin_);
}

int QmShadowFrame::shadowMargin() const
{
    return margin_;
}