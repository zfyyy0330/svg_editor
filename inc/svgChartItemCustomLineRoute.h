#ifndef SVGCHARTCUSTOMLINEROUTE_H
#define SVGCHARTCUSTOMLINEROUTE_H

#include "svgChartItemBase.h"

class SvgChartCustomLineRoute : public SvgChartItemBase{
    Q_OBJECT
public:
    explicit SvgChartCustomLineRoute(QGraphicsItem *parent = nullptr);
    explicit SvgChartCustomLineRoute(const QList<QPointF>& custom_line_list, QGraphicsItem *parent = nullptr);
    explicit SvgChartCustomLineRoute(const SvgChartCustomLineRoute& custom_line_list, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartCustomLineRoute() = default;

    void addPoint(const QPointF &point);
    void setPoint(const QList<QPointF> &points);
    QList<QPointF> getPoints();
    void clear();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    SvgChartItemBase* setPreviewDict() override;
    QRectF boundingRect() const override;
    void setBaseRect(const QRectF &new_rect) override;
private:
    QList<QPointF> points;
};

#endif // SVGCHARTCUSTOMLINEROUTE_H
