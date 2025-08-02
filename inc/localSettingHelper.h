#ifndef LOCALSETTINGHELPER_H
#define LOCALSETTINGHELPER_H

#include <QSettings>

class LocalSettingHelper{
public:
    LocalSettingHelper(const QString &path);
    virtual ~LocalSettingHelper() = default;
    // 窗口位置
    void setWindowPos(const QPoint& pos);
    QPoint getWindowPos();

    // 窗口大小
    void setWindowSize(const QSize& size);
    QSize getWindowSize();

    // 画布大小
    void setCanvasSize(const QSize& size);
    QSize getCanvasSize();

    // 设置画布背景颜色
    void setCanvasBgColor(const QColor& color);
    QColor getCanvasBgColor();

    // 读取所有配置
    QMap<QString, QVariant> getAllSettings();

private:
    QSettings settings;
};

#endif // LOCALSETTINGHELPER_H
