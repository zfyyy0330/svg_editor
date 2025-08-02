#ifndef GRAPHICATTRIBUTEINTVAILDATOR_H
#define GRAPHICATTRIBUTEINTVAILDATOR_H

#include <QIntValidator>

class GraphicAttributeIntVaildator : public QIntValidator
{
    Q_OBJECT
public:
    explicit GraphicAttributeIntVaildator(QObject *parent = nullptr);
    explicit GraphicAttributeIntVaildator(int bottom, int top, QObject *parent);
    virtual ~GraphicAttributeIntVaildator() = default;

protected:
    void setRange(int bottom, int top) override;
    virtual State validate(QString &s, int &n) const override;
    virtual void fixup(QString &input) const override;
};

#endif // GRAPHICATTRIBUTEINTVAILDATOR_H
