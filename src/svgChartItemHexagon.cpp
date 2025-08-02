#include "svgChartItemHexagon.h"
#include "math.h"

SvgChartItemHexagon::SvgChartItemHexagon(const QRectF &rect, QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawHexagon, parent){
    setBaseRect(rect);
}

SvgChartItemBase* SvgChartItemHexagon::setPreviewDict(){
    return new SvgChartItemHexagon(boundingRect());
}

void SvgChartItemHexagon::setBaseRect(const QRectF &new_rect) {
    qreal left = new_rect.left();
    qreal top = new_rect.top();
    qreal right = new_rect.right();
    qreal bottom = new_rect.bottom();

    QPolygonF points;
    points.append(QPointF(left + (right - left) / 4, top));
    points.append(QPointF(right - (right - left) / 4, top));
    points.append(QPointF(right, top + (bottom - top) / 2));
    points.append(QPointF(right - (right - left) / 4, bottom));
    points.append(QPointF(left + (right - left) / 4, bottom));
    points.append(QPointF(left, top + (bottom - top) / 2));
    hexagon_points = points;

    setRect(new_rect);
}

void SvgChartItemHexagon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setBrush(draw_fill_brush);
    painter->setPen(draw_bound_pen);
    painter->drawPolygon(hexagon_points);
    painter->setBrush(Qt::NoBrush);

    if (this->isSelected()) {
        painter->setPen(draw_selected_pen);
        painter->drawRect(boundingRect());
    }
}


