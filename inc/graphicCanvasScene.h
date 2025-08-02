#ifndef SVGCHARTSCENE_H
#define SVGCHARTSCENE_H

#include <QGraphicsScene>
#include "svgGlobalConfig.h"

class GraphicCanvasView;
class SvgChartItemBase;
class UndoQueue;
class GraphicCanvasCommand;

class GraphicCanvasScene : public QGraphicsScene{
    Q_OBJECT

public:
    explicit GraphicCanvasScene(QObject *parent = nullptr);
    explicit GraphicCanvasScene(qreal width, qreal hight, QObject *parent = nullptr);
    virtual ~GraphicCanvasScene();

    void setSceneRange(qreal width, qreal hight);
    void addItemWithCursorManager(SvgChartItemBase *new_graphic_item, bool is_need_add_item = true);
    GraphicCanvasView* getCurGraphicCanvasView();
    CurrentBarState getCurBarState();

    // 对于QGraphicsItem在scene中的图层管理通用API
    int getItemIndex(QGraphicsItem *item);
    void placeItemUpperLayer(QGraphicsItem *item);
    void placeItemLowerLayer(QGraphicsItem *item);
    void placeItemBottomLayer(QGraphicsItem *item);
    void placeItemTopLayer(QGraphicsItem *item);
    // 移动到指定层
    void placeItemOrderedLayer(QGraphicsItem *item, int target_order);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
    bool is_brush_mode = false;
    bool is_drawing_mode = false;

private:
    QPointF start_point;
    SvgChartItemBase *new_qgraphic_item = nullptr;
    UndoQueue* undo_command_record = nullptr;

private:
    CurrentBarState cur_bar_state = CurrentBarState::originMouse;
    GraphicCanvasCommand *graphic_mouseRightClicked_command = nullptr;

signals:
    void setMagnifier(MagnifierZoomType);
    void setSelectedItemAttributeDisplay(SvgChartItemBase*);
    void loadSvgSceneSizeAndColorChanged(const QSize& cur_size, const QColor& cur_color);
    void loadNewEmptySvgScene();

public slots:
    void changeCurrentBarMode(const CurrentBarState change_bar_state);
};

#endif // SVGCHARTSCENE_H
