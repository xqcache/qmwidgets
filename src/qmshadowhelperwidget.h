#pragma once

#include "qmwidgets_global.h"

#include <QWidget>

struct QmShadowHelperWidgetPrivate;

class QMWIDGETS_EXPORT QmShadowHelperWidget : public QWidget {
public:
    explicit QmShadowHelperWidget(QWidget* parent);
    ~QmShadowHelperWidget() noexcept override;

    void setColor(const QColor& color);
    void setBlurRadius(qreal radius);
    void setOffset(qreal d);

    void setBackgroundBrush(const QBrush& brush);
    void setBorderPen(const QPen& pen);
    void setBorderRadius(qreal x_radius, qreal y_radius);

    qreal blurRadius() const;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QmShadowHelperWidgetPrivate* d_ { nullptr };
};