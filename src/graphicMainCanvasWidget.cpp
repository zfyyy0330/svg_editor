#include "graphicMainCanvasWidget.h"
#include "graphicCanvasScene.h"
#include "graphicCanvasView.h"
#include "graphicMenuFileMgOperation.h"
#include <QDebug>

GrahpicMainCanvasWidget::GrahpicMainCanvasWidget(QSize init_size, QColor init_color, QWidget *parent) : QWidget(parent){
    this->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    this->setMinimumSize(500, 800);

    m_scene = new GraphicCanvasScene(this);
    setMainCanvasSceneSize(init_size);
    setMainCanvasSceneColor(init_color);

    m_view = new GraphicCanvasView(m_scene, this);
    QVBoxLayout *graphic_main_window_layout = new QVBoxLayout(this);
    graphic_main_window_layout->setContentsMargins(6, 9, 0, 0);
    graphic_main_window_layout->addWidget(m_view);

    // 同步到全局File操作
    GraphicMenuFileMgOperation::getInstance().setScene(m_scene);
    GraphicMenuFileMgOperation::getInstance().setView(m_view);
    initSlots();
}

void GrahpicMainCanvasWidget::setMainCanvasSceneSize(QSize change_size){
    setMainCanvasSceneWidth(change_size.width());
    setMainCanvasSceneHeight(change_size.height());
}

bool GrahpicMainCanvasWidget::setMainCanvasSceneHeight(int change_height){
    if (change_height>= SceneMinHeightThreshold && change_height <= SceneMaxHeightThreshold){
        m_sceneHight = change_height;
        m_scene->setSceneRange(m_sceneWidth, change_height);
        return true;
    }
    return false;
}

bool GrahpicMainCanvasWidget::setMainCanvasSceneWidth(int change_weight){
    if (change_weight>= SceneMinWidthThreshold && change_weight <= SceneMaxWidthThreshold){
        m_sceneWidth = change_weight;
        m_scene->setSceneRange(change_weight, m_sceneHight);
        return true;
    }
    return false;
}

bool GrahpicMainCanvasWidget::setMainCanvasSceneColor(const QColor& change_color){
    if (change_color.isValid()){
        background_color = change_color;
        m_scene->setBackgroundBrush(QBrush(change_color));
        return true;
    }
    return false;
}

void GrahpicMainCanvasWidget::replySvgSceneSizeAndColorChanged(const QSize& cur_size, const QColor& cur_color){
    if (setMainCanvasSceneWidth(cur_size.width())){
        emit sendSceneWidthChanged(cur_size.width());
    }
    if (setMainCanvasSceneHeight(cur_size.height())){
        emit sendSceneHeightChanged(cur_size.height());
    }
    if (setMainCanvasSceneColor(cur_color)){
        emit sendSceneColorChanged(cur_color);
    }
}

void GrahpicMainCanvasWidget::replyNewEmptySvgScene(){
    m_scene->clear();

    int width = GlobalCommonSetting::getInstance().getSceneWidth();
    int height = GlobalCommonSetting::getInstance().getSceneHeight();
    QColor color = GlobalCommonSetting::getInstance().getSceneColor();
    if (setMainCanvasSceneWidth(width)){
        emit sendSceneWidthChanged(width);
    }
    if (setMainCanvasSceneHeight(height)){
        emit sendSceneHeightChanged(height);
    }
    if (setMainCanvasSceneColor(color)){
        emit sendSceneColorChanged(color);
    }
}


void GrahpicMainCanvasWidget::initSlots(){
    connect(this, &GrahpicMainCanvasWidget::replyBarStateChange, m_scene, &GraphicCanvasScene::changeCurrentBarMode);
    connect(m_scene, &GraphicCanvasScene::setMagnifier, m_view, &GraphicCanvasView::handleMagnifierZoom);
    connect(m_scene, &GraphicCanvasScene::setSelectedItemAttributeDisplay, this, &GrahpicMainCanvasWidget::replyChangeSelectedItem);
    connect(m_view, &GraphicCanvasView::setViewZoomProportion, this, &GrahpicMainCanvasWidget::replyViewZoomProportion);
    connect(m_scene, &GraphicCanvasScene::loadSvgSceneSizeAndColorChanged, this, &GrahpicMainCanvasWidget::replySvgSceneSizeAndColorChanged);
    connect(m_scene, &GraphicCanvasScene::loadNewEmptySvgScene, this, &GrahpicMainCanvasWidget::replyNewEmptySvgScene);
}