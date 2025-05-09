#include "qmshadowhelperwidget.h"

#include <QGraphicsDropShadowEffect>
#include <QLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollArea>

struct QmShadowHelperWidgetPrivate {
    QWidget* target = nullptr;
    QGraphicsDropShadowEffect* effect { nullptr };
    QBrush background_brush { Qt::white };
    QPen border_pen { Qt::white };
    QPointF border_radius { 0, 0 };
};

QmShadowHelperWidget::QmShadowHelperWidget(QWidget* target)
    : QWidget(target)
    , d_(new QmShadowHelperWidgetPrivate)
{
    d_->target = target;
    d_->effect = new QGraphicsDropShadowEffect(this);
    d_->effect->setColor(Qt::black);
    d_->effect->setBlurRadius(10);
    d_->effect->setOffset(0);
    setGraphicsEffect(d_->effect);
    lower();

    updateParentLayout();

    target->installEventFilter(this);
    target->adjustSize();
}

QmShadowHelperWidget::~QmShadowHelperWidget() noexcept
{
    delete d_;
}

void QmShadowHelperWidget::updateParentLayout()
{
    if (auto* layout = parentWidget()->layout(); layout) {
        qreal layout_margin = qMax(qMax(d_->border_radius.x(), d_->border_radius.y()) / 2.0, d_->effect->blurRadius() + 3);
        layout->setContentsMargins(layout_margin, layout_margin, layout_margin, layout_margin);
    }
}

void QmShadowHelperWidget::setColor(const QColor& color)
{
    d_->effect->setColor(color);
}

void QmShadowHelperWidget::setBlurRadius(qreal radius)
{
    d_->effect->setBlurRadius(radius);
    updateParentLayout();
}

void QmShadowHelperWidget::setOffset(qreal d)
{
    d_->effect->setOffset(d);
}

void QmShadowHelperWidget::setBackgroundBrush(const QBrush& brush)
{
    d_->background_brush = brush;
    update();
}

void QmShadowHelperWidget::setBorderPen(const QPen& pen)
{
    d_->border_pen = pen;
    update();
}

void QmShadowHelperWidget::setBorderRadius(qreal x_radius, qreal y_radius)
{
    d_->border_radius = QPointF(x_radius, y_radius);
    updateParentLayout();
}

qreal QmShadowHelperWidget::blurRadius() const
{
    return d_->effect->blurRadius();
}

bool QmShadowHelperWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Resize) {
        auto* resize_evt = static_cast<QResizeEvent*>(event);
        qreal shadow_padding = blurRadius() + 2;
        setGeometry(shadow_padding / 2.0, shadow_padding / 2.0, resize_evt->size().width() - shadow_padding,
            resize_evt->size().height() - shadow_padding);
    }
    return QWidget::eventFilter(watched, event);
}

void QmShadowHelperWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    qreal half_pen_width = d_->border_pen.widthF() / 2.0;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(d_->border_pen);
    painter.setBrush(d_->background_brush);

    QRectF shadow_rect = rect();
    if (!qFuzzyCompare(half_pen_width, 0)) {
        shadow_rect.adjust(half_pen_width, half_pen_width, -half_pen_width, -half_pen_width);
    }
    painter.drawRoundedRect(shadow_rect, d_->border_radius.x(), d_->border_radius.y());
}
QmShadowHelperWidget* QmShadowHelperWidget::generateShadow(QWidget* widget, qreal blur_radius, qreal offset, const QColor& shadow_color)
{
    auto* shadow_target = widget;
    if (!widget->layout()) {
        if (auto* parent_widget = widget->parentWidget(); parent_widget && parent_widget->property("Usage").toString() == "ShadowEffect") {
            shadow_target = parent_widget;
        } else {
            auto* container = new QWidget(parent_widget);
            if (parent_widget && parent_widget->layout()) {
                parent_widget->layout()->replaceWidget(widget, container);
            }
            container->setProperty("Usage", "ShadowEffect");
            widget->setParent(container);
            auto* layout = new QHBoxLayout(container);
            layout->addWidget(widget);
            shadow_target = container;
        }
    }
    auto* shadow_widget = new QmShadowHelperWidget(shadow_target);
    shadow_widget->setBlurRadius(blur_radius);
    shadow_widget->setOffset(offset);
    shadow_widget->setColor(shadow_color);
    return shadow_widget;
}
