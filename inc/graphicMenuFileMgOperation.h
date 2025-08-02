#ifndef GRAPHICMENUFILEMGOPERATION_H
#define GRAPHICMENUFILEMGOPERATION_H

#include <QObject>
#include <QGraphicsView>
#include "svgGlobalConfig.h"

struct SvgChartItemBaseProperty;
class GraphicCanvasScene;
class GraphicCanvasView;

enum class ExportPNGMode{
    View,
    Scene
};

class GraphicMenuFileMgOperation : public QObject
{
    Q_OBJECT
public:
    struct SvgElement {
        QString type;
        QString text;
        QHash<QString, QString> attributes;
        QList<QPointF> points;
    };
    static GraphicMenuFileMgOperation& getInstance();
    virtual ~GraphicMenuFileMgOperation() = default;

    void setScene(GraphicCanvasScene* m_scene);
    void setView(GraphicCanvasView* m_view);

    bool createNewScene();
    void saveToSvg();
    void loadFromSvg();
    void exportPng(ExportPNGMode export_mode = ExportPNGMode::View);

    void parseSvgFile(const QString& filePath);
    QList<SvgChartItemBaseProperty> loadSvgElements();
    QList<SvgElement> getSvgElements() const;
    void dumpSvgElement(const SvgElement& element);

private:
    explicit GraphicMenuFileMgOperation() = default;
    GraphicMenuFileMgOperation(const GraphicMenuFileMgOperation&) = delete;
    void operator=(const GraphicMenuFileMgOperation&) = delete;

    Qt::PenStyle getPenStyleFromSvg(const QString& attributes);
    QList<QPointF> getPolygonPathFromSvg(const QString& pathData);
    CurrentDrawItemType checkPolygonType(const QList<QPointF>& points);

    GraphicCanvasScene* m_scene = nullptr;
    GraphicCanvasView* m_view = nullptr;
private:
    QList<SvgElement> base_elements;
};

#endif // GRAPHICMENUFILEMGOPERATION_H
