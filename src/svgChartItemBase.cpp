#include "svgChartItemBase.h"
#include "graphicCanvasScene.h"
#include "svgchartItemFactory.h"
#include <QDebug>

SvgChartItemBase::SvgChartItemBase(CurrentDrawItemType item_type, QGraphicsItem *parent) : QGraphicsRectItem(parent){
    current_item_type = item_type;

    initDrawPen();
    initDrawBrush();
    initSelectedPen();

    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
}

SvgChartItemBase::SvgChartItemBase(CurrentDrawItemType item_type, const QRectF &rect, QGraphicsItem *parent) : QGraphicsRectItem(rect, parent){
    current_item_type = item_type;

    initDrawPen();
    initDrawBrush();
    initSelectedPen();

    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
}

SvgChartItemBase::SvgChartItemBase(CurrentDrawItemType item_type, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) : QGraphicsRectItem(x, y, w, h, parent){
    current_item_type = item_type;

    initDrawPen();
    initDrawBrush();
    initSelectedPen();

    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
}

void SvgChartItemBase::initDrawBrush(){
    draw_fill_brush.setColor(QColor(255, 255, 255, 255));
    draw_fill_brush.setStyle(Qt::SolidPattern);
}

void SvgChartItemBase::initDrawPen(){
    draw_bound_pen.setWidth(2);
    draw_bound_pen.setColor(Qt::black);
    draw_bound_pen.setStyle(Qt::SolidLine);
}

void SvgChartItemBase::initSelectedPen(){
    draw_selected_pen.setColor(Qt::blue);
    draw_selected_pen.setStyle(Qt::DashLine);
    draw_selected_pen.setWidth(2);
}

void SvgChartItemBase::setDrawBrushColor(const QColor& change_color){
    draw_fill_brush.setColor(QColor(change_color.red(), change_color.green(), change_color.blue(), 255));
    update();
}

void SvgChartItemBase::setDrawPenWidth(int change_width){
    draw_bound_pen.setWidth(change_width);
    update();
}

void SvgChartItemBase::setDrawPenColor(const QColor& change_color){
    draw_bound_pen.setColor(change_color);
    update();
}

void SvgChartItemBase::setDrawPenStyle(const Qt::PenStyle& change_pen_style){
    draw_bound_pen.setStyle(change_pen_style);
    update();
}

QColor SvgChartItemBase::getDrawBrushColor(){
    return draw_fill_brush.color();
}

int SvgChartItemBase::getDrawPenWidth(){
    return draw_bound_pen.width();
}

QColor SvgChartItemBase::getDrawPenColor(){
    return draw_bound_pen.color();
}

Qt::PenStyle SvgChartItemBase::getDrawPenStyle(){
    return draw_bound_pen.style();
}

QBrush SvgChartItemBase::getDrawFillBrush(){
    return draw_fill_brush;
}

QPen SvgChartItemBase::getDrawBoundPen(){
    return draw_bound_pen;
}

void SvgChartItemBase::setDrawFillBrush(const QBrush& set_brush){
    this->draw_fill_brush = set_brush;
    update();
}

void SvgChartItemBase::setDrawBoundPen(const QPen& set_pen){
    this->draw_bound_pen = set_pen;
    update();
}

void SvgChartItemBase::setDictDragFlag(bool is_need_drag){
    this->is_need_drag = is_need_drag;
}

void SvgChartItemBase::setDictMoveFlag(bool is_need_move){
    this->is_need_move = is_need_move;
}

void SvgChartItemBase::setDictHoverAllDirFlag(bool is_hove_all_dir){
    this->is_hove_all_dir = is_hove_all_dir;
}

void SvgChartItemBase::setDictMoveByRelativePos(bool is_update_position_by_relative){
    this->is_update_position_by_relative = is_update_position_by_relative;
}

SvgChartItemBase* SvgChartItemBase::setPreviewDict(){
    return new SvgChartItemBase(CurrentDrawItemType::drawRect, boundingRect());
}

CurrentDrawItemType SvgChartItemBase::getCurrentDrawItemType(){
    return this->current_item_type;
}

QRectF SvgChartItemBase::getCustomDictBoundingRect(const QList<QPointF>& cur_points) {
    if (cur_points.isEmpty()) {
        return QRectF();
    }

    qreal minX = cur_points.first().x();
    qreal minY = cur_points.first().y();
    qreal maxX = cur_points.first().x();
    qreal maxY = cur_points.first().y();

    for (const QPointF& point : cur_points) {
        minX = qMin(minX, point.x());
        minY = qMin(minY, point.y());
        maxX = qMax(maxX, point.x());
        maxY = qMax(maxY, point.y());
    }

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

// TODO: mousePressEvent触发保证了鼠标一定在SvgChartItemRect内部
void SvgChartItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QRectF cur_win_rectF = boundingRect();
    cur_drag_dir = isMousePointOnBorder(event->scenePos());
    if (cur_drag_dir != DragDirection::Content && !is_on_dragging && is_need_drag){
        is_on_dragging = true;
        if (!preview_rect){
            is_vaild_drag = false;
            preview_rect = setPreviewDict();
            preview_rect->setDrawFillBrush(this->draw_fill_brush);
            preview_rect->setDrawBoundPen(this->draw_bound_pen);
            preview_rect->setOpacity(0.5);
            scene()->addItem(preview_rect);
        }
        initial_rect = cur_win_rectF;
        aftchange_rect = cur_win_rectF;
    }
    else if (!is_on_moving && is_need_move){
        is_vaild_move = false;
        is_on_moving = true;
        start_press_pos = event->scenePos();

        bool is_contain_this = false;
        for(auto *select_item : scene()->selectedItems()){
            SvgChartItemBase* select_tp = qgraphicsitem_cast<SvgChartItemBase*>(select_item);
            if (select_tp && select_tp->is_need_move){
                if(select_item == this) is_contain_this = true;
                SvgChartItemBase* select_preview_rect_move = select_tp->setPreviewDict();
                if (select_preview_rect_move){
                    select_preview_rect_move->setDrawFillBrush(select_tp->draw_fill_brush);
                    select_preview_rect_move->setDrawBoundPen(select_tp->draw_bound_pen);
                    select_preview_rect_move->setOpacity(0.5);

                    all_origin_selected_rect_lists.append(select_tp);
                    all_preview_rect_move_lists.append(select_preview_rect_move);
                    scene()->addItem(select_preview_rect_move);
                }
            }
        }

        if (!is_contain_this){
            is_on_moving = false;
            for(int i = 0 ; i < all_preview_rect_move_lists.size() ; ++i){
                SvgChartItemBase *cur_preview_rect = all_preview_rect_move_lists[i];
                scene()->removeItem(cur_preview_rect);
                delete cur_preview_rect;
            }
            all_preview_rect_move_lists.clear();
            all_origin_selected_rect_lists.clear();
        }
    }
    QGraphicsRectItem::mousePressEvent(event);
}

// TODO: mouseMoveEvent触发保证了鼠标一定触发mousePressEvent
void SvgChartItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF cur_mouse_pos = event->scenePos();
    if (is_need_drag && is_on_dragging){
        QRectF rect = initial_rect;

        switch (cur_drag_dir) {
        case DragDirection::Top:
            rect.setTop(cur_mouse_pos.y());
            break;
        case DragDirection::Bottom:
            rect.setBottom(cur_mouse_pos.y());
            break;
        case DragDirection::Left:
            rect.setLeft(cur_mouse_pos.x());
            break;
        case DragDirection::Right:
            rect.setRight(cur_mouse_pos.x());
            break;
        case DragDirection::TopLeft:
            rect.setTopLeft(cur_mouse_pos);
            break;
        case DragDirection::TopRight:
            rect.setTopRight(cur_mouse_pos);
            break;
        case DragDirection::BottomLeft:
            rect.setBottomLeft(cur_mouse_pos);
            break;
        case DragDirection::BottomRight:
            rect.setBottomRight(cur_mouse_pos);
            break;
        default:
            break;
        }
        if (abs(rect.x() - initial_rect.x()) > vaild_drag_threshold
            || abs(rect.y() - initial_rect.y()) > vaild_drag_threshold
            || abs(rect.width() - initial_rect.width()) > vaild_drag_threshold
            || abs(rect.height() - initial_rect.height()) > vaild_drag_threshold){
            is_vaild_drag = true;
            preview_rect->setBaseRect(rect);
            // TODO: 直接使用preview_rect有精度问题
            aftchange_rect = rect;
        }
        event->accept();
    }
    else if (is_on_moving && is_need_move){
        qreal x_offset = cur_mouse_pos.x() - start_press_pos.x();
        qreal y_offset = cur_mouse_pos.y() - start_press_pos.y();
        if (abs(x_offset) > vaild_move_threshold || abs(y_offset) > vaild_move_threshold){
            is_vaild_move = true;

            for(int i = 0 ; i < all_preview_rect_move_lists.size() ; ++i){
                SvgChartItemBase *cur_preview_rect = all_preview_rect_move_lists[i];
                SvgChartItemBase *cur_origin_rect = all_origin_selected_rect_lists[i];
                QSize origin_size = QSize(cur_origin_rect->boundingRect().width(), cur_origin_rect->boundingRect().height());
                if (!cur_origin_rect->is_update_position_by_relative){
                    cur_preview_rect->setBaseRect(QRectF(event->scenePos().x(), event->scenePos().y(), cur_origin_rect->boundingRect().width(), cur_origin_rect->boundingRect().height()));
                }
                else{
                    QPointF new_move_point = QPointF(cur_origin_rect->boundingRect().x() + x_offset, cur_origin_rect->boundingRect().y() + y_offset);
                    QRectF new_move_pos = QRectF(new_move_point, origin_size);
                    cur_preview_rect->setBaseRect(new_move_pos);
                }
            }
        }
        event->accept();
    }
}

void SvgChartItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (is_need_drag && is_on_dragging){
        is_on_dragging = false;
        if (preview_rect) {
            if (is_vaild_drag){
                setBaseRect(aftchange_rect);
            }
            delete preview_rect;
            preview_rect = nullptr;
        }
    }
    else if (is_on_moving && is_need_move) {
        is_on_moving = false;
        for(int i = 0 ; i < all_preview_rect_move_lists.size() ; ++i){
            SvgChartItemBase *cur_preview_rect = all_preview_rect_move_lists[i];
            SvgChartItemBase *cur_origin_rect = all_origin_selected_rect_lists[i];
            if (is_vaild_move){
                QRectF new_rect = cur_preview_rect->boundingRect();
                if (!cur_origin_rect->is_update_position_by_relative){
                    cur_origin_rect->setBaseRect(QRectF(event->scenePos().x(), event->scenePos().y(), new_rect.width(), new_rect.height()));
                }
                else cur_origin_rect->setBaseRect(new_rect);
            }
            scene()->removeItem(cur_preview_rect);
            delete cur_preview_rect;
        }
        all_preview_rect_move_lists.clear();
        all_origin_selected_rect_lists.clear();
    }
    // QGraphicsRectItem::mouseReleaseEvent(event);
}


void SvgChartItemBase::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsScene* cur_scene_ptr = scene();
    if(cur_scene_ptr){
        GraphicCanvasScene* cur_scene = qobject_cast<GraphicCanvasScene*>(cur_scene_ptr);
        if(cur_scene &&cur_scene->getCurBarState() == CurrentBarState::originMouse && isSelected()){
            if (!is_hove_all_dir){
                emit itemHoverStateChanged(DragDirection::Content);
            }
            else emit itemHoverStateChanged(isMousePointOnBorder(event->scenePos()));
        }
        else {
            emit itemHoverStateChanged(DragDirection::None);
        }
    }
    QGraphicsRectItem::hoverMoveEvent(event);
}

void SvgChartItemBase::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    emit itemHoverStateChanged(DragDirection::None);
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void SvgChartItemBase::setBaseRect(const QRectF& rect){
    this->setRect(rect.normalized());
}

QRectF SvgChartItemBase::boundingRect() const{
    return QGraphicsRectItem::boundingRect();
}

void SvgChartItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    return QGraphicsRectItem::paint(painter, option, widget);
}

DragDirection SvgChartItemBase::isMousePointOnBorder(const QPointF& pos) {
    QRectF rect = boundingRect();
    const int borderWidth = vaild_mouse_on_bound_threshold;

    if (pos.y() <= rect.top() + borderWidth) {
        if (pos.x() <= rect.left() + borderWidth) {
            return DragDirection::TopLeft;
        } else if (pos.x() >= rect.right() - borderWidth) {
            return DragDirection::TopRight;
        } else {
            return DragDirection::Top;
        }
    } else if (pos.y() >= rect.bottom() - borderWidth) {
        if (pos.x() <= rect.left() + borderWidth) {
            return DragDirection::BottomLeft;
        } else if (pos.x() >= rect.right() - borderWidth) {
            return DragDirection::BottomRight;
        } else {
            return DragDirection::Bottom;
        }
    } else {
        if (pos.x() <= rect.left() + borderWidth) {
            return DragDirection::Left;
        } else if (pos.x() >= rect.right() - borderWidth) {
            return DragDirection::Right;
        } else {
            return DragDirection::Content;
        }
    }
}

SvgChartItemBase* SvgChartItemBase::deserializeProperty(const SvgChartItemBaseProperty& item_property) {
    SvgChartItemBase* new_qgraphic_item = SvgChartItemFactory::createGraphicItem(item_property.cur_draw_item_type, item_property.cur_del_pos);
    if (new_qgraphic_item){
        new_qgraphic_item->setDrawBoundPen(item_property.cur_draw_pen);
        new_qgraphic_item->setDrawFillBrush(item_property.cur_draw_brush);

        // TODO: 为了高扩展性，只能把该函数定义为虚函数，让子类自己去重写
        if (item_property.cur_draw_item_type == CurrentDrawItemType::drawText){
            SvgChartItemText* cur_text = qgraphicsitem_cast<SvgChartItemText*>(new_qgraphic_item);
            if (cur_text){
                cur_text->setDrawPenColor(item_property.cur_draw_pen.color());
                cur_text->setCurItemText(item_property.cur_text);
            }
        }

        if (item_property.cur_draw_item_type == CurrentDrawItemType::drawLine){
            SvgChartItemLine* cur_line = qgraphicsitem_cast<SvgChartItemLine*>(new_qgraphic_item);
            if (cur_line) cur_line->setLineState(item_property.cur_line_state);
        }

        if (item_property.cur_draw_item_type == CurrentDrawItemType::drawCustomLineRoute){
            SvgChartCustomLineRoute* cur_custom_line = qgraphicsitem_cast<SvgChartCustomLineRoute*>(new_qgraphic_item);
            if (cur_custom_line) cur_custom_line->setPoint(item_property.points);
        }
    }

    return new_qgraphic_item;
}

SvgChartItemBase* SvgChartItemBase::deserializeByteArray(const QByteArray& data) {
    SvgChartItemBaseProperty property;
    QDataStream stream(data);
    int typeInt;
    stream >> typeInt;
    property.cur_draw_item_type = static_cast<CurrentDrawItemType>(typeInt);
    stream >> property.cur_del_pos;
    stream >> property.cur_draw_pen;
    stream >> property.cur_draw_brush;
    stream >> property.cur_line_state;
    stream >> property.points;
    stream >> property.cur_text;
    return deserializeProperty(property);
}


SvgChartItemBaseProperty SvgChartItemBase::serializeProperty() {
    int cur_line_state = 0;
    if (current_item_type == CurrentDrawItemType::drawLine){
        SvgChartItemLine* cur_line = qgraphicsitem_cast<SvgChartItemLine*>(this);
        if (cur_line) cur_line_state = cur_line->cur_line_state;
    }
    QList<QPointF> points;
    if (current_item_type == CurrentDrawItemType::drawCustomLineRoute){
        SvgChartCustomLineRoute* cur_custom_line = qgraphicsitem_cast<SvgChartCustomLineRoute*>(this);
        if (cur_custom_line) points = cur_custom_line->getPoints();
    }
    QString cur_text;
    if (current_item_type == CurrentDrawItemType::drawText){
        SvgChartItemText* cur_custom_text = qgraphicsitem_cast<SvgChartItemText*>(this);
        if (cur_custom_text) cur_text = cur_custom_text->getCurItemText();
    }


    if (!this->is_update_position_by_relative){
        return {this->current_item_type,
                mapToScene(this->boundingRect()).boundingRect(),
                this->draw_bound_pen,
                this->draw_fill_brush,
                cur_line_state,
                points,
                cur_text
        };
    }
    else return {this->current_item_type,
            this->boundingRect(),
            this->draw_bound_pen,
            this->draw_fill_brush,
            cur_line_state,
            points,
            cur_text
        };
}

QByteArray SvgChartItemBase::serializeByteArray() {
    int cur_line_state = 0;
    if (current_item_type == CurrentDrawItemType::drawLine){
        SvgChartItemLine* cur_line = qgraphicsitem_cast<SvgChartItemLine*>(this);
        if (cur_line) cur_line_state = cur_line->cur_line_state;
    }
    QList<QPointF> points;
    if (current_item_type == CurrentDrawItemType::drawCustomLineRoute){
        SvgChartCustomLineRoute* cur_custom_line = qgraphicsitem_cast<SvgChartCustomLineRoute*>(this);
        if (cur_custom_line) points = cur_custom_line->getPoints();
    }
    QString cur_text;
    if (current_item_type == CurrentDrawItemType::drawText){
        SvgChartItemText* cur_custom_text = qgraphicsitem_cast<SvgChartItemText*>(this);
        if (cur_custom_text) cur_text = cur_custom_text->getCurItemText();
    }

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << static_cast<int>(this->current_item_type);
    if (!this->is_update_position_by_relative){
        stream << mapToScene(this->boundingRect()).boundingRect();
    }
    else stream << this->boundingRect();
    stream << this->draw_bound_pen;
    stream << this->draw_fill_brush;
    stream << cur_line_state;
    stream << points;
    stream << cur_text;
    return data;
}
