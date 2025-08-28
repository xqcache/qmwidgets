#include "qmframelesswindow.h"
#include "qmwidgets_config.h"

#if defined(WIN32) || defined(WIN64)
// clang-format off
#include <Windows.h>
#include <WinUser.h>
#include <dwmapi.h>
#include <windowsx.h>
#pragma comment(lib, "dwmapi.lib")
// clang-format on
#endif

namespace {
static constexpr int kResizeBorderWidth = 8;
}

struct QmFramelessWindowPrivate {
    QWidget* title_bar { nullptr };

#if (!defined(WIN32) || !defined(WIN64)) || !defined(TAKEOVER_WIN32_HTCAPTION_EVENT)
    QPoint mouse_pos;
    bool mouse_pressed { false };
#endif
};

QmFramelessWindow::QmFramelessWindow(QWidget* parent)
    : QMainWindow(parent)
    , d_(new QmFramelessWindowPrivate)
{
    setWindowFlags(
        windowFlags() | Qt::WindowType::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

#if defined(WIN32) || defined(WIN64)
    auto hwnd = reinterpret_cast<HWND>(this->winId());
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_MINIMIZEBOX | WS_CAPTION | WS_MAXIMIZEBOX);

    // 必须保留一个像素的边框，不然Windows不会绘制窗口阴影
    const MARGINS shadow = { 1, 1, 1, 1 };
    ::DwmExtendFrameIntoClientArea(hwnd, &shadow);
#endif
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
#if (!defined(WIN32) || !defined(WIN64)) || !defined(TAKEOVER_WIN32_HTCAPTION_EVENT)
        d_->title_bar->removeEventFilter(this);
#endif
        removeEventFilter(d_->title_bar);
    }

    d_->title_bar = widget;
    connect(d_->title_bar, &QObject::destroyed, this, [this] { d_->title_bar = nullptr; });
    setMenuWidget(d_->title_bar);
    installEventFilter(d_->title_bar);

#if (!defined(WIN32) || !defined(WIN64)) || !defined(TAKEOVER_WIN32_HTCAPTION_EVENT)
    d_->title_bar->installEventFilter(this);
#endif
}

bool QmFramelessWindow::nativeEvent(const QByteArray& eventType, void* message, qint64* result)
{
#if defined(WIN32) || defined(WIN64)
    MSG* msg = reinterpret_cast<MSG*>(message);
    switch (msg->message) {
    case WM_NCCALCSIZE: {
        *result = 0;
        return true;
    }
    case WM_NCHITTEST: {
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        QPoint mouse_pos(x, y);
        // 判断当前鼠标点击的位置在什么地方（让Windows决定如何进行Resize）
        *result = adjustResizeWindow(msg->hwnd, mouse_pos);
        if (0 != *result) {
            return true;
        }

#if defined(TAKEOVER_WIN32_HTCAPTION_EVENT)
        QWidget* menu_widget = menuWidget();
        if (menu_widget) {
            // support highdpi
            double dpr = devicePixelRatioF();
            QPoint pos = menu_widget->mapFromGlobal(QPoint(x / dpr, y / dpr));
            if (menu_widget->rect().contains(pos)) {
                // 判断当前是否点击在了MenuWidget的子控件上了，如果点在空白区域，就支持标题栏移动
                QWidget* child = menu_widget->childAt(pos);
                if (!child) {
                    *result = HTCAPTION;
                    return true;
                }
            }
        }
#endif
    } break;
    default:
        return QMainWindow::nativeEvent(eventType, message, result);
    }
#else
    return QMainWindow::nativeEvent(eventType, message, result);
#endif
    return QMainWindow::nativeEvent(eventType, message, result);
}

bool QmFramelessWindow::event(QEvent* event)
{
#if defined(WIN32) || defined(WIN64)
    if (event->type() == QEvent::WindowStateChange) {
        if (isMaximized()) {
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
            double dpr = devicePixelRatioF();
            frame.left = std::abs(frame.left / dpr);
            frame.top = std::abs(frame.bottom / dpr) - 1;
            frame.bottom = std::abs(frame.bottom / dpr);
            frame.right = std::abs(frame.right / dpr) - 1;
            setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
        } else {
            setContentsMargins(QMargins(0, 0, 0, 0));
        }
    }
#endif
    return QMainWindow::event(event);
}

bool QmFramelessWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched != d_->title_bar) {
        return QMainWindow::eventFilter(watched, event);
    }

#if (!defined(WIN32) || !defined(WIN64)) || !defined(TAKEOVER_WIN32_HTCAPTION_EVENT)
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
            d_->mouse_pos = static_cast<QMouseEvent*>(event)->pos();
            d_->mouse_pressed = true;
            return true;
        }
        break;
    case QEvent::MouseMove:
        if (d_->mouse_pressed && !window()->isMaximized()) {
            window()->move(window()->pos() + static_cast<QMouseEvent*>(event)->pos() - d_->mouse_pos);
            return true;
        }
        break;
    case QEvent::MouseButtonRelease:
        if (d_->mouse_pressed) {
            d_->mouse_pressed = false;
            return true;
        }
        break;
    default:
        break;
    }
#endif
    return QMainWindow::eventFilter(watched, event);
}

QWidget* QmFramelessWindow::titleBar() const
{
    return d_->title_bar;
}

#if defined(WIN32) || defined(WIN64)
int QmFramelessWindow::adjustResizeWindow(HWND hwnd, const QPoint& pos)
{
    int result = 0;

    RECT winrect;
    GetWindowRect(HWND(winId()), &winrect);

    int mouse_x = pos.x();
    int mouse_y = pos.y();

    bool resizeWidth = minimumWidth() != maximumWidth();
    bool resizeHieght = minimumHeight() != maximumHeight();

    if (resizeWidth) {
        if (mouse_x > winrect.left && mouse_x < winrect.left + kResizeBorderWidth)
            result = HTLEFT;
        if (mouse_x < winrect.right && mouse_x >= winrect.right - kResizeBorderWidth)
            result = HTRIGHT;
    }
    if (resizeHieght) {
        if (mouse_y < winrect.top + kResizeBorderWidth && mouse_y >= winrect.top)
            result = HTTOP;

        if (mouse_y <= winrect.bottom && mouse_y > winrect.bottom - kResizeBorderWidth)
            result = HTBOTTOM;
    }
    if (resizeWidth && resizeHieght) {
        // topleft corner
        if (mouse_x >= winrect.left && mouse_x < winrect.left + kResizeBorderWidth && mouse_y >= winrect.top
            && mouse_y < winrect.top + kResizeBorderWidth) {
            result = HTTOPLEFT;
        }
        // topRight corner
        if (mouse_x <= winrect.right && mouse_x > winrect.right - kResizeBorderWidth && mouse_y >= winrect.top
            && mouse_y < winrect.top + kResizeBorderWidth)
            result = HTTOPRIGHT;
        // leftBottom  corner
        if (mouse_x >= winrect.left && mouse_x < winrect.left + kResizeBorderWidth && mouse_y <= winrect.bottom
            && mouse_y > winrect.bottom - kResizeBorderWidth)
            result = HTBOTTOMLEFT;
        // rightbottom  corner
        if (mouse_x <= winrect.right && mouse_x > winrect.right - kResizeBorderWidth && mouse_y <= winrect.bottom
            && mouse_y > winrect.bottom - kResizeBorderWidth)
            result = HTBOTTOMRIGHT;
    }
    return result;
}
#endif
