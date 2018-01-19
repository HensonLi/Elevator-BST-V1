#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include "globalfun.h"
#include "tool.h"

#ifdef IDE
    class DeviceScene;
    class DeviceItem;
#elif DISPLAY
    class DeviceManager;
#endif

class IODeviceBase : public QIODevice
{
    Q_OBJECT
public:
    IODeviceBase(QObject *parent = 0);
    ~IODeviceBase();

    virtual bool isSequential() const { return true; }
    virtual bool open(OpenMode mode){ return true; }
    virtual void close(){}

    virtual qint64 pos() const  {return 0;}
    virtual qint64 size() const  {return 0;}
    virtual bool seek(qint64 pos)  {return true;}
    virtual bool atEnd() const  {return true;}
    virtual bool reset()  {return true;}

    virtual qint64 bytesAvailable() const  {return 0;}
    virtual qint64 bytesToWrite() const  {return 0;}

    virtual bool canReadLine() const  {return true;}

    virtual qint64 write(QByteArray data, QString pAddr) {return 0;} //>@��ĳһ��ַ���豸��������

    virtual bool waitForReadyRead(int msecs)  {return true;}
    virtual bool waitForBytesWritten(int msecs)  {return true;}

    virtual qint64 readData(char *data, qint64 maxlen)  {return 0;}
    virtual qint64 readLineData(char *data, qint64 maxlen)  {return 0;}
    virtual qint64 writeData(const char *data, qint64 len)  {return 0;}

    virtual bool isOpen()const{return (QIODevice::isOpen());}
    virtual inline qint64 write(const QByteArray &data){return (QIODevice::write(data));}
    virtual QByteArray readAll(){return (QIODevice::readAll());}

    QFrame* CreateLine();

    //>@��������ʼ���ؼ�
    QHBoxLayout* CreateButton(QString pName, QStringList pBtnListTag);
    //>@���Ŀؼ����ؼ����Զ������źŸ���XML��
    //>@ִ�����̣����Ŀؼ����ؼ��Զ��ж��߼����Ƿ���Ը��ģ�������������XML�����򲻸���XML���ָ��ؼ�Ϊ��һ��״̬����ʱ�������ٴν����Զ��ж��߼����Ƿ���Ը��ģ���������ʲô�����ˡ�
    //bool SetButton(ButtonList *pComponent, QDomElement &pElement);
    //>@���ؼ�����ʱ��ͨ���������������£�������XML��
    bool ModifyButton(ButtonList *pComponent, QDomElement &pElement, QStringList &pDstValue, QStringList pNewValue);

    QHBoxLayout* CreateTextEdit(QString pName, QString pValue);
    bool ModifyTextEdit(TextEdit *pComponent, QDomElement &pElement, QString &pDstValue, QString pNewValue);

    QHBoxLayout* CreateEnum(QString pName, QStringList pParaList, QString pSelectPara);
    bool ModifyEnum(ComboBox *pComponent, QDomElement &pElement, QString &pDstValue, QString pNewValue);

    QHBoxLayout* CreateBool(QString pName, QString pPara);
    bool ModifyBool(Switch *pComponent, QDomElement &pElement, bool &pDstValue, bool pNewValue);

    QHBoxLayout* CreateSwitch(QString pName, QString pPara);
    bool ModifySwitch(Switch *pComponent, QDomElement &pElement, bool &pDstValue, bool pNewValue);

    QHBoxLayout* CreateENetAddrEdit(QString pName, QString pIP);
    bool ModifyENetAddrEdit(ENetAddrEdit *pComponent, QDomElement &pElement, ENET_ADDR &pDstValue, ENET_ADDR pNewValue);

    QHBoxLayout* CreateDateTimeEdit(QString pName, QDateTime pDateTime);
    bool ModifyDateTimeEdit(DateTimeEdit *pComponent, QDomElement &pElement, QDateTime &pDstValue, QDateTime pNewValue);

    QHBoxLayout* CreateCheck(QString pName, QStringList pParaList, QStringList pSelectParaList);
    bool ModifyCheck(CheckList *pComponent, QDomElement &pElement, QStringList &pDstValue, QStringList pNewValue);

    QHBoxLayout* CreateNum(QString pName, int pValue, int pMinValue, int pMaxValue);
    bool ModifyNum(SpinEdit *pComponent, QDomElement &pElement, int &pDstValue, int pNewValue, int pMinValue, int pMaxValue);

    virtual quint32 InitPara() {return 0;}  //>@���ش���Ĳ�������
    bool SetPara(QDomElement &pElement, xmlParse *pParse); //>@�޸�pElementΪ���Element
    bool GetPara(QDomElement &pElement, xmlParse *pParse);
    virtual bool SetValue(QString pValue) {return false;}

    bool SetElementText(QDomElement pElement, QString pValue);
    void UpdateProgress(QString pMsg, int pValue);

    //>@��ȡ���ڵ��е���Ҫ��Ϣ
    virtual QString GetInformation() { return QString("%1").arg(mDevName); }

public slots:
    virtual void UpdatePara(QObject* pObject) {}
    virtual void slot_ProcessConnect() {}

public:
#ifdef IDE
    DeviceScene                 *m_DeviceScene;
    DeviceItem                  *m_DeviceItem;
    void DeleteDevItem();
#elif DISPLAY
    DeviceManager               *m_DevManager;
#endif
    QDomElement                  m_ParaElement;
    Groupbox                    *m_SettingGroup;
    QHash<QString, QObject*>     mParaItems;       //>@���������Ӧ�Ŀؼ�

    QString                      mDevName;
};

#endif // PHYSICALBASE_H
