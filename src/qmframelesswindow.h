#pragma once

#include "qmwidgets_global.h"
#include <QMainWindow>

class QMWIDGETS_EXPORT QmFramelessWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit QmFramelessWindow(QWidget* parent = nullptr);

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qint64* result) override;
    bool event(QEvent* event) override;

private:
#if defined(WIN32) || defined(WIN64)
    int adjustResizeWindow(HWND hwnd, const QPoint& pos);
#endif

private:
    bool m_isMaximized = false;
};
