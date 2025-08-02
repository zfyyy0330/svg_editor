#include "svgGlobalConfig.h"
#include "graphicCanvasAllBasicCommand.h"
#include "graphicMenuFileMgOperation.h"
#include <QDebug>

// undo ---------
UndoQueue::UndoQueue(int max_size) : max_size(max_size){
}
UndoQueue::~UndoQueue(){
    while (!undo_stack.isEmpty()) {
        Command* command = undo_stack.takeLast();
        delete command;
    }
    while (!redo_stack.isEmpty()) {
        Command* command = redo_stack.takeLast();
        delete command;
    }
}
void UndoQueue::push(Command* command) {
    if (undo_stack.size() >= max_size){
        Command* first = undo_stack.dequeue();
        delete first;
    }
    undo_stack.enqueue(command);
    command->execute();

    while (!redo_stack.isEmpty()) {
        Command* command = redo_stack.takeLast();
        delete command;
    }
}
void UndoQueue::pop() {
    if (!undo_stack.isEmpty()) {
        Command* command = undo_stack.takeLast();
        command->undo();
        redo_stack.enqueue(command);
    }
}
bool UndoQueue::isEmpty(){
    return undo_stack.isEmpty();
}

bool UndoQueue::isRedoEmpty(){
    return redo_stack.isEmpty();
}

void UndoQueue::redo(){
    if (!redo_stack.isEmpty()) {
        Command* command = redo_stack.takeLast();
        if (command) {
            command->execute();
            undo_stack.enqueue(command);
        }
    }
}

GlobalCommonSetting& GlobalCommonSetting::getInstance(){
    static GlobalCommonSetting common_setting_opertor;
    return common_setting_opertor;
}

GlobalCommonSetting::~GlobalCommonSetting(){
    if (undo_command_record){
        delete undo_command_record;
        undo_command_record = nullptr;
    }
}

void GlobalCommonSetting::setSceneWidth(int scene_width){
    this->scene_width = scene_width;
}

int GlobalCommonSetting::getSceneWidth(){
    return scene_width;
}

void GlobalCommonSetting::setSceneHeight(int scene_height){
    this->scene_height = scene_height;
}

int GlobalCommonSetting::getSceneHeight(){
    return scene_height;
}

void GlobalCommonSetting::setSceneColor(const QColor& scene_color){
    this->scene_color = scene_color;
}

QColor GlobalCommonSetting::getSceneColor(){
    return scene_color;
}

CurrentBarState GlobalCommonSetting::mapBarKeyToEnum(const QString &key) {
    if (key == "linedrawbutton") return CurrentBarState::drawLine;
    if (key == "selectionbutton") return CurrentBarState::originMouse;
    if (key == "freehandlinedrawbutton") return CurrentBarState::drawCustomLineRoute;
    if (key == "rectdrawbutton") return CurrentBarState::drawRect;
    if (key == "quadrilateraldrawbutton") return CurrentBarState::None;
    if (key == "pentagondrawbutton") return CurrentBarState::drawPentagon;
    if (key == "zoominbutton") return CurrentBarState::None;
    if (key == "zoomoutbutton") return CurrentBarState::Magnifier;
    if (key == "stardrawbutton") return CurrentBarState::drawStars;
    return CurrentBarState::None;
}

void GlobalCommonSetting::addBarButtonHoverInfo(CurrentBarState cur_bar_state, const QPair<QString, QString>& value){
    bar_button_hover_info[cur_bar_state] = value;
}

QPair<QString, QString> GlobalCommonSetting::getBarButtonHoverInfo(CurrentBarState cur_bar_state){
    auto it = bar_button_hover_info.find(cur_bar_state);
    if(it != bar_button_hover_info.end()){
        return *it;
    }
    return {"",""};
}

QMap<CurrentBarState, QPair<QString, QString>> GlobalCommonSetting::getBarButton(){
    return bar_button_hover_info;
}

void GlobalCommonSetting::setUndoQueue(UndoQueue *undo_command){
    undo_command_record = undo_command;
}

UndoQueue* GlobalCommonSetting::getUndoQueue(){
    return undo_command_record;
}

