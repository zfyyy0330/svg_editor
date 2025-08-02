#include "svgChartItemCustomLineRoute.h"
#include "graphicCanvasScene.h"
#include <QDebug>

SvgChartCustomLineRoute::SvgChartCustomLineRoute(QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawCustomLineRoute, parent){
    setBaseRect(this->boundingRect());
    setDictDragFlag(false);
    setDictHoverAllDirFlag(false);
}

SvgChartCustomLineRoute::SvgChartCustomLineRoute(const QList<QPointF>& custom_line_list, QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawCustomLineRoute, parent){
    points = custom_line_list;

    setBaseRect(this->boundingRect());
    setDictDragFlag(false);
    setDictHoverAllDirFlag(false);
}

QRectF SvgChartCustomLineRoute::boundingRect() const {
    if (points.isEmpty()) {
        return QRectF();
    }

    qreal minX = points.first().x();
    qreal minY = points.first().y();
    qreal maxX = points.first().x();
    qreal maxY = points.first().y();

    for (const QPointF& point : points) {
        minX = qMin(minX, point.x());
        minY = qMin(minY, point.y());
        maxX = qMax(maxX, point.x());
        maxY = qMax(maxY, point.y());
    }

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

QList<QPointF> SvgChartCustomLineRoute::getPoints(){
    return points;
}

SvgChartItemBase* SvgChartCustomLineRoute::setPreviewDict(){
    return new SvgChartCustomLineRoute(this->points);
}

void SvgChartCustomLineRoute::setBaseRect(const QRectF &new_rect) {
    qreal x_offset = new_rect.x() - boundingRect().x();
    qreal y_offset = new_rect.y() - boundingRect().y();

    for(int i = 0 ; i < points.size() ; ++i){
        points[i] = QPointF(points[i].x() + x_offset, points[i].y() + y_offset);
    }
    setRect(new_rect);
}

void SvgChartCustomLineRoute::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setPen(draw_bound_pen);
    for(int i = 0 ; i < points.size() - 1 ; ++i){
        painter->drawLine(points[i], points[i + 1]);
    }
    if (this->isSelected()) {
        painter->setPen(draw_selected_pen);
        painter->drawRect(this->boundingRect());
    }
}

void SvgChartCustomLineRoute::setPoint(const QList<QPointF> &points){
    this->points = points;
    setBaseRect(boundingRect());
}

void SvgChartCustomLineRoute::addPoint(const QPointF &point){
    points.append(point);
    setBaseRect(boundingRect());
}

void SvgChartCustomLineRoute::clear(){
    points.clear();
    setBaseRect(boundingRect());
}
