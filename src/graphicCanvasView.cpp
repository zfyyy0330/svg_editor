#include "graphicCanvasView.h"
#include "graphicCanvasScene.h"
#include <QWheelEvent>

GraphicCanvasView::GraphicCanvasView(GraphicCanvasScene *scene, QWidget* parent): QGraphicsView(scene, parent){
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    m_scene = scene;
    setAcceptDrops(true);
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
}

GraphicCanvasView::~GraphicCanvasView(){
}

void GraphicCanvasView::zoomIn(){
    qreal zoomProperty = m_zoomProperty;
    zoomProperty += m_zoomStep;
    if (zoomProperty >= m_zoomFairRange.first && zoomProperty <= m_zoomFairRange.second){
        m_zoomProperty = zoomProperty;
        setScale(m_zoomInFactor, m_zoomInFactor);
    }
}

void GraphicCanvasView::zoomOut(){
    qreal zoomProperty = m_zoomProperty;
    zoomProperty -= m_zoomStep;
    if (zoomProperty >= m_zoomFairRange.first && zoomProperty <= m_zoomFairRange.second){
        m_zoomProperty = zoomProperty;
        setScale(m_zoomOutFactor, m_zoomOutFactor);
    }
}

void GraphicCanvasView::setZoomProperty(int target_property){
    if (target_property % 10 == 0 && target_property >= m_zoomFairRange.first && target_property <= m_zoomFairRange.second){
        setScale(target_property / m_zoomProperty, target_property / m_zoomProperty);
        m_zoomProperty = target_property;
    }
}

void GraphicCanvasView::wheelEvent(QWheelEvent* event){
    if (event->modifiers() == Qt::ControlModifier){
        qreal zoomFactor = 0.0;

        int zoomProperty = m_zoomProperty;
        if (event->angleDelta().y() > 0){
            zoomFactor = m_zoomInFactor;
            zoomProperty += m_zoomStep;
        }
        else{
            zoomFactor = m_zoomOutFactor;
            zoomProperty -= m_zoomStep;
        }

        if (zoomProperty >= m_zoomFairRange.first && zoomProperty <= m_zoomFairRange.second){
            m_zoomProperty = zoomProperty;
            setScale(zoomFactor, zoomFactor);
        }
        event->accept();
    }
    else{
        QGraphicsView::wheelEvent(event);
    }
}

void GraphicCanvasView::setScale(qreal sx, qreal sy){
    emit setViewZoomProportion(m_zoomProperty);
    scale(sx, sy);
};

void GraphicCanvasView::enterEvent(QEvent* event){
    setCurBarStateCursor(m_scene->getCurBarState());
    QGraphicsView::enterEvent(event);
};


// TODO: 全局光标的管理部分
//      1. 如果不为DragDirection::None, 则为拖拉拽的光标设置；
//      2. 如果为DragDirection::None，则表示退出拖拉拽，后面（可选参数，默认为None）CurrentBarState，即自定义cur_bar_state的光标
//          2.1 如果不为None，则根据输入的光标状态去设置
//          2.1 如果为None，则根据当前菜单栏的curBarstate去自动设置
void GraphicCanvasView::handleItemHoverStateChanged(DragDirection cur_drag_dir, CurrentBarState cur_bar_state){
    if (cur_drag_dir == DragDirection::None){
        if (cur_bar_state == CurrentBarState::None){
            setCurBarStateCursor(m_scene->getCurBarState());
        }
        else{
            setCurBarStateCursor(cur_bar_state);
        }
    }
    else{
        updateMouseCursorOnBorder(cur_drag_dir);
    }
}

void GraphicCanvasView::setCurBarStateCursor(CurrentBarState cur_bar_state){
    if (cur_bar_state == CurrentBarState::originMouse){
        setCursor(QCursor(Qt::ArrowCursor)); // 设置普通光标
    }
    else if(cur_bar_state == CurrentBarState::drawCustomLineRoute){
        QPixmap pixmap(":/images/pen.png");
        pixmap = pixmap.scaled(30, 30, Qt::KeepAspectRatio);
        QCursor penCursor(pixmap, 0, 30);
        setCursor(penCursor);
    }
    else if(cur_bar_state >= CurrentBarState::drawLine && cur_bar_state <= CurrentBarState::drawStars){
        setCursor(QCursor(Qt::CrossCursor)); // 设置十字光标
    }
    else if(cur_bar_state == CurrentBarState::drawText){
        setCursor(QCursor(Qt::IBeamCursor));
    }
    else{
        QPixmap pixmap(":/images/magnifier.png");
        pixmap = pixmap.scaled(30, 30, Qt::KeepAspectRatio);
        QCursor penCursor(pixmap, 15, 15);
        setCursor(penCursor);
    }
}

void GraphicCanvasView::updateMouseCursorOnBorder(DragDirection cur_drag_dir){
    QCursor cursor;
    switch (cur_drag_dir) {
    case DragDirection::Top:
        cursor.setShape(Qt::SizeVerCursor);
        break;
    case DragDirection::Bottom:
        cursor.setShape(Qt::SizeVerCursor);
        break;
    case DragDirection::Left:
        cursor.setShape(Qt::SizeHorCursor);
        break;
    case DragDirection::Right:
        cursor.setShape(Qt::SizeHorCursor);
        break;
    case DragDirection::TopLeft:
        cursor.setShape(Qt::SizeFDiagCursor);
        break;
    case DragDirection::TopRight:
        cursor.setShape(Qt::SizeBDiagCursor);
        break;
    case DragDirection::BottomLeft:
        cursor.setShape(Qt::SizeBDiagCursor);
        break;
    case DragDirection::BottomRight:
        cursor.setShape(Qt::SizeFDiagCursor);
        break;
    case DragDirection::Content:
        cursor.setShape(Qt::SizeAllCursor);
        break;
    case DragDirection::None:
        break;
    }
    this->setCursor(cursor);
}

void GraphicCanvasView::handleMagnifierZoom(MagnifierZoomType zoom_type){
    if (zoom_type == MagnifierZoomType::ZoomIn){
        for(int i = 0 ; i < MagnifierZoomTimes ; ++i){
            this->zoomIn();
        }
    }
    else{
        for(int i = 0 ; i < MagnifierZoomTimes ; ++i){
            this->zoomOut();
        }
    }
}

