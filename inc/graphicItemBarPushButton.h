#ifndef GRAPHICITEMBARPUSHBUTTOM_H
#define GRAPHICITEMBARPUSHBUTTOM_H

#include <QPushButton>
#include <QMap>
#include "svgGlobalConfig.h"
#include <QJsonObject>

class GraphicItemBarPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit GraphicItemBarPushButton(CurrentBarState cur_button_state, const QIcon& current_button_icon, QWidget *parent = nullptr);
    virtual ~GraphicItemBarPushButton() = default;

    CurrentBarState getCurrentButtonState();
    bool eventFilter(QObject *obj, QEvent *event) override;
    void setHoverText(const QString& cur_text);
    void setHoverTitle(const QString& cur_title);
    void changeSelectedIconOriginColor();
    void changeSelectedIconColor2Blue();

private:
    QString hover_text;
    QString hover_title;
    CurrentBarState cur_button_state;
    QIcon cur_button_icon;
};

#endif // GRAPHICITEMBARPUSHBUTTOM_H
