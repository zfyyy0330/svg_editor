#ifndef SVGCHARTVIEW_H
#define SVGCHARTVIEW_H

#include <QGraphicsView>
#include "svgGlobalConfig.h"

class GraphicCanvasScene;

class GraphicCanvasView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicCanvasView(GraphicCanvasScene *scene, QWidget* parent = nullptr);
    virtual ~GraphicCanvasView();

    void zoomIn();
    void zoomOut();
    void setZoomProperty(int target_property);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void enterEvent(QEvent *event) override;

private:
    void setCurBarStateCursor(CurrentBarState cur_bar_state);
    void updateMouseCursorOnBorder(DragDirection cur_drag_dir);
    void setScale(qreal sx, qreal sy);

private:
    int m_zoomProperty = 100;
    qreal m_zoomInFactor = 1.1;
    qreal m_zoomOutFactor = 0.9;
    int m_zoomStep = 10;
    bool m_zoomClamp = true;
    QPair<qreal, qreal> m_zoomFairRange = QPair<qreal, qreal>(-50, 250);

    GraphicCanvasScene* m_scene;

signals:
    void setViewZoomProportion(int);

public slots:
    void handleItemHoverStateChanged(DragDirection cur_drag_dir, CurrentBarState cur_bar_state = CurrentBarState::None);
    void handleMagnifierZoom(MagnifierZoomType);
};

#endif // SVGCHARTVIEW_H
