#include "qmcrossbutton.h"
#include <QEnterEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <QTimer>
#include <optional>

class QmCrossButtonPrivate {
    using DivisionMode = QmCrossButton::DivisionMode;

public:
    QmCrossButtonPrivate(QmCrossButton* parent);

    void switchPixmap(const QPointF& pos);
    void switchPixmap(QmCrossButton::ClickedArea clicked_area);
    void setNormal();

    inline QmCrossButton::DivisionMode divisionMode() const
    {
        return division_mode_;
    }

private:
    QRectF buttonRect();
    QmCrossButton::ClickedArea clickedArea(const QPointF& pos);

private:
    Q_DECLARE_PUBLIC(QmCrossButton);
    QmCrossButton* q_ptr = nullptr;

    int repeat_util_ { 0 };
    int repeat_interval_ { 500 };
    QTimer* repeat_timer_ { nullptr };

    static constexpr int kBorderSpacing = 0;
    std::array<QPixmap, 5> pixmaps_;
    DivisionMode division_mode_ { DivisionMode::FourDirections };

    QPainterPath ignored_area_;

    bool pressed_ { false };

    QPixmap* curr_pixmap_ { nullptr };
    QmCrossButton::ClickedArea curr_area_ { QmCrossButton::ClickedArea::None };
    static std::map<QmCrossButton::ClickedArea, std::pair<float, float>> sFourDirectionsAngles;
    static std::map<QmCrossButton::ClickedArea, std::pair<float, float>> sEastWestAngles;
    static std::map<QmCrossButton::ClickedArea, std::pair<float, float>> sSouthNorthAngles;
};

std::map<QmCrossButton::ClickedArea, std::pair<float, float>> QmCrossButtonPrivate::sFourDirectionsAngles {
    { QmCrossButton::ClickedArea::North, { 45, 90 } }, { QmCrossButton::ClickedArea::South, { -45, -90 } },
    { QmCrossButton::ClickedArea::West, { 135, 90 } }, { QmCrossButton::ClickedArea::East, { 45, -90 } }
};

std::map<QmCrossButton::ClickedArea, std::pair<float, float>> QmCrossButtonPrivate::sEastWestAngles {
    { QmCrossButton::ClickedArea::West, { 90, 180 } }, { QmCrossButton::ClickedArea::East, { 90, -180 } }
};

std::map<QmCrossButton::ClickedArea, std::pair<float, float>> QmCrossButtonPrivate::sSouthNorthAngles {
    { QmCrossButton::ClickedArea::North, { 0, 180 } }, { QmCrossButton::ClickedArea::South, { 180, 180 } }
};

QmCrossButtonPrivate::QmCrossButtonPrivate(QmCrossButton* parent)
    : q_ptr(parent)
{
    pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::None)].load(":/qmwidgets/assets/icons/cross_button_4d_normal.png");
    pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::East)].load(":/qmwidgets/assets/icons/cross_button_4d_east_clicked.png");
    pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::South)].load(":/qmwidgets/assets/icons/cross_button_4d_south_clicked.png");
    pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::West)].load(":/qmwidgets/assets/icons/cross_button_4d_west_clicked.png");
    pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::North)].load(":/qmwidgets/assets/icons/cross_button_4d_north_clicked.png");
    repeat_timer_ = new QTimer(parent);
    curr_area_ = QmCrossButton::ClickedArea::None;
    curr_pixmap_ = &pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::None)];
}

QRectF QmCrossButtonPrivate::buttonRect()
{
    Q_Q(QmCrossButton);
    int min_dimen = qMin(q->width(), q->height());
    QRectF result(0, 0, min_dimen, min_dimen);
    result.moveCenter(q->rect().center());
    return result.marginsRemoved(QMarginsF(kBorderSpacing, kBorderSpacing, kBorderSpacing, kBorderSpacing));
}

QmCrossButton::ClickedArea QmCrossButtonPrivate::clickedArea(const QPointF& pos)
{
    QRectF body_rect = buttonRect();
    QPointF center = body_rect.center();

    if (!ignored_area_.isEmpty()) {

        qreal pixmap_dimen = qMin(curr_pixmap_->width(), curr_pixmap_->height());
        qreal body_dimen = qMin(body_rect.width(), body_rect.height());
        qreal ratio = body_dimen / pixmap_dimen;

        QTransform matrix;
        matrix.translate(body_rect.left(), body_rect.top());
        matrix.scale(ratio, ratio);

        auto ignored_polys = ignored_area_.toFillPolygons(matrix);
        for (const auto& poly : ignored_polys) {
            if (poly.containsPoint(pos, Qt::FillRule::OddEvenFill)) {
                return QmCrossButton::ClickedArea::None;
            }
        }
    }

    std::map<QmCrossButton::ClickedArea, std::pair<float, float>> area_angles;
    switch (division_mode_) {
    case DivisionMode::EastWest:
        area_angles = sEastWestAngles;
        break;
    case DivisionMode::SouthNorth:
        area_angles = sSouthNorthAngles;
        break;
    default:
        area_angles = sFourDirectionsAngles;
        break;
    }
    for (const auto& [area, angles] : area_angles) {
        QPainterPath path;
        path.moveTo(center);
        path.arcTo(body_rect, angles.first, angles.second);
        if (path.contains(pos)) {
            return area;
        }
    }
    return QmCrossButton::ClickedArea::None;
}

void QmCrossButtonPrivate::switchPixmap(const QPointF& pos)
{
    auto clicked_area = clickedArea(pos);
    switchPixmap(clicked_area);
}

void QmCrossButtonPrivate::switchPixmap(QmCrossButton::ClickedArea clicked_area)
{
    curr_area_ = clicked_area;
    curr_pixmap_ = &pixmaps_[static_cast<size_t>(curr_area_)];
}

void QmCrossButtonPrivate::setNormal()
{
    curr_pixmap_ = &pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::None)];
}

///////////////////////

QmCrossButton::QmCrossButton(QWidget* parent)
    : QFrame(parent)
    , d_(new QmCrossButtonPrivate(this))
{
    setMouseTracking(true);
}

QmCrossButton::~QmCrossButton() noexcept
{
    delete d_;
}

void QmCrossButton::setDivisionMode(DivisionMode mode)
{
    d_->division_mode_ = mode;
    d_->setNormal();
    update();
}

QmCrossButton::DivisionMode QmCrossButton::divisionMode() const
{
    return d_->divisionMode();
}

void QmCrossButton::setNormalPixmap(const QPixmap& pixmap)
{
    d_->pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::None)] = pixmap;
    // setFixedSize(pixmap.size());
}

void QmCrossButton::setEastClickedPixmap(const QPixmap& pixmap)
{
    d_->pixmaps_[static_cast<size_t>(ClickedArea::East)] = pixmap;
}

void QmCrossButton::setSouthClickedPixmap(const QPixmap& pixmap)
{
    d_->pixmaps_[static_cast<size_t>(ClickedArea::South)] = pixmap;
}

void QmCrossButton::setWestClickedPixmap(const QPixmap& pixmap)
{
    d_->pixmaps_[static_cast<size_t>(ClickedArea::West)] = pixmap;
}

void QmCrossButton::setNorthClickedPixmap(const QPixmap& pixmap)
{
    d_->pixmaps_[static_cast<size_t>(ClickedArea::North)] = pixmap;
}

void QmCrossButton::setClickedPixmap(const QPixmap& east, const QPixmap& south, const QPixmap& west, const QPixmap& north)
{
    d_->pixmaps_[static_cast<size_t>(ClickedArea::East)] = east;
    d_->pixmaps_[static_cast<size_t>(ClickedArea::South)] = south;
    d_->pixmaps_[static_cast<size_t>(ClickedArea::West)] = west;
    d_->pixmaps_[static_cast<size_t>(ClickedArea::North)] = north;
}

QSize QmCrossButton::sizeHint() const
{
    return QSize(100, 100);
}

QSize QmCrossButton::minimumSizeHint() const
{
    return sizeHint();
}

void QmCrossButton::click(ClickedArea area)
{
    emit clicked(area);
}

void QmCrossButton::mousePressEvent(QMouseEvent* event)
{
    auto clicked_area = d_->clickedArea(event->pos());
    if (clicked_area != ClickedArea::None) {
        d_->pressed_ = true;
        emit clicked(clicked_area);
    }
    if (d_->repeat_timer_->isActive()) {
        d_->repeat_timer_->stop();
    }
    if (d_->repeat_util_ > 0) {
        QTimer::singleShot(d_->repeat_util_, this, [this] {
            if (d_->pressed_) {
                d_->repeat_timer_->start();
            }
        });
    }

    update();
}

void QmCrossButton::mouseReleaseEvent(QMouseEvent* event)
{
    d_->repeat_timer_->stop();

    d_->pressed_ = false;
    update();
}

void QmCrossButton::mouseMoveEvent(QMouseEvent* event)
{
    d_->switchPixmap(event->pos());
    update();
}

void QmCrossButton::leaveEvent(QEvent* event)
{
    d_->setNormal();
    update();
    d_->repeat_timer_->stop();
}

void QmCrossButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    if (d_->curr_pixmap_) {
        QRect pixmap_rect = d_->buttonRect().toRect();
        if (d_->pressed_) {
            switch (d_->curr_area_) {
            case ClickedArea::East:
                pixmap_rect.setRight(pixmap_rect.right() - 1);
                break;
            case ClickedArea::South:
                pixmap_rect.setBottom(pixmap_rect.bottom() - 1);
                break;
            case ClickedArea::West:
                pixmap_rect.setLeft(pixmap_rect.left() + 1);
                break;
            case ClickedArea::North:
                pixmap_rect.setTop(pixmap_rect.top() + 1);
                break;
            default:
                break;
            }
        }
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.drawPixmap(pixmap_rect, *d_->curr_pixmap_);
    }
}

void QmCrossButton::setRepeatUtil(int ms)
{
    if (ms > 0) {
        connect(d_->repeat_timer_, &QTimer::timeout, this, [this] { emit clicked(d_->curr_area_); });
    } else {
        d_->repeat_timer_->disconnect(this);
    }
    d_->repeat_util_ = ms;
}

void QmCrossButton::setRepeatInterval(int ms)
{
    d_->repeat_timer_->setInterval(ms);
}

void QmCrossButton::setInteractiveIgnored(const QPainterPath& area)
{
    d_->ignored_area_ = area;
}
