#pragma once

#include "qmwidgets_global.h"
#include <QMainWindow>

struct QmFramelessWindowPrivate;

class QMWIDGETS_EXPORT QmFramelessWindow : public QMainWindow {
    Q_OBJECT
public:
    enum class ResizeRegion {
        None,
        Left,
        Right,
        Top,
        Bottom,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };
    Q_ENUM(ResizeRegion)

    explicit QmFramelessWindow(QWidget* parent = nullptr);
    ~QmFramelessWindow() noexcept override;

    void setTitleBar(QWidget* widget);
    QWidget* titleBar() const;

protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, qint64* result) override;

private:
    ResizeRegion hitTest(const QPoint& pos) const;
    void updateCursor(ResizeRegion region);
    void performResize(const QPoint& pos);

private:
    QmFramelessWindowPrivate* d_ { nullptr };
};
