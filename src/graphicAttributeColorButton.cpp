#include "graphicAttributeColorButton.h"
#include <QWidget>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QColor>
#include <QStyleFactory>

GraphicAttributeColorButton::GraphicAttributeColorButton(const QColor &color, QWidget *parent): QPushButton(parent) {
    this->setCursor(Qt::PointingHandCursor);
    this->setMinimumHeight(20);

    setColor(color);
    connect(this, &QPushButton::clicked, this, &GraphicAttributeColorButton::chooseColor);
}

void GraphicAttributeColorButton::chooseColor() {
    QColorDialog dialog;
    dialog.setCurrentColor(Qt::white);
    if (dialog.exec() == QDialog::Accepted) {
        QColor color = dialog.currentColor();
        setColor(color);
    }
}

void GraphicAttributeColorButton::setColor(const QColor &color, bool is_send_signal) {
    QString styleSheet = QString("QPushButton { border: none; background-color: %1; }").arg(color.name());
    setStyleSheet(styleSheet);
    if (is_send_signal) emit colorChanged(color);
}
