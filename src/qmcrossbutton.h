#pragma once

#include "qmwidgets_global.h"
#include <QFrame>

class QmCrossButtonPrivate;
class QMWIDGETS_EXPORT QmCrossButton : public QFrame {
    Q_OBJECT
public:
    enum class ClickedArea : unsigned char {
        None,
        East,
        South,
        West,
        North
    };
    Q_ENUM(ClickedArea)

    enum class DivisionMode {
        // 南北分割
        SouthNorth,
        // 东西分割
        EastWest,
        // （东，南，西，北）分割
        FourDirections
    };
    Q_ENUM(DivisionMode)

    explicit QmCrossButton(QWidget* parent = nullptr);
    ~QmCrossButton() noexcept override;

    void setDivisionMode(DivisionMode mode);
    DivisionMode divisionMode() const;

    void setNormalPixmap(const QPixmap& pixmap);
    void setEastClickedPixmap(const QPixmap& pixmap);
    void setSouthClickedPixmap(const QPixmap& pixmap);
    void setWestClickedPixmap(const QPixmap& pixmap);
    void setNorthClickedPixmap(const QPixmap& pixmap);
    void setClickedPixmap(const QPixmap& east, const QPixmap& south, const QPixmap& west, const QPixmap& north);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setAutoRepeat(bool on);
    void setAutoRepeatDelay(int ms);
    void setAutoRepeatInterval(int ms);

    void setInteractiveIgnored(const QPainterPath& area);

signals:
    void clicked(ClickedArea area);
    void buttonPressed(ClickedArea area);

public slots:
    // 主动触发点击
    void click(ClickedArea area);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QmCrossButtonPrivate* d_ = nullptr;
    Q_DISABLE_COPY_MOVE(QmCrossButton);
};