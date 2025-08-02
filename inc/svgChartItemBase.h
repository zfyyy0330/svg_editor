#ifndef SVGCHARTITEMBASE_H
#define SVGCHARTITEMBASE_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include "svgGlobalConfig.h"
#include <QCursor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QList>
#include <QPolygon>

// TODO: 可优化当前为基类，子类继承并拓展当前类，这样就可将下方的反序列化转化到当前属性类中。
struct SvgChartItemBaseProperty{
    // 基础属性
    CurrentDrawItemType cur_draw_item_type;
    QRectF cur_del_pos;
    QPen cur_draw_pen;
    QBrush cur_draw_brush;

    // 额外属性
    int cur_line_state;
    QList<QPointF> points;
    QString cur_text;
};

class SvgChartItemBase : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    // TODO: CurrentDrawItemType为当前初始化的类型，一开始是为了处理逻辑的时候去做特判，后续优化为外部设置标志位，因此留下来当作一个拓展位，不影响使用
    explicit SvgChartItemBase(CurrentDrawItemType item_type, QGraphicsItem *parent = nullptr);
    explicit SvgChartItemBase(CurrentDrawItemType item_type, const QRectF &rect, QGraphicsItem *parent = nullptr);
    explicit SvgChartItemBase(CurrentDrawItemType item_type, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartItemBase() = default;

    // TODO: 设置修改当前图形Rect的函数，因此绘制图形不一定直接利用boundingRect()绘制，而是利用额外的数据，例如多边形，边和自定义绘图都需要PointF去维护
    //       因此交给子类去重写和维护，在最后setRect调整当前矩形框即可，如果不重写，内部就是直接调用this->setRect修改大小和位置
    virtual void setBaseRect(const QRectF& rect);
    // TODO: 设置当前图形的范围，如果不重写，默认就是当前矩形框的boundingRect()
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    // 获得自定义PointF的boundingRect
    static QRectF getCustomDictBoundingRect(const QList<QPointF>& cur_points);

    CurrentDrawItemType getCurrentDrawItemType();
    virtual void setDrawBrushColor(const QColor& change_color);
    virtual void setDrawPenWidth(int change_width);
    virtual void setDrawPenColor(const QColor& change_color);
    virtual void setDrawPenStyle(const Qt::PenStyle& change_pen_style);
    virtual QColor getDrawBrushColor();
    virtual int getDrawPenWidth();
    virtual QColor getDrawPenColor();
    virtual Qt::PenStyle getDrawPenStyle();
    QBrush getDrawFillBrush();
    QPen getDrawBoundPen();
    void setDrawFillBrush(const QBrush& set_brush);
    void setDrawBoundPen(const QPen& set_pen);

    // 对结构体进行反序列化
    static SvgChartItemBase* deserializeProperty(const SvgChartItemBaseProperty& item_property);
    static SvgChartItemBase* deserializeByteArray(const QByteArray& data);
    // 对结构体进行序列化，后续可优化为每一个子类都重写
    virtual SvgChartItemBaseProperty serializeProperty();
    virtual QByteArray serializeByteArray();

protected:
    // TODO: 重写信号驱动函数
    // 1.根据外部设定的is_need_drag, is_need_move, is_hove_all_dir和is_update_position_by_relative，内部做分支处理
    // 2.提供了默认的信号驱动函数，外部可重写，不重写则调用该类默认处理。
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    // TODO: 设置预览框的形状，如果不重写，默认拖拉拽就是矩形框
    virtual SvgChartItemBase* setPreviewDict();

    // TODO: 检查当前鼠标是否在边界
    DragDirection isMousePointOnBorder(const QPointF& pos);

    // TODO: 用于派生类自定义状态
    void setDictDragFlag(bool is_need_drag);
    void setDictMoveFlag(bool is_need_move);
    void setDictHoverAllDirFlag(bool is_hove_all_dir);
    void setDictMoveByRelativePos(bool is_update_position_by_relative);

protected:
    // TODO: 用于默认初始化笔刷和笔
    virtual void initDrawBrush();
    virtual void initDrawPen();
    virtual void initSelectedPen();

protected:
    QBrush draw_fill_brush;
    QPen draw_bound_pen;
    QPen draw_selected_pen;

protected:
    QPointF start_press_pos;
    bool is_on_moving = false;
    bool is_vaild_move = false;
    QList<SvgChartItemBase *> all_origin_selected_rect_lists;
    QList<SvgChartItemBase *> all_preview_rect_move_lists;
    const int vaild_move_threshold = 3;

protected:
    DragDirection cur_drag_dir;
    bool is_on_dragging = false;
    bool is_vaild_drag = false;
    SvgChartItemBase* preview_rect = nullptr;
    QRectF initial_rect;
    QRectF aftchange_rect;
    const int vaild_drag_threshold = 3;

protected:
    const int vaild_mouse_on_bound_threshold = 5;

protected:
    CurrentDrawItemType current_item_type;

private:
    bool is_need_drag = true;
    bool is_need_move = true;
    bool is_hove_all_dir = true;
    bool is_update_position_by_relative = true;

signals:
    void itemHoverStateChanged(DragDirection, CurrentBarState = CurrentBarState::None);
};

#endif // SVGCHARTITEMBASE_H
