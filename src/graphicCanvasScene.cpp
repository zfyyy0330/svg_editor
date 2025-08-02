#include "graphicCanvasScene.h"
#include "graphicCanvasView.h"
#include "graphicCanvasCommand.h"
#include "svgchartItemFactory.h"
#include "graphicMenuFileMgOperation.h"
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <math.h>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

GraphicCanvasScene::GraphicCanvasScene(QObject *parent): QGraphicsScene(parent){
    setSceneRange(SceneDefaultWidth, SceneDefaultHeight);

    undo_command_record = new UndoQueue(SceneMaxUndoOperTime);
    // 同步到全局GlobalCommonSetting
    if (undo_command_record) GlobalCommonSetting::getInstance().setUndoQueue(undo_command_record);
    graphic_mouseRightClicked_command = new GraphicCanvasCommand(this);
    installEventFilter(this);
}

GraphicCanvasScene::GraphicCanvasScene(qreal width, qreal hight, QObject *parent): QGraphicsScene(parent){
    setSceneRange(width, hight);

    undo_command_record = new UndoQueue(SceneMaxUndoOperTime);
    // 同步到全局GlobalCommonSetting
    if (undo_command_record) GlobalCommonSetting::getInstance().setUndoQueue(undo_command_record);
    graphic_mouseRightClicked_command = new GraphicCanvasCommand(this);
    installEventFilter(this);
}

GraphicCanvasScene::~GraphicCanvasScene(){
    if (!graphic_mouseRightClicked_command){
        delete graphic_mouseRightClicked_command;
        graphic_mouseRightClicked_command = nullptr;
    }
}

void GraphicCanvasScene::setSceneRange(qreal width, qreal hight){
    setSceneRect(-width / 2, -hight / 2, width, hight);
}

void GraphicCanvasScene::drawBackground(QPainter* painter, const QRectF& rect){
    QGraphicsScene::drawBackground(painter, rect);
}

void GraphicCanvasScene::keyPressEvent(QKeyEvent *event) {
    if (graphic_mouseRightClicked_command){
        if (event->modifiers() & Qt::ControlModifier) {
            Command* action_command = nullptr;
            if (event->key() == Qt::Key_A) {
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::AllSelect);
            } else if (event->key() == Qt::Key_Z) {
                undo_command_record->pop();
            } else if (event->key() == Qt::Key_Y) {
                undo_command_record->redo();
            } else if (event->key() == Qt::Key_X) {
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::Cut);
            } else if (event->key() == Qt::Key_C) {
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::Copy);
            } else if (event->key() == Qt::Key_V) {
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::Paste);
            } else if (event->key() == Qt::Key_D){
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::CopyAndPaste);
            } else if (event->key() == Qt::Key_Up){
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::PlaceUpperLayer);
            } else if (event->key() == Qt::Key_Down){
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::PlaceLowerLayer);
            } else if (event->key() == Qt::Key_Left){
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::PlaceTopLayer);
            } else if (event->key() == Qt::Key_Right){
                action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::PlaceBottomLayer);
            } else if (event->key() == Qt::Key_N){
                GraphicMenuFileMgOperation::getInstance().createNewScene();
            } else if (event->key() == Qt::Key_S){
                GraphicMenuFileMgOperation::getInstance().saveToSvg();
            } else if (event->key() == Qt::Key_O){
                GraphicMenuFileMgOperation::getInstance().loadFromSvg();
            } else if (event->key() == Qt::Key_E){
                GraphicMenuFileMgOperation::getInstance().exportPng();
            }

            if (action_command) {
                undo_command_record->push(action_command);
                event->accept();
            }
            else QGraphicsScene::keyPressEvent(event);
        }
        else if (event->key() == Qt::Key_Backspace){
            Command* action_command = graphic_mouseRightClicked_command->createCanvasCommand(CanvasCommandType::Delete);
            if (action_command) undo_command_record->push(action_command);
        }
        else QGraphicsScene::keyPressEvent(event);
    }
    else QGraphicsScene::keyPressEvent(event);
}

void GraphicCanvasScene::changeCurrentBarMode(const CurrentBarState change_bar_state) {
    cur_bar_state = change_bar_state;
}

void GraphicCanvasScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        QGraphicsView *view = views().isEmpty()? nullptr : views().first();
        if (view){
            QPoint view_pos = view->mapFromScene(event->scenePos());
            QPoint screen_pos = view->mapToGlobal(view_pos);
            graphic_mouseRightClicked_command->showContextMenu(screen_pos);
        }
        event->accept();
    }
    else if (event->button() == Qt::LeftButton){
        if (cur_bar_state >= CurrentBarState::drawCustomLineRoute && cur_bar_state <= CurrentBarState::drawText) {
            start_point = event->scenePos();
            new_qgraphic_item = SvgChartItemFactory::createGraphicItem(cur_bar_state, QRectF{start_point.x(), start_point.y(), 1, 1});
            if (new_qgraphic_item){
                new_qgraphic_item->setOpacity(0.5);
                this->addItem(new_qgraphic_item);
                this->clearSelection();
                is_drawing_mode = true;
            }
            event->accept();
        }
        else if (cur_bar_state == CurrentBarState::Magnifier){
            if (event->modifiers() & Qt::ControlModifier){
                emit setMagnifier(MagnifierZoomType::ZoomOut);
            }
            else emit setMagnifier(MagnifierZoomType::ZoomIn);
        }
        else QGraphicsScene::mousePressEvent(event);
    }
    else QGraphicsScene::mousePressEvent(event);
}

void GraphicCanvasScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (cur_bar_state >= CurrentBarState::drawCustomLineRoute && cur_bar_state <= CurrentBarState::drawStars && is_drawing_mode) {
        // 临时渲染当前QGraphicItem的大小形状
        if (new_qgraphic_item) {
            QPointF cur_scene_pos = event->scenePos();
            if (cur_bar_state == CurrentBarState::drawText){
                QGraphicsScene::mouseMoveEvent(event);
            }
            else if (cur_bar_state == CurrentBarState::drawCustomLineRoute){
                SvgChartCustomLineRoute* new_customed_line = qgraphicsitem_cast<SvgChartCustomLineRoute *>(new_qgraphic_item);
                if(new_customed_line) new_customed_line->addPoint(cur_scene_pos);
            }
            else if (cur_bar_state == CurrentBarState::drawLine) {
                SvgChartItemLine* new_line_item = qgraphicsitem_cast<SvgChartItemLine *>(new_qgraphic_item);
                // 这里要设置线段的斜率属性, 使后续移动不会出现线段只能从左上点到右下点
                if (new_line_item){
                    qreal dx = event->scenePos().x() - start_point.x();
                    qreal dy = event->scenePos().y() - start_point.y();
                    new_line_item->setLineStateBySlop(dx, dy);
                    new_line_item->setBaseRect(QRectF(start_point, cur_scene_pos).normalized());
                }
            }
            else {
                new_qgraphic_item->setBaseRect(QRectF(start_point, cur_scene_pos).normalized());
            }
        }
        event->accept();
    }
    else QGraphicsScene::mouseMoveEvent(event);
}

void GraphicCanvasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (cur_bar_state >= CurrentBarState::drawCustomLineRoute && cur_bar_state <= CurrentBarState::drawText && is_drawing_mode) {
            if (new_qgraphic_item) {
                if (new_qgraphic_item->boundingRect().width() < SceneVaildDrawDiceThreshold && new_qgraphic_item->boundingRect().height() < SceneVaildDrawDiceThreshold){
                    this->removeItem(new_qgraphic_item);
                }
                else{
                    new_qgraphic_item->setOpacity(1);
                    new_qgraphic_item->setSelected(true);
                    // TODO: 为了将每一个qgraphicitem的光标事件统一到view去管理
                    addItemWithCursorManager(new_qgraphic_item, false);
                    emit setSelectedItemAttributeDisplay(new_qgraphic_item);
                }
                new_qgraphic_item = nullptr;
                is_drawing_mode = false;
            }
            event->accept();
        }
        else QGraphicsScene::mouseReleaseEvent(event);
    }
    else QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicCanvasScene::addItemWithCursorManager(SvgChartItemBase *new_graphic_item, bool is_need_add_item){
    if (new_graphic_item) {
        if (is_need_add_item) addItem(new_graphic_item);
        GraphicCanvasView* cur_view = this->getCurGraphicCanvasView();
        if (cur_view) {
            connect(new_graphic_item, &SvgChartItemBase::itemHoverStateChanged, cur_view, &GraphicCanvasView::handleItemHoverStateChanged);
        }
    }
}

GraphicCanvasView* GraphicCanvasScene::getCurGraphicCanvasView(){
    if (!this->views().isEmpty()){
        return qobject_cast<GraphicCanvasView*>(views().first());
    }
    return nullptr;
}


CurrentBarState GraphicCanvasScene::getCurBarState(){
    return cur_bar_state;
}

bool GraphicCanvasScene::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton && cur_bar_state == CurrentBarState::originMouse) {
            QGraphicsItem *click_item = itemAt(mouseEvent->scenePos(), QTransform());
            if (click_item){
                SvgChartItemBase* clickedItem = qgraphicsitem_cast<SvgChartItemBase*>(click_item);
                if (clickedItem) {
                    emit setSelectedItemAttributeDisplay(clickedItem);
                }
            }
            else emit setSelectedItemAttributeDisplay(nullptr);
        }
    }
    return QGraphicsScene::eventFilter(obj, event);
}

int GraphicCanvasScene::getItemIndex(QGraphicsItem *target_item){
    QList<QGraphicsItem *> collid_list = collidingItems(target_item);
    if (collid_list.size() == 0) return 0;
    // TODO: 通过重叠位置上第一个item是否是自己判断
    if (collid_list.size() == 1){
        QRectF rect1(target_item->mapToScene(target_item->boundingRect()).boundingRect());
        QRectF rect2(collid_list.first()->mapToScene(collid_list.first()->boundingRect()).boundingRect());
        QRectF rectMix = rect1.intersected(rect2);
        QPointF point = rectMix.center();
        QRectF rect(point.x() - 0.5, point.y() - 0.5, 1, 1);
        const auto all_items = this->items(rect);
        return all_items.indexOf(target_item);  //0为上层（共2层）
    }

    // 与之碰撞2个以上，且item位于顶层
    QList<QGraphicsItem *> bottom_collid_list = collidingItems(collid_list.last());
    if (collid_list.first() != bottom_collid_list.first()) return 0;
    else{
        QList<QGraphicsItem *> first_collid_list = collidingItems(collid_list.first());
        return (first_collid_list.indexOf(target_item) + 1);
    }
}

void GraphicCanvasScene::placeItemUpperLayer(QGraphicsItem *target_item){
    if (target_item){
        QList<QGraphicsItem *> collideItems = this->collidingItems(target_item);
        if (collideItems.size() == 0)
            return;
        int nCurIndex = this->getItemIndex(target_item);
        if (nCurIndex == 0)
            return;
        else collideItems.at(nCurIndex - 1)->stackBefore(target_item);
        this->update();
    }
}

void GraphicCanvasScene::placeItemLowerLayer(QGraphicsItem *target_item){
    if (target_item){
        QList<QGraphicsItem *> collideItems = this->collidingItems(target_item);
        if (collideItems.size() == 0)
            return;
        int nCurIndex = this->getItemIndex(target_item);
        if (nCurIndex == collideItems.size())
            return;
        target_item->stackBefore(collideItems.at(nCurIndex));
        this->update();
    }
}

void GraphicCanvasScene::placeItemBottomLayer(QGraphicsItem *target_item){
    if (target_item){
        QList<QGraphicsItem *> collideItems = this->collidingItems(target_item);
//        for (int i = 0; i < collideItems.size(); ++i)
//            target_item->stackBefore(collideItems.at(i));
        if (collideItems.size() == 0) return;
        target_item->stackBefore(collideItems.last());

        this->update();
    }
}

void GraphicCanvasScene::placeItemTopLayer(QGraphicsItem *target_item){
    if (target_item){
        QList<QGraphicsItem *> collideItems = this->collidingItems(target_item);
        for (int i = collideItems.size() - 1; i >= 0; --i)
            collideItems.at(i)->stackBefore(target_item);

        this->update();
    }
}

void GraphicCanvasScene::placeItemOrderedLayer(QGraphicsItem *target_item, int target_order){
    if (target_item){
        QList<QGraphicsItem *> collideItems = this->collidingItems(target_item);
        if (collideItems.size() == 0)
            return;
        int nCurIndex = this->getItemIndex(target_item);
        if (target_order >= 0 && target_order <= collideItems.size()){
            // 注意往顶层移动，需要一个一个往前插入，因为图层具有前向依赖，而向底层移动只需要插在后方即可，类似于排队
            if (nCurIndex > target_order){
                for (int i = nCurIndex - 1; i >= target_order; --i)
                    collideItems.at(i)->stackBefore(target_item);
            }
            else if (nCurIndex < target_order){
                if(target_order == collideItems.size()) target_item->stackBefore(collideItems.last());
                else target_item->stackBefore(collideItems.at(target_order - 1));
            }
            this->update();
        }
    }
}