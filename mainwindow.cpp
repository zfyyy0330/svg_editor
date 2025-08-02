#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "localSettingHelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);

    setWindowTitle("SVG编辑器");
    initGlobalSetting();
    initHttpRequest();

    QSize origin_size = QSize(GlobalCommonSetting::getInstance().getSceneWidth(), GlobalCommonSetting::getInstance().getSceneHeight());
    QColor origin_color = GlobalCommonSetting::getInstance().getSceneColor();
    graphic_main_canvas = new GrahpicMainCanvasWidget(origin_size, origin_color, this);
    graphic_item_bar = new GrahpicItemBarWidget(this);
    attribute_display = new GrahpicAttributeDisplayWidget(origin_size, origin_color, this);
    global_setting_widget = new GraphicMenuLocalSettingWidget(this);
    global_setting_widget->hide();

    initWinLayout();
    initSlots();
    initMenuConnect();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::initGlobalSetting(){
    LocalSettingHelper local_setting("HKEY_CURRENT_USER\\SOFTWARE");

    GlobalCommonSetting::getInstance().setSceneWidth(local_setting.getCanvasSize().width());
    GlobalCommonSetting::getInstance().setSceneHeight(local_setting.getCanvasSize().height());
    GlobalCommonSetting::getInstance().setSceneColor(local_setting.getCanvasBgColor());
}

void MainWindow::initHttpRequest(){
    http_request_helper = new HttpRequestHelper(this);
    http_request_helper->sendGetRequest(QUrl("https://m1.apifoxmock.com/m1/6237106-5930859-default/app/buttontips"));
    connect(http_request_helper, &HttpRequestHelper::responseReceived, this, &MainWindow::parseBarHoverJsonToMap);
}

void MainWindow::parseBarHoverJsonToMap(const QString &jsonString) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonDoc.isNull() ||!jsonDoc.isObject()) {
        return ;
    }
    QJsonObject jsonObject = jsonDoc.object();
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        QString key = it.key();
        CurrentBarState state = GlobalCommonSetting::mapBarKeyToEnum(key);
        if (state != CurrentBarState::None) {
            QJsonObject valueObj = it.value().toObject();
            QString title = valueObj["title"].toString();
            QString text = valueObj["text"].toString();
            GlobalCommonSetting::getInstance().addBarButtonHoverInfo(state, qMakePair(title, text));
        }
    }

    emit graphic_item_bar->replyBarbuttonHoverInfo();
}

void MainWindow::initMenuConnect(){
    QMenu *file_menu = menuBar()->addMenu("文件");
    QAction *create_new_scene = new QAction("新建文件 \t Ctrl+N", this);
    file_menu->addAction(create_new_scene);
    QAction *load_svg = new QAction("打开SVG \t Ctrl+O", this);
    file_menu->addAction(load_svg);
    QAction *save_svg = new QAction("保存 \t Ctrl+S", this);
    file_menu->addAction(save_svg);
    QAction *export_png = new QAction("导出PNG \t Ctrl+E", this);
    file_menu->addAction(export_png);

    QMenu *setting_menu = menuBar()->addMenu("配置");
    QAction *local_setting = new QAction("本地设置", this);
    setting_menu->addAction(local_setting);

    connect(create_new_scene, &QAction::triggered, this, [&](){
        GraphicMenuFileMgOperation::getInstance().createNewScene();
    });
    connect(load_svg, &QAction::triggered, this, [&](){
        GraphicMenuFileMgOperation::getInstance().loadFromSvg();
    });
    connect(save_svg, &QAction::triggered, this, [&](){
        GraphicMenuFileMgOperation::getInstance().saveToSvg();
    });
    connect(export_png, &QAction::triggered, this, [&](){
        GraphicMenuFileMgOperation::getInstance().exportPng();
    });
    connect(local_setting, &QAction::triggered, this, [&](){
        global_setting_widget->show();
    });
}

void MainWindow::initWinLayout(){
    QHBoxLayout *main_window = new QHBoxLayout(this->centralWidget());
    main_window->setContentsMargins(0, 0, 0, 0);
    main_window->addWidget(graphic_item_bar);
    main_window->addWidget(graphic_main_canvas);
    main_window->addWidget(attribute_display);
}

void MainWindow::initSlots(){
    connect(graphic_item_bar, &GrahpicItemBarWidget::onBarChangeState, graphic_main_canvas, &GrahpicMainCanvasWidget::replyBarStateChange);

    connect(attribute_display, &GrahpicAttributeDisplayWidget::changeMainCanvasSceneSize, graphic_main_canvas, &GrahpicMainCanvasWidget::setMainCanvasSceneSize);
    connect(attribute_display, &GrahpicAttributeDisplayWidget::changeMainCanvasSceneWidth, graphic_main_canvas, &GrahpicMainCanvasWidget::setMainCanvasSceneWidth);
    connect(attribute_display, &GrahpicAttributeDisplayWidget::changeMainCanvasSceneHeight, graphic_main_canvas, &GrahpicMainCanvasWidget::setMainCanvasSceneHeight);
    connect(attribute_display, &GrahpicAttributeDisplayWidget::changeMainCanvasSceneColor, graphic_main_canvas, &GrahpicMainCanvasWidget::setMainCanvasSceneColor);

    connect(graphic_main_canvas, &GrahpicMainCanvasWidget::sendSceneWidthChanged, attribute_display, &GrahpicAttributeDisplayWidget::replyMainCanvasSceneWidth);
    connect(graphic_main_canvas, &GrahpicMainCanvasWidget::sendSceneHeightChanged, attribute_display, &GrahpicAttributeDisplayWidget::replyMainCanvasSceneHeight);
    connect(graphic_main_canvas, &GrahpicMainCanvasWidget::sendSceneColorChanged, attribute_display, &GrahpicAttributeDisplayWidget::replyMainCanvasSceneColor);
    connect(graphic_main_canvas, &GrahpicMainCanvasWidget::replyChangeSelectedItem, attribute_display, &GrahpicAttributeDisplayWidget::replyChangeSelectedItem);
    connect(graphic_main_canvas, &GrahpicMainCanvasWidget::replyViewZoomProportion, attribute_display, &GrahpicAttributeDisplayWidget::replyViewZoomProportion);
}
