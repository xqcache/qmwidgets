#include "qmcirclestatus.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

struct QmCircleStatusPrivate {
    QmCircleStatus::Status status { QmCircleStatus::Status::Idle };
    std::array<QColor, 4> colors { "#fc1b4f", "#30e0a0", Qt::red, Qt::green };
    QString text;
};

QmCircleStatus::QmCircleStatus(QWidget* parent)
    : QFrame(parent)
    , d_ptr(new QmCircleStatusPrivate)
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(5, 5);
    effect->setBlurRadius(8);
    effect->setColor("#303238");
    setGraphicsEffect(effect);
}

QmCircleStatus::~QmCircleStatus() noexcept
{
    delete d_ptr;
}

void QmCircleStatus::setStatus(Status status)
{
    Q_D(QmCircleStatus);
    d->status = status;
    update();
}

void QmCircleStatus::setText(const QString& text)
{
    Q_D(QmCircleStatus);
    d->text = text;
    update();
}

QSize QmCircleStatus::minimumSizeHint() const
{
    QSize text_size = fontMetrics().boundingRect(d_ptr->text).size();
    int dimen = qMax(text_size.width(), text_size.height()) * 2;
    return { dimen, dimen };
}

void QmCircleStatus::paintEvent(QPaintEvent* event)
{
    Q_D(QmCircleStatus);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPointF center = QRectF(rect()).center();
    QSizeF size = QSizeF(this->size()) * 0.95;
    qreal radius = qMin(size.width(), size.height()) / 2.0;

    qreal inner_radius = radius * 0.8;

    QPainterPath path;
    path.addEllipse(center, inner_radius, inner_radius);
    path.addEllipse(center, radius, radius);

    QColor color = d->colors[static_cast<int>(d->status)];
    QPen pen = painter.pen();
    pen.setColor(color);
    painter.setPen(pen);
    painter.setBrush(color);
    painter.drawPath(path);

    QRectF text_boundary;
    text_boundary.setWidth(inner_radius * 1.8);
    text_boundary.setHeight(text_boundary.width());
    text_boundary.moveLeft(center.x() - text_boundary.width() / 2.0);
    text_boundary.moveTop(center.y() - text_boundary.height() / 2.0);

    QFont font = painter.font();
    qreal text_width = QFontMetricsF(font).boundingRect(d->text).width();
    qreal curr_size = font.pointSizeF();
    qreal new_size = text_boundary.width() / (text_width / curr_size) * 0.9;
    font.setPointSizeF(new_size);
    painter.setFont(font);
    painter.drawText(text_boundary, Qt::AlignCenter, d->text);
}