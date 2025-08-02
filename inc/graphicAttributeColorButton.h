#ifndef GRAPHICATTRIBUTECOLORBUTTON_H
#define GRAPHICATTRIBUTECOLORBUTTON_H

#include <QPushButton>

class GraphicAttributeColorButton : public QPushButton
{
    Q_OBJECT
public:
    explicit GraphicAttributeColorButton(const QColor &color, QWidget *parent = nullptr);
    virtual ~GraphicAttributeColorButton() = default;

    void setColor(const QColor &color, bool is_send_signal = true);

private slots:
    void chooseColor();

signals:
    void colorChanged(const QColor &newColor);
};

#endif // GRAPHICATTRIBUTECOLORBUTTON_H
