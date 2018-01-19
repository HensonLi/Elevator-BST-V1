#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    m_UiManager = 0;
    m_SelectComponet = 0;
    m_UiDirection = Horizontal;
}

GraphicsScene::~GraphicsScene()
{
    ReleaseUI();
}

bool GraphicsScene::Initialize(QString pFileName)
{
    if(m_UiManager)
        delete m_UiManager;
    m_UiManager = new uiManager(pFileName);
    if(isFileExist(m_UiManager->m_FileDir+m_UiManager->m_FileName) == false)
    {
        IDE_TRACE_STR(m_UiManager->m_FileName);
        m_UiManager->NewUI();
    }
    else
    {
        IDE_TRACE_STR(m_UiManager->m_FileName);
        m_UiManager->OpenUI();
    }
    return InitUI();
}

bool GraphicsScene::InitUI()
{
    DeleteAllCom();
    //>@��ʼ������༭��
    if(!m_UiManager)
        return false;
    return true;
}

void GraphicsScene::ReleaseUI()
{
    DeleteAllCom();
    if(m_UiManager && m_UiManager->m_Valid)
    {
        //>@�˴������κα����ļ�������
        delete m_UiManager;
        m_UiManager = 0;
    }
}


void GraphicsScene::DeleteAllCom()
{
    hideFrame(true);
    //>@ɾ��scene�µ�����item
    QList<COM_TYPE> tmpComList = m_ComDiagramGroup.keys();
    for(int i=tmpComList.count()-1; i>=0; i--)
    {
        GraphicsOperate* tmpComItem = m_ComDiagramGroup.take(tmpComList.at(i));
        if(tmpComItem)
        {
            IDE_TRACE_STR(tmpComItem->m_ComPath);
            removeItem(tmpComItem);
            tmpComItem->deleteLater();
        }
    }
    m_ComDiagramGroup.clear();
}

bool GraphicsScene::AddComponent(COM_TYPE pComType, QDomElement pComElement, QPointF pScenePos)
{
    return true;
}

//>@ɾ��ĳ���ؼ�ʱ��Ҫ��
//>@  1���Ͽ����丸�����ӡ�
//>@  2����Scene/m_UiManager/m_ComDiagramGroup��ɾ���Լ��Լ��ӽڵ�
bool GraphicsScene::DelComponent(GraphicsOperate *&pComPtr)
{
    if(!pComPtr)
        return true;
    if(m_SelectComponet == pComPtr)
        hideFrame();
    QList<QGraphicsItem *> tmpChildItems = pComPtr->childItems();
    for(int i=tmpChildItems.count()-1;i>=0;i--)
    {
        GraphicsOperate *tmpChildItem = (GraphicsOperate *)tmpChildItems.at(i);
        if(!tmpChildItem)
            continue;
        DelComponent(tmpChildItem);
    }
    COM_TYPE tmpType = (COM_TYPE)(pComPtr->data(COMTYPE).toInt());
    m_ComDiagramGroup.remove(tmpType);
    removeItem(pComPtr);
    pComPtr->DeleteComponent();
    pComPtr->deleteLater();
    pComPtr = 0;
    return true;
}

bool GraphicsScene::SelectComponent(COM_TYPE pComType)
{
    GraphicsOperate *tmpComItem = m_ComDiagramGroup.value(pComType);
    return SelectComponent(tmpComItem);
}

bool GraphicsScene::SelectComponent(GraphicsOperate *pComPtr)
{
    if(!pComPtr)
        return false;
    if(pComPtr == m_SelectComponet)
        return true;
    //>@�������ѡ��ؼ�����֮ǰ��ѡ��Ŀؼ���Ҫֹͣ��Ч������
    if(m_SelectComponet)
        m_SelectComponet->StopAnimate();
    m_SelectComponet = pComPtr; //>@���浱ǰѡ�еĿؼ�
    m_SelectComponet->RefreshParameter();
    updateFrame();  //>@��Frame����Ϊ��ǰCOMINFO�Ŀ�
    return true;
}
