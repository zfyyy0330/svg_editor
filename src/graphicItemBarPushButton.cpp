#include "graphicItemBarPushButton.h"
#include <QToolTip>
#include <QEvent>

GraphicItemBarPushButton::GraphicItemBarPushButton(CurrentBarState cur_button_state, const QIcon& cur_button_icon, QWidget *parent) : QPushButton(parent){
    this->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
    this->setMinimumSize(QSize(65, 65));
    this->setMaximumSize(QSize(65, 65));
    this->setStyleSheet("QPushButton { background-color: transparent; border: none; }" "QPushButton:hover { background-color: rgba(0, 0, 0, 0.2);}");
    this->setCursor(Qt::PointingHandCursor);
    this->setIcon(cur_button_icon);

    this->cur_button_state = cur_button_state;
    this->cur_button_icon = cur_button_icon;

    installEventFilter(this);
}

CurrentBarState GraphicItemBarPushButton::getCurrentButtonState(){
    return cur_button_state;
}

void GraphicItemBarPushButton::changeSelectedIconOriginColor(){
    this->setIcon(cur_button_icon);
}

void GraphicItemBarPushButton::setHoverText(const QString& cur_text){
    this->hover_text = cur_text;
}
void GraphicItemBarPushButton::setHoverTitle(const QString& cur_title){
    this->hover_title = cur_title;
}

bool GraphicItemBarPushButton::eventFilter(QObject *obj, QEvent *event){
    switch (event->type()) {
    case QEvent::HoverEnter:
        if (!hover_title.isEmpty() && !hover_text.isEmpty()){
            QToolTip::showText(mapToGlobal(QPoint(60, 0)), hover_title + "\n" + hover_text);
        }
        break;
    case QEvent::HoverLeave:
        QToolTip::hideText();
        break;
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}

void GraphicItemBarPushButton::changeSelectedIconColor2Blue(){
    QPixmap pixmap = this->icon().pixmap(65, 65);
    QImage image = pixmap.toImage();
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QRgb color = image.pixel(x, y);
            if (qAlpha(color)!= 0) {
                image.setPixel(x, y, qRgb(0, 204, 255));
            }
        }
    }
    QPixmap newPixmap = QPixmap::fromImage(image);
    this->setIcon(QIcon(newPixmap));
}
