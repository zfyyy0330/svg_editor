#include "graphicCanvasCommand.h"
#include "graphicCanvasScene.h"
#include "graphicCanvasCommandMenuAction.h"
#include <QMenu>
#include <QDebug>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>

// menu show---------
GraphicCanvasCommand::GraphicCanvasCommand(QGraphicsScene* scene)
    : scene(scene), undo_record(GlobalCommonSetting::getInstance().getUndoQueue()) {
    initAllActionMap();
}

void GraphicCanvasCommand::initAllActionMap(){
    if (scene) {
        command_type_to_action[CanvasCommandType::AllSelect] = new GraphicCanvasCommandMenuAction(CanvasCommandType::AllSelect, "全选 \t  A  ", scene);
        command_type_to_action[CanvasCommandType::Undo] = new GraphicCanvasCommandMenuAction(CanvasCommandType::Undo, "撤销 \t  Z  ", scene);
        command_type_to_action[CanvasCommandType::Redo] = new GraphicCanvasCommandMenuAction(CanvasCommandType::Redo, "重做 \t  Y  ", scene);
        command_type_to_action[CanvasCommandType::Cut] = new GraphicCanvasCommandMenuAction(CanvasCommandType::Cut, "剪切 \t  X  ", scene);
        command_type_to_action[CanvasCommandType::Copy] = new GraphicCanvasCommandMenuAction(CanvasCommandType::Copy, "复制 \t  C  ", scene);
        command_type_to_action[CanvasCommandType::Paste] = new GraphicCanvasCommandMenuAction(CanvasCommandType::Paste, "粘贴 \t  V  ", scene);
        command_type_to_action[CanvasCommandType::CopyAndPaste] = new GraphicCanvasCommandMenuAction(CanvasCommandType::CopyAndPaste, "复写 \t  D  ", scene);
        command_type_to_action[CanvasCommandType::Delete] = new GraphicCanvasCommandMenuAction(CanvasCommandType::Delete, "删除 \t  Backspace  ", scene);
        command_type_to_action[CanvasCommandType::PlaceUpperLayer] = new GraphicCanvasCommandMenuAction(CanvasCommandType::PlaceUpperLayer, "置上一层 \t  Up  ", scene);
        command_type_to_action[CanvasCommandType::PlaceLowerLayer] = new GraphicCanvasCommandMenuAction(CanvasCommandType::PlaceLowerLayer, "置下一层 \t  Down  ", scene);
        command_type_to_action[CanvasCommandType::PlaceTopLayer] = new GraphicCanvasCommandMenuAction(CanvasCommandType::PlaceTopLayer, "置顶层 \t  Left  ", scene);
        command_type_to_action[CanvasCommandType::PlaceBottomLayer] = new GraphicCanvasCommandMenuAction(CanvasCommandType::PlaceBottomLayer, "置底层 \t  Right  ", scene);
    }
}

// TODO: 命令工厂
Command* GraphicCanvasCommand::createCanvasCommand(CanvasCommandType command_type){
    if (scene) {
        if (command_type == CanvasCommandType::AllSelect) {
            return new AllSelectCommand(scene);
        }
        else if (command_type == CanvasCommandType::Cut) {
            return new CutCommand(scene, scene->selectedItems());
        }
        else if (command_type == CanvasCommandType::Copy) {
            return new CopyCommand(scene, scene->selectedItems());
        }
        else if (command_type == CanvasCommandType::Paste) {
            return new PasteCommand(scene);
        }
        else if (command_type == CanvasCommandType::CopyAndPaste) {
            return new copyAndPasteCommand(scene, scene->selectedItems());
        }
        else if (command_type == CanvasCommandType::Delete) {
            return new DeleteCommand(scene, scene->selectedItems());
        }
        else if (command_type == CanvasCommandType::PlaceUpperLayer) {
            return new placeUpperLayerCommand(scene, scene->selectedItems().first());
        }
        else if (command_type == CanvasCommandType::PlaceLowerLayer) {
            return new placeLowerLayerCommand(scene, scene->selectedItems().first());
        }
        else if (command_type == CanvasCommandType::PlaceTopLayer) {
            return new placeTopLayerCommand(scene, scene->selectedItems().first());
        }
        else if (command_type == CanvasCommandType::PlaceBottomLayer) {
            return new placeBottomLayerCommand(scene, scene->selectedItems().first());
        }
    }
    return nullptr;
}

void GraphicCanvasCommand::showContextMenu(const QPoint& pos) {
    QMenu menu;
    if (scene && undo_record){
        menu.addAction(command_type_to_action[CanvasCommandType::AllSelect]);
        menu.addAction(command_type_to_action[CanvasCommandType::Undo]);
        if (!undo_record->isRedoEmpty()){
            menu.addAction(command_type_to_action[CanvasCommandType::Redo]);
        }

        if (!scene->selectedItems().empty()){
            menu.addAction(command_type_to_action[CanvasCommandType::Cut]);
            menu.addAction(command_type_to_action[CanvasCommandType::Copy]);
        }

        const QMimeData* mimeData = QApplication::clipboard()->mimeData();
        if (mimeData->hasFormat("application/svgChartItemBaseProperty") && !mimeData->data("application/svgChartItemBaseProperty").isEmpty()) {
            menu.addAction(command_type_to_action[CanvasCommandType::Paste]);
        }

        if (!scene->selectedItems().empty()){
            menu.addAction(command_type_to_action[CanvasCommandType::CopyAndPaste]);
            menu.addAction(command_type_to_action[CanvasCommandType::Delete]);

            if (scene->selectedItems().size() == 1){
                menu.addAction(command_type_to_action[CanvasCommandType::PlaceUpperLayer]);
                menu.addAction(command_type_to_action[CanvasCommandType::PlaceLowerLayer]);
                menu.addAction(command_type_to_action[CanvasCommandType::PlaceTopLayer]);
                menu.addAction(command_type_to_action[CanvasCommandType::PlaceBottomLayer]);
            }
        }

        QAction* cur_clicked_action = menu.exec(pos);
        if(cur_clicked_action == command_type_to_action[CanvasCommandType::Undo]){
            undo_record->pop();
        }
        else if(cur_clicked_action == command_type_to_action[CanvasCommandType::Redo]){
            undo_record->redo();
        }
        else {
            GraphicCanvasCommandMenuAction* cur_cast_aciton = dynamic_cast<GraphicCanvasCommandMenuAction *>(cur_clicked_action);
            if (cur_cast_aciton){
                Command* action_command = createCanvasCommand(cur_cast_aciton->getCommandType());
                if (action_command) undo_record->push(action_command);
            }
        }
    }
}
