#ifndef SVGCHARTITEMTEXT_H
#define SVGCHARTITEMTEXT_H

#include "svgChartItemBase.h"
#include "svgChartItemTextBase.h"

class SvgChartItemText : public SvgChartItemBase {
    Q_OBJECT
public:
    explicit SvgChartItemText(const QPointF& pos, QGraphicsItem* parent = nullptr);
    explicit SvgChartItemText(const QPointF& pos, const QString& custom, QGraphicsItem* parent = nullptr);
    virtual ~SvgChartItemText() = default;

    void setCurItemText(const QString& text);
    QString getCurItemText() const;
    void setDrawPenColor(const QColor& change_color) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    SvgChartItemBase* setPreviewDict() override;
    void setBaseRect(const QRectF& new_rect) override;
    QRectF boundingRect() const override;
private:
    void initCurTextItem(const QString& custom_text = "请输入文本");
private:
    SvgChartItemTextBase* cur_text_item = nullptr;
    QPointF cur_pos;
};
#endif // SVGCHARTITEMTEXT_H
