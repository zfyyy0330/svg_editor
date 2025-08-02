#include "localSettingHelper.h"
#include <QColor>
#include <QSize>
#include <QPoint>

LocalSettingHelper::LocalSettingHelper(const QString &path){
    settings.setPath(QSettings::NativeFormat, QSettings::UserScope, path);
}

// 窗口位置
void LocalSettingHelper::setWindowPos(const QPoint& pos){
    settings.setValue("window/pos", pos);
}
QPoint LocalSettingHelper::getWindowPos(){
    return settings.value("window/pos", QPoint(100, 100)).toPoint();
}

// 窗口大小
void LocalSettingHelper::setWindowSize(const QSize& size){
    settings.setValue("window/size", size);
}
QSize LocalSettingHelper::getWindowSize(){
    return settings.value("window/size", QSize(1000, 1000)).toSize();
}

// 画布大小
void LocalSettingHelper::setCanvasSize(const QSize& size){
    settings.setValue("canvas/size", size);
}
QSize LocalSettingHelper::getCanvasSize(){
    return settings.value("canvas/size", QSize(3000, 3000)).toSize();
}

// 设置画布背景颜色
void LocalSettingHelper::setCanvasBgColor(const QColor& color){
    settings.setValue("canvas/bgColor", color.name());
}
QColor LocalSettingHelper::getCanvasBgColor(){
    return QColor(settings.value("canvas/bgColor", "#FFFFFF").toString());
}

// 读取所有配置
QMap<QString, QVariant> LocalSettingHelper::getAllSettings(){
    QMap<QString, QVariant> allSettings;
    allSettings["window/pos"] = getWindowPos();
    allSettings["window/size"] = getWindowSize();
    allSettings["canvas/size"] = getCanvasSize();
    allSettings["canvas/bgColor"] = getCanvasBgColor();
    return allSettings;
}
