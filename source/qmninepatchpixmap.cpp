#include "qmninepatchpixmap.h"
#include <QPixmapCache>

QmNinePatchPixmap::QmNinePatchPixmap(const QString& path, const QRect& inner_rect)
    : path_(path)
    , origin_(path)
{
    setInnerRect(inner_rect);
}

const QPixmap& QmNinePatchPixmap::partPixmap(Part part) const
{
    if (inner_rect_.isNull()) {
        return origin_;
    }
    return pixmaps_[static_cast<size_t>(part)];
}

QRect QmNinePatchPixmap::partRect(Part part) const
{
    if (inner_rect_.isNull()) {
        return rect();
    }

    QRect result = rect();
    switch (part) {
    case Part::TopLeft:
        result.setBottomRight(inner_rect_.topLeft());
        break;
    case Part::Top:
        result.setLeft(inner_rect_.left());
        result.setBottomRight(inner_rect_.topRight());
        break;
    case Part::TopRight:
        result.setBottomLeft(inner_rect_.topRight());
        break;
    case Part::Left:
        result.setTopRight(inner_rect_.topLeft());
        result.setBottomRight(inner_rect_.bottomLeft());
        break;
    case Part::Inner:
        result = inner_rect_;
        break;
    case Part::Right:
        result.setTopLeft(inner_rect_.topRight());
        result.setBottomLeft(inner_rect_.bottomRight());
        break;
    case Part::BottomLeft:
        result.setTopRight(inner_rect_.bottomLeft());
        break;
    case Part::Bottom:
        result.setTopLeft(inner_rect_.bottomLeft());
        result.setTopRight(inner_rect_.bottomRight());
        break;
    case Part::BottomRight:
        result.setTopLeft(inner_rect_.bottomRight());
        break;
    default:
        break;
    }
    return result;
}

QRect expandRect(const QRect& r, float ratio)
{
    int w = r.width() * ratio;
    int h = r.height() * ratio;

    QRect result = r;

    result.setWidth(w);
    result.setHeight(h);
    result.moveCenter(r.center());
    return result;
}

void QmNinePatchPixmap::draw(const QRect& target, QPainter* painter)
{
    if (origin_.isNull()) {
        return;
    }

    if (inner_rect_.isNull()) {
        painter->drawPixmap(target, origin_);
        return;
    }
    painter->save();
    if (painter->testRenderHint(QPainter::SmoothPixmapTransform)) {
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
    }
    QSize target_size = target.size();
    QSize origin_size = origin_.size();
    target_size.setWidth(target_size.width() - (origin_size.width() - inner_rect_.width()));
    target_size.setHeight(target_size.height() - (origin_size.height() - inner_rect_.height()));

    float x_ratio = (target_size.width() * 1.0f / inner_rect_.width());
    float y_ratio = (target_size.height() * 1.0f / inner_rect_.height());

    auto top_left_rect = partRect(Part::TopLeft);
    top_left_rect.moveTopLeft(target.topLeft());
    drawPixmap(Part::TopLeft, top_left_rect, painter);

    auto top_rect = partRect(Part::Top);
    top_rect.moveTopLeft(top_left_rect.topRight());
    top_rect.setWidth(top_rect.width() * x_ratio);
    top_rect.moveLeft(top_rect.left() + 1);
    drawPixmap(Part::Top, top_rect, painter);

    auto top_right_rect = partRect(Part::TopRight);
    top_right_rect.moveTopLeft(top_rect.topRight());
    top_right_rect.moveLeft(top_right_rect.left() + 1);
    drawPixmap(Part::TopRight, top_right_rect, painter);

    auto left_rect = partRect(Part::Left);
    left_rect.moveTopLeft(top_left_rect.bottomLeft());
    left_rect.setHeight(left_rect.height() * y_ratio);
    left_rect.moveTop(left_rect.top() + 1);
    drawPixmap(Part::Left, left_rect, painter);

    auto inner_rect = partRect(Part::Inner);
    inner_rect.moveTopLeft(left_rect.topRight());
    inner_rect.setWidth(inner_rect.width() * x_ratio);
    inner_rect.setHeight(inner_rect.height() * y_ratio);
    inner_rect.moveLeft(inner_rect.left() + 1);
    drawPixmap(Part::Inner, inner_rect, painter);

    auto right_rect = partRect(Part::Right);
    right_rect.moveTopLeft(inner_rect.topRight());
    right_rect.setHeight(right_rect.height() * y_ratio);
    right_rect.moveLeft(right_rect.left() + 1);
    drawPixmap(Part::Right, right_rect, painter);

    auto bottom_left_rect = partRect(Part::BottomLeft);
    bottom_left_rect.moveTopLeft(left_rect.bottomLeft());
    bottom_left_rect.moveTop(bottom_left_rect.top() + 1);
    drawPixmap(Part::BottomLeft, bottom_left_rect, painter);

    auto bottom_rect = partRect(Part::Bottom);
    bottom_rect.moveTopLeft(bottom_left_rect.topRight());
    bottom_rect.setWidth(bottom_rect.width() * x_ratio);
    bottom_rect.moveLeft(bottom_rect.left() + 1);
    drawPixmap(Part::Bottom, bottom_rect, painter);

    auto bottom_right_rect = partRect(Part::BottomRight);
    bottom_right_rect.moveTopLeft(bottom_rect.topRight());
    bottom_right_rect.moveLeft(bottom_right_rect.left() + 1);
    drawPixmap(Part::BottomRight, bottom_right_rect, painter);
    painter->restore();
}

void QmNinePatchPixmap::drawPixmap(Part part, const QRect& target, QPainter* painter)
{
    QString raw_key = QString("%1_%2_%3_%4_%5_%6")
                          .arg(origin_.cacheKey())
                          .arg(static_cast<int>(part))
                          .arg(target.x())
                          .arg(target.y())
                          .arg(target.width())
                          .arg(target.height());
    QString cache_key = "shadow_" + QString::number(qHash(raw_key));
    QPixmap pixmap;
    if (QPixmapCache::find(cache_key, &pixmap)) {
        painter->drawPixmap(target, pixmap);
    } else {
        QPixmapCache::insert(cache_key, partPixmap(part));
        painter->drawPixmap(target, partPixmap(part));
    }
}

QSize QmNinePatchPixmap::minimumSize() const
{
    if (inner_rect_.isNull()) {
        return { 0, 0 };
    }
    QSize min_size = size() - inner_rect_.size();
    min_size.setWidth(qMax(0, min_size.width()));
    min_size.setHeight(qMax(0, min_size.height()));
    return min_size;
}

QSize QmNinePatchPixmap::size() const
{
    return origin_.size();
}

QRect QmNinePatchPixmap::rect() const
{
    return origin_.rect();
}

void QmNinePatchPixmap::setInnerRect(const QRect& rect)
{
    inner_rect_ = rect;

    for (size_t i = 0; i < 9; ++i) {
        pixmaps_[i] = origin_.copy(partRect(static_cast<Part>(i)));
    }
}

QRect QmNinePatchPixmap::innerRect() const
{
    return inner_rect_;
}

QRect QmNinePatchPixmap::scaledInnerRect(const QRect& target)
{
    QSize target_size = target.size();
    QSize origin_size = origin_.size();
    target_size.setWidth(target_size.width() - (origin_size.width() - inner_rect_.width()));
    target_size.setHeight(target_size.height() - (origin_size.height() - inner_rect_.height()));
    return QRect(inner_rect_.topLeft(), target_size);
}

bool QmNinePatchPixmap::isNull() const
{
    return origin_.isNull();
}