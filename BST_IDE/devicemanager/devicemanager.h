#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "global.h"
#include "devicewathcer.h"
#include "physicalinc.h"
#include "logicinc.h"

#define D_PROGRESSDELAYTIME 1

/*
    0、通过MDP建立活动设备表
    1、通过查看DEV文件，确定UDISK、TFlash等热插拔设备是否存在（只检索活动设备表的设备）
    2、通过查看/dev目录，确定是否存在key或者LED等非热插拔设备（只检索活动设备表的设备）
    3、建立设备索引表，用于映射物理与逻辑设备，可加载、删除某个设备的逻辑协议或者整个设备
    4、生成系统信息（Devinfo中的信息，比如CPU、MEM占用率等，）
    5、生成设置菜单中MDP界面
    6、恢复出厂设置（包括格式化TFlash卡、对TFlash或者UDisk进行分区等）
    7、设备控制（包括LED开启、关闭、查询状态等）
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
    void Restart();  //>@重启应用程序
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
