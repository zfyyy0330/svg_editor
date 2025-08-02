#include "graphicAttributePenStyleComboBox.h"

GraphicAttributePenStyleComboBox::GraphicAttributePenStyleComboBox(QWidget *parent)
    : QComboBox(parent){
    this->setMinimumWidth(60);
    this->setCursor(Qt::PointingHandCursor);
    this->setStyleSheet("QComboBox { border: none; background - color: transparent; }");
    initEditPenStyleMap();
    for(int i = 0 ; i < comboBoxIndex_to_icon.size() ; ++i){
        this->addItem(comboBoxIndex_to_icon[i], "");
    }

    connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,[&](){
        emit this->penStyleChanged(comboBoxIndex_to_penStyle[this->currentIndex()]);
    });
}

void GraphicAttributePenStyleComboBox::setPenStyle(const Qt::PenStyle& change_pen_style){
    auto it = penStyle_to_comboBoxIndex.find(change_pen_style);
    if(it != penStyle_to_comboBoxIndex.end()){
        this->setCurrentIndex(penStyle_to_comboBoxIndex[change_pen_style]);
    }
}

void GraphicAttributePenStyleComboBox::initEditPenStyleMap(){
    comboBoxIndex_to_icon[0] = QIcon(":/images/solidLine.png");
    comboBoxIndex_to_icon[1] = QIcon(":/images/dashline.png");
    comboBoxIndex_to_icon[2] = QIcon(":/images/Dotline.png");
    comboBoxIndex_to_icon[3] = QIcon(":/images/dashDotLine.png");
    comboBoxIndex_to_icon[4] = QIcon(":/images/dashDotDotLine.png");

    comboBoxIndex_to_penStyle[0] = Qt::PenStyle::SolidLine;
    comboBoxIndex_to_penStyle[1] = Qt::PenStyle::DashLine;
    comboBoxIndex_to_penStyle[2] = Qt::PenStyle::DotLine;
    comboBoxIndex_to_penStyle[3] = Qt::PenStyle::DashDotLine;
    comboBoxIndex_to_penStyle[4] = Qt::PenStyle::DashDotDotLine;

    penStyle_to_comboBoxIndex[Qt::PenStyle::SolidLine] = 0;
    penStyle_to_comboBoxIndex[Qt::PenStyle::DashLine] = 1;
    penStyle_to_comboBoxIndex[Qt::PenStyle::DotLine] = 2;
    penStyle_to_comboBoxIndex[Qt::PenStyle::DashDotLine] = 3;
    penStyle_to_comboBoxIndex[Qt::PenStyle::DashDotDotLine] = 4;
}
