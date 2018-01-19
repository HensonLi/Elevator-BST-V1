#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "global.h"
#include "devicewathcer.h"
#include "physicalinc.h"
#include "logicinc.h"

#define D_PROGRESSDELAYTIME 1

/*
    0��ͨ��MDP������豸��
    1��ͨ���鿴DEV�ļ���ȷ��UDISK��TFlash���Ȳ���豸�Ƿ���ڣ�ֻ������豸����豸��
    2��ͨ���鿴/devĿ¼��ȷ���Ƿ����key����LED�ȷ��Ȳ���豸��ֻ������豸����豸��
    3�������豸����������ӳ���������߼��豸���ɼ��ء�ɾ��ĳ���豸���߼�Э����������豸
    4������ϵͳ��Ϣ��Devinfo�е���Ϣ������CPU��MEMռ���ʵȣ���
    5���������ò˵���MDP����
    6���ָ��������ã�������ʽ��TFlash������TFlash����UDisk���з����ȣ�
    7���豸���ƣ�����LED�������رա���ѯ״̬�ȣ�
*/

class DeviceManager : public QWidget
{
    Q_OBJECT
public:
    DeviceManager(QRect pRect, QWidget *parent=0);
    ~DeviceManager();

    bool OpenBootMdp(QString pPath);
    void CloseBootMdp();
    bool SaveBootMdp();

    bool OpenDeviceWatcher();
    void CloseDeviceWatcher();

    void ReleaseDevices();

    bool AddPHDevice(PH_Type pPHType, QDomElement pPHElement, bool pCover=false);
    bool DelPHDevice(PH_Type pPHType);

    bool AddLGDevice(LG_Type pLGType, QDomElement pPHElement, bool pCover=false);
    bool DelLGDevice(LG_Type pLGType);

    PhysicalDevice* GetPHDev(PH_Type pPHType);
    LogicDevice*    GetLGDev(LG_Type pLGType);

    QPixmap GetBufferImage();
    QPixmap GetProgressImage();
    void paintEvent(QPaintEvent *e);

signals:
    void sDeviceInitialized();

public slots:
    void InitDevices();
    void Restart();  //>@����Ӧ�ó���
    void UpdateProgress(QString pMsg);
    void UpdateProgress(QString pMsg, int pValue);

    void slot_DeviceAdded(QString dev);
    void slot_DeviceRemoved(QString dev);
    void keyPressEvent(QKeyEvent *);

public:
    QHash<PH_Type, PhysicalDevice*> m_PHDevList;
    QHash<LG_Type, LogicDevice*>    m_LGDevList;
    DeviceWatcher                  *m_DeviceWathcer;

    mdpManager            *mMdpManager;

    bool                   m_PaintEnable;

    typedef struct{
        int                mProgressValue;
        QRect              mProgressRect;
        //
        int                mPrgTextFontSize;
        QFont              mPrgTextFont;
        //
        QStringList        mMessageQueue;
        QRect              mMessageRect;
        QFont              mMsgTextFont;
        int                mMaxMsgRow;
    } MSGPRG_INFO;

    MSGPRG_INFO     m_MsgPrgInfo;
    QPixmap         m_BufferPixmap, m_BgPixmap;
};

#endif // DEVICEMANAGER_H
