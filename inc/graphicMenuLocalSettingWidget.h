#ifndef GRAPHICLOCALSETTINGWIDGET_H
#define GRAPHICLOCALSETTINGWIDGET_H

#include <QDialog>

class GraphicMenuLocalSettingWidget : public QDialog
{
    Q_OBJECT
public:
    explicit GraphicMenuLocalSettingWidget(QWidget *parent = nullptr);
    virtual ~GraphicMenuLocalSettingWidget() = default;
};

#endif // GRAPHICLOCALSETTINGWIDGET_H
