#ifndef GRAPHICCANVASRIGHTCLICKEDMENU_H
#define GRAPHICCANVASRIGHTCLICKEDMENU_H

#include <QAction>
#include "svgGlobalConfig.h"

class GraphicCanvasCommandMenuAction : public QAction
{
    Q_OBJECT
public:
    explicit GraphicCanvasCommandMenuAction(CanvasCommandType command_type, QObject *parent = nullptr);
    explicit GraphicCanvasCommandMenuAction(CanvasCommandType command_type, const QString &text, QObject *parent = nullptr);
    virtual ~GraphicCanvasCommandMenuAction() = default;

    CanvasCommandType getCommandType();
private:
    CanvasCommandType command_type = CanvasCommandType::None;
};

#endif // GRAPHICCANVASRIGHTCLICKEDMENU_H
