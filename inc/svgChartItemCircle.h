#ifndef SVGCHARTITEMCIRCLE_H
#define SVGCHARTITEMCIRCLE_H

#include "svgChartItemBase.h"

class SvgChartItemCircle : public SvgChartItemBase {
    Q_OBJECT
public:
    explicit SvgChartItemCircle(const QRectF &rect, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartItemCircle() = default;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    SvgChartItemBase* setPreviewDict() override;
};

#endif // SVGCHARTITEMCIRCLE_H
