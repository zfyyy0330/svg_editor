#ifndef SVGGLOBALCONFIG_H
#define SVGGLOBALCONFIG_H

enum class DragDirection {
    Content,
    Top,
    Bottom,
    Left,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    None
};

enum class CurrentBarState{
    originMouse,
    drawCustomLineRoute,
    drawLine,
    drawCircle,
    drawRect,
    drawPentagon,
    drawHexagon,
    drawStars,
    drawText,
    Magnifier,
    None
};

enum class CurrentDrawItemType{
    drawCustomLineRoute,
    drawLine,
    drawCircle,
    drawRect,
    drawPentagon,
    drawHexagon,
    drawStars,
    drawText,
    None
};

enum class CurrentItemAttributeDisplay{
    showNoneItem,
    showFontColor,
    showDrawBoundPen,
    showDrawBoundPenAndBrush
};

enum class MagnifierZoomType{
    ZoomIn,
    ZoomOut
};

enum class CanvasCommandType{
    AllSelect,
    Cut,
    Copy,
    Paste,
    CopyAndPaste,
    Delete,
    PlaceUpperLayer,
    PlaceLowerLayer,
    PlaceTopLayer,
    PlaceBottomLayer,
    Undo,
    Redo,
    None
};

#define SceneMinWidthThreshold 1
#define SceneMaxWidthThreshold 8000
#define SceneMinHeightThreshold 1
#define SceneMaxHeightThreshold 8000

#define ItemPenBoundMinWidthThreshold 1
#define ItemPenBoundMaxWidthThreshold 8

#define MagnifierZoomTimes 5

#define SceneVaildDrawDiceThreshold 10

#define SceneMaxUndoOperTime 40
#define SceneDefaultWidth 1000
#define SceneDefaultHeight 1000

#include <QString>
#include <QQueue>
#include <QColor>
#include <QMap>

class Command;

// 撤销栈管理类
class UndoQueue final {
public:
    explicit UndoQueue(int max_size);
    ~UndoQueue();
    void push(Command* command);
    void pop();
    bool isEmpty();

    void redo();
    bool isRedoEmpty();

private:
    int max_size;
    QQueue<Command*> undo_stack;
    QQueue<Command*> redo_stack;
};

// 单例
struct GlobalCommonSetting{
    static GlobalCommonSetting& getInstance();
    virtual ~GlobalCommonSetting();

    void setUndoQueue(UndoQueue *);
    UndoQueue *getUndoQueue();

    void setSceneWidth(int);
    int getSceneWidth();
    void setSceneHeight(int);
    int getSceneHeight();
    void setSceneColor(const QColor&);
    QColor getSceneColor();

    static CurrentBarState mapBarKeyToEnum(const QString &key);
    void addBarButtonHoverInfo(CurrentBarState, const QPair<QString, QString>&);
    QPair<QString, QString> getBarButtonHoverInfo(CurrentBarState);
    QMap<CurrentBarState, QPair<QString, QString>> getBarButton();

private:
    explicit GlobalCommonSetting() = default;
    GlobalCommonSetting(const GlobalCommonSetting&) = delete;
    void operator=(const GlobalCommonSetting&) = delete;

private:
    int scene_width;
    int scene_height;
    QColor scene_color;

    QColor bound_pen_color;
    int bound_pen_width;
    Qt::PenStyle bound_pen_style;
    QColor brush_fill_color;
    QMap<CurrentBarState, QPair<QString, QString>> bar_button_hover_info;

    // TODO: 是否undo和redo放在全局更好
    UndoQueue *undo_command_record = nullptr;
};

#endif // SVGGLOBALCONFIG_H
