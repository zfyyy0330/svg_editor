#ifndef SVGCHARTITEMSTARS_H
#define SVGCHARTITEMSTARS_H

#include "svgChartItemBase.h"

class SvgChartItemstars : public SvgChartItemBase{
    Q_OBJECT
public:
    explicit SvgChartItemstars(const QRectF &rect, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartItemstars() = default;

    void setBaseRect(const QRectF &new_rect) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    SvgChartItemBase* setPreviewDict() override;
private:
    QPolygonF stars_points;
};

#endif // SVGCHARTITEMSTARS_H
