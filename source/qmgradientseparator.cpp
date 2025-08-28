#include "qmgradientseparator.h"
#include <QLinearGradient>
#include <QPainter>

QmGradientSeparator::QmGradientSeparator(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    // 在styleSheet中配置
    // background-color: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #d9d6d6, stop:0.8 #262626, stop:1 #262626);
}

QSize QmGradientSeparator::sizeHint() const
{
    return QSize(2 * line_width_, 100);
}

QSize QmGradientSeparator::minimumSizeHint() const
{
    return QSize(line_width_, 30);
}

void QmGradientSeparator::setLineWidth(int width)
{
    line_width_ = width;
}

void QmGradientSeparator::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect();
    initPainter(&painter);
    QRect target_rect(rect.right() - line_width_, rect.y(), line_width_, rect.height()); // 分隔线区域
    target_rect.moveCenter(rect.center());
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(target_rect);
}
