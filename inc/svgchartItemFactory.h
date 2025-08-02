#ifndef SVGCHARTITEMFACTORY_H
#define SVGCHARTITEMFACTORY_H

#include "svgGlobalConfig.h"
#include "svgChartItemText.h"
#include "svgChartItemCustomLineRoute.h"
#include "svgChartItemCircle.h"
#include "svgChartItemLine.h"
#include "svgChartItemRect.h"
#include "svgChartItemHexagon.h"
#include "svgChartItemPentagon.h"
#include "svgChartItemStars.h"

struct SvgChartItemFactory{
    static SvgChartItemBase* createGraphicItem(CurrentBarState which_type, const QRectF &rect);
    static SvgChartItemBase* createGraphicItem(CurrentDrawItemType which_type, const QRectF &rect);
    static QRectF normalizePoint(const QPointF& start_point, const QPointF& end_point);
};

#endif // SVGCHARTITEMFACTORY_H
