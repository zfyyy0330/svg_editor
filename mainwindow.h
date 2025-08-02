#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicAttributeDisplayWidget.h"
#include "graphicItemBarWidget.h"
#include "graphicMainCanvasWidget.h"
#include "graphicMenuLocalSettingWidget.h"
#include "graphicMenuFileMgOperation.h"
#include "httpRequestHelper.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    GrahpicMainCanvasWidget *graphic_main_canvas;
    GrahpicItemBarWidget *graphic_item_bar;
    GrahpicAttributeDisplayWidget *attribute_display;
    GraphicMenuLocalSettingWidget *global_setting_widget;

private:
    void initGlobalSetting();
    void initMenuConnect();
    void initWinLayout();
    void initSlots();
    void initHttpRequest();

    void parseBarHoverJsonToMap(const QString &jsonString);
    HttpRequestHelper* http_request_helper;
};
#endif // MAINWINDOW_H
