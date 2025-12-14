#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QmSwitchPrivate;

class QMWIDGETS_EXPORT QmSwitch : public QFrame {
    Q_OBJECT
    Q_PROPERTY(QPointF handlePos READ _handlePos WRITE _setHandlePos DESIGNABLE false)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled)
    Q_PROPERTY(int rectRadius READ rectRadius WRITE setRectRadius)
    Q_PROPERTY(int padding READ padding WRITE setPadding)
    Q_PROPERTY(QColor checkedHandleColor READ checkedHandleColor WRITE setCheckedHandleColor)
    Q_PROPERTY(QColor uncheckedHandleColor READ uncheckedHandleColor WRITE setUncheckedHandleColor)
    Q_PROPERTY(QColor checkedBgColor READ checkedBgColor WRITE setCheckedBgColor)
    Q_PROPERTY(QColor uncheckedBgColor READ uncheckedBgColor WRITE setUncheckedBgColor)
public:
    QmSwitch(QWidget* parent);
    ~QmSwitch();

    void setCheckedBgColor(const QColor& color);
    QColor checkedBgColor() const;

    void setUncheckedBgColor(const QColor& color);
    QColor uncheckedBgColor() const;

    void setCheckedHandleColor(const QColor& color);
    QColor checkedHandleColor() const;

    void setUncheckedHandleColor(const QColor& color);
    QColor uncheckedHandleColor() const;

    void setRectRadius(int rectRadius);
    int rectRadius();

    void setPadding(int padding);
    int padding();

    void _setHandlePos(const QPointF& pos);
    QPointF _handlePos() const;

    void setChecked(bool checked);
    bool isChecked();

    void setCheckedText(const QString& text);
    void setUncheckedText(const QString& text);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

Q_SIGNALS:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QmSwitchPrivate* d;

    void updateHandlePos();
};
