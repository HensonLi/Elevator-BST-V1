#ifndef UIPAGE_H
#define UIPAGE_H

#include "tabpage.h"
#include <QGLWidget>
#include "simulatorform.h"
#include "treeitemclass.h"
#include "uipageview.h"

namespace Ui {
class uiPage;
}

class FileTreeItem;
class uiPage : public TabPage
{
    Q_OBJECT
public:
    uiPage(QWidget *parent, QString fileName);
    ~uiPage();

    bool eventFilter(QObject *target, QEvent *event);

    void SyncTreeItem(FileTreeItem *pTreeItem);
    
public slots:
    void slot_ComNumberUpdate();
    void slot_EnableUI();
    void slot_DisableUI();
    void slot_InitUI();

    void on_CollisionBtn_clicked(bool checked);
    void on_SetComFront_clicked(bool checked);
    void on_RotateBtn_clicked(bool checked);

    void on_ZoomInBtn_clicked();
    void on_ZoomOutBtn_clicked();
    void on_ZoomSlider_valueChanged(int value);

    void on_BtnShowSetbar_clicked(bool checked);
    void on_BtnShowPrgs_clicked(bool checked);
    void on_BtnShowMsg_clicked(bool checked);

    void slot_Refresh();  //>@ˢ�µ�ǰUI
    void slot_Save();     //>@���浱ǰUI����Դ�����·��
    void slot_Generate(); //>@������Դ��OutputĿ¼��
    void slot_Simulate();

public:
    Ui::uiPage      *ui;
    uiPageView *     m_GraphicsView;
    QString          m_UiFilePath;
    QTimer           m_Timer;
};

#endif // UIPAGE_H
