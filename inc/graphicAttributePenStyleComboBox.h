#ifndef GRAPHICATTRIBUTEPENSTYLECOMBOBOX_H
#define GRAPHICATTRIBUTEPENSTYLECOMBOBOX_H

#include <QComboBox>

class GraphicAttributePenStyleComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit GraphicAttributePenStyleComboBox(QWidget *parent = nullptr);
    virtual ~GraphicAttributePenStyleComboBox() = default;

    void setPenStyle(const Qt::PenStyle& change_pen_style);

private:
    void initEditPenStyleMap();

private:
    QMap<int, Qt::PenStyle> comboBoxIndex_to_penStyle;
    QMap<Qt::PenStyle, int> penStyle_to_comboBoxIndex;
    QMap<int, QIcon> comboBoxIndex_to_icon;

signals:
    void penStyleChanged(const Qt::PenStyle&);
};

#endif // GRAPHICATTRIBUTEPENSTYLECOMBOBOX_H
