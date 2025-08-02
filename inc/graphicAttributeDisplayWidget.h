#ifndef GRAHPICATTRIBUTEDISPLAYWIDGET_H
#define GRAHPICATTRIBUTEDISPLAYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include "svgGlobalConfig.h"

class SvgChartItemBase;
class GraphicAttributeColorButton;
class GraphicAttributePenStyleComboBox;

class GrahpicAttributeDisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GrahpicAttributeDisplayWidget(QSize init_size, QColor init_color, QWidget *parent = nullptr);
    virtual ~GrahpicAttributeDisplayWidget() = default;

private:
    SvgChartItemBase* cur_operate_item = nullptr;
    CurrentItemAttributeDisplay cur_attribute_display_mod = CurrentItemAttributeDisplay::showNoneItem;

private:
    void initAttributeWinTitle(const QString &title);
    void initFixScenePropertyWinShow(QSize init_size, QColor init_color);
    void initDymanicItemPropertyWinShow();
    void updateDymanicItemPropertyWinShow();
    void dynamicChangeDisplayLine4layout(bool is_need_stretch);

    void initEditPenStyleMap();

private:
    // 在当前状态和操作对象修改后，对于DymanicItemProperty进行展示和更新
    void boundWidthWidgetShowAndUpdate();
    void boundStyleWidgetShowAndUpdate();
    void boundPenColorWidgetShowAndUpdate();
    void brushFillColorWidgetShowAndUpdate();

signals:
    // 对画布大小和宽度修改后，发送信号给scene
    void changeMainCanvasSceneHeight(int);
    void changeMainCanvasSceneWidth(int);
    void changeMainCanvasSceneSize(QSize);
    // 对画布颜色修改后，发送信号给scene
    void changeMainCanvasSceneColor(const QColor&);
    // 当前画布变化时，当前lineEdit同步变化
    void replyMainCanvasSceneWidth(int);
    void replyMainCanvasSceneHeight(int);
    void replyMainCanvasSceneColor(const QColor&);

public slots:
    // 回应视图zoom变化
    void replyViewZoomProportion(int);
    // 对画布颜色修改后，发送信号给scene
    void onCanvasSceneEditColorFinished(const QColor& change_color);
    // 回应视图中所选的物体，修改当前状态和操作对象
    void replyChangeSelectedItem(SvgChartItemBase*);
    // 直接对当前SvgChartItemBase对象的画笔和笔刷进行修改
    void onCanvasEditPenBoundWidthFinished(int change_width);
    void onCanvasEditBrushFillColorFinished(const QColor& change_color);
    void onCanvasEditPenBoundColorFinished(const QColor& change_color);
    void onCanvasEditPenStyleFinished(const Qt::PenStyle& change_pen_style);

private:
    QVBoxLayout *attribute_display_layout = nullptr;
private:
    QLineEdit *edit_proportion = nullptr;
private:
    // TODO: 可优化为单独写一个类继承并封装4个QWidget
    QWidget *bound_width;
    QLineEdit *edit_bound_width;

    QWidget *bound_style;
    GraphicAttributePenStyleComboBox* style_comboBox;

    QWidget *bound_pen_color;
    GraphicAttributeColorButton *edit_bound_pen_color;

    QWidget *brush_fill_color;
    GraphicAttributeColorButton *edit_brush_fill_color;

    QSpacerItem *spacerItem;
    bool is_need_expand = false;
    QHBoxLayout* scene_display_line4_layout;
};

#endif // GRAHPICATTRIBUTEDISPLAYWIDGET_H
