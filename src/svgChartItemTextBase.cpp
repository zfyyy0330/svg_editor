#include "svgChartItemTextBase.h"
#include "graphicCanvasScene.h"
#include <QTextCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QApplication>

SvgChartItemTextBase::SvgChartItemTextBase(QGraphicsItem *parent) : QGraphicsTextItem(parent){
}

SvgChartItemTextBase::SvgChartItemTextBase(const QString &text, QGraphicsItem *parent) : QGraphicsTextItem(text, parent){
}

void SvgChartItemTextBase::focusInEvent(QFocusEvent *event){
    if(event->reason() != Qt::PopupFocusReason){
        m_store_str = toPlainText();
    }
    QGraphicsTextItem::focusInEvent(event);
}

void SvgChartItemTextBase::focusOutEvent(QFocusEvent *event){
    if(event->reason() == Qt::MouseFocusReason && QApplication::mouseButtons()==Qt::RightButton){
        setPlainText(m_store_str);
        setTextInteractionFlags(Qt::NoTextInteraction);
    }
    else if(event->reason() == Qt::PopupFocusReason){
        //右键弹出菜单时不做处理
    }
    else{
        setTextInteractionFlags(Qt::NoTextInteraction);
    }

    QTextCursor cursor = this->textCursor();
    cursor.clearSelection();
    this->setTextCursor(cursor);
    is_edit_mode = false;

    QGraphicsTextItem::focusOutEvent(event);
}

void SvgChartItemTextBase::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Return||event->key() == Qt::Key_Enter){
        clearFocus();
    }else{
        QGraphicsTextItem::keyPressEvent(event);
    }
}

void SvgChartItemTextBase::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsScene* cur_scene_ptr = scene();
    if(cur_scene_ptr){
        GraphicCanvasScene* cur_scene = qobject_cast<GraphicCanvasScene*>(cur_scene_ptr);
        if(cur_scene && cur_scene->getCurBarState() == CurrentBarState::originMouse && !is_edit_mode){
            emit itemHoverStateChanged(DragDirection::Content);
        }
    }
    QGraphicsTextItem::hoverMoveEvent(event);
}
