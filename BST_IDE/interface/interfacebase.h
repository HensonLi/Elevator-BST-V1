#ifndef INTERFACEBASE_H
#define INTERFACEBASE_H

#include "global.h"
#include "tool.h"
#include "errorparsebase.h"

class InterfaceBase : public Groupbox
{
    Q_OBJECT
public:
    typedef enum {ANALYSIS=0, GENERATOR} TYPE;
    explicit InterfaceBase(QWidget *parent = 0);

    QFrame* CreateLine();
    //>@��������ʼ���ؼ�
    QHBoxLayout* CreateButton(QString pName, QStringList pBtnListTag);
    QHBoxLayout* CreateTextEdit(QString pName, QString pValue);
    QHBoxLayout* CreateEnum(QString pName, QStringList pParaList, QString pSelectPara);
    QHBoxLayout* CreateBool(QString pName, QString pPara);
    QHBoxLayout* CreateSwitch(QString pName, QString pPara);
    QHBoxLayout* CreateCheck(QString pName, QStringList pParaList, QStringList pSelectParaList);
    QHBoxLayout* CreateNum(QString pName, int pValue, int pMinValue, int pMaxValue);

    bool InitAnalysis(QDomElement pElement);
    virtual quint32 InitSubAnalysis(QDomNodeList pParaList);  //>@���ض�̬�����Ĵ���ĸ߶�
    bool InitGenerator(QDomElement pElement);
    virtual quint32 InitSubGenerator(QDomNodeList pParaList);

    //>@���ݽ���ĵ�ǰ��������ָ��֡
    virtual QByteArray GenerateFrame() {return QByteArray();}

public slots:
    //>@����pArray����ʾ�ڽ�����
    virtual void ParseFrame(QByteArray pFrame) {}

    virtual void UpdateAnalysis(QObject* pObject) {}
    virtual void UpdateGenerator(QObject* pObject) {}

public:
    QHash<QString, ErrorParseBase*>  mErrorParses;
    TYPE                             mUIType;
};

#endif // INTERFACEBASE_H
