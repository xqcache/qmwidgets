#pragma once

#include "qmwidgets_global.h"
#include <QPixmap>

class QMWIDGETS_EXPORT QmNinePatchPixmap {
public:
    enum class Part {
        TopLeft = 0,
        Top,
        TopRight,
        Left,
        Inner,
        Right,
        BottomLeft,
        Bottom,
        BottomRight,
    };

    explicit QmNinePatchPixmap(const QString& path, const QRect& inner_rect);

    void setInnerRect(const QRect& rect);
    QRect innerRect() const;
    QRect scaledInnerRect(const QRect& target);

    QSize minimumSize() const;
    QSize size() const;
    QRect rect() const;

    void draw(const QRect& target, QPainter* painter);

private:
    const QPixmap& partPixmap(Part part) const;
    QRect partRect(Part part) const;

    void drawPixmap(Part part, const QRect& target, QPainter* painter);

private:
    QRect inner_rect_;
    QPixmap origin_;
    QString path_;
    std::array<QPixmap, 9> pixmaps_;
};