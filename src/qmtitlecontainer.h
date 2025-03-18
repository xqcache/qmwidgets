#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

struct QmTitleContainerPrivate;
class QMWIDGETS_EXPORT QmTitleContainer : public QFrame {
    Q_OBJECT
    Q_PROPERTY(QWidget* widget READ widget WRITE setWidget)
    Q_PROPERTY(Qt::Alignment titleAlignment READ titleAlignment WRITE setTitleAlignment)
public:
    explicit QmTitleContainer(QWidget* parent = nullptr);
    ~QmTitleContainer() noexcept override;

    void setTitle(const QString& text);
    void setTitleAlignment(Qt::Alignment align);
    void setTitleWidget(QWidget* widget);
    void setWidget(QWidget* widget);

    QWidget* widget() const;

    Qt::Alignment titleAlignment() const;

private:
    QmTitleContainerPrivate* d_ { nullptr };
};
