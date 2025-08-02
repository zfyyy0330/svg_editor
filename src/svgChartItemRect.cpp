#include "svgChartItemRect.h"

SvgChartItemRect::SvgChartItemRect(QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawRect, parent){
}
SvgChartItemRect::SvgChartItemRect(const QRectF &rect, QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawRect, rect, parent){
}
SvgChartItemRect::SvgChartItemRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawRect, x, y, w, h, parent){
}

SvgChartItemBase* SvgChartItemRect::setPreviewDict(){
    return new SvgChartItemRect(boundingRect());
}

void SvgChartItemRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setBrush(draw_fill_brush);
    painter->setPen(draw_bound_pen);
    painter->drawRect(boundingRect());
    painter->setBrush(Qt::NoBrush);

    if (this->isSelected()) {
        painter->setPen(draw_selected_pen);
        painter->drawRect(boundingRect());
    }
}

