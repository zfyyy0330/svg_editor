#ifndef SVGCHARTITEMHEXAGON_H
#define SVGCHARTITEMHEXAGON_H

#include "svgChartItemBase.h"

class SvgChartItemHexagon : public SvgChartItemBase{
    Q_OBJECT
public:
    explicit SvgChartItemHexagon(const QRectF &rect, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartItemHexagon() = default;

    void setBaseRect(const QRectF &new_rect) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    SvgChartItemBase* setPreviewDict() override;
private:
    QPolygonF hexagon_points;
};

#endif // SVGCHARTITEMHEXAGON_H
