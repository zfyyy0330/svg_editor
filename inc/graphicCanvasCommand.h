#ifndef GRAPHICCANVASCOMMAND_H
#define GRAPHICCANVASCOMMAND_H

#include "graphicCanvasAllBasicCommand.h"
#include "svgGlobalConfig.h"
#include <QAction>
#include <QMap>

class UndoQueue;

// 右键菜单处理类
class GraphicCanvasCommand {
public:
    explicit GraphicCanvasCommand(QGraphicsScene* scene);
    ~GraphicCanvasCommand() = default;

    void showContextMenu(const QPoint& pos);
    Command* createCanvasCommand(CanvasCommandType command_type);

private:
    void initAllActionMap();

private:
    QGraphicsScene* scene = nullptr;
    UndoQueue* undo_record = nullptr;

    QMap<CanvasCommandType, QAction*> command_type_to_action;
};

#endif // GRAPHICCANVASCOMMAND_H
