#pragma once

#include "qmwidgets_global.h"
#include <QDialog>

struct QmFramelessDialogPrivate;

class QMWIDGETS_EXPORT QmFramelessDialog : public QDialog {
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

    explicit QmFramelessDialog(QWidget* parent = nullptr);
    ~QmFramelessDialog() noexcept override;

    void setTitleBar(QWidget* widget);
    QWidget* titleBar() const;

    void setWidget(QWidget* widget);
    QWidget* widget() const;

protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, qint64* result) override;

private:
    ResizeRegion hitTest(const QPoint& pos) const;
    void updateCursor(ResizeRegion region);
    void performResize(const QPoint& pos);

private:
    QmFramelessDialogPrivate* d_ { nullptr };
};
