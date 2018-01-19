#ifndef INTERFACE3GPP_H
#define INTERFACE3GPP_H

#include "interfacebase.h"
#include "logicelevator.h"

//>@ͨ��ErrorParse�������Ҵ���

class Interface3GPP : public InterfaceBase
{
    Q_OBJECT
public:
    Interface3GPP(QWidget *parent=0);

    quint32 InitSubAnalysis(QDomNodeList pParaList);
    quint32 InitSubGenerator(QDomNodeList pParaList);

    //>@���ݽ���ĵ�ǰ��������ָ��֡
    QByteArray GenerateFrame();

signals:

public slots:
    //>@����pArray����ʾ�ڽ�����
    void ParseFrame(QByteArray pArray);

    void UpdateAnalysis(QObject* pObject);
    void UpdateGenerator(QObject* pObject);
};

#endif // INTERFACE3GPP_H
