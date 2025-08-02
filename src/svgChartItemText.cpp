#include "svgChartItemText.h"
#include "graphicCanvasScene.h"
#include <QDebug>
#include <QTextCursor>

SvgChartItemText::SvgChartItemText(const QPointF &click_pos, QGraphicsItem *parent) : SvgChartItemBase(CurrentDrawItemType::drawText, parent){
    initCurTextItem();
    setDictDragFlag(false);
    setDictHoverAllDirFlag(false);
    setDictMoveByRelativePos(false);

    this->setPos(click_pos);
    cur_pos = click_pos;
}

SvgChartItemText::SvgChartItemText(const QPointF &click_pos, const QString& custom_text, QGraphicsItem *parent): SvgChartItemBase(CurrentDrawItemType::drawText, parent){
    initCurTextItem(custom_text);
    setDictDragFlag(false);
    setDictHoverAllDirFlag(false);
    setDictMoveByRelativePos(false);

    this->setPos(click_pos);
    cur_pos = click_pos;
}

void SvgChartItemText::setCurItemText(const QString& text) {
    this->cur_text_item->setPlainText(text);
}

QString SvgChartItemText::getCurItemText() const{
    return this->cur_text_item->toPlainText();
}

void SvgChartItemText::initCurTextItem(const QString& custom_text){
    cur_text_item = new SvgChartItemTextBase(custom_text, this);
    cur_text_item->setFont(QFont(custom_text, 14));
    cur_text_item->setDefaultTextColor(draw_bound_pen.color());
    connect(cur_text_item, &SvgChartItemTextBase::itemHoverStateChanged, this, [&](DragDirection drag_dir){
        if (this->isSelected()){
            emit itemHoverStateChanged(drag_dir);
        }
    });
}

SvgChartItemBase* SvgChartItemText::setPreviewDict(){
    SvgChartItemText *preview_dict = new SvgChartItemText(this->cur_pos, this->cur_text_item->toPlainText());
    preview_dict->cur_text_item->setDefaultTextColor(draw_bound_pen.color());
    return preview_dict;
}

void SvgChartItemText::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (event->button() == Qt::MouseButton::LeftButton){
        QGraphicsScene* item_scene = this->scene();
        if (item_scene) {
            GraphicCanvasScene* cur_item_scene = static_cast<GraphicCanvasScene*>(item_scene);
            emit cur_item_scene->setSelectedItemAttributeDisplay(this);
        }
    }
    SvgChartItemBase::mousePressEvent(event);
}

void SvgChartItemText::setBaseRect(const QRectF &new_rect){
    // 拉取的时候鼠标垂直居中文本
    QPointF new_point = QPointF(new_rect.topLeft().x() - new_rect.width() / 2, new_rect.topLeft().y() - new_rect.height() / 2);
    this->setPos(new_point);
    cur_pos = new_point;
}

QRectF SvgChartItemText::boundingRect() const{
    return QRectF(cur_text_item->boundingRect());
}

void SvgChartItemText::setDrawPenColor(const QColor& change_color){
    draw_bound_pen.setColor(change_color);
    cur_text_item->setDefaultTextColor(draw_bound_pen.color());
}

void SvgChartItemText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget);
    Q_UNUSED(option);

    setRect(boundingRect());
    if (this->isSelected()) {
        painter->setPen(draw_selected_pen);
        painter->drawRect(boundingRect());
    }
}

void SvgChartItemText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
        cur_text_item->setTextInteractionFlags(Qt::TextEditorInteraction);
        cur_text_item->setFocus();

        QTextCursor cursor = cur_text_item->textCursor();
        cursor.select(QTextCursor::Document);
        cur_text_item->setTextCursor(cursor);

        emit itemHoverStateChanged(DragDirection::None, CurrentBarState::drawText);
        cur_text_item->is_edit_mode = true;
    }
    SvgChartItemBase::mouseDoubleClickEvent(event);
}
