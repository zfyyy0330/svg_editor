#ifndef GRAHPICMAINCANVASWIDGET_H
#define GRAHPICMAINCANVASWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "svgGlobalConfig.h"
#include "svgchartItemFactory.h"

class GraphicCanvasScene;
class GraphicCanvasView;

class GrahpicMainCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GrahpicMainCanvasWidget(QSize init_size, QColor init_color, QWidget *parent = nullptr);
    virtual ~GrahpicMainCanvasWidget() = default;

private:
    GraphicCanvasScene *m_scene = nullptr;
    GraphicCanvasView *m_view = nullptr;

private:
    qreal m_sceneWidth;
    qreal m_sceneHight;
    QColor background_color;

private:
    void initSlots();

signals:
    void replyBarStateChange(CurrentBarState change_bar_state);
    void replyChangeSelectedItem(SvgChartItemBase*);
    void replyViewZoomProportion(int);
    void sendSceneWidthChanged(int);
    void sendSceneHeightChanged(int);
    void sendSceneColorChanged(const QColor&);

public slots:
    void setMainCanvasSceneSize(QSize);
    bool setMainCanvasSceneHeight(int);
    bool setMainCanvasSceneWidth(int);
    bool setMainCanvasSceneColor(const QColor&);

    void replySvgSceneSizeAndColorChanged(const QSize& cur_size, const QColor& cur_color);
    void replyNewEmptySvgScene();
};

#endif // GRAHPICMAINCANVASWIDGET_H
