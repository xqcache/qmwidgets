#include "qmshadowhelperwidget.h"

#include <QGraphicsDropShadowEffect>
#include <QLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollArea>

struct QmShadowHelperWidgetPrivate {
    QGraphicsDropShadowEffect* effect { nullptr };
    QSize parent_size;
    QBrush background_brush { Qt::white };
    QPen border_pen { Qt::white };
    QPointF border_radius { 0, 0 };
};

QmShadowHelperWidget::QmShadowHelperWidget(QWidget* parent)
    : QWidget(parent)
    , d_(new QmShadowHelperWidgetPrivate)
{
    if (auto* layout = parent->layout(); layout) {
        layout->setContentsMargins(25, 25, 25, 25);
    }
    d_->parent_size = parent->size();
    parent->installEventFilter(this);

    d_->effect = new QGraphicsDropShadowEffect(this);
    d_->effect->setColor(Qt::black);
    d_->effect->setBlurRadius(20);
    d_->effect->setOffset(0);
    setGraphicsEffect(d_->effect);
    lower();
}

QmShadowHelperWidget::~QmShadowHelperWidget() noexcept
{
    delete d_;
}

void QmShadowHelperWidget::setColor(const QColor& color)
{
    d_->effect->setColor(color);
}

void QmShadowHelperWidget::setBlurRadius(qreal radius)
{
    d_->effect->setBlurRadius(radius);
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
}

qreal QmShadowHelperWidget::blurRadius() const
{
    return d_->effect->blurRadius();
}

bool QmShadowHelperWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Resize) {
        auto* resize_evt = static_cast<QResizeEvent*>(event);
        d_->parent_size = resize_evt->size();
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