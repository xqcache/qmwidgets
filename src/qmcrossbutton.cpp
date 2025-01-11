#include "qmcrossbutton.h"
#include <QEnterEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <optional>

class QmCrossButtonPrivate {
    using DivisionMode = QmCrossButton::DivisionMode;

public:
    QmCrossButtonPrivate(QmCrossButton* parent);

    void switchPixmap(const QPointF& pos);
    void switchPixmap(const std::optional<QmCrossButton::ClickedArea>& clicked_area);
    void setNormal();

    inline QmCrossButton::DivisionMode divisionMode() const
    {
        return division_mode_;
    }

private:
    QRectF buttonRect();
    std::optional<QmCrossButton::ClickedArea> clickedArea(const QPointF& pos);

private:
    Q_DECLARE_PUBLIC(QmCrossButton);
    QmCrossButton* q_ptr = nullptr;

    static constexpr int kBorderSpacing = 0;
    static constexpr int kPixmapSpacing = 10;
    std::array<QPixmap, 4> clicked_pixmaps_;
    QPixmap normal_pixmap_;
    DivisionMode division_mode_ { DivisionMode::FourDirections };

    QMargins pressed_margins_;
    bool pressed_ { false };

    QPixmap* curr_pixmap_ { nullptr };
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
    normal_pixmap_.load(":/qmwidgets/assets/icons/cross_button_4d_normal.png");
    clicked_pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::East)].load(
        ":/qmwidgets/assets/icons/cross_button_4d_east_clicked.png");
    clicked_pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::South)].load(
        ":/qmwidgets/assets/icons/cross_button_4d_south_clicked.png");
    clicked_pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::West)].load(
        ":/qmwidgets/assets/icons/cross_button_4d_west_clicked.png");
    clicked_pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::North)].load(
        ":/qmwidgets/assets/icons/cross_button_4d_north_clicked.png");

    // normal_pixmap_.load(":/qmwidgets/assets/icons/cross_button_ew_normal.png");
    // clicked_pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::East)].load(
    //     ":/qmwidgets/assets/icons/cross_button_ew_east_clicked.png");
    // clicked_pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::West)].load(
    //     ":/qmwidgets/assets/icons/cross_button_ew_west_clicked.png");

    // normal_pixmap_.load(":/qmwidgets/assets/icons/cross_button_sn_normal.png");
    // clicked_pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::South)].load(
    //     ":/qmwidgets/assets/icons/cross_button_sn_south_clicked.png");
    // clicked_pixmaps_[static_cast<size_t>(QmCrossButton::ClickedArea::North)].load(
    //     ":/qmwidgets/assets/icons/cross_button_sn_north_clicked.png");

    curr_pixmap_ = &normal_pixmap_;
    pressed_margins_ = { 1, 1, 1, 1 };
}

QRectF QmCrossButtonPrivate::buttonRect()
{
    Q_Q(QmCrossButton);
    int min_dimen = qMin(q->width(), q->height());
    QRectF result(0, 0, min_dimen, min_dimen);
    result.moveCenter(q->rect().center());
    constexpr int spacing = kBorderSpacing + kPixmapSpacing;
    return result.marginsRemoved(QMarginsF(spacing, spacing, spacing, spacing));
}

std::optional<QmCrossButton::ClickedArea> QmCrossButtonPrivate::clickedArea(const QPointF& pos)
{
    QRectF body_rect = buttonRect();
    QPointF center = body_rect.center();

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
    return std::nullopt;
}

void QmCrossButtonPrivate::switchPixmap(const QPointF& pos)
{
    auto clicked_area = clickedArea(pos);
    switchPixmap(clicked_area);
}

void QmCrossButtonPrivate::switchPixmap(const std::optional<QmCrossButton::ClickedArea>& clicked_area)
{
    if (clicked_area.has_value()) {
        curr_pixmap_ = &clicked_pixmaps_[static_cast<size_t>(clicked_area.value())];
    } else {
        curr_pixmap_ = &normal_pixmap_;
    }
}

void QmCrossButtonPrivate::setNormal()
{
    curr_pixmap_ = &normal_pixmap_;
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
    d_->normal_pixmap_ = pixmap;
}

void QmCrossButton::setEastClickedPixmap(const QPixmap& pixmap)
{
    d_->clicked_pixmaps_[static_cast<size_t>(ClickedArea::East)] = pixmap;
}

void QmCrossButton::setSouthClickedPixmap(const QPixmap& pixmap)
{
    d_->clicked_pixmaps_[static_cast<size_t>(ClickedArea::South)] = pixmap;
}

void QmCrossButton::setWestClickedPixmap(const QPixmap& pixmap)
{
    d_->clicked_pixmaps_[static_cast<size_t>(ClickedArea::West)] = pixmap;
}

void QmCrossButton::setNorthClickedPixmap(const QPixmap& pixmap)
{
    d_->clicked_pixmaps_[static_cast<size_t>(ClickedArea::North)] = pixmap;
}

void QmCrossButton::setClickedPixmap(const QPixmap& east, const QPixmap& south, const QPixmap& west, const QPixmap& north)
{
    d_->clicked_pixmaps_[static_cast<size_t>(ClickedArea::East)] = east;
    d_->clicked_pixmaps_[static_cast<size_t>(ClickedArea::South)] = south;
    d_->clicked_pixmaps_[static_cast<size_t>(ClickedArea::West)] = west;
    d_->clicked_pixmaps_[static_cast<size_t>(ClickedArea::North)] = north;
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
    if (clicked_area.has_value()) {
        emit clicked(clicked_area.value());
    }
    d_->pressed_ = true;
    update();
}

void QmCrossButton::mouseReleaseEvent(QMouseEvent* event)
{
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
}

void QmCrossButton::paintEvent(QPaintEvent* event)
{
    if (d_->curr_pixmap_) {
        QPainter painter(this);
        QRect pixmap_rect = d_->buttonRect().toRect();
        if (d_->pressed_) {
            pixmap_rect = pixmap_rect.marginsRemoved(d_->pressed_margins_);
        }
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.drawPixmap(pixmap_rect, *d_->curr_pixmap_);
    }
}
