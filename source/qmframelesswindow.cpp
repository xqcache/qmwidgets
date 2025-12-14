#include "qmframelesswindow.h"
#include "qmshadowframe.h"
#include <QLayout>
#include <QMouseEvent>

#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#include <windowsx.h>
#endif

struct QmFramelessWindowPrivate {
    QWidget* title_bar { nullptr };
    QmShadowFrame* shadow_frame { nullptr };

    QmFramelessWindow::ResizeRegion resize_region { QmFramelessWindow::ResizeRegion::None };

    QRect resize_geo;
    QPoint resize_mouse_pos;
    bool resize_pressed { false };

    QPoint title_mouse_pos;
    bool title_mouse_pressed { false };
};

QmFramelessWindow::QmFramelessWindow(QWidget* parent)
    : QMainWindow(parent)
    , d_(new QmFramelessWindowPrivate)
{
    setWindowFlags(Qt::WindowType::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
        | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(false);

    d_->shadow_frame = new QmShadowFrame(this);
    d_->shadow_frame->installEventFilter(this);
    setContentsMargins(d_->shadow_frame->shadowMargins());
}

QmFramelessWindow::~QmFramelessWindow() noexcept
{
    delete d_;
}

void QmFramelessWindow::setTitleBar(QWidget* widget)
{
    if (!widget) {
        return;
    }
    if (d_->title_bar) {
        d_->title_bar->removeEventFilter(this);
        removeEventFilter(d_->title_bar);
    }

    d_->title_bar = widget;
    connect(d_->title_bar, &QObject::destroyed, this, [this] { d_->title_bar = nullptr; });
    setMenuWidget(d_->title_bar);
    installEventFilter(d_->title_bar);
    d_->title_bar->installEventFilter(this);
}

bool QmFramelessWindow::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::WindowStateChange:
        if (isMaximized()) {
            setContentsMargins(QMargins(0, 0, 0, 0));
            d_->shadow_frame->setShadowEnabled(false);
        } else {
            setContentsMargins(d_->shadow_frame->shadowMargins());
            d_->shadow_frame->setShadowEnabled(true);
        }
        return true;
    case QEvent::Resize:
        d_->shadow_frame->setGeometry(rect());
        return true;
    case QEvent::MouseButtonPress: {
        auto* mouse_evt = static_cast<QMouseEvent*>(event);
        if (d_->resize_region != ResizeRegion::None && mouse_evt->buttons() == Qt::LeftButton) {
            d_->resize_geo = geometry();
            d_->resize_mouse_pos = mouse_evt->globalPosition().toPoint();
            d_->resize_pressed = true;
        }
    } break;
    case QEvent::MouseMove: {
        auto* mouse_evt = static_cast<QMouseEvent*>(event);
        if (d_->resize_pressed && mouse_evt->buttons() == Qt::NoButton || d_->resize_region != ResizeRegion::None) {
            performResize(mouse_evt->globalPosition().toPoint());
        }
    } break;
    default:
        break;
    }
    return QMainWindow::event(event);
}

bool QmFramelessWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == d_->title_bar) {
        switch (event->type()) {
        case QEvent::MouseButtonDblClick:
            if (window()->isMaximized()) {
                window()->showNormal();
            } else {
                window()->showMaximized();
            }
            return true;
        case QEvent::MouseButtonPress:
            if (static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
                d_->title_mouse_pos = static_cast<QMouseEvent*>(event)->pos();
                d_->title_mouse_pressed = true;
                return true;
            }
            break;
        case QEvent::MouseMove:
            if (d_->resize_region == ResizeRegion::None && d_->title_mouse_pressed && !window()->isMaximized()) {
                window()->move(window()->pos() + static_cast<QMouseEvent*>(event)->pos() - d_->title_mouse_pos);
                return true;
            }
            break;
        case QEvent::MouseButtonRelease:
            if (d_->title_mouse_pressed) {
                d_->title_mouse_pressed = false;
                return true;
            }
            break;
        default:
            break;
        }
    } else if (watched == d_->shadow_frame) {
        switch (event->type()) {
        case QEvent::MouseMove:
            return false;
        default:
            break;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

bool QmFramelessWindow::nativeEvent(const QByteArray& event_type, void* message, qint64* result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
    switch (msg->message) {
    case WM_NCCALCSIZE: {
        *result = 0;
        return true;
    }
    case WM_NCHITTEST: {
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        qreal dpr = devicePixelRatioF();
        QPoint mouse_pos(x / dpr, y / dpr);
        // 判断当前鼠标点击的位置在什么地方（让Windows决定如何进行Resize）
        d_->resize_region = hitTest(mapFromGlobal(mouse_pos));
        updateCursor(d_->resize_region);
    }
    }

    return QMainWindow::nativeEvent(event_type, message, result);
}

QWidget* QmFramelessWindow::titleBar() const
{
    return d_->title_bar;
}

QmFramelessWindow::ResizeRegion QmFramelessWindow::hitTest(const QPoint& pos) const
{
    if (isMaximized() || isFullScreen()) {
        return ResizeRegion::None;
    }
    const auto& geo = d_->shadow_frame->geometry().marginsRemoved(d_->shadow_frame->shadowMargins() - 2);
    qreal corner_radius = d_->shadow_frame->cornerRadius();

    bool is_left = pos.x() <= geo.left();
    bool is_top = pos.y() <= geo.top();
    bool is_right = pos.x() >= geo.right();
    bool is_bottom = pos.y() >= geo.bottom();

    if (pos.x() <= geo.left() + corner_radius && pos.y() <= geo.top() + corner_radius) {
        return ResizeRegion::TopLeft;
    }
    if (pos.x() <= geo.left() + corner_radius && pos.y() >= geo.bottom() - corner_radius) {
        return ResizeRegion::BottomLeft;
    }
    if (pos.x() >= geo.right() - corner_radius && pos.y() <= geo.top() + corner_radius) {
        return ResizeRegion::TopRight;
    }
    if (pos.x() >= geo.right() - corner_radius && pos.y() >= geo.bottom() - corner_radius) {
        return ResizeRegion::BottomRight;
    }

    if (is_left) {
        return ResizeRegion::Left;
    }
    if (is_right) {
        return ResizeRegion::Right;
    }
    if (is_top) {
        return ResizeRegion::Top;
    }
    if (is_bottom) {
        return ResizeRegion::Bottom;
    }

    return ResizeRegion::None;
}

void QmFramelessWindow::updateCursor(ResizeRegion r)
{
    switch (r) {
    case ResizeRegion::Left:
    case ResizeRegion::Right:
        setCursor(Qt::SizeHorCursor);
        break;
    case ResizeRegion::Top:
    case ResizeRegion::Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case ResizeRegion::TopLeft:
    case ResizeRegion::BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case ResizeRegion::TopRight:
    case ResizeRegion::BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void QmFramelessWindow::performResize(const QPoint& pos)
{
    QRect geo = geometry();

    if (d_->resize_region == ResizeRegion::Left || d_->resize_region == ResizeRegion::TopLeft
        || d_->resize_region == ResizeRegion::BottomLeft) {
        geo.setLeft(pos.x() - d_->shadow_frame->shadowMargin());
    }
    if (d_->resize_region == ResizeRegion::Right || d_->resize_region == ResizeRegion::TopRight
        || d_->resize_region == ResizeRegion::BottomRight) {
        geo.setRight(pos.x() + d_->shadow_frame->shadowMargin());
    }
    if (d_->resize_region == ResizeRegion::Top || d_->resize_region == ResizeRegion::TopRight
        || d_->resize_region == ResizeRegion::TopLeft) {
        geo.setTop(pos.y() - d_->shadow_frame->shadowMargin());
    }
    if (d_->resize_region == ResizeRegion::Bottom || d_->resize_region == ResizeRegion::BottomLeft
        || d_->resize_region == ResizeRegion::BottomRight) {
        geo.setBottom(pos.y() + d_->shadow_frame->shadowMargin());
    }
    setGeometry(geo);
}