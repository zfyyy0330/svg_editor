#include "svgChartItemStars.h"
#include "math.h"

SvgChartItemstars::SvgChartItemstars(const QRectF &rect, QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawStars, parent){
    setBaseRect(rect);
}

// 计算两条线段交点
static inline QPointF lineIntersection(const QPointF& p1, const QPointF& p2, const QPointF& p3, const QPointF& p4) {
    qreal denominator = (p4.y() - p3.y()) * (p2.x() - p1.x()) - (p4.x() - p3.x()) * (p2.y() - p1.y());
    if (denominator == 0) {
        return QPointF();
    }
    qreal numerator1 = (p4.x() - p3.x()) * (p1.y() - p3.y()) - (p4.y() - p3.y()) * (p1.x() - p3.x());
    qreal numerator2 = (p2.x() - p1.x()) * (p1.y() - p3.y()) - (p2.y() - p1.y()) * (p1.x() - p3.x());
    qreal u = numerator1 / denominator;
    qreal v = numerator2 / denominator;
    if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
        return QPointF(p1.x() + u * (p2.x() - p1.x()), p1.y() + u * (p2.y() - p1.y()));
    }
    return QPointF();
}

void SvgChartItemstars::setBaseRect(const QRectF &new_rect){
    qreal left = new_rect.left();
    qreal top = new_rect.top();
    qreal right = new_rect.right();
    qreal bottom = new_rect.bottom();

    QPolygonF points;
    points.append(QPointF(left + (right -  left) / 2, top));
    points.append(QPointF(right, top + (bottom - top) * 0.39));
    points.append(QPointF(right - (right - left) * 0.2, bottom));
    points.append(QPointF(left + (right - left) * 0.2, bottom));
    points.append(QPointF(left, top + (bottom - top) * 0.39));

    QPolygonF stars_points_tmp;
    for (int i = 0; i < 5; ++i) {
        int next_index = (i + 1) % 5;
        int next_next_index = (i + 2) % 5;
        int pre_cur_index = (i - 1 + 5) % 5;
        QPointF intersection = lineIntersection(points[i], points[next_next_index], points[next_index], points[pre_cur_index]);
        if (!intersection.isNull()) {
            stars_points_tmp.append(intersection);
        }
        stars_points_tmp.append(points[next_index]);
    }
    stars_points = stars_points_tmp;
    setRect(new_rect);
}

SvgChartItemBase* SvgChartItemstars::setPreviewDict(){
    return new SvgChartItemstars(boundingRect());
}

void SvgChartItemstars::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setBrush(draw_fill_brush);
    painter->setPen(draw_bound_pen);
    painter->drawPolygon(stars_points);
    painter->setBrush(Qt::NoBrush);

    if (this->isSelected()) {
        painter->setPen(draw_selected_pen);
        painter->drawRect(boundingRect());
    }
}
