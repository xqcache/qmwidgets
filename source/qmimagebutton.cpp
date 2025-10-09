#include "qmimagebutton.h"
#include <QIcon>
#include <QPainter>

struct QmImageButtonPrivate {
    bool hovered { false };
    bool pressed { false };
};

QmImageButton::QmImageButton(QWidget* parent)
    : QAbstractButton(parent)
    , d_(new QmImageButtonPrivate)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QmImageButton::QmImageButton(const QIcon& icon, QWidget* parent)
    : QmImageButton(parent)
{
    setIcon(icon);
}

QmImageButton::~QmImageButton() noexcept
{
    delete d_;
}

QSize QmImageButton::sizeHint() const
{
    return iconSize();
}

void QmImageButton::mousePressEvent(QMouseEvent* event)
{
    d_->pressed = true;
    QAbstractButton::mousePressEvent(event);
}

void QmImageButton::mouseReleaseEvent(QMouseEvent* event)
{
    d_->pressed = false;
    QAbstractButton::mouseReleaseEvent(event);
}

void QmImageButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    // initPainter(&painter);
    QSize icon_size = iconSize();

    if (d_->pressed) {
        icon_size.setWidth(icon_size.width() - 2);
        icon_size.setHeight(icon_size.height() - 2);
    }

    int x = (width() - icon_size.width()) / 2;
    int y = (height() - icon_size.height()) / 2;

    if (d_->pressed) {
        x += 1;
        y += 1;
    }

    QPixmap pixmap;
    QStyleOptionButton opt;
    opt.initFrom(this);
    style()->drawControl(QStyle::CE_PushButtonBevel, &opt, &painter, this);

    if (isEnabled()) {
        if (isCheckable()) {
            QIcon::State state = isChecked() ? QIcon::On : QIcon::Off;
            pixmap = icon().pixmap(icon_size, QIcon::Normal, state);
        } else if (d_->hovered) {
            pixmap = icon().pixmap(icon_size, QIcon::Active);
        } else {
            pixmap = icon().pixmap(icon_size, QIcon::Normal);
        }
    } else {
        pixmap = icon().pixmap(icon_size, QIcon::Disabled);
    }
    painter.drawPixmap(x, y, pixmap);
}

void QmImageButton::enterEvent(QEnterEvent* event)
{
    d_->hovered = true;
}

void QmImageButton::leaveEvent(QEvent* event)
{
    d_->hovered = false;
}
