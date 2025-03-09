#include "qmframelesswindow.h"

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

QmFramelessWindow::QmFramelessWindow(QWidget* parent)
    : QMainWindow(parent)
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
        QWidget* menu_widget = menuWidget();
        if (!menu_widget) {
            return false;
        }
        // support highdpi
        double dpr = devicePixelRatioF();
        QPoint pos = menu_widget->mapFromGlobal(QPoint(x / dpr, y / dpr));
        if (!menu_widget->rect().contains(pos)) {
            return false;
        }
        // 判断当前是否点击在了MenuWidget的子控件上了，如果点在空白区域，就支持标题栏移动
        QWidget* child = menu_widget->childAt(pos);
        if (!child) {
            *result = HTCAPTION;
            return true;
        }
        return false;
    }
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
