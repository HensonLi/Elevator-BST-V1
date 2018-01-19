#ifndef DEVICESCENE_H
#define DEVICESCENE_H

#include "upitem.h"
#include "math.h"
#include "deviceitem.h"
#include "deviceparaform.h"
#include <QGraphicsScene>
#include <QState>
#include <QStateMachine>

#define D_Radius        200
#define D_InitColor     QColor(170,90,45,230)
#define D_ModifyColor   QColor(180,160,100,230)
#define D_ShadowColor   QColor(150,130,100,120)
#define D_SelectColor   QColor(100,0,0,150)

/*
    ����Ϊ1600x1600�� ���ĵ�Ϊ800,800��СԲ�뾶225����Բ�뾶760����Բ�뾶100
    ��ѡ������ʾģʽ������->�߼�ͼ���߼�->����ͼ
    �������߼�->����ͼģʽ�У���ק���������߼��豸�ϵ�����ؼ������Ŵ��߼��豸��ת
                            ����Χ�����ĵ���ת

*/

class DeviceScene : public QGraphicsScene
{
    Q_OBJECT
public:
    DeviceScene(QString pMdpPath, QObject *parent = 0);
    ~DeviceScene();

    void OpenParaForm(QGraphicsItem* pItem);

    PhysicalDevice* CreateDevItem(PH_Type pType, QDomElement pElement);
    LogicDevice* CreateDevItem(LG_Type pType, QDomElement pElement);

    DeviceItem* AddDevice(PH_Type pType, LogicDevice* pLGDev, QDomElement pElement);
    void DelDevice(PH_Type pType);
    DeviceItem* AddDevice(LG_Type pType, QDomElement pElement);
    void DelDeviceItem(LG_Type pType);
    void DelDevice(DeviceItem* pDevItem);

    bool OpenBootMdp(QString pPath);
    void CloseBootMdp();
    bool SaveBootMdp();

    void Connect(PhysicalDevice *pPHDev, LogicDevice *pLGDev);
    void StartTimeline();

protected:
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    void deviceMoved();
    void timerEvent(QTimerEvent *event);

public slots:
    void Initilize();
    void configurationAdded(const QNetworkConfiguration &config);
    void configurationRemoved(const QNetworkConfiguration &config);
    void configurationChanged(const QNetworkConfiguration &config);

public:
    QHash<PH_Type, PhysicalDevice*> m_PHDevList;
    QList<DeviceItem *>             m_PHDevItems;

    QHash<LG_Type, LogicDevice*>    m_LGDevList;

    QTimeLine                       m_Animator;

    QGraphicsPixmapItem            *m_BackgroundItem;

    DeviceItem                     *m_CurDeviceItem;

    DeviceEllipse                  *m_DeviceEllipse[2];

    mdpManager*                     mMdpManager;
    QString                         mMDPPath;

    QMap<DEV_TYPE, qreal>           offset;
    QMultiMap<DEV_TYPE, QString>    configStates;
    int                             mModifyTimerId;
};


inline void DeviceScene::StartTimeline()
{
    m_Animator.stop();
    m_Animator.setFrameRange(0, D_TIMELINERANGE);
    m_Animator.setDuration(500);
    m_Animator.setCurveShape(QTimeLine::EaseInOutCurve);
    m_Animator.start();
}

class DeviceView : public UpQGraphicsView
{
    Q_OBJECT
public:
    DeviceView(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
};


#endif // DEVICESCENE_H
