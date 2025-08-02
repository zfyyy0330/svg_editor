#ifndef SVGCHARTITEMLINE_H
#define SVGCHARTITEMLINE_H

#include "svgChartItemBase.h"

class SvgChartItemLine : public SvgChartItemBase{
    Q_OBJECT
public:
    explicit SvgChartItemLine(const QLineF &line, int cur_line_state, QGraphicsItem *parent = nullptr);
    explicit SvgChartItemLine(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartItemLine() = default;

    void setBaseRect(const QRectF &rect) override;
    void setLineStateBySlop(qreal dx, qreal dy);
    void setLineState(int cur_line_state);
    static int calLineStateBySlop(qreal dx, qreal dy);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
public:
    // TODO: ��enum class�滻, -1 ��ʾ�����ϵ����£�1��ʾ�����ϵ�����
    int cur_line_state = -1;
protected:
    SvgChartItemBase* setPreviewDict() override;
private:
    QPointF start_point;
    QPointF end_point;
};

#endif // SVGCHARTITEMLINE_H
