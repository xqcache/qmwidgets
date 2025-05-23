#include "qmimagebutton.h"
#include <QIcon>
#include <QPainter>

struct QmImageButtonPrivate {
    bool hovered { false };
};

QmImageButton::QmImageButton(QWidget* parent)
    : QAbstractButton(parent)
    , d_(new QmImageButtonPrivate)
{
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

void QmImageButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    int x = (width() - iconSize().width()) / 2;
    int y = (height() - iconSize().height()) / 2;
    QPixmap pixmap;

    if (isEnabled()) {
        if (d_->hovered) {
            pixmap = icon().pixmap(iconSize(), QIcon::Active);
        } else {
            pixmap = icon().pixmap(iconSize(), QIcon::Normal, (!isCheckable() || (isCheckable() && isChecked())) ? QIcon::On : QIcon::Off);
        }
    } else {
        pixmap = icon().pixmap(iconSize(), QIcon::Disabled);
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
