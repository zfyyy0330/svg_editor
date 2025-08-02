#ifndef SVGCHARTITEMRECT_H
#define SVGCHARTITEMRECT_H

#include "svgChartItemBase.h"

class SvgChartItemRect : public SvgChartItemBase
{
    Q_OBJECT
public:
    explicit SvgChartItemRect(QGraphicsItem *parent = nullptr);
    explicit SvgChartItemRect(const QRectF &rect, QGraphicsItem *parent = nullptr);
    explicit SvgChartItemRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartItemRect() = default;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    SvgChartItemBase* setPreviewDict() override; 
};

#endif // SVGCHARTITEMRECT_H
