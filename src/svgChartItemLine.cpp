#include "svgChartItemLine.h"
#include "math.h"
#include <QDebug>

// 该构造函数只用于setPreviewDict, 即类似拷贝构造
SvgChartItemLine::SvgChartItemLine(const QLineF &line, int cur_line_state, QGraphicsItem *parent)
    : SvgChartItemBase(CurrentDrawItemType::drawLine, QRectF(line.x1(), line.y1(), abs(line.x2() - line.x1()), abs(line.y2() - line.y1())), parent){
    start_point = line.p1();
    end_point = line.p2();
    this->cur_line_state = cur_line_state;
}

// 该构造函数只用于初始化，因为在line初始化时必须直到其状态才能确定start_point, end_point;
SvgChartItemLine::SvgChartItemLine(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
    : SvgChartItemBase(CurrentDrawItemType::drawLine, QRectF(x1, y1, abs(x2 - x1), abs(y2 - y1)), parent){
}

int SvgChartItemLine::calLineStateBySlop(qreal dx, qreal dy){
    if (dx == 0) {
        return -1;
    } else {
        if (dy / dx >= 0) return -1;
        else return 1;
    }
}

void SvgChartItemLine::setLineStateBySlop(qreal dx, qreal dy){
    if (dx == 0) {
        this->cur_line_state = -1;
    } else {
        if (dy / dx >= 0) this->cur_line_state = -1;
        else this->cur_line_state = 1;
    }
    setBaseRect(boundingRect());
}

void SvgChartItemLine::setLineState(int cur_line_state){
    this->cur_line_state = cur_line_state;
    setBaseRect(boundingRect());
}

void SvgChartItemLine::setBaseRect(const QRectF &rect){
    QRectF normal_rect = rect.normalized();
    if (cur_line_state == 1){
        start_point = QPointF(normal_rect.x(), normal_rect.y() + normal_rect.height());
        end_point = QPointF(normal_rect.x() + normal_rect.width(), normal_rect.y());
    }
    else{
        start_point = QPointF(normal_rect.x(), normal_rect.y());
        end_point = QPointF(normal_rect.x() + normal_rect.width(), normal_rect.y() + normal_rect.height());
    }
    setRect(normal_rect);
}

SvgChartItemBase* SvgChartItemLine::setPreviewDict(){
    return new SvgChartItemLine(QLineF(start_point, end_point), cur_line_state);
}

void SvgChartItemLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setPen(draw_bound_pen);
    painter->drawLine(start_point, end_point);

    if (this->isSelected()) {
        painter->setPen(draw_selected_pen);
        painter->drawRect(boundingRect());
    }
}
