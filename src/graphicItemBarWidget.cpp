#include "graphicItemBarWidget.h"
#include "graphicItemBarPushButton.h"
#include <QVBoxLayout>
#include <QDebug>

GrahpicItemBarWidget::GrahpicItemBarWidget(QWidget *parent) : QWidget(parent){
    this->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
    this->setMinimumSize(70, 0);

    initBar2IconMap();
    initPushButton();
}

void GrahpicItemBarWidget::initBar2IconMap(){
    current_state_to_icon[CurrentBarState::originMouse] = QIcon(":/images/mouse.png");
    current_state_to_icon[CurrentBarState::drawCustomLineRoute] = QIcon(":/images/pen.png");
    current_state_to_icon[CurrentBarState::drawLine] = QIcon(":/images/line.png");
    current_state_to_icon[CurrentBarState::drawCircle] = QIcon(":/images/circle.png");
    current_state_to_icon[CurrentBarState::drawRect] = QIcon(":/images/rect.png");
    current_state_to_icon[CurrentBarState::drawPentagon] = QIcon(":/images/pentagon.png");
    current_state_to_icon[CurrentBarState::drawHexagon] = QIcon(":/images/hexagon.png");
    current_state_to_icon[CurrentBarState::drawStars] = QIcon(":/images/stars.png");
    current_state_to_icon[CurrentBarState::drawText] = QIcon(":/images/T.png");
    current_state_to_icon[CurrentBarState::Magnifier] = QIcon(":/images/magnifier.png");
}

void GrahpicItemBarWidget::initPushButton(){
    QVBoxLayout *graphic_item_bar_layout = new QVBoxLayout(this);
    graphic_item_bar_layout->setContentsMargins(9, 6, 0, 0);

    int start = static_cast<int>(CurrentBarState::originMouse);
    int end = static_cast<int>(CurrentBarState::Magnifier);
    for(int i = start ; i <= end ; ++i){
        CurrentBarState cur_bar_state = static_cast<CurrentBarState>(i);
        GraphicItemBarPushButton *new_bar_btn = new GraphicItemBarPushButton(cur_bar_state, current_state_to_icon[cur_bar_state], this);
        connect(new_bar_btn, &QPushButton::clicked, this, [&, cur_bar_state](){
            changeButtonIconColor();
            emit this->onBarChangeState(cur_bar_state);
        });

        connect(this, &GrahpicItemBarWidget::replyBarbuttonHoverInfo, new_bar_btn, [new_bar_btn, cur_bar_state](){
            auto pair = GlobalCommonSetting::getInstance().getBarButtonHoverInfo(cur_bar_state);
            new_bar_btn->setHoverTitle(pair.first);
            new_bar_btn->setHoverText(pair.second);
        });
        graphic_item_bar_layout->addWidget(new_bar_btn);
    }

    QSpacerItem *spacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    graphic_item_bar_layout->addItem(spacerItem);
}

void GrahpicItemBarWidget::changeButtonIconColor(){
    GraphicItemBarPushButton *clicked_button = dynamic_cast<GraphicItemBarPushButton*>(sender());
    if (!clicked_button) return;

    if (current_selected_button && current_selected_button != clicked_button) {
        current_selected_button->changeSelectedIconOriginColor();
    }

    clicked_button->changeSelectedIconColor2Blue();
    current_selected_button = clicked_button;
}

