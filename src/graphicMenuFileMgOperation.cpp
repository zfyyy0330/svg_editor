#include "graphicMenuFileMgOperation.h"
#include <QFileDialog>
#include <QMessageBox>
#include "svgchartItemFactory.h"
#include "graphicCanvasScene.h"
#include "graphicCanvasView.h"
#include <QtSvg>

GraphicMenuFileMgOperation& GraphicMenuFileMgOperation::getInstance(){
    static GraphicMenuFileMgOperation file_operator;
    return file_operator;
}

void GraphicMenuFileMgOperation::setScene(GraphicCanvasScene* m_scene){
    this->m_scene = m_scene;
}

void GraphicMenuFileMgOperation::setView(GraphicCanvasView* m_view){
    this->m_view = m_view;
}

bool GraphicMenuFileMgOperation::createNewScene(){
    QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Question", "是否需要先进行保存？",
                                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        saveToSvg();
        if (m_scene) emit m_scene->loadNewEmptySvgScene();
        return true;
    } else if (reply == QMessageBox::No) {
        if (m_scene) emit m_scene->loadNewEmptySvgScene();
        return true;
    }

    return false;
}

void GraphicMenuFileMgOperation::saveToSvg(){
    if (m_scene){
        m_scene->clearSelection();
        QString file_name = QFileDialog::getSaveFileName(nullptr, "Save SVG File", "", "SVG Files (*.svg);;All Files (*)");
        if (file_name.isEmpty()) {
            return; 
        }

        QSvgGenerator generator;
        generator.setFileName(file_name);
        generator.setSize(QSize(m_scene->width(), m_scene->height()));
        generator.setViewBox(m_scene->sceneRect());
        generator.setTitle(tr("SVG Example"));
        generator.setDescription(tr("An SVG drawing created by the SvgEditor."));
        QPainter painter(&generator);
        painter.begin(&generator);
        m_scene->render(&painter);
        painter.end();
    }
}

void GraphicMenuFileMgOperation::loadFromSvg(){
    if (!m_scene->items().isEmpty()) {
        if (!createNewScene()) return;
    }
    QString file_name = QFileDialog::getOpenFileName(nullptr, "Open SVG File", "", "SVG Files (*.svg)");
    if (file_name.isEmpty()) {
        return; 
    }

    parseSvgFile(file_name);
    qDebug() << "elements size is " << base_elements.size();
//    for (const auto& it : base_elements){
//        dumpSvgElement(it);
//    }

    if (!base_elements.isEmpty() && m_scene){
        QColor scene_color = QColor(base_elements[0].attributes["fill"]);
        emit m_scene->loadSvgSceneSizeAndColorChanged(QSize(base_elements[0].attributes["width"].toDouble(), base_elements[0].attributes["height"].toDouble()), scene_color);

        QList<SvgChartItemBaseProperty> all_elements = loadSvgElements();
        for(const auto &ele : all_elements){
            // TODO: 这里单独提出来是因为用deserializeProperty创建drawCustomLineRoute后移动时有一个bug
            //       单独创建后无bug
            if (ele.cur_draw_item_type == CurrentDrawItemType::drawCustomLineRoute){
                SvgChartCustomLineRoute* new_qgraphic_item = new SvgChartCustomLineRoute();
                if (new_qgraphic_item && m_scene){
                    new_qgraphic_item->setDrawBoundPen(ele.cur_draw_pen);
                    new_qgraphic_item->setDrawFillBrush(ele.cur_draw_brush);
                    new_qgraphic_item->setPoint(ele.points);
                    m_scene->addItemWithCursorManager(new_qgraphic_item);
                }
            }
            else{
                SvgChartItemBase* new_qgraphic_item = SvgChartItemBase::deserializeProperty(ele);
                if (new_qgraphic_item && m_scene){
                    m_scene->addItemWithCursorManager(new_qgraphic_item);
                }
            }
        }
    }
}

void GraphicMenuFileMgOperation::parseSvgFile(const QString& filePath) {
    base_elements.clear();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << filePath;
    }

    QXmlStreamReader xml(&file);
    bool inG = false;
    SvgElement total_polyline_element;

    QStack<SvgElement> group_stack;
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement){
            if (xml.name() == "g"){
                inG = true;
                SvgElement element;
                element.type = xml.name().toString();
                foreach(const QXmlStreamAttribute & attr, xml.attributes()){
                    element.attributes.insert(attr.name().toString(), attr.value().toString());
                }
                group_stack.push_back(element);
            }
            else if (inG && (xml.name() == "rect" || xml.name() == "ellipse" || xml.name() == "line" || xml.name() == "path" || xml.name() == "text")){
                SvgElement element;
                if (!group_stack.empty()){
                    element = group_stack.top(); // 复制抬头的属性
                }

                element.type = xml.name().toString();
                 // 添加多余的属性
                foreach(const QXmlStreamAttribute & attr, xml.attributes()){
                    element.attributes[attr.name().toString()] =  attr.value().toString();
                }
                element.text = xml.readElementText();
                base_elements.push_back(element);
            }
            else if (inG && xml.name() == "polyline"){
                if (!group_stack.empty()){
                    total_polyline_element.attributes = group_stack.top().attributes;
                }
                total_polyline_element.type = xml.name().toString();
                QString pointsStr = xml.attributes().value("points").toString();
                QStringList pointPairs = pointsStr.split(' ');

                if (pointPairs.size() >= 2){
                    if (total_polyline_element.points.isEmpty()){
                        for (const QString& pair : pointPairs) {
                            QStringList xy = pair.split(',');
                            if (xy.size() == 2) {
                                qreal x = xy[0].toDouble();
                                qreal y = xy[1].toDouble();
                                total_polyline_element.points.append(QPointF(x, y));
                            }
                        }
                    }
                    else{
                        QStringList xy = pointPairs[1].split(',');
                        if (xy.size() == 2) {
                            qreal x = xy[0].toDouble();
                            qreal y = xy[1].toDouble();
                            total_polyline_element.points.append(QPointF(x, y));
                        }
                    }
                }
                total_polyline_element.text = xml.readElementText();
            }
        }
        else if (token == QXmlStreamReader::EndElement){
            if (xml.name() == "g"){
                inG = false;
                if (!total_polyline_element.points.isEmpty()) {
                    base_elements.push_back(total_polyline_element);
                    total_polyline_element.points.clear();
                }
                //抬头属性出栈
                group_stack.pop();
            }
        }
    }
    file.close();
    return;
}

void GraphicMenuFileMgOperation::dumpSvgElement(const SvgElement& element){
    qDebug() << "[DEBUG] [type] >> " << element.type;
    qDebug() << "[DEBUG] [text] >> " << element.text;
    qDebug() << "[DEBUG] [attributes] >> ";
    for (auto it = element.attributes.constBegin(); it != element.attributes.constEnd(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }
    qDebug() << "[DEBUG] [Points]:";
    for (const auto& point : element.points){
        qDebug() << point;
    }
    qDebug() << "[DEBUG] >>>> \n";
}

Qt::PenStyle GraphicMenuFileMgOperation::getPenStyleFromSvg(const QString& attributes) {
    if (attributes.isEmpty()) {
        return Qt::SolidLine;
    }

    QStringList dash_values = attributes.split(',');
    QVector<qreal> dash_pattern;
    for (const QString& value : dash_values) {
        bool ok;
        qreal number = value.toDouble(&ok);
        if (ok) {
            dash_pattern.append(number);
        }
    }

    if (dash_pattern.size() == 2 && dash_pattern[0] == 8 && dash_pattern[1] == 4) {
        return Qt::DashLine;
    } else if (dash_pattern.size() == 2 && dash_pattern[0] == 2 && dash_pattern[1] == 4) {
        return Qt::DotLine;
    } else if (dash_pattern.size() == 4 && dash_pattern[0] == 8 && dash_pattern[1] == 4 && dash_pattern[2] == 2 && dash_pattern[3] == 4) {
        return Qt::DashDotLine;
    } else if (dash_pattern.size() == 6 && dash_pattern[0] == 8 && dash_pattern[1] == 4 && dash_pattern[2] == 2 && dash_pattern[3] == 4 && dash_pattern[4] == 2 && dash_pattern[5] == 4) {
        return Qt::DashDotDotLine;
    }
    return Qt::CustomDashLine;
}

QList<QPointF> GraphicMenuFileMgOperation::getPolygonPathFromSvg(const QString& pathData) {
    if (pathData.isEmpty()){
        return {};
    }

    QList<QPointF> points;
    QStringList commands = pathData.split(' ');
    for(int i = 0 ; i < commands.size() - 1 ; ++i){
        if (commands[i].isEmpty()) continue;

        QStringList values = commands[i].split(',');
        if (values.size() != 2) continue;
        qreal x = values[0].right(values[0].size() - 1).toDouble();
        qreal y = values[1].toDouble();
        points.append(QPointF(x, y));
    }
    return points;
}

CurrentDrawItemType GraphicMenuFileMgOperation::checkPolygonType(const QList<QPointF>& points){
    if (points.size() == 5) {
        return CurrentDrawItemType::drawPentagon;
    } else if (points.size() == 6) {
        return CurrentDrawItemType::drawHexagon;
    } else if (points.size() == 10) {
        return CurrentDrawItemType::drawStars;
    } else {
        return CurrentDrawItemType::None;
    }
}

QList<SvgChartItemBaseProperty> GraphicMenuFileMgOperation::loadSvgElements(){
    QList<SvgChartItemBaseProperty> ret;

    for(int i = 1 ; i < base_elements.size() ; ++i){
        SvgChartItemBaseProperty cur_element_property;
        SvgElement cur_element = base_elements[i];

        QString type = cur_element.type;
        QString text = cur_element.text;
        QString shape_color = cur_element.attributes.value("fill");
        QString line_color = cur_element.attributes.value("stroke");
        int line_width = cur_element.attributes.value("stroke-width").toInt();
        Qt::PenStyle line_style = getPenStyleFromSvg(cur_element.attributes.value("stroke-dasharray"));

        // QString stroke_opacity = element.attributes.value("stroke-opacity"); // 边框透明度
        // QString font = element.attributes.value("font-family"); // 字体
        // int font_size = element.attributes.value("font-size").toInt(); // 字号
        // int font_weight = element.attributes.value("font-weight").toInt(); // 字重

        qreal x = cur_element.attributes.value("x").toDouble();
        qreal y = cur_element.attributes.value("y").toDouble();
        int width = cur_element.attributes.value("width").toInt();
        int height = cur_element.attributes.value("height").toInt();

        qreal cx = cur_element.attributes.value("cx").toDouble();
        qreal cy = cur_element.attributes.value("cy").toDouble();
        qreal rx = cur_element.attributes.value("rx").toDouble();
        qreal ry = cur_element.attributes.value("ry").toDouble();
        QString path_d = cur_element.attributes.value("d");

        if (type == "rect"){
            cur_element_property.cur_draw_item_type = CurrentDrawItemType::drawRect;
            cur_element_property.cur_del_pos = QRectF(QPointF(x, y), QSize(width, height));
            qDebug()<<cur_element_property.cur_del_pos;
        }
        else if (type == "ellipse"){
            cur_element_property.cur_draw_item_type = CurrentDrawItemType::drawCircle;
            qreal e_x = cx - rx;
            qreal e_y = cy - ry;
            qreal e_width = 2 * rx;
            qreal e_height = 2 * ry;
            cur_element_property.cur_del_pos = QRectF(QPointF(e_x, e_y), QSize(e_width, e_height));
        }
        else if (type == "path"){
            QList<QPointF> cur_draw_path = getPolygonPathFromSvg(path_d);
            if (!cur_draw_path.isEmpty()){
                cur_element_property.cur_draw_item_type = checkPolygonType(cur_draw_path);
                cur_element_property.cur_del_pos = SvgChartItemBase::getCustomDictBoundingRect(cur_draw_path);
            }
        }
        else if (type == "text"){
            cur_element_property.cur_draw_item_type = CurrentDrawItemType::drawText;
            cur_element_property.cur_text = text;
            // 因为这里文本使用的Pen: line_color
            line_color = shape_color;
            cur_element_property.cur_del_pos = QRectF(QPointF(x, y), QSize());
        }
        else if (type == "polyline"){
            if (cur_element.points.size() == 2){
                cur_element_property.cur_draw_item_type = CurrentDrawItemType::drawLine;
                qreal dx = cur_element.points[1].x() - cur_element.points[0].x();
                qreal dy = cur_element.points[1].y() - cur_element.points[0].y();
                cur_element_property.cur_line_state = SvgChartItemLine::calLineStateBySlop(dx, dy);
                cur_element_property.cur_del_pos = QRectF(cur_element.points[0], cur_element.points[1]).normalized();
            }
            else{
                cur_element_property.cur_draw_item_type = CurrentDrawItemType::drawCustomLineRoute;
                cur_element_property.points = cur_element.points;
            }
        }

        // 共有属性：cur_draw_pen，cur_draw_brush
        QPen cur_bound_pen;
        cur_bound_pen.setWidth(line_width);
        QColor cur_pen_color = QColor(line_color);
        if (cur_pen_color.isValid()){
            cur_bound_pen.setColor(cur_pen_color);
        }
        cur_bound_pen.setStyle(line_style);
        QBrush cur_fill_brush;
        cur_fill_brush.setStyle(Qt::SolidPattern);
        QColor cur_fill_color = QColor(shape_color);
        if (cur_fill_color.isValid()){
            cur_fill_brush.setColor(cur_fill_color);
        }

        cur_element_property.cur_draw_pen = cur_bound_pen;
        cur_element_property.cur_draw_brush = cur_fill_brush;
        ret.push_back(cur_element_property);
    }

    return ret;
}
QList<GraphicMenuFileMgOperation::SvgElement> GraphicMenuFileMgOperation::getSvgElements() const{
    return this->base_elements;
}

void GraphicMenuFileMgOperation::exportPng(ExportPNGMode export_mode){
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PNG File", "", "PNG Files (*.png);;All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QPixmap cur_pix;
    if (export_mode == ExportPNGMode::View && m_view){
        QPixmap pixmap(m_view->width(), m_view->height());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        m_view->render(&painter);
        painter.end();

        cur_pix = pixmap;
    }
    else if(export_mode == ExportPNGMode::Scene && m_scene){
        QPixmap pixmap(m_scene->width(), m_scene->height());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        m_scene->render(&painter);
        painter.end();

        cur_pix = pixmap;
    }

    if (cur_pix.save(fileName, "PNG")) {
        QMessageBox::information(nullptr, "Success", "成功导出到" + fileName);
    }
    else {
        QMessageBox::warning(nullptr, "Error", "导出失败！");
    }
}
