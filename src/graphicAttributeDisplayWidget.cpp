#include "graphicAttributeDisplayWidget.h"
#include "svgChartItemBase.h"
#include "graphicAttributeColorButton.h"
#include "graphicAttributeIntVaildator.h"
#include "graphicAttributePenStyleComboBox.h"
#include <QDebug>

GrahpicAttributeDisplayWidget::GrahpicAttributeDisplayWidget(QSize init_size, QColor init_color, QWidget *parent) : QWidget(parent){
    this->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
    this->setMinimumSize(220, 0);
    this->setMaximumSize(220, 16635);

    attribute_display_layout = new QVBoxLayout(this);
    attribute_display_layout->setContentsMargins(6, 4, 6, 4);

    initAttributeWinTitle("参数配置");
    initFixScenePropertyWinShow(init_size, init_color);
    initDymanicItemPropertyWinShow();
    updateDymanicItemPropertyWinShow();
    attribute_display_layout->addStretch();
}

void GrahpicAttributeDisplayWidget::initAttributeWinTitle(const QString &title){
    // 标题
    QWidget* display_title = new QWidget(this);
    QHBoxLayout* display_title_layout = new QHBoxLayout(display_title);
    display_title_layout->setContentsMargins(9, 9, 0, 4);
    QLabel *parm_label = new QLabel(title, display_title);
    QFont font("Microsoft YaHei", 10);
    parm_label->setFont(font);
    display_title_layout->addWidget(parm_label);
    attribute_display_layout->addWidget(display_title);
}

void GrahpicAttributeDisplayWidget::initFixScenePropertyWinShow(QSize init_size, QColor init_color){
    // TODO: 优化为封装为一个QWidget避免冗余的设置，目前较难维护
    // 第一行
    QWidget* scene_display_line1 = new QWidget(this);
    QHBoxLayout* scene_display_line1_layout = new QHBoxLayout(scene_display_line1);
    scene_display_line1_layout->setContentsMargins(6, 4, 6, 4);

    // 第一行：窗口一
    QWidget *scene_width = new QWidget(this);
    scene_width->setStyleSheet("QWidget { background-color: rgb(230, 230, 230); }");
    QVBoxLayout* scene_width_layout = new QVBoxLayout(scene_width);
    scene_width_layout->setContentsMargins(6, 4, 6, 4);
    QLabel *scene_width_label = new QLabel("画布宽度", scene_width);
    QLineEdit *edit_scene_width = new QLineEdit(QString::number(init_size.width()), scene_width);
    GraphicAttributeIntVaildator *width_validator = new GraphicAttributeIntVaildator(SceneMinWidthThreshold, SceneMaxWidthThreshold, edit_scene_width);
    edit_scene_width->setValidator(width_validator);
    connect(edit_scene_width, &QLineEdit::editingFinished, this, [&, edit_scene_width](){
        if (edit_scene_width->text() != ""){
            emit changeMainCanvasSceneWidth(edit_scene_width->text().toInt());
        }
    });
    connect(this, &GrahpicAttributeDisplayWidget::replyMainCanvasSceneWidth, this, [&, edit_scene_width](int change_width){
        edit_scene_width->setText(QString::number(change_width));
    });

    edit_scene_width->setCursor(Qt::IBeamCursor);
    edit_scene_width->setAlignment(Qt::AlignCenter);
    edit_scene_width->setStyleSheet("QLineEdit { background-color: transparent; border: none; color: rgb(79, 127, 255); font: 87 11pt \"Arial Black\";}");
    scene_width_layout->addWidget(scene_width_label);
    scene_width_layout->addWidget(edit_scene_width);
    scene_width_layout->setAlignment(scene_width_label, Qt::AlignHCenter | Qt::AlignVCenter);
    // 窗口一添加
    scene_display_line1_layout->addWidget(scene_width);
    scene_display_line1_layout->setStretch(0, 1);

    // 第一行：窗口二
    QWidget *scene_height = new QWidget(this);
    scene_height->setStyleSheet("QWidget { background-color: rgb(230, 230, 230); }");
    QVBoxLayout* scene_height_layout = new QVBoxLayout(scene_height);
    scene_height_layout->setContentsMargins(6, 4, 6, 4);
    QLabel *scene_height_label = new QLabel("画布高度", scene_height);
    QLineEdit *edit_scene_height = new QLineEdit(QString::number(init_size.height()), scene_height);
    GraphicAttributeIntVaildator *height_validator = new GraphicAttributeIntVaildator(SceneMinHeightThreshold, SceneMaxHeightThreshold, edit_scene_height);
    edit_scene_height->setValidator(height_validator);
    connect(edit_scene_height, &QLineEdit::editingFinished, this, [&, edit_scene_height](){
        if (edit_scene_height->text() != ""){
            emit changeMainCanvasSceneHeight(edit_scene_height->text().toInt());
        }
    });
    connect(this, &GrahpicAttributeDisplayWidget::replyMainCanvasSceneHeight, this, [&, edit_scene_height](int change_height){
        edit_scene_height->setText(QString::number(change_height));
    });

    edit_scene_height->setCursor(Qt::IBeamCursor);
    edit_scene_height->setAlignment(Qt::AlignCenter);
    edit_scene_height->setStyleSheet("QLineEdit { background-color: transparent; border: none; color: rgb(79, 127, 255); font: 87 11pt \"Arial Black\";}");
    scene_height_layout->addWidget(scene_height_label);
    scene_height_layout->addWidget(edit_scene_height);
    scene_height_layout->setAlignment(scene_height_label, Qt::AlignHCenter | Qt::AlignVCenter);
    // 窗口二添加
    scene_display_line1_layout->addWidget(scene_height);
    scene_display_line1_layout->setStretch(1, 1);
    attribute_display_layout->addWidget(scene_display_line1);

    // 第二行
    QWidget* scene_display_line2 = new QWidget(this);
    QHBoxLayout* scene_display_line2_layout = new QHBoxLayout(scene_display_line2);
    scene_display_line2_layout->setContentsMargins(6, 4, 6, 4);

    // 第二行：窗口一
    QWidget *scene_proportion = new QWidget(this);
    scene_proportion->setStyleSheet("QWidget { background-color: rgb(230, 230, 230); }");
    QVBoxLayout* scene_proportion_layout = new QVBoxLayout(scene_proportion);
    scene_proportion_layout->setContentsMargins(6, 4, 6, 4);
    QLabel *proportion = new QLabel("缩放比例", scene_proportion);
    edit_proportion = new QLineEdit("100", scene_proportion);
    edit_proportion->setAlignment(Qt::AlignCenter);
    edit_proportion->setReadOnly(true);
    edit_proportion->setCursor(Qt::PointingHandCursor);
    edit_proportion->setStyleSheet("QLineEdit { background-color: transparent; border: none; color: rgb(79, 127, 255); font: 87 11pt \"Arial Black\";}");
    scene_proportion_layout->addWidget(proportion);
    scene_proportion_layout->addWidget(edit_proportion);
    scene_proportion_layout->setAlignment(proportion, Qt::AlignHCenter | Qt::AlignVCenter);
    // 添加
    scene_display_line2_layout->addWidget(scene_proportion);
    scene_display_line2_layout->setStretch(0, 1);

    // 第二行：窗口二
    QWidget *scene_back_color = new QWidget(this);
    scene_back_color->setStyleSheet("QWidget { background-color: rgb(230, 230, 230); }");
    QVBoxLayout* scene_back_color_layout = new QVBoxLayout(scene_back_color);
    scene_back_color_layout->setContentsMargins(6, 4, 6, 4);
    QLabel *back_color = new QLabel("画布颜色", scene_back_color);
    GraphicAttributeColorButton *edit_back_color = new GraphicAttributeColorButton(init_color, scene_back_color);
    connect(edit_back_color, &GraphicAttributeColorButton::colorChanged, this, &GrahpicAttributeDisplayWidget::onCanvasSceneEditColorFinished);
    connect(this, &GrahpicAttributeDisplayWidget::replyMainCanvasSceneColor, this, [&, edit_back_color](const QColor& change_color){
        edit_back_color->setColor(change_color, false);
    });
    scene_back_color_layout->addWidget(back_color);
    scene_back_color_layout->addWidget(edit_back_color);
    scene_back_color_layout->setAlignment(back_color, Qt::AlignHCenter | Qt::AlignVCenter);
    scene_back_color_layout->setAlignment(Qt::AlignCenter);

    // 添加
    scene_display_line2_layout->addWidget(scene_back_color);
    scene_display_line2_layout->setStretch(1, 1);
    attribute_display_layout->addWidget(scene_display_line2);
}

void GrahpicAttributeDisplayWidget::initDymanicItemPropertyWinShow(){
    // 第三行
    QWidget* scene_display_line3 = new QWidget(this);
    QHBoxLayout* scene_display_line3_layout = new QHBoxLayout(scene_display_line3);
    scene_display_line3_layout->setContentsMargins(6, 4, 6, 4);

    // 第三行：窗口一
    bound_width = new QWidget(this);
    bound_width->setStyleSheet("QWidget { background-color: rgb(230, 230, 230); }");
    QVBoxLayout* bound_width_layout = new QVBoxLayout(bound_width);
    bound_width_layout->setContentsMargins(6, 4, 6, 4);
    QLabel *bound_width_label = new QLabel("边框宽度", bound_width);
    edit_bound_width = new QLineEdit(QString::number(2), bound_width);
    GraphicAttributeIntVaildator *bound_width_validator = new GraphicAttributeIntVaildator(ItemPenBoundMinWidthThreshold, ItemPenBoundMaxWidthThreshold, edit_bound_width);
    edit_bound_width->setValidator(bound_width_validator);
    connect(edit_bound_width, &QLineEdit::editingFinished, this, [&](){
        if (edit_bound_width->text() != ""){
            onCanvasEditPenBoundWidthFinished(edit_bound_width->text().toInt());
        }
    });
    edit_bound_width->setCursor(Qt::IBeamCursor);
    edit_bound_width->setAlignment(Qt::AlignCenter);
    edit_bound_width->setStyleSheet("QLineEdit { background-color: transparent; border: none; color: rgb(79, 127, 255); font: 87 11pt \"Arial Black\";}");
    bound_width_layout->addWidget(bound_width_label);
    bound_width_layout->addWidget(edit_bound_width);
    bound_width_layout->setAlignment(bound_width_label, Qt::AlignHCenter | Qt::AlignVCenter);
    // 添加
    scene_display_line3_layout->addWidget(bound_width);
    scene_display_line3_layout->setStretch(0, 1);

    // 第三行：窗口二
    bound_style = new QWidget(this);
    bound_style->setStyleSheet("QWidget { background-color: rgb(230, 230, 230); }");
    QVBoxLayout* bound_style_layout = new QVBoxLayout(bound_style);
    bound_style_layout->setContentsMargins(6, 4, 6, 4);
    QLabel *bound_style_label = new QLabel("边框样式", bound_style);
    // 下拉框
    style_comboBox = new GraphicAttributePenStyleComboBox(this);
    connect(style_comboBox, &GraphicAttributePenStyleComboBox::penStyleChanged, this, &GrahpicAttributeDisplayWidget::onCanvasEditPenStyleFinished);
    bound_style_layout->addWidget(bound_style_label);
    bound_style_layout->addWidget(style_comboBox);
    bound_style_layout->setAlignment(bound_style_label, Qt::AlignHCenter | Qt::AlignVCenter);
    bound_style_layout->setAlignment(style_comboBox, Qt::AlignHCenter | Qt::AlignVCenter);
    scene_display_line3_layout->addWidget(bound_style);
    // 第三行：窗口二
    scene_display_line3_layout->setStretch(1, 1);
    attribute_display_layout->addWidget(scene_display_line3);

    // 第四行
    QWidget* scene_display_line4 = new QWidget(this);
    scene_display_line4_layout = new QHBoxLayout(scene_display_line4);
    scene_display_line4_layout->setContentsMargins(6, 4, 6, 4);

    // 第四行：窗口一
    bound_pen_color = new QWidget(this);
    bound_pen_color->setStyleSheet("QWidget { background-color: rgb(230, 230, 230); }");
    QVBoxLayout* bound_pen_color_layout = new QVBoxLayout(bound_pen_color);
    bound_pen_color_layout->setContentsMargins(6, 4, 6, 4);
    QLabel *bound_pen_color_label = new QLabel("边框颜色", bound_pen_color);
    edit_bound_pen_color = new GraphicAttributeColorButton(Qt::white, bound_pen_color);
    connect(edit_bound_pen_color, &GraphicAttributeColorButton::colorChanged, this, &GrahpicAttributeDisplayWidget::onCanvasEditPenBoundColorFinished);
    bound_pen_color_layout->addWidget(bound_pen_color_label);
    bound_pen_color_layout->addWidget(edit_bound_pen_color);
    bound_pen_color_layout->setAlignment(bound_pen_color_label, Qt::AlignHCenter | Qt::AlignVCenter);
    bound_pen_color_layout->setAlignment(Qt::AlignCenter);
    // 添加
    scene_display_line4_layout->addWidget(bound_pen_color);
    scene_display_line4_layout->setStretch(0, 1);

    // 第四行：窗口二
    brush_fill_color = new QWidget(this);
    brush_fill_color->setStyleSheet("QWidget { background-color: rgb(230, 230, 230); }");
    QVBoxLayout* brush_fill_color_layout = new QVBoxLayout(brush_fill_color);
    brush_fill_color_layout->setContentsMargins(6, 4, 6, 4);
    QLabel *brush_fill_color_label = new QLabel("填充颜色", brush_fill_color);
    edit_brush_fill_color = new GraphicAttributeColorButton(Qt::white, brush_fill_color);
    connect(edit_brush_fill_color, &GraphicAttributeColorButton::colorChanged, this, &GrahpicAttributeDisplayWidget::onCanvasEditBrushFillColorFinished);
    brush_fill_color_layout->addWidget(brush_fill_color_label);
    brush_fill_color_layout->addWidget(edit_brush_fill_color);
    brush_fill_color_layout->setAlignment(brush_fill_color_label, Qt::AlignHCenter | Qt::AlignVCenter);
    brush_fill_color_layout->setAlignment(Qt::AlignCenter);
    // 添加
    scene_display_line4_layout->addWidget(brush_fill_color);
    scene_display_line4_layout->setStretch(1, 1);
    attribute_display_layout->addWidget(scene_display_line4);

    // 弹簧以自适应修改窗口排版
    spacerItem = new QSpacerItem(102, 0, QSizePolicy::Preferred, QSizePolicy::Minimum);
}

void GrahpicAttributeDisplayWidget::updateDymanicItemPropertyWinShow(){
    /*
    QWidget *bound_width;
    QWidget *bound_style;
    QWidget *bound_pen_color;
    QWidget *brush_fill_color;
    */
    if (cur_attribute_display_mod == CurrentItemAttributeDisplay::showNoneItem){
        if(bound_width) bound_width->hide();
        if(bound_style) bound_style->hide();
        if(bound_pen_color) bound_pen_color->hide();
        if(brush_fill_color) brush_fill_color->hide();

        dynamicChangeDisplayLine4layout(false);
    }
    else if(cur_attribute_display_mod == CurrentItemAttributeDisplay::showDrawBoundPenAndBrush){
        boundWidthWidgetShowAndUpdate();
        boundStyleWidgetShowAndUpdate();
        boundPenColorWidgetShowAndUpdate();
        brushFillColorWidgetShowAndUpdate();

        dynamicChangeDisplayLine4layout(false);
    }
    else if(cur_attribute_display_mod == CurrentItemAttributeDisplay::showDrawBoundPen){
        boundWidthWidgetShowAndUpdate();
        boundStyleWidgetShowAndUpdate();
        boundPenColorWidgetShowAndUpdate();
        if(brush_fill_color) brush_fill_color->hide();

        dynamicChangeDisplayLine4layout(true);

    }
    else if(cur_attribute_display_mod == CurrentItemAttributeDisplay::showFontColor){
        if(bound_width) bound_width->hide();
        if(bound_style) bound_style->hide();
        boundPenColorWidgetShowAndUpdate();
        if(brush_fill_color) brush_fill_color->hide();

        dynamicChangeDisplayLine4layout(true);
    }
}

void GrahpicAttributeDisplayWidget::dynamicChangeDisplayLine4layout(bool is_need_stretch){
    if (!is_need_stretch && scene_display_line4_layout){
        scene_display_line4_layout->setStretch(0, 1);
        scene_display_line4_layout->setStretch(1, 1);
        if (is_need_expand){
            scene_display_line4_layout->removeItem(spacerItem);
            is_need_expand = false;
        }
    }
    else{
        scene_display_line4_layout->setStretch(0, 0);
        scene_display_line4_layout->setStretch(1, 0);
        if (!is_need_expand){
            scene_display_line4_layout->addItem(spacerItem);
            is_need_expand = true;
        }
    }
}

void GrahpicAttributeDisplayWidget::boundWidthWidgetShowAndUpdate(){
    if (cur_operate_item && edit_bound_width){
        edit_bound_width->setText(QString::number(cur_operate_item->getDrawPenWidth()));
    }
    if (bound_width) bound_width->show();
}

void GrahpicAttributeDisplayWidget::boundStyleWidgetShowAndUpdate(){
    if (cur_operate_item && style_comboBox) {
        // 下拉框
        Qt::PenStyle cur_pen_style = cur_operate_item->getDrawPenStyle();
        style_comboBox->setPenStyle(cur_pen_style);
    }

    if (bound_style) bound_style->show();
}

void GrahpicAttributeDisplayWidget::boundPenColorWidgetShowAndUpdate(){
    if (cur_operate_item && edit_bound_pen_color) {
        edit_bound_pen_color->setColor(cur_operate_item->getDrawPenColor());
    }
    if (bound_pen_color) bound_pen_color->show();
}

void GrahpicAttributeDisplayWidget::brushFillColorWidgetShowAndUpdate(){
    if (cur_operate_item && edit_brush_fill_color) {
        edit_brush_fill_color->setColor(cur_operate_item->getDrawBrushColor());
    }
    if (brush_fill_color) brush_fill_color->show();
}

// 对画布颜色修改后，发送信号给scene
void GrahpicAttributeDisplayWidget::onCanvasSceneEditColorFinished(const QColor& change_color){
    emit changeMainCanvasSceneColor(change_color);
}

// 回应视图zoom变化
void GrahpicAttributeDisplayWidget::replyViewZoomProportion(int cur_proportion){
    if(edit_proportion) edit_proportion->setText(QString::number(cur_proportion));
}

// 直接对当前SvgChartItemBase对象的画笔和笔刷进行修改
void GrahpicAttributeDisplayWidget::onCanvasEditPenBoundWidthFinished(int change_width){
    if (cur_operate_item){
        cur_operate_item->setDrawPenWidth(change_width);
    }
}

void GrahpicAttributeDisplayWidget::onCanvasEditBrushFillColorFinished(const QColor& change_color){
    if (cur_operate_item){
        cur_operate_item->setDrawBrushColor(change_color);
    }
}

void GrahpicAttributeDisplayWidget::onCanvasEditPenBoundColorFinished(const QColor& change_color){
    if (cur_operate_item){
        cur_operate_item->setDrawPenColor(change_color);
    }
}

void GrahpicAttributeDisplayWidget::onCanvasEditPenStyleFinished(const Qt::PenStyle& change_pen_style){
    if (this->cur_operate_item){
        cur_operate_item->setDrawPenStyle(change_pen_style);
    }
}

// 回应视图中所选的物体，修改当前状态和操作对象
void GrahpicAttributeDisplayWidget::replyChangeSelectedItem(SvgChartItemBase* cur_selected_item){
    this->cur_operate_item = cur_selected_item;

    if (!cur_selected_item){
        cur_attribute_display_mod = CurrentItemAttributeDisplay::showNoneItem;
    }
    else{
        CurrentDrawItemType cur_selected_item_type = cur_selected_item->getCurrentDrawItemType();
        if (cur_selected_item_type >= CurrentDrawItemType::drawCircle &&
            cur_selected_item_type <= CurrentDrawItemType::drawStars){
            cur_attribute_display_mod = CurrentItemAttributeDisplay::showDrawBoundPenAndBrush;
        }
        else if(cur_selected_item_type >= CurrentDrawItemType::drawCustomLineRoute &&
                 cur_selected_item_type <= CurrentDrawItemType::drawLine){
            cur_attribute_display_mod = CurrentItemAttributeDisplay::showDrawBoundPen;
        }
        else if(cur_selected_item_type == CurrentDrawItemType::drawText){
            cur_attribute_display_mod = CurrentItemAttributeDisplay::showFontColor;
        }
        else{
            qDebug()<<"unKnown CurrentDrawItemType";
        }
    }

    updateDymanicItemPropertyWinShow();
}
