#ifndef GRAHPICITEMBARWIDGET_H
#define GRAHPICITEMBARWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMap>
#include "svgGlobalConfig.h"

class GraphicItemBarPushButton;

class GrahpicItemBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GrahpicItemBarWidget(QWidget *parent = nullptr);
    virtual ~GrahpicItemBarWidget() = default;

private:
    void initBar2IconMap();
    void initPushButton();
    void changeButtonIconColor();

private:
    GraphicItemBarPushButton *current_selected_button = nullptr;
    QMap<CurrentBarState, QIcon> current_state_to_icon;

signals:
    void onBarChangeState(CurrentBarState state);
    void replyBarbuttonHoverInfo();
};

#endif // GRAHPICITEMBARWIDGET_H
