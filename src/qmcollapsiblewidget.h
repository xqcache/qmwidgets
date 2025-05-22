#pragma once

#include "qmwidgets_global.h"
#include <QWidget>

struct QmCollapsibleWidgetPrivate;

class QMWIDGETS_EXPORT QmCollapsibleWidget : public QWidget {
    Q_OBJECT
public:
    explicit QmCollapsibleWidget(QWidget* parent = nullptr);
    ~QmCollapsibleWidget() noexcept override;

    void setWidget(QWidget* widget);
    void setAnimated(bool enabled);

protected:
    bool event(QEvent* event) override;

private:
    void initUi();
    void setupSignals();

    void collapse();
    void expand();

private slots:
    void onCollapsibleChanged(bool collapse);

private:
    QmCollapsibleWidgetPrivate* d_ { nullptr };
};
