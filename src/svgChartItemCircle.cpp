#include "svgChartItemCircle.h"
#include <QDebug>

SvgChartItemCircle::SvgChartItemCircle(const QRectF &rect, QGraphicsItem *parent): SvgChartItemBase(CurrentDrawItemType::drawCircle, rect, parent){
}

SvgChartItemBase* SvgChartItemCircle::setPreviewDict(){
    return new SvgChartItemCircle(boundingRect());
}

void SvgChartItemCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setBrush(draw_fill_brush);
    painter->setPen(draw_bound_pen);
    painter->drawEllipse(boundingRect());
    painter->setBrush(Qt::NoBrush);

    if (this->isSelected()) {
        painter->setPen(draw_selected_pen);
        painter->drawRect(boundingRect());
        painter->setPen(draw_bound_pen);
    }
}
