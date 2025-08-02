#include "graphicAttributeIntVaildator.h"

GraphicAttributeIntVaildator::GraphicAttributeIntVaildator(QObject *parent)
    : QIntValidator{parent}{
}

GraphicAttributeIntVaildator::GraphicAttributeIntVaildator(int bottom, int top, QObject *parent)
    :QIntValidator(bottom, top, parent){
}

void GraphicAttributeIntVaildator::setRange(int bottom, int top){
    QIntValidator::setRange(bottom, top);
}

//validate()是验证的过程，返回结果是State。每当输入有变化时调用此方法
//enum State {
//    Invalid,        //验证通不过
//    Intermediate,   //输入未完成，不确定是否能通过验证
//    Acceptable      //验证通过
//}
QValidator::State GraphicAttributeIntVaildator::validate(QString &s, int &n) const{
    if (s.isEmpty()){
        return QValidator::Intermediate;
    }
    int intValue = s.toInt();
    if(intValue > top() || intValue < bottom())
        return QIntValidator::Invalid;

    return QIntValidator::validate(s, n);
}

// 函数修复错误
void GraphicAttributeIntVaildator::fixup(QString &input) const{
    int intValue = input.toInt();
    if(intValue < bottom())
        input = QString("%1").arg(bottom());
    if(intValue > top())
        input = QString("%1").arg(top());
}
