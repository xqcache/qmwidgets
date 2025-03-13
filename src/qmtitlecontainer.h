#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

struct QmTitleContainerPrivate;
class QMWIDGETS_EXPORT QmTitleContainer : public QFrame {
    Q_OBJECT
public:
    explicit QmTitleContainer(QWidget* parent = nullptr);
    ~QmTitleContainer() noexcept override;

    void setTitle(const QString& text);
    void setTitleAlignment(Qt::Alignment align);
    void setTitleWidget(QWidget* widget);
    void setWidget(QWidget* widget);

private:
    QmTitleContainerPrivate* d_ { nullptr };
};
