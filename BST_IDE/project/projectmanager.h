#ifndef RPOJECTMANAGER_H
#define RPOJECTMANAGER_H

#include "global.h"
#include "upitem.h"
#include "treeitemclass.h"
#include "newfileform.h"
#include "modalwindow.h"
#include "toolbar.h"

//>@��DeviceToolBar��
class DeviceToolBar;

class ProjectManager : public UpQTreeWidget
{
    Q_OBJECT
public:
    ProjectManager(QWidget *parent = 0);
    ~ProjectManager();

    void CreateMenu();
    void CreateDeviceTool();

    void InitScan();
    void ReleaseScan();

    DeviceTreeItem* GetDevice(BaseDevice *pDev);
    DeviceTreeItem* GetDeviceFromAddr(QString pDevAddr);
    DeviceTreeItem* GetDeviceFromBuffer(QString pDevBuffer);

    DeviceTreeItem* AddDevice(BaseDevice::DEVICETYPE pType, QString pDevName,
                              QString pBufferPath=QString(), DEVICESTATE pState=S_OFFLINE);
    DeviceTreeItem* AddDevice(BaseDevice* pDev);
    bool DelDevice(DeviceTreeItem *&pTreeItem, bool pDelFile=false);

    void mousePressEvent(QMouseEvent *event);

public slots:
    void slot_ItemDoubleClicked(QTreeWidgetItem *item, int column);
    void slot_ItemPressed(QTreeWidgetItem *pItem, int column);

    void slot_DeviceAdded(QString dev);
    void slot_DeviceRemoved(QString dev);
    void slot_NetScan();   //>@ɨ�������豸

    void slot_Options();   //>@��������

    void slot_NewDevice();  //>@���һ�����ص��豸
    void slot_AddDevice();
    void slot_ClearDevice();
    void slot_RefreshDevice();
    void slot_SyncDevice();       //>@����ͬ��ģʽ����ʱ���Դ��豸�еĻ�������κβ�����������ͬ�����豸��
    void slot_DeleteDevice();
    void slot_ExploreDevice();    //>@
    void slot_DeviceOptions();    //>@

    void slot_SetRunningProject();
    void slot_NewProject();
    void slot_AddProject();
    void slot_OpenProject();
    void slot_CloseProject();
    void slot_DeleteProject();
    void slot_RegularProject();
    void slot_DownloadProject();
    void slot_UploadProject();    //>@�ϴ�ĳ������
    void slot_ExploreProject();
    void slot_ProjectOptions();

    void slot_NewFile();
    void slot_AddFile();
    void slot_OpenFile();
    void slot_CloseFile();
    void slot_DeletFile();
    void slot_ExploreFile();

    void slot_StartSync();
    void slot_StopSync();
    void slot_PauseSync();

    void slot_OpenUiDesignerTool();
    void slot_OpenMdpSetTool();
    void slot_OpenScheduleMapTool();
    void slot_OpenSoftwareTestTool();
    void slot_OpenLogQueryTool();

public:
    QMenu*                 m_BlankMenu;
    QMenu*                 m_DeviceMenu;
    QMenu*                 m_ProjectMenu;
    QMenu*                 m_FileMenu;

    DeviceToolBar         *m_DevToolBar;

    QTreeWidgetItem*       m_MenuOnItem;

    //>@Arpɨ��
    ToolUdp*               mToolUdp;
    HostNetDiscovery      *mHostNetDiscovery;
    QTimer                 mScanTimer;

    DeviceWatcher         *mStorageWatcher;
};




#endif // RPOJECTMANAGER_H
