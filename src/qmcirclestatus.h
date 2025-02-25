#pragma once

#include "qmwidgets_global.h"
#include <QColor>
#include <QFrame>

class QmCircleStatusPrivate;

class QMWIDGETS_EXPORT QmCircleStatus : public QFrame {
    Q_OBJECT
public:
    enum class Status : int {
        Idle,
        Running,
        Failed,
        Finished,
    };
    Q_ENUM(Status)

    explicit QmCircleStatus(QWidget* parent = nullptr);
    ~QmCircleStatus() noexcept override;

    void setStatus(Status status);
    void setText(const QString& text);

    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Q_DECLARE_PRIVATE(QmCircleStatus);
    QmCircleStatusPrivate* d_ptr { nullptr };
};