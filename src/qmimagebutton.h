#pragma once

#include "qmwidgets_global.h"
#include <QAbstractButton>
#include <QWidget>

struct QmImageButtonPrivate;

class QMWIDGETS_EXPORT QmImageButton : public QAbstractButton {
    Q_OBJECT
public:
    explicit QmImageButton(QWidget* parent = nullptr);
    explicit QmImageButton(const QIcon& icon, QWidget* parent = nullptr);
    ~QmImageButton() noexcept override;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QmImageButtonPrivate* d_ { nullptr };
};