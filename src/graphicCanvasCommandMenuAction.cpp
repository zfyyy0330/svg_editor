#include "graphicCanvasCommandMenuAction.h"

GraphicCanvasCommandMenuAction::GraphicCanvasCommandMenuAction(CanvasCommandType command_type, QObject *parent)
    : QAction(parent){
    this->command_type = command_type;
}

GraphicCanvasCommandMenuAction::GraphicCanvasCommandMenuAction(CanvasCommandType command_type, const QString &text, QObject *parent)
    : QAction(text, parent){
    this->command_type = command_type;
}

CanvasCommandType GraphicCanvasCommandMenuAction::getCommandType(){
    return command_type;
}

