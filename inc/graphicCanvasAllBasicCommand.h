#ifndef GRAPHICCANVASALLBASICCOMMAND_H
#define GRAPHICCANVASALLBASICCOMMAND_H

#include <QStack>
#include <QGraphicsScene>

class SvgChartItemBaseProperty;
class SvgChartItemBase;

// 命令基类
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

// 全选命令
class AllSelectCommand final : public Command {
public:
    explicit AllSelectCommand(QGraphicsScene* scene);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
};

// 删除命令
class DeleteCommand : public Command {
public:
    explicit DeleteCommand(QGraphicsScene* scene, const QList<QGraphicsItem *>& selected_items);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QList<QGraphicsItem *> selected_items;
    QList<SvgChartItemBaseProperty> copy_selected_property;
};

// 复制命令
class CopyCommand : public Command {
public:
    explicit CopyCommand(QGraphicsScene* scene, const QList<QGraphicsItem *>& selected_items);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QList<QGraphicsItem*> selected_items;
};

// 剪切命令
class CutCommand : public Command {
public:
    explicit CutCommand(QGraphicsScene* scene, const QList<QGraphicsItem *>& selected_items);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QList<QGraphicsItem*> selected_items;
    QList<SvgChartItemBaseProperty> copy_selected_property;
};

// 粘贴命令
class PasteCommand : public Command {
public:
    explicit PasteCommand(QGraphicsScene* scene);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QList<SvgChartItemBase*> new_graphic_item_lists;
};

// 复写命令
class copyAndPasteCommand : public Command{
public:
    explicit copyAndPasteCommand(QGraphicsScene* scene, const QList<QGraphicsItem *>& selected_items);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QList<QGraphicsItem*> selected_items;
    QList<SvgChartItemBase*> copy_selected_property;
};

// 置上一层命令
class placeUpperLayerCommand : public Command{
public:
    explicit placeUpperLayerCommand(QGraphicsScene* scene, QGraphicsItem* selected_item);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QGraphicsItem* cur_change_item = nullptr;
};

// 置下一层命令
class placeLowerLayerCommand : public Command{
public:
    explicit placeLowerLayerCommand(QGraphicsScene* scene, QGraphicsItem* selected_item);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QGraphicsItem* cur_change_item = nullptr;
};

// 置顶部命令
class placeTopLayerCommand : public Command{
public:
    explicit placeTopLayerCommand(QGraphicsScene* scene, QGraphicsItem* selected_item);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QGraphicsItem* cur_change_item = nullptr;
    int last_selected_item_index = -1;
};

// 置底部
class placeBottomLayerCommand : public Command{
public:
    explicit placeBottomLayerCommand(QGraphicsScene* scene, QGraphicsItem* selected_item);
    void execute() override;
    void undo() override;
private:
    QGraphicsScene* scene;
    QGraphicsItem* cur_change_item = nullptr;
    int last_selected_item_index = -1;
};

#endif // GRAPHICCANVASALLBASICCOMMAND_H
