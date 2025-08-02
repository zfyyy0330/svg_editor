#ifndef SVGCHARTITEMPENTAGON_H
#define SVGCHARTITEMPENTAGON_H

#include "svgChartItemBase.h"

class SvgChartItemPentagon : public SvgChartItemBase{
    Q_OBJECT
public:
    explicit SvgChartItemPentagon(const QRectF &rect, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartItemPentagon() = default;

    void setBaseRect(const QRectF &new_rect) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    SvgChartItemBase* setPreviewDict() override;
private:
    QPolygonF pentagon_points;
};

#endif // SVGCHARTITEMPENTAGON_H
