#include "diagramscene.h"
#include "mainwindow.h"

DiagramAddCommand:: DiagramAddCommand(COM_TYPE pComType, QDomElement pComElement, QPointF pPos, DiagramScene *pDiagramScene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_IsValid = true;
    m_ComType = pComType;
    m_Component = 0;
    m_ComElement = pComElement;
    m_DiagramScene = pDiagramScene;
    m_InitScenePos = pPos;
    if(m_DiagramScene == 0)
    {
        m_IsValid = false;
        return;
    }
    setText(QString("Add %1").arg(m_ComElement.tagName()));
}

void DiagramAddCommand::undo()
{
    if(!m_IsValid)
        return;
    if(!m_Component)
        return;
    if(!m_DiagramScene->DelComponent(m_Component))
        m_IsValid = false;
    m_ComElement = QDomElement();
}

void DiagramAddCommand::redo()
{
    if(!m_IsValid)
        return;
    if(!m_DiagramScene->AddComponent(m_ComType, m_ComElement, m_InitScenePos))
        m_IsValid = false;
    m_Component = m_DiagramScene->m_ComDiagramGroup.value(m_ComType);
}

DiagramDeleteCommand::DiagramDeleteCommand(GraphicsOperate *pComPtr, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_IsValid = true;
    m_Component = pComPtr;
    if(m_Component == 0)
    {
        m_IsValid = false;
        return;
    }
    m_ComType = (COM_TYPE)(m_Component->data(COMTYPE).toInt());
    m_DiagramScene = (DiagramScene*)m_Component->m_UiScene;
    if(m_DiagramScene == 0)
    {
        m_IsValid = false;
        return;
    }
    m_OldElement = m_Component->m_ComElement.cloneNode(true).toElement();
    setText(QObject::tr("Delete %1").arg(m_Component->m_ComElement.tagName()));
}

DiagramDeleteCommand::~DiagramDeleteCommand()
{
    m_OldElement.clear();
}

void DiagramDeleteCommand::undo()
{
    if(!m_IsValid)
        return;
    if(!m_DiagramScene->AddComponent(m_ComType, m_OldElement))
        m_IsValid = false;
    m_Component = m_DiagramScene->m_ComDiagramGroup.value(m_ComType);
}

void DiagramDeleteCommand::redo()
{
    if(!m_IsValid)
        return;
    if(!m_Component)
        return;
    if(!m_DiagramScene->DelComponent(m_Component))
        m_IsValid = false;
}

DiagramMoveCommand::DiagramMoveCommand(GraphicsOperate *pComPtr, QRectF pNewSceneRect, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_IsValid = true;
    m_Component = pComPtr;
    if(!(m_Component && m_Component->mParComponet))
    {
        m_IsValid = false;
        return;
    }
    m_NewRect = pNewSceneRect;
    m_OldRect = m_Component->sceneBoundingRect();
    setText(QObject::tr("Move %1").arg(m_Component->m_ComElement.tagName()));
}

void DiagramMoveCommand::undo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifySceneRect(m_OldRect))
        m_IsValid = false;
}

void DiagramMoveCommand::redo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifySceneRect(m_NewRect))
        m_IsValid = false;
}

DiagramChangeEffectCommand::DiagramChangeEffectCommand(GraphicsOperate *pComPtr, QString pStateName, QString pEffectName, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_IsValid = true;
    m_Component = pComPtr;
    if(!m_Component)
    {
        m_IsValid = false;
        return;
    }
    m_StateName = pStateName;
    m_NewEffectName = pEffectName;
    m_OldEffectName = m_Component->GetComStateEffect(pStateName);
    setText(QObject::tr("Change %1 Effect from %2 to %3").arg(m_StateName).arg(m_OldEffectName).arg(m_NewEffectName));
}

void DiagramChangeEffectCommand::undo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifyStateEffect(m_StateName, m_OldEffectName))
        m_IsValid = false;
}

void DiagramChangeEffectCommand::redo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifyStateEffect(m_StateName, m_NewEffectName))
        m_IsValid = false;
}

DiagramSetEffectParaCommand::DiagramSetEffectParaCommand(GraphicsOperate *pComPtr, QString pStateName, QString pParaName, QString pParaValue, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_IsValid = true;
    m_Component = pComPtr;
    if(!m_Component)
    {
        m_IsValid = false;
        return;
    }
    m_StateName = pStateName;
    m_ParaName = pParaName;
    m_NewParaValue = pParaValue;
    m_OldParaValue = m_Component->GetComStateEffectPara(pStateName, pParaName);
    setText(QObject::tr("Change Para %1 from %2 to %3").arg(m_ParaName).arg(m_OldParaValue).arg(m_NewParaValue));
}

void DiagramSetEffectParaCommand::undo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifyEffectPara(m_StateName, m_ParaName, m_OldParaValue))
        m_IsValid = false;
}

void DiagramSetEffectParaCommand::redo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifyEffectPara(m_StateName, m_ParaName, m_NewParaValue))
        m_IsValid = false;
}

DiagramModifyRcCommand::DiagramModifyRcCommand(GraphicsOperate *pComPtr, QString pStateName, QString pRcName, QString pRcFile, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_IsValid = true;
    m_Component = pComPtr;
    if(!m_Component)
    {
        m_IsValid = false;
        return;
    }
    m_StateName = pStateName;
    m_RcName = pRcName;
    m_NewRcFile = pRcFile;
    m_OldRcFile = m_Component->GetComStateRcFile(pStateName, pRcName);
    setText(QObject::tr("Change file %1 from %2 to %3").arg(m_RcName).arg(m_OldRcFile).arg(m_NewRcFile));
}

void DiagramModifyRcCommand::undo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifyRcFile(m_StateName, m_RcName, m_OldRcFile))
        m_IsValid = false;
}

void DiagramModifyRcCommand::redo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifyRcFile(m_StateName, m_RcName, m_NewRcFile))
        m_IsValid = false;
}

DiagramDelRcCommand::DiagramDelRcCommand(GraphicsOperate *pComPtr, QString pStateName, QString pRcName, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_IsValid = true;
    m_Component = pComPtr;
    if(!m_Component)
    {
        m_IsValid = false;
        return;
    }
    m_StateName = pStateName;
    m_RcName = pRcName;
    m_OldRcFile = m_Component->GetComStateRcFile(pStateName, pRcName);
    setText(QObject::tr("Delete file %1").arg(m_RcName));
}

void DiagramDelRcCommand::undo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->ModifyRcFile(m_StateName, m_RcName, m_OldRcFile))
        m_IsValid = false;
}

void DiagramDelRcCommand::redo()
{
    if(!m_IsValid)
        return;
    if(!m_Component->DelRcFile(m_StateName, m_RcName))
        m_IsValid = false;
}

DiagramScene::DiagramScene(QObject *parent) :
    GraphicsScene(parent)
{
    m_InitializeOK = false;
    m_UndoStack = 0;
    m_RegularThread = 0;
    m_TreeItem = 0;
    m_ProgressBarValue = 0;
    m_FrameItem = 0;
    m_UiRcNickLabel = 0;

    m_CollisionTimer = new QTimer(this);
    m_CollisionTimer->setInterval(1000);
    connect(m_CollisionTimer, SIGNAL(timeout()), this, SLOT(slot_TimeoutEvent()));
    connect(this,SIGNAL(sMessage(QString, int)), &GlobalPara, SLOT(updateProgress(QString, int)));

    CreateMenu();
}

DiagramScene::~DiagramScene()
{
    ReleaseRegularThread();
    ReleaseMenu();
}

bool DiagramScene::InitUI()
{
    m_InitializeOK = false;
    ReleasePropertyTree();
    DeleteAllCom();

    //>@��ʼ������༭��
    if(!m_UiManager)
        return false;
    //>@���ص�ǰ���������е����пؼ�
    QList<COM_TYPE> tmpComTypeList = m_UiManager->m_ComElementGroup.keys();
    //��ʼ��������
    emit sMessage(QString("Loading UI module..."), m_ProgressBarValue += 10);
    int count = tmpComTypeList.count();
    int diff = (int)(90.0/((qreal)count));

    QDomElement tmpDesktopElement = m_UiManager->m_ComElementGroup.value(COM_DESKTOP);
    if(tmpDesktopElement.isNull())
        return false;
    QRectF rect = m_UiManager->GetComRect(tmpDesktopElement, m_UiDirection);
    rect.setTopLeft(QPointF(0.0,0.0));
    rect.setBottomRight(rect.bottomRight()+QPointF(2*StartPosX,2*StartPosX));
    setSceneRect(rect);
    if(AddComponent(COM_DESKTOP, tmpDesktopElement))
    {
        QString tmpMessage;
        for(int i=0;i<count;i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);
            COM_TYPE tmpParType = getParentComType(tmpType);
            AddComponent(tmpParType, m_UiManager->m_ComElementGroup.value(tmpParType));
            if(!AddComponent(tmpType, m_UiManager->m_ComElementGroup.value(tmpType)))
                tmpMessage = QString("Load Componet %1 error...").arg(getComponetName(tmpType));
            else
                tmpMessage = QString("Load Componet %1 success...").arg(getComponetName(tmpType));
            emit sMessage(tmpMessage, m_ProgressBarValue += diff);//������
        }
    }
    emit sMessage(QString(""), m_ProgressBarValue = 0);//������
    emit sInitUI();
    m_InitializeOK = true;
    return true;
}

bool DiagramScene::AddComponent(COM_TYPE pComType, QDomElement pComElement, QPointF pScenePos)
{
    if(pComType < COM_DESKTOP)
        return false;
    //>@������ڴ˿ؼ��������ظ�����
    if(m_ComDiagramGroup.contains(pComType))
        return true;
    //>@�����Ҫ���ؼ�,����Ҫ�鿴��ǰ�����Ƿ���ڸ��ؼ������û�����˳�
    GraphicsOperate *tmpParComPtr = 0;
    COM_TYPE tmpParComType = getParentComType(pComType);
    if(tmpParComType >= COM_DESKTOP)
    {
        tmpParComPtr = m_ComDiagramGroup.value(tmpParComType);
        if(!tmpParComPtr)
            return false;
        if(pComElement.isNull())
        {
            //>@����˿ؼ��Ĵ�С����֤�ӿؼ����������ؼ���Χ
            QRectF rect = tmpParComPtr->sceneBoundingRect();
            if(!rect.contains(pScenePos))
                return false;
            int right = rect.right();
            int bottom = rect.bottom();
            int width = (right - pScenePos.x() > 100)?(100):(right - pScenePos.x());
            int height = (bottom - pScenePos.y() > 100)?(100):(bottom - pScenePos.y());
            QPointF tmpPoint = pScenePos - rect.topLeft();
            pComElement = m_UiManager->AddComDom(pComType, QRect(tmpPoint.toPoint(), QSize(width, height)), m_UiDirection);
            if(pComElement.isNull())
                return false;
        }
    }
    GraphicsOperate *tmpComPtr = 0;
    switch(pComType)
    {
        case COM_DESKTOP:
            tmpComPtr = new DesignDesktop(tmpParComPtr);
            addItem(tmpComPtr);
            break;
        case COM_SETBAR:
            tmpComPtr = new DesignSetBar(tmpParComPtr);
            break;
        case COM_MESSAGEBAR:
            tmpComPtr = new DesignMessage(tmpParComPtr);
            break;
        case COM_PROGRESSBAR:
            tmpComPtr = new DesignProgress(tmpParComPtr);
            break;
        case COM_ANIMAL:
            tmpComPtr = new DesignAnimal(tmpParComPtr);
            break;
        case COM_FLOOR:
            tmpComPtr = new DesignFloor(tmpParComPtr);
            break;
        case COM_ARROW:
            tmpComPtr = new DesignArrow(tmpParComPtr);
            break;
        case COM_FUNCTION:
            tmpComPtr = new DesignFunction(tmpParComPtr);
            break;
        case COM_VIDEO:
            tmpComPtr = new DesignVideo(tmpParComPtr);
            break;
        case COM_WEBVIEW:
            tmpComPtr = new DesignWebview(tmpParComPtr);
            break;
        case COM_QML:
            tmpComPtr = new DesignQML(tmpParComPtr);
            break;
        case COM_OPENGL:
            tmpComPtr = new DesignOpenGL(tmpParComPtr);
            break;
        case COM_IMAGE:
            tmpComPtr = new DesignImage(tmpParComPtr);
            break;
        case COM_AUDIO:
            tmpComPtr = new DesignAudio(tmpParComPtr);
            break;
        case COM_TEXT:
            tmpComPtr = new DesignText(tmpParComPtr);
            break;
        case COM_LOGO:
            tmpComPtr = new DesignLogo(tmpParComPtr);
            break;
        case COM_WEATHER:
            tmpComPtr = new DesignWeather(tmpParComPtr);
            break;
        case COM_TIME:
            tmpComPtr = new DesignClock(tmpParComPtr);
            break;
        case COM_NAMEPLATE:
            tmpComPtr = new DesignNamePlate(tmpParComPtr);
            break;
        case COM_FLOORBTN:
            tmpComPtr = new DesignFloorButton(tmpParComPtr);
            break;
        case COM_KEYBOARDBTN:
            tmpComPtr = new DesignKeyboard(tmpParComPtr);
            break;
        case COM_PHONEBTN:
        case COM_EMERGENCYBTN:
        case COM_OPENBTN:
        case COM_CLOSEBTN:
        case COM_DISABILITYBTN:
            tmpComPtr = new DesignButton(tmpParComPtr);
            break;
        default:
            return false;
    }
    if(tmpComPtr == 0)
        return false;
    if(tmpParComPtr == 0)
        tmpComPtr->setZValue(0);
    else
        tmpComPtr->setZValue(tmpParComPtr->zValue()+2);
    tmpComPtr->InitScene(this);
    tmpComPtr->InitComponent(pComElement, (pComType==COM_DESKTOP)?(true):(false));
    m_ComDiagramGroup.insert(pComType, tmpComPtr);
    SelectComponent(tmpComPtr);
    return true;
}

bool DiagramScene::StackMoveComponent(GraphicsOperate *pComPtr, QRectF pNewSceneRect)
{
    if(m_UndoStack)
    {
        DiagramMoveCommand *tmpCommand = new DiagramMoveCommand(pComPtr, pNewSceneRect);
        m_UndoStack->push(tmpCommand);
        return tmpCommand->IsValid();
    }
    return false;
}

bool DiagramScene::StackChangeComEffect(GraphicsOperate *pComPtr, QString pStateName, QString pEffectName)
{
    if(m_UndoStack)
    {
        DiagramChangeEffectCommand *tmpCommand = new DiagramChangeEffectCommand(pComPtr, pStateName, pEffectName);
        m_UndoStack->push(tmpCommand);
        return tmpCommand->IsValid();
    }
    return false;
}

bool DiagramScene::StackSetComEffectPara(GraphicsOperate *pComPtr, QString pStateName, QString pParaName, QString pParaValue)
{
    if(m_UndoStack)
    {
        DiagramSetEffectParaCommand *tmpCommand = new DiagramSetEffectParaCommand(pComPtr, pStateName, pParaName, pParaValue);
        m_UndoStack->push(tmpCommand);
        return tmpCommand->IsValid();
    }
    return false;
}

bool DiagramScene::StackModifyComRc(GraphicsOperate *pComPtr, QString pStateName, QString pRcName, QString pRcFile)
{
    if(m_UndoStack)
    {
        DiagramModifyRcCommand *tmpCommand = new DiagramModifyRcCommand(pComPtr, pStateName, pRcName, pRcFile);
        m_UndoStack->push(tmpCommand);
        return tmpCommand->IsValid();
    }
    return false;
}

bool DiagramScene::StackDelComRc(GraphicsOperate *pComPtr, QString pStateName, QString pRcName)
{
    if(m_UndoStack)
    {
        DiagramDelRcCommand *tmpCommand = new DiagramDelRcCommand(pComPtr, pStateName, pRcName);
        m_UndoStack->push(tmpCommand);
        return tmpCommand->IsValid();
    }
    return false;
}

void DiagramScene::CreateMenu()
{
    /*
        Delete
        ---------
        Copy
        Paste
        ---------
        Simulator
        ---------
        Test
    */
    m_ContexMenu = new QMenu;
    m_DelCom = m_ContexMenu->addAction(QIcon(":/Menu/rc/Menu/minus.png"), tr("Delete"),
                                                    this, SLOT(slot_DeleteCom()));
    m_ContexMenu->addSeparator();
    m_CopyComRC = m_ContexMenu->addAction(QIcon(":/Menu/rc/Menu/copy.png"), tr("Copy"),
                                                       this, SLOT(slot_CopyCom()));
    m_PasteComRC = m_ContexMenu->addAction(QIcon(":/Menu/rc/Menu/paste.png"), tr("Paste"),
                                                        this, SLOT(slot_PasteCom()));
    m_ContexMenu->addSeparator();
    m_SimulatorCom = m_ContexMenu->addMenu(QIcon(":/Menu/rc/Menu/debug.png"), tr("Simulator"));

    m_SoftTestCom = m_ContexMenu->addAction(QIcon(":/Menu/rc/Menu/toolView.png"), tr("Test"),
                                                       this, SLOT(slot_TestDialog()));
}

void DiagramScene::ReleaseMenu()
{
    m_CopyComRC->deleteLater();
    m_PasteComRC->deleteLater();
    m_SoftTestCom->deleteLater();
    m_DelCom->deleteLater();
    m_SimulatorCom->deleteLater();
    m_ContexMenu->deleteLater();
}

//>@���ڱ߽���
void DiagramScene::slot_TimeoutEvent()
{
    //>@������
    for(int i=COM_AREA+1;i<COM_COMPONENT;i++)
    {
        GraphicsOperate *tmpCom = m_ComDiagramGroup.value((COM_TYPE)i);
        if(tmpCom != 0)
        {
            if(CollisionDetecting(tmpCom))
            {
                Effect_color(tmpCom, 150, 0, 0);
            }
            else
            {
                Effect_color(tmpCom, -1, -1, -1);
            }
        }
    }
}

bool DiagramScene::CollisionDetecting(QGraphicsItem *item)
{
    if(item == 0)
        return false;
    //>@ֻ������ؼ�����ײ���
    COM_TYPE tmpType = (COM_TYPE)(item->data(COMTYPE).toInt());
    if( tmpType > COM_AREA && tmpType < COM_COMPONENT)
    {
        QList<QGraphicsItem *> CollisionList = collidingItems(item);
        for(int i=0; i<CollisionList.count(); i++)
        {
            QGraphicsItem *tmpItem = CollisionList.at(i);
            if(tmpItem->type() == 11)
            {
                COM_TYPE tmpType = (COM_TYPE)(item->data(COMTYPE).toInt());
                if( tmpType > COM_AREA && tmpType < COM_COMPONENT)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void DiagramScene::showFrame(GraphicsOperate *pComItem)
{
    if(m_FrameItem == 0)
    {
        m_FrameItem = new DiagramFrame;
        addItem(m_FrameItem);
    }
    m_FrameItem->InitFrame(pComItem);
    m_FrameItem->show();
}

void DiagramScene::updateFrame()
{
    if(!m_SelectComponet)
        return;
    updatePropertyTree();
    COM_TYPE tmpType = (COM_TYPE)(m_SelectComponet->data(COMTYPE).toInt());
    if(tmpType == COM_DESKTOP)
        hideFrame();
    else
        showFrame(m_SelectComponet);
}

void DiagramScene::hideFrame(bool pDelFlag)
{
    //>@ע��Ҫ��mySelectCom���㣬�����´ε���˿ؼ�ʱ���������ͬ��������Frame�������
    //>@Ҳ�������ȵ�������ؼ�����ܵ���˿ؼ���
    m_SelectComponet = 0;
    //>@
    if(m_FrameItem == 0)
        return;
    m_FrameItem->hide();
    if(pDelFlag)
    {
        disconnect(m_FrameItem, SIGNAL(updateControl()), 0, 0);
        removeItem(m_FrameItem);
        m_FrameItem->deleteLater();
        m_FrameItem = 0;
    }
}

void DiagramScene::updatePropertyTree()
{
    if(!m_SelectComponet)
        return;
    ProjectBar* tmpProjectBar = VAR_CAST<ProjectBar*>(GlobalPara.m_ProjectBar);
    if(tmpProjectBar && tmpProjectBar->mPropertyWidget)
        tmpProjectBar->mPropertyWidget->RefreshTree(m_SelectComponet);
}

void DiagramScene::ReleasePropertyTree()
{
    ProjectBar* tmpProjectBar = VAR_CAST<ProjectBar*>(GlobalPara.m_ProjectBar);
    if(tmpProjectBar && tmpProjectBar->mPropertyWidget)
    {
        tmpProjectBar->mPropertyWidget->mComponet = 0;
        tmpProjectBar->mPropertyWidget->RefreshTree((GraphicsOperate *)0);
    }
}

void DiagramScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton)
    {
        if(m_ContexMenu)
        {
            QList<QGraphicsItem *> SelectItems = items(mouseEvent->scenePos());
            if(SelectItems.count()>0)
            {
                GraphicsOperate *tmpCom = 0;
                for(int i=0;i<SelectItems.count();i++)
                {
                    tmpCom = VAR_CAST<GraphicsOperate *>(SelectItems.at(i));
                    if(tmpCom == 0)  //>@�Ƿ�ΪGraphicsBase
                        continue;
                    break;
                }
                if(!tmpCom)
                {
                    IDE_TRACE();
                    return;
                }
                COM_TYPE tmpType = (COM_TYPE)(tmpCom->data(COMTYPE).toInt());
                if(tmpType < COM_DESKTOP)
                    return;
                if(tmpType == COM_DESKTOP)
                {
                    m_CopyComRC->setEnabled(false);
                    m_PasteComRC->setEnabled(false);
                    m_SoftTestCom->setEnabled(true);
                    m_DelCom->setDisabled(false);
                }
                else if(tmpType >= COM_SETBAR)
                {
                    m_CopyComRC->setEnabled(true);
                    m_PasteComRC->setEnabled(true);
                    m_SoftTestCom->setEnabled(true);
                    m_DelCom->setEnabled(true);
                }
                if(m_SimulatorCom)
                    m_SimulatorCom->clear();


                QList<QAction*> tmpActionList;
                tmpActionList = tmpCom->GetActionList();
                if(tmpActionList.count()>0)
                {
                    m_SimulatorCom->addActions(tmpActionList);
                }
                m_ContexMenu->exec(QCursor::pos());//�ڵ�ǰ���λ����ʾ
            }
        }
        return;
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::RightButton)
    {
        if(m_ContexMenu)
        {
            QList<QGraphicsItem *> SelectItems = items(mouseEvent->scenePos());
            if(SelectItems.count()>0)
            {
                DiagramFrame *tmpFrame = 0;
                for(int i=0;i<SelectItems.count();i++)
                {
                    tmpFrame = VAR_CAST<DiagramFrame *>(SelectItems.at(i));
                    if(tmpFrame == 0)  //>@�Ƿ�ΪDiagramFrame
                        continue;
                    break;
                }
                if(!(tmpFrame && tmpFrame->m_SelectFlag))
                {
                    IDE_TRACE();
                    return;
                }
                if(tmpFrame->m_Component && tmpFrame->m_Component == m_SelectComponet)
                {
                    COM_TYPE tmpType = (COM_TYPE)(m_SelectComponet->data(COMTYPE).toInt());
                    if(tmpType < COM_DESKTOP)
                        return;
                    if(tmpType == COM_DESKTOP)
                    {
                        m_CopyComRC->setEnabled(false);
                        m_PasteComRC->setEnabled(false);
                        m_SoftTestCom->setEnabled(true);
                        m_DelCom->setDisabled(false);
                    }
                    else if(tmpType >= COM_SETBAR)
                    {
                        m_CopyComRC->setEnabled(true);
                        m_PasteComRC->setEnabled(true);
                        m_SoftTestCom->setEnabled(true);
                        m_DelCom->setEnabled(true);
                    }
                    if(m_SimulatorCom)
                        m_SimulatorCom->clear();


                    QList<QAction*> tmpActionList;
                    if(m_SimulatorCom == 0)
                        return;
                    tmpActionList = m_SelectComponet->GetActionList();
                    if(tmpActionList.count()>0)
                    {
                        m_SimulatorCom->addActions(tmpActionList);
                    }
                    m_ContexMenu->exec(QCursor::pos());//�ڵ�ǰ���λ����ʾ
                }
            }
        }
        return;
    }
    else if(mouseEvent->button() != Qt::LeftButton)
    {
        IDE_TRACE();
        return;
    }
    QList<QGraphicsItem *> SelectItems = items(mouseEvent->scenePos());
    if(SelectItems.count()>0)
    {
        for(int i=0;i<SelectItems.count();i++)
        {
            QGraphicsItem *tmp = SelectItems.at(i);
            if(tmp)  //>@�Ƿ�ΪQGraphicsWidget��type
            {
                COM_TYPE tmpComType = (COM_TYPE)(tmp->data(COMTYPE).toInt());
                if(tmpComType < COM_DESKTOP)  //>@һ���������ǵ���˴˿ؼ�����UI����ҪָButton
                {
                    tmp = tmp->parentItem();
                    if(tmp == 0)  //>@frame�ĸ�Ϊ��
                        break;
                    tmpComType = (COM_TYPE)(tmp->data(COMTYPE).toInt());
                    if(tmpComType < COM_DESKTOP)
                        break;
                }
                SelectComponent(tmpComType);
                break;
            }
        }
    }
    //>@ע�⣬�˴���mousePressEvent�����ִ�У�����scene�е�����Item�����ղ���press�źš�
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void DiagramScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void DiagramScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(!event->mimeData()->hasFormat("COM"))
        return;
    QByteArray itemData = event->mimeData()->data("COM");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);
    dataStream>>m_InsertComName;
    IDE_TRACE_STR(m_InsertComName);

    QList<QGraphicsItem *> SelectItems = items(event->scenePos());
    if(SelectItems.count()>0)
    {
        GraphicsOperate *tmp = 0;
        for(int i=0;i<SelectItems.count();i++)
        {
            tmp = (GraphicsOperate*)(SelectItems.at(i));
            if(tmp == 0)  //>@�Ƿ�ΪQGraphicsWidget��type
                continue;
            break;
        }
        if(tmp == 0)
        {
            IDE_TRACE();
            return;
        }
        if(m_UndoStack)
            m_UndoStack->push(new DiagramAddCommand(getCurComType(m_InsertComName), QDomElement(), event->scenePos(), this));
    }
}

void DiagramScene::slot_DeleteCom()
{
    if(m_UndoStack)
        m_UndoStack->push(new DiagramDeleteCommand(m_SelectComponet));
    m_SelectComponet = 0;
    ReleasePropertyTree();
}

void DiagramScene::slot_CopyCom()
{

}

void DiagramScene::slot_PasteCom()
{

}

void DiagramScene::slot_TestDialog()
{
    if(!m_SelectComponet)
        return;
    DiagramSetting SettingDialog(m_SelectComponet);
    int ret = SettingDialog.exec();
    if(ret == QDialog::Accepted)
    {
    }
}

void DiagramScene::SaveUI()
{
    m_UiManager->m_IsChanged = true;
    m_UiManager->SaveUI();
}

//>@����Դ������UI����Ŀ¼�У������޸���Դ·��
void DiagramScene::SaveRelativeUI()
{
    if(m_UiManager->isValid() == false)
        return;

    QList<COM_TYPE> tmpList = m_UiManager->m_ComElementGroup.keys();
    //��ʼ��������
    emit sMessage(tr("Saving UI module to Relative Floder..."), m_ProgressBarValue += 10);
    int count = tmpList.count();
    int diff = (int)(90.0/((qreal)count));
    //>@�޸�Desktop
    m_UiManager->SaveRelativeComDom(m_UiManager->m_ComElementGroup.value(COM_DESKTOP));
    //>@���������и����ڵĿؼ�
    for(int i=0;i<count;i++)
    {
        COM_TYPE tmpComType = tmpList.at(i);
        if(tmpComType != COM_INVALID)
        {
            //>@������丸�ؼ�
            COM_TYPE tmpParComType = getParentComType(tmpComType);
            QDomElement tmpParElement = m_UiManager->m_ComElementGroup.value(tmpParComType);
            if(tmpParElement.isNull())
                continue;
            m_UiManager->SaveRelativeComDom(tmpParElement);
            //>@����ӿؼ�
            QDomElement tmpElement = m_UiManager->m_ComElementGroup.value(tmpComType);
            if(tmpElement.isNull())
                continue;
            m_UiManager->SaveRelativeComDom(tmpElement);
        }
        emit sMessage(QString("Saving regular UI ..."), m_ProgressBarValue += diff);//������
    }

    m_UiManager->m_IsChanged = true;
    m_UiManager->SaveUI();

    emit sMessage(QString(""), m_ProgressBarValue = 0);//������
}

bool DiagramScene::RegularComToManager(GraphicsOperate *pComponent, uiManager *pUiManager)
{
    if(!m_UiManager->isValid())
        return false;
    if(!pComponent || pComponent->m_ComElement.isNull())
        return false;
    if(!(pUiManager && pUiManager->isValid()))
        return false;
    COM_TYPE tmpComType = getComponetType(pComponent->m_ComElement.tagName());
    //>@������ڴ˿ؼ������˳�
    QDomElement tmpElement = pUiManager->m_ComElementGroup.value(tmpComType);
    if(tmpElement.isNull() == false)
        return true;
    //>@��pUiManager�½���COM
    DIRECTION tmpValidDir = m_UiManager->canSwitch()?Horizontal:m_UiManager->m_DirectionPermisson;
    QRect tmpComRect = m_UiManager->GetComRect(pComponent->m_ComElement, tmpValidDir);
    QDomElement tmpComElement = pUiManager->AddComDom(tmpComType, tmpComRect, tmpValidDir);
    if(tmpComElement.isNull())
        return false;
    QDomNodeList tmpStateList = m_UiManager->GetComStateList(pComponent->m_ComElement);
    //>@��Դ����
    for(int i=0;i<tmpStateList.count();i++)
    {
        QDomElement tmpStateElement = tmpStateList.at(i).toElement();
        if(tmpStateElement.isNull())
            continue;
        QDomElement tmpNewStateElement = pUiManager->ModifyComState(tmpComElement, tmpStateElement.tagName(), tmpStateElement.attribute(D_EFFECTTYPE), tmpStateElement.attribute(D_EFFECTPARA));
        if(tmpNewStateElement.isNull())
            continue;
        QDomNodeList tmpRcList = tmpStateElement.childNodes();
        for(int j=0;j<tmpRcList.count();j++)
        {
            QDomElement tmpRcElement = tmpRcList.at(j).toElement();
            if(tmpRcElement.isNull())
                continue;
            QString tmpRcName = tmpRcElement.tagName();
            QString tmpRcFile = tmpRcElement.text();
            //>@���������ͬ�޸�ѹ���ķֱ���
            QSize tmpSize = pComponent->GetRcSize(tmpRcName);
            if(m_UiManager->RegularFile(tmpComType, tmpRcName, tmpRcFile, pUiManager->m_FileDir, tmpSize) == false)
                continue;
            pUiManager->CreateItemElement(tmpNewStateElement, tmpRcName, tmpRcFile);
        }
    }
    return true;
}

void DiagramScene::RegularUIToOutputFloder()
{
    //>@���ȱ���UI
    SaveRelativeUI();
    //>@��ȡ��������ѹ����������Ϊ������
    QString tmpProjectPath = m_UiManager->m_FileDir;
    if(tmpProjectPath.endsWith('/') == false)
        tmpProjectPath.append('/');
    QString tmpProjectName = GetDirectoryName(tmpProjectPath);
    m_OutputDir = tmpProjectPath + OUTPUTDIR + QString("/") + tmpProjectName + QString("/");

    //��ʼ��������
    m_ProgressBarValue = 0;
    emit sMessage(QString("Regular UI module to Output Folder..."), m_ProgressBarValue += 5);

    //>@ɾ������OUTPUT�ļ�������Դ���Ŀ¼�е�����
    emit sMessage(QString("Deleting %1...").arg(m_OutputDir), m_ProgressBarValue += 5);
    if(!ClearFolder(m_OutputDir))
    {
        emit sMessage(tr("Can't clear folder, go to check %1 !").arg(m_OutputDir), m_ProgressBarValue = 99);
        QString tmpPath = QString("file:///%1").arg(m_OutputDir);
        QDesktopServices::openUrl(QUrl(tmpPath, QUrl::TolerantMode));
        return;
    }

    //>@�ڴ������UI�ļ��Ĺ���OUTPUT�ļ���Ŀ¼�µ��빤������ͬ���ļ����д���һ���µ�XML�ļ�
    uiManager *tmpUiManager = new uiManager(m_OutputDir + m_UiManager->m_FileName);
    tmpUiManager->NewUI();

    QList<COM_TYPE> tmpList = m_ComDiagramGroup.keys();
    emit sMessage(QString("Start regular resources..."), m_ProgressBarValue += 5);
    int count = tmpList.count();
    int diff = (int)(90.0/((qreal)count));

    //>@�޸�Desktop
    if(!RegularComToManager(m_ComDiagramGroup.value(COM_DESKTOP), tmpUiManager))
    {
        tmpUiManager->m_IsChanged = true;
        delete tmpUiManager;
        return;
    }
    //>@���������и����ڵĿؼ�
    for(int i=0;i<count;i++)
    {
        COM_TYPE tmpComType = tmpList.at(i);
        COM_TYPE tmpParType = getParentComType(tmpComType);
        if(tmpParType != COM_INVALID)
        {
            //>@������丸�ؼ�
            RegularComToManager(m_ComDiagramGroup.value(tmpParType), tmpUiManager);
            //>@����ӿؼ�
            RegularComToManager(m_ComDiagramGroup.value(tmpComType), tmpUiManager);
        }
        emit sMessage(QString("Regular %1...").arg(getComponetName(tmpComType)), m_ProgressBarValue += diff);//������
    }
    tmpUiManager->m_IsChanged = true;
    delete tmpUiManager;

    emit sMessage(QString(""), m_ProgressBarValue = 0);//������
}

void DiagramScene::RegularUI()
{
    InitRegularThread();
}

void DiagramScene::InitRegularThread()
{
    ReleaseRegularThread();
    m_RegularThread = new RegularThread(this);
    connect(m_RegularThread, SIGNAL(finished()),this,SIGNAL(sEnableUI()));
    m_RegularThread->start(QThread::LowPriority);
}

void DiagramScene::ReleaseRegularThread()
{
    if(m_RegularThread != 0)
    {
        if(m_RegularThread != 0)
        {
            if (m_RegularThread->isRunning())
            {
                if (QThread::currentThread() != m_RegularThread)
                    m_RegularThread->wait();
            }
        }
        m_RegularThread->quit();
        m_RegularThread->deleteLater();
        m_RegularThread = 0;
    }
}

RegularThread::RegularThread(DiagramScene* pQesp):
    QThread()
{
    this->qesp = pQesp;
}

RegularThread::~RegularThread()
{

}

void RegularThread::run()
{
    qesp->RegularUIToOutputFloder();
}
