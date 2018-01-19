#ifndef DEVICELISTFORM_H
#define DEVICELISTFORM_H

#include "logicnetmanager.h"
#include "logicnetdiscovery.h"
#include "devicemessagedlg.h"

#include <QWidget>
#include <QMultiHash>
#include <QThread>
#include <QReadWriteLock>

namespace Ui {
class devicelistform;
}

/*
    ɨ�赽�Ľڵ��Ϊoffline״̬��
*/
class devicelistform : public QWidget
{
    Q_OBJECT
public:
    explicit devicelistform(QWidget *parent = 0);
    ~devicelistform();

    void ReleaseAct();
    void InitAct();

    void ReleaseTreeWidget();
    void InitTreeWidget();

    void ReleaseNetCmdline();
    void InitNetCmdline();

    void EnumerateNet();
    void EnumerateUSB();
    void EnumerateCOM();

    QStringList GetConnectedNets();
    NetInfo* GetCurNetInfo();
    NetInfo* GetNetInfo(QString pIP);
    NetInfo* AddNetInfo(QString pIP);
    QTreeWidgetItem* AddNetTreeItem(NetInfo *pNetInfo);
    QTreeWidgetItem* GetTreeItem(NetInfo *pNetInfo);
    void SetTreeItemBusState(QTreeWidgetItem *pItem, BusState pState);
    BusState GetTreeItemBusState(QTreeWidgetItem *pItem);

signals:
    void sConnstate(NetInfo *pNet);

public slots:
    void slot_NetConn();
    void slot_NetDisconn();

    //>@������ѯ���硢U���Լ������豸
    void slot_Scan();
    void slot_ConnStateChanged();

    void slot_ModifyTempPath();
    void slot_Broadcast();
    void slot_SyncTime(){}
    void slot_Message();
    void Slot_ItemClicked(QTreeWidgetItem* pItem, int pColumn);
    void slot_RefreshTreelist();
    void on_BtnHideCmdRecv_clicked(bool checked);
    void on_BtnAdd_clicked();

public:
    //>@����·��
    QString             mTempPath;

    //>@�б���
    QTreeWidgetItem     *mNetitem,
                        *mUseritem,
                        *mUdiskitem,
                        *mComitem;
    //>@����
    QAction             *TempDirAct,   //>@�޸���ʱĿ¼
                        *BroadcastAct, //>@�㲥
                        *MessageAct,   //>@������Ϣ
                        //>@
                        *NetconnAct,   //>@����NFD����
                        *NetDisconnAct;

public:
    //>@�豸��
    QHash<QString, NetInfo*>    mNets;
    QHash<QString, UdiskInfo*>  mUdisks;
    QHash<QString, ComInfo*>    mComs;

private:
    Ui::devicelistform *ui;
};

HostNetManager* GetNetManager();

#endif // DEVICELISTFORM_H
