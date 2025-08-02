#include "graphicCanvasAllBasicCommand.h"
#include "graphicCanvasScene.h"
#include "graphicCanvasView.h"
#include <QStack>
#include <QDebug>
#include <QMimeData>
#include <QClipboard>
#include "svgchartItemFactory.h"
#include "graphicCanvasCommandMenuAction.h"
#include <QApplication>


// 1.---------
AllSelectCommand::AllSelectCommand(QGraphicsScene* scene)
    :scene(scene) {
}

void AllSelectCommand::execute() {
    if (scene){
        QList<QGraphicsItem*> allItems = scene->items();
        for(auto* it : allItems){
            if (!it->isSelected()) it->setSelected(true);
        }
    }
}
void AllSelectCommand::undo() {
    if (scene){
        QList<QGraphicsItem*> allItems = scene->selectedItems();
        for(auto* it : allItems){
            it->setSelected(false);
        }
    }
}

// 2.---------
DeleteCommand::DeleteCommand(QGraphicsScene* scene, const QList<QGraphicsItem *>& selected_items)
    :scene(scene), selected_items(selected_items){
}
void DeleteCommand::execute() {
    if (scene){
        for(auto* item : selected_items) {
            SvgChartItemBase* cur_base_item = qgraphicsitem_cast<SvgChartItemBase*>(item);
            if (cur_base_item){
                copy_selected_property.append(cur_base_item->serializeProperty());
            }
            scene->removeItem(item);
            delete item;
        }
    }
}
void DeleteCommand::undo() {
    selected_items.clear();
    for(const auto& item_property : copy_selected_property) {
        SvgChartItemBase* new_qgraphic_item = SvgChartItemBase::deserializeProperty(item_property);
        if (new_qgraphic_item && scene){
            scene->addItem(new_qgraphic_item);
            selected_items.append(new_qgraphic_item);
            scene->clearSelection();
        }
    }
}

// 3.---------
CopyCommand::CopyCommand(QGraphicsScene* scene, const QList<QGraphicsItem *>& selected_items)
    : scene(scene), selected_items(selected_items){
}
void CopyCommand::execute() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    for (QGraphicsItem* item : selected_items) {
        SvgChartItemBase* cur_base_item = qgraphicsitem_cast<SvgChartItemBase*>(item);
        if (cur_base_item){
            stream<<cur_base_item->serializeByteArray();
        }
    }
    if (!data.isEmpty()){
        QMimeData* mimeData = new QMimeData();
        mimeData->setData("application/svgChartItemBaseProperty", data);
        QApplication::clipboard()->setMimeData(mimeData);
    }
}
void CopyCommand::undo() {
    QApplication::clipboard()->clear();
}

// 4.---------
CutCommand::CutCommand(QGraphicsScene* scene, const QList<QGraphicsItem *>& selected_items)
    :scene(scene), selected_items(selected_items){
}
void CutCommand::execute() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    for (QGraphicsItem* item : selected_items) {
        SvgChartItemBase* cur_base_item = qgraphicsitem_cast<SvgChartItemBase*>(item);
        if (cur_base_item && scene){
            copy_selected_property.append(cur_base_item->serializeProperty());
            stream<<cur_base_item->serializeByteArray();
            scene->removeItem(cur_base_item);
        }
    }
    if (!data.isEmpty() && !copy_selected_property.isEmpty()){
        QMimeData* mimeData = new QMimeData();
        mimeData->setData("application/svgChartItemBaseProperty", data);
        QApplication::clipboard()->setMimeData(mimeData);
    }
}
void CutCommand::undo() {
    QApplication::clipboard()->clear();
    for(const auto& item_property : copy_selected_property) {
        SvgChartItemBase* new_qgraphic_item = SvgChartItemBase::deserializeProperty(item_property);
        if (new_qgraphic_item && scene){
            scene->addItem(new_qgraphic_item);
            scene->clearSelection();
        }
    }
}

// 5.---------
PasteCommand::PasteCommand(QGraphicsScene* scene)
    :scene(scene){

}
void PasteCommand::execute() {
    const QMimeData* mimeData = QApplication::clipboard()->mimeData();
    if (mimeData->hasFormat("application/svgChartItemBaseProperty")) {
        QByteArray data = mimeData->data("application/svgChartItemBaseProperty");
        QDataStream stream(data);
        while (!stream.atEnd()) {
            QByteArray itemData;
            stream >> itemData;
            SvgChartItemBase* new_graphic_item = SvgChartItemBase::deserializeByteArray(itemData);
            if (new_graphic_item && scene) {
                GraphicCanvasScene *cur_scene = dynamic_cast<GraphicCanvasScene *>(scene);
                if(cur_scene){
                    cur_scene->addItemWithCursorManager(new_graphic_item);
                }
                new_graphic_item_lists.append(new_graphic_item);
            }
        }
    }
}
void PasteCommand::undo() {
    for(SvgChartItemBase* new_item : new_graphic_item_lists){
        if (scene) scene->removeItem(new_item);
    }
}

// 6.---------
copyAndPasteCommand::copyAndPasteCommand(QGraphicsScene* scene, const QList<QGraphicsItem *>& selected_items)
    : scene(scene), selected_items(selected_items){
}
void copyAndPasteCommand::execute() {
    for (QGraphicsItem* item : selected_items) {
        SvgChartItemBase* cur_base_item = qgraphicsitem_cast<SvgChartItemBase*>(item);
        if (cur_base_item){
            // TODO: 相对于绝对坐标做一点偏移量
            SvgChartItemBaseProperty cur_item_property = cur_base_item->serializeProperty();
            cur_item_property.cur_del_pos = QRectF(QPointF(cur_item_property.cur_del_pos.x() + 10, cur_item_property.cur_del_pos.y() + 10), QSize(cur_item_property.cur_del_pos.width(), cur_item_property.cur_del_pos.height()));
            SvgChartItemBase* new_graphic_item = SvgChartItemBase::deserializeProperty(cur_item_property);
            if (new_graphic_item && scene) {
                GraphicCanvasScene *cur_scene = dynamic_cast<GraphicCanvasScene *>(scene);
                if(cur_scene){
                    cur_scene->addItemWithCursorManager(new_graphic_item);
                }
                copy_selected_property.append(new_graphic_item);
            }
        }
    }
}
void copyAndPasteCommand::undo() {
    for(SvgChartItemBase* new_item : copy_selected_property){
        if (scene) scene->removeItem(new_item);
    }
}

placeUpperLayerCommand::placeUpperLayerCommand(QGraphicsScene* scene, QGraphicsItem* selected_item)
    : scene(scene), cur_change_item(selected_item){
}
void placeUpperLayerCommand::execute(){
    if (scene && cur_change_item){
        GraphicCanvasScene* cur_scene = dynamic_cast<GraphicCanvasScene*>(scene);
        if (cur_scene) { 
            cur_scene->placeItemUpperLayer(cur_change_item); 
        }
    }
}
void placeUpperLayerCommand::undo(){
    if (scene && cur_change_item){
        GraphicCanvasScene* cur_scene = dynamic_cast<GraphicCanvasScene*>(scene);
        if (cur_scene){
            cur_scene->placeItemLowerLayer(cur_change_item);
        }
    }
}

// 置下一层命令
placeLowerLayerCommand::placeLowerLayerCommand(QGraphicsScene* scene, QGraphicsItem* selected_item)
    : scene(scene), cur_change_item(selected_item){
}
void placeLowerLayerCommand::execute(){
    if (scene && cur_change_item){
        GraphicCanvasScene* cur_scene = dynamic_cast<GraphicCanvasScene*>(scene);
        if (cur_scene) {
            cur_scene->placeItemLowerLayer(cur_change_item);
        }
    }
}
void placeLowerLayerCommand::undo(){
    if (scene && cur_change_item){
        GraphicCanvasScene* cur_scene = dynamic_cast<GraphicCanvasScene*>(scene);
        if (cur_scene) {
            cur_scene->placeItemUpperLayer(cur_change_item);
        }
    }
}

// 置顶部命令
placeTopLayerCommand::placeTopLayerCommand(QGraphicsScene* scene, QGraphicsItem* selected_item)
    : scene(scene), cur_change_item(selected_item) {
}
void placeTopLayerCommand::execute(){
    if (scene && cur_change_item){
        GraphicCanvasScene* cur_scene = dynamic_cast<GraphicCanvasScene*>(scene);
        if (cur_scene){
            last_selected_item_index = cur_scene->getItemIndex(cur_change_item);
            cur_scene->placeItemTopLayer(cur_change_item);
        }
    }
}
void placeTopLayerCommand::undo(){
    if (scene && last_selected_item_index != -1 && cur_change_item){
        GraphicCanvasScene* cur_scene = dynamic_cast<GraphicCanvasScene*>(scene);
        if (cur_scene){
            cur_scene->placeItemOrderedLayer(cur_change_item, last_selected_item_index);
        }
    }
}

// 置底部
placeBottomLayerCommand::placeBottomLayerCommand(QGraphicsScene* scene, QGraphicsItem* selected_item)
    : scene(scene), cur_change_item(selected_item) {
}
void placeBottomLayerCommand::execute(){
    if (scene && cur_change_item){
        GraphicCanvasScene* cur_scene = dynamic_cast<GraphicCanvasScene*>(scene);
        if (cur_scene){
            last_selected_item_index = cur_scene->getItemIndex(cur_change_item);
            cur_scene->placeItemBottomLayer(cur_change_item);
        }
    }
}

void placeBottomLayerCommand::undo(){
    if (scene && last_selected_item_index != -1 && cur_change_item){
        GraphicCanvasScene* cur_scene = dynamic_cast<GraphicCanvasScene*>(scene);
        if (cur_scene){
            cur_scene->placeItemOrderedLayer(cur_change_item, last_selected_item_index);
        }
    }
}