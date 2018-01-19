#ifndef MODALWINDOW_H
#define MODALWINDOW_H

#include <QWizard>
#include "global.h"
#include "deviceclass.h"

namespace Ui {
class ProWizard;
}

class ProWizard : public QWizard
{
    Q_OBJECT
public:
    ProWizard(QWidget *parent=0);                           //>@�����½�һ������
    ProWizard(QString pFilePath, QWidget *parent=0);        //>@�������һ���Ѿ����ڵĹ���
    ProWizard(bstproManager *&pManager, QWidget *parent=0);  //>@���ڴ�һ���Ѿ��򿪵Ĺ���
    ~ProWizard();

    void InitUI();
    void InitWizard();
    void SyncWizard();  //>@��UI�ĵ��޸Ĳ���ͬ������Ӧ���ļ��Լ��ļ��еĲ�����

    void accept();
    void rejected();

    //>@��Ŀ¼ѡ��ļ����ַ����
    void SetMemoryDir(QString pPath, bool pLock=false);

    bstproManager* GetManager() {return m_ProManager;}

private slots:
    void on_Browser_clicked();
    void slot_ProjPathChanged(QString pProPath);

public:
    Ui::ProWizard       *ui;
    bool                 m_MemoryDirLock;
    QString              m_MemoryDir;
    bstproManager       *m_ProManager;
    bool                 m_CloseFlag;       //>@����ָʾ�����ȡ����ťʱ���Ѿ�������bstproManager�Ƿ�ɾ����
};

namespace Ui {
class DevWizard;
}

class DevWizard : public QWizard
{
    Q_OBJECT
public:
    DevWizard(QWidget *parent=0);                           //>@�����½�һ���豸
    DevWizard(QString pBufferPath, QWidget *parent=0);      //>@�������һ���Ѿ����ڵ��豸
    DevWizard(BaseDevice *&pDev, QWidget *parent=0);
    ~DevWizard();

    void InitUI();
    void SyncUI();  //>@�������޸�ͬ����BaseDevice�С�

    void accept();
    void rejected();

    BaseDevice* GetDevice() {return m_Dev;}

private slots:
    void on_Browser_clicked();
    void slot_DevBufferPathChanged(QString pBufferPath) {}

public:
    Ui::DevWizard       *ui;
    QString              m_MemoryDir;
    BaseDevice          *m_Dev;
    bool                 m_CloseFlag;
};

#endif // MODALWINDOW_H
