#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "global.h"
#include "graphicsoperate.h"

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent = 0);
    ~GraphicsScene();

    virtual bool AddComponent(COM_TYPE pComType, QDomElement pComElement, QPointF pScenePos);
    bool DelComponent(GraphicsOperate *&pComPtr);
    bool SelectComponent(COM_TYPE pComType);
    bool SelectComponent(GraphicsOperate *pComPtr);

    virtual bool Initialize(QString pFileName);
    //>@���溯��
    virtual bool InitUI();     //>@��ʼ��UI�ļ�
    void ReleaseUI();  //>@�ͷ�UI�ļ�
    void DeleteAllCom();  //>@�ͷ�Scene�е��������

    virtual void showFrame(GraphicsOperate *pComItem) {}
    virtual void hideFrame(bool pDelFlag = false) {}
    virtual void updateFrame() {}

public:
    DIRECTION            m_UiDirection;
    uiManager           *m_UiManager;
    GraphicsOperate     *m_SelectComponet;   //>@��ǰѡ�еĿؼ�����
    QHash<COM_TYPE, GraphicsOperate*> m_ComDiagramGroup;   //>@xx.uiҳ���е������subui��control����GraphicsBaseָ��
};

#endif // GRAPHICSSCENE_H
