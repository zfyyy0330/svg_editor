#include "svgChartItemFactory.h"

SvgChartItemBase* SvgChartItemFactory::createGraphicItem(CurrentBarState which_type, const QRectF &rect){
    if (which_type == CurrentBarState::drawCustomLineRoute){
        return new SvgChartCustomLineRoute();
    } else if (which_type == CurrentBarState::drawRect) {
        return new SvgChartItemRect(rect);
    } else if (which_type == CurrentBarState::drawLine) {
        return new SvgChartItemLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height());
    } else if (which_type == CurrentBarState::drawCircle) {
        return new SvgChartItemCircle(rect);
    } else if (which_type == CurrentBarState::drawPentagon) {
        return new SvgChartItemPentagon(rect);
    } else if (which_type == CurrentBarState::drawHexagon) {
        return new SvgChartItemHexagon(rect);
    } else if (which_type == CurrentBarState::drawStars) {
        return new SvgChartItemstars(rect);
    } else if (which_type == CurrentBarState::drawText){
        return new SvgChartItemText(rect.topLeft(), "请输入文本");
    }
    return nullptr;
}

SvgChartItemBase* SvgChartItemFactory::createGraphicItem(CurrentDrawItemType which_type, const QRectF &rect){
    if (which_type == CurrentDrawItemType::drawCustomLineRoute){
        return new SvgChartCustomLineRoute();
    } else if (which_type == CurrentDrawItemType::drawRect) {
        return new SvgChartItemRect(rect);
    } else if (which_type == CurrentDrawItemType::drawLine) {
        return new SvgChartItemLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height());
    } else if (which_type == CurrentDrawItemType::drawCircle) {
        return new SvgChartItemCircle(rect);
    } else if (which_type == CurrentDrawItemType::drawPentagon) {
        return new SvgChartItemPentagon(rect);
    } else if (which_type == CurrentDrawItemType::drawHexagon) {
        return new SvgChartItemHexagon(rect);
    } else if (which_type == CurrentDrawItemType::drawStars) {
        return new SvgChartItemstars(rect);
    } else if (which_type == CurrentDrawItemType::drawText){
        return new SvgChartItemText(rect.topLeft(), "请输入文本");
    }
    return nullptr;
}

QRectF SvgChartItemFactory::normalizePoint(const QPointF& start_point, const QPointF& end_point){
    if (end_point.x() >= start_point.x()){
        if (end_point.y() >= start_point.y()) return QRectF(start_point, end_point);
        else return QRectF(start_point.x(), end_point.y(), end_point.x() - start_point.x(), start_point.y() - end_point.y());
    }
    else{
        if (end_point.y() <= start_point.y()) return QRectF(end_point, start_point);
        else return QRectF(end_point.x(), start_point.y(), start_point.x() - end_point.x(), end_point.y() - start_point.y());

    }
}
