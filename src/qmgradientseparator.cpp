#include "qmgradientseparator.h"
#include <QLinearGradient>
#include <QPainter>

QmGradientSeparator::QmGradientSeparator(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

QSize QmGradientSeparator::sizeHint() const
{
    return QSize(10, 100);
}

QSize QmGradientSeparator::minimumSizeHint() const
{
    return QSize(5, 30);
}

void QmGradientSeparator::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect rect = this->rect();

    QRect gradient_line_rect(rect.right()-3, rect.y(), 3, rect.height()); // 分隔线区域
    QLinearGradient gradient(gradient_line_rect.topLeft(), gradient_line_rect.bottomRight());
    gradient.setColorAt(0.0, QColor(37, 37, 37, 0)); // 左端透明
    gradient.setColorAt(0.2, QColor(37, 37, 37, 255)); // 中间灰色
    gradient.setColorAt(0.8, QColor(37, 37, 37, 255)); // 中间灰色
    gradient.setColorAt(1.0, QColor(37, 37, 37, 0)); // 右端透明
    painter.fillRect(gradient_line_rect, gradient);
}
