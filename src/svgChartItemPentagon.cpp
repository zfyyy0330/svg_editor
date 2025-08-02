#include "svgChartItemPentagon.h"
#include "math.h"

SvgChartItemPentagon::SvgChartItemPentagon(const QRectF &rect, QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawPentagon, parent){
    setBaseRect(rect);
}

void SvgChartItemPentagon::setBaseRect(const QRectF &new_rect) {
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
    pentagon_points = points;

    setRect(new_rect);
}

SvgChartItemBase* SvgChartItemPentagon::setPreviewDict(){
    return new SvgChartItemPentagon(boundingRect());
}

void SvgChartItemPentagon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setBrush(draw_fill_brush);
    painter->setPen(draw_bound_pen);
    painter->drawPolygon(pentagon_points);
    painter->setBrush(Qt::NoBrush);

    if (this->isSelected()) {
        painter->setPen(draw_selected_pen);
        painter->drawRect(boundingRect());
    }
}
