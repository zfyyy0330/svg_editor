#ifndef SVGCHARTITEMTEXTBASE_H
#define SVGCHARTITEMTEXTBASE_H

#include <QGraphicsTextItem>
#include "svgGlobalConfig.h"

class SvgChartItemTextBase : public QGraphicsTextItem{
    Q_OBJECT
public:
    explicit SvgChartItemTextBase(QGraphicsItem *parent = nullptr);
    explicit SvgChartItemTextBase(const QString &text, QGraphicsItem *parent = nullptr);
    virtual ~SvgChartItemTextBase() = default;
public:
    QString m_store_str = "";
    bool is_edit_mode = false;
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
signals:
    void itemHoverStateChanged(DragDirection);
};

#endif // SVGCHARTITEMTEXTBASE_H
