#include "diagramscene.h"

DiagramFrame::DiagramFrame(QGraphicsItem *parent)
    : QGraphicsWidget(parent, Qt::FramelessWindowHint)
{
    setCursor(Qt::ArrowCursor);
    setFlags(ItemIsMovable | ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    QApplication::setStartDragDistance(5);

    m_Component = 0;
    m_Mutex = 0;
    m_Mutex = new QMutex(QMutex::Recursive);
}

void DiagramFrame::Connect()
{
    if(!m_Component)
        return;
    connect(m_Component, SIGNAL(sSceneRectChanged(QRectF)), this, SLOT(slot_SceneRectChanged(QRectF)));
}

void DiagramFrame::Disconnect()
{
    if(!m_Component)
        return;
    disconnect(m_Component, SIGNAL(sSceneRectChanged(QRect)), 0, 0);
}

void DiagramFrame::InitFrame(GraphicsOperate *pComponent)
{
    QApplication::restoreOverrideCursor();
    m_SavedComOperate = DRAGNONE;
    m_SelectFlag = false;

    Disconnect();
    m_Component = pComponent;
    if(!(m_Component && m_Component->mDragValid))
        return;
    Connect();

    //>@����Frame
    setGeometry(m_Component->m_CurRect.adjusted(-FRAMEVALIDWIDTH, -FRAMEVALIDWIDTH, FRAMEVALIDWIDTH, FRAMEVALIDWIDTH));
    setZValue(m_Component->zValue()+1);
    m_SelectFlag = true;
    setColor(D_NORMAL_COLOR);
}

COM_OPERTATE DiagramFrame::GetCursorShape(QPointF Point)
{
    if ((Point.x()>m_Component->m_CurLeft+FRAMEVALIDWIDTH)&&(Point.x()<m_Component->m_CurRight-FRAMEVALIDWIDTH)&&
             (Point.y()>m_Component->m_CurTop+FRAMEVALIDWIDTH)&&(Point.y()<m_Component->m_CurBottom-FRAMEVALIDWIDTH))	//�ƶ�
    {
        m_Component->setParentItem(this);
        m_Component->setPos(QPointF(FRAMEVALIDWIDTH, FRAMEVALIDWIDTH));
         QApplication::setOverrideCursor(Qt::SizeAllCursor);
        return DRAGALL;
    }
    else if ((Point.x()<=m_Component->m_CurLeft+FRAMEVALIDWIDTH)&&(Point.x()>=m_Component->m_CurLeft-FRAMEVALIDWIDTH)&&
             (Point.y()<m_Component->m_CurTop+FRAMEVALIDWIDTH)&&(Point.y()>=m_Component->m_CurTop-FRAMEVALIDWIDTH))	//���Ͻ�����
    {
         QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
        return DRAGLEFTTOP;
    }
    else if ((Point.x()<=m_Component->m_CurRight+FRAMEVALIDWIDTH)&&(Point.x()>m_Component->m_CurRight-FRAMEVALIDWIDTH)&&
             (Point.y()<m_Component->m_CurTop+FRAMEVALIDWIDTH)&&(Point.y()>=m_Component->m_CurTop-FRAMEVALIDWIDTH))	//���Ͻ�����
    {
         QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
        return DRAGRIGHTTOP;
    }
    else if ((Point.x()<=m_Component->m_CurRight+FRAMEVALIDWIDTH)&&(Point.x()>m_Component->m_CurRight-FRAMEVALIDWIDTH)&&
             (Point.y()<=m_Component->m_CurBottom+FRAMEVALIDWIDTH)&&(Point.y()>m_Component->m_CurBottom-FRAMEVALIDWIDTH))	//���½�����
    {
         QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
        return DRAGRIGHTBOTTOM;
    }
    else if ((Point.x()<m_Component->m_CurLeft+FRAMEVALIDWIDTH)&&(Point.x()>=m_Component->m_CurLeft-FRAMEVALIDWIDTH)&&
             (Point.y()<=m_Component->m_CurBottom+FRAMEVALIDWIDTH)&&(Point.y()>m_Component->m_CurBottom-FRAMEVALIDWIDTH))	//���½�����
    {
         QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
        return DRAGLEFTBOTTOM;
    }
    else if((Point.x()<m_Component->m_CurLeft+FRAMEVALIDWIDTH)&&(Point.x()>=m_Component->m_CurLeft-FRAMEVALIDWIDTH)&&
            (Point.y()<m_Component->m_CurBottom-FRAMEVALIDWIDTH)&&(Point.y()>m_Component->m_CurTop+FRAMEVALIDWIDTH))		//������
    {
          QApplication::setOverrideCursor(Qt::SizeHorCursor);
        return DRAGLEFT;
    }
    else if((Point.x()<m_Component->m_CurRight-FRAMEVALIDWIDTH)&&(Point.x()>m_Component->m_CurLeft+FRAMEVALIDWIDTH)&&
            (Point.y()<m_Component->m_CurTop+FRAMEVALIDWIDTH)&&(Point.y()>m_Component->m_CurTop-FRAMEVALIDWIDTH))		//������
    {
         QApplication::setOverrideCursor(Qt::SizeVerCursor);
        return DRAGTOP;
    }
    else if ((Point.x()<=m_Component->m_CurRight+FRAMEVALIDWIDTH)&&(Point.x()>m_Component->m_CurRight-FRAMEVALIDWIDTH)&&
             (Point.y()>m_Component->m_CurTop+FRAMEVALIDWIDTH)&&(Point.y()<m_Component->m_CurBottom-FRAMEVALIDWIDTH))	//������
    {
         QApplication::setOverrideCursor(Qt::SizeHorCursor);
        return DRAGRIGHT;
    }
    else if ((Point.x()<m_Component->m_CurRight-FRAMEVALIDWIDTH)&&(Point.x()>m_Component->m_CurLeft+FRAMEVALIDWIDTH)&&
             (Point.y()<=m_Component->m_CurBottom+FRAMEVALIDWIDTH)&&(Point.y()>m_Component->m_CurBottom-FRAMEVALIDWIDTH))	//������
    {
         QApplication::setOverrideCursor(Qt::SizeVerCursor);
        return DRAGBOTTOM;
    }
    return DRAGNONE;
}

void DiagramFrame::ReleaseCursorShape(COM_OPERTATE pOperate)
{
    switch(pOperate)
    {
        case DRAGNONE:
             return;
        case DRAGALL:
        {
            QRectF tmpRect = m_Component->sceneBoundingRect();
            QPointF tmpPos = m_Component->ModifyComPos(tmpRect, true);
            setPos(tmpPos - QPointF(FRAMEVALIDWIDTH, FRAMEVALIDWIDTH));
            break;
        }
        case DRAGRIGHT:
        case DRAGBOTTOM:
        case DRAGRIGHTBOTTOM:
        case DRAGLEFT:
        case DRAGTOP:
        case DRAGLEFTTOP:
        case DRAGRIGHTTOP:
        case DRAGLEFTBOTTOM:
        {
            DiagramScene *tmpScene = (DiagramScene *)(m_Component->m_UiScene);
            tmpScene->StackMoveComponent(m_Component, m_Component->GetVirtualSceneRect());
            break;
        }
        default:
            return;
    }
    setColor(D_NORMAL_COLOR);
    QApplication::restoreOverrideCursor();
    m_SavedComOperate = DRAGNONE;
}

void DiagramFrame::UpdateFrame(QGraphicsSceneMouseEvent *event)
{
    //m_Mutex->lock();
    QPointF ScenePoint = event->scenePos();
    if(ScenePoint.isNull())
    {
        //m_Mutex->unlock();
        return;
    }
    //if (QLineF(Point, event->buttonDownScreenPos(Qt::LeftButton)).length() < QApplication::startDragDistance())
    //{
    //    //m_Mutex->unlock();
    //    return;
    //}
    switch(m_SavedComOperate)
    {
        case DRAGALL:
        {
            QGraphicsWidget::mouseMoveEvent(event);
            //m_Mutex->unlock();
            return;
        }
        case DRAGLEFT:                  //������
        {
            m_Component->ModifyComLeft(ScenePoint.x(), false);
            break;
        }
        case DRAGTOP:                   //������
        {
            m_Component->ModifyComTop(ScenePoint.y(), false);
            break;
        }
        case DRAGRIGHT:                 //������
        {
            m_Component->ModifyComRight(ScenePoint.x(), false);
            break;
        }
        case DRAGBOTTOM:                //������
        {
            m_Component->ModifyComBottom(ScenePoint.y(), false);
            break;
        }
        case DRAGLEFTTOP:               //���Ͻ�����
        {
            m_Component->ModifyComLeft(ScenePoint.x(), false);
            m_Component->ModifyComTop(ScenePoint.y(), false);
            break;
        }
        case DRAGRIGHTTOP:          //���Ͻ�����
        {
            m_Component->ModifyComRight(ScenePoint.x(), false);
            m_Component->ModifyComTop(ScenePoint.y(), false);
            break;
        }
        case DRAGRIGHTBOTTOM:       //���½�����
        {
            m_Component->ModifyComRight(ScenePoint.x(), false);
            m_Component->ModifyComBottom(ScenePoint.y(), false);
            break;
        }
        case DRAGLEFTBOTTOM:        //���½�����
        {
            m_Component->ModifyComLeft(ScenePoint.x(), false);
            m_Component->ModifyComBottom(ScenePoint.y(), false);
            break;
        }
        default:
        {
            //m_Mutex->unlock();
            return;
        }
    }
    //>@����Frame��С
    SetFrameRect(QRectF(QPointF(m_Component->m_CurLeft, m_Component->m_CurTop),
                        QPointF(m_Component->m_CurRight, m_Component->m_CurBottom)));
    //>@����ָʾɫ
    if(m_Component->m_CurRight-m_Component->m_CurLeft > m_Component->m_OppMaxWidth ||
       m_Component->m_CurBottom-m_Component->m_CurTop > m_Component->m_OppMaxHeight)
        setColor(D_FRAMEOVER_COLOR);
    else
        setColor(D_NORMAL_COLOR);
    update();
    //m_Mutex->unlock();
}

void DiagramFrame::SetFrameRect(QRectF pRect)
{
    setParentItem((QGraphicsItem*)0);
    setGeometry(pRect.adjusted(-FRAMEVALIDWIDTH, -FRAMEVALIDWIDTH, FRAMEVALIDWIDTH, FRAMEVALIDWIDTH));
}

void DiagramFrame::slot_SceneRectChanged(QRectF pRect)
{
    SetFrameRect(pRect);
}

void DiagramFrame::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event == 0 || !m_SelectFlag)
        return;
    m_Mutex->lock();
    //>@���û����һ�䣬������϶��쳣����
    m_Component->RefreshParameter();
    m_SavedComOperate = GetCursorShape(event->scenePos());  //>@������OverideCursor
    if(m_SavedComOperate == DRAGALL)
        QGraphicsWidget::mousePressEvent(event);
    m_Mutex->unlock();
}

void DiagramFrame::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event == 0 || !m_SelectFlag)
        return;
    static bool busy = false;
    if(!busy)
        busy = true;
    else
        return;

    m_Mutex->lock();
    UpdateFrame(event);
    m_Mutex->unlock();

    busy = false;
}

void DiagramFrame::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event == 0 || !m_SelectFlag)
        return;
    m_Mutex->lock();
    if(m_SavedComOperate == DRAGALL)
        QGraphicsWidget::mouseReleaseEvent(event);
    ReleaseCursorShape(m_SavedComOperate);
    m_Mutex->unlock();
}

void DiagramFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(!m_SelectFlag)
        return;
    static bool busy = false;
    if(!busy)
        busy = true;
    else
        return;

    m_Mutex->lock();
    //painter->translate(size().width()/2.0, size().height()/2.0);
    //>@�����߶�
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(m_Color, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF tmpRect = rect().adjusted(FRAMEVALIDWIDTH, FRAMEVALIDWIDTH, -FRAMEVALIDWIDTH, -FRAMEVALIDWIDTH);
    painter->drawRect(tmpRect);
    //>@����ʵ�ķ���
    painter->setBrush(m_Color);
    painter->setPen(QPen(m_Color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPointF tmpCenterPoint = tmpRect.center();
    //>@�����ϱ߿�
    //>@���ƴ󷽿�
    QPointF tmpPoint = tmpRect.topLeft();
    QPointF topleftPoint = tmpPoint - QPointF(VERTEXRADIUS, VERTEXRADIUS);
    QPointF bottomrightPoint = tmpPoint + QPointF(VERTEXRADIUS, VERTEXRADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
    //>@����С����
    QPointF referPoint = QPointF(tmpCenterPoint.x(), tmpPoint.y());
    topleftPoint = referPoint - QPointF(SIDERADIUS, SIDERADIUS);
    bottomrightPoint = referPoint + QPointF(SIDERADIUS, SIDERADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
    //>@�����ұ߿�
    //>@���ƴ󷽿�
    tmpPoint = tmpRect.topRight();
    topleftPoint = tmpPoint - QPointF(VERTEXRADIUS, VERTEXRADIUS);
    bottomrightPoint = tmpPoint + QPointF(VERTEXRADIUS, VERTEXRADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
    //>@����С����
    referPoint = QPointF(tmpPoint.x(), tmpCenterPoint.y());
    topleftPoint = referPoint - QPointF(SIDERADIUS, SIDERADIUS);
    bottomrightPoint = referPoint + QPointF(SIDERADIUS, SIDERADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
    //>@�����±߿�
    //>@���ƴ󷽿�
    tmpPoint = tmpRect.bottomRight();
    topleftPoint = tmpPoint - QPointF(VERTEXRADIUS, VERTEXRADIUS);
    bottomrightPoint = tmpPoint + QPointF(VERTEXRADIUS, VERTEXRADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
    //>@����С����
    referPoint = QPointF(tmpCenterPoint.x(), tmpPoint.y());
    topleftPoint = referPoint - QPointF(SIDERADIUS, SIDERADIUS);
    bottomrightPoint = referPoint + QPointF(SIDERADIUS, SIDERADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
    //>@������߿�
    //>@���ƴ󷽿�
    tmpPoint = tmpRect.bottomLeft();
    topleftPoint = tmpPoint - QPointF(VERTEXRADIUS, VERTEXRADIUS);
    bottomrightPoint = tmpPoint + QPointF(VERTEXRADIUS, VERTEXRADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
    //>@����С����
    referPoint = QPointF(tmpPoint.x(), tmpCenterPoint.y());
    topleftPoint = referPoint - QPointF(SIDERADIUS, SIDERADIUS);
    bottomrightPoint = referPoint + QPointF(SIDERADIUS, SIDERADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));

    painter->translate(tmpRect.width() / 2.0, tmpRect.height() / 2.0);
    QFont font("Arial", 10, QFont::Black, true);
    font.setUnderline(false);
    painter->setFont(font);
    //>@��������
    painter->drawText(QRectF(-50, -20, 100, 40), Qt::AlignCenter, getCurComName(m_Component->m_ComPath));
    m_Mutex->unlock();

    busy = false;
}

DiagramFrameItem::DiagramFrameItem(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::red;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF DiagramFrameItem::boundingRect() const
{
    QPointF LineStartPos = line().p1();
    QSizeF LineEndSize = QSizeF(line().p2().x() - line().p1().x(),
                                line().p2().y() - line().p1().y());
    return QRectF(LineStartPos.rx()-5,LineStartPos.ry()-5,
                  LineEndSize.width()+10,LineEndSize.height()+10);
}

QPainterPath DiagramFrameItem::shape() const
{
    QPointF LineStartPos = line().p1();
    QSizeF LineEndSize = QSizeF(line().p2().x() - line().p1().x(),
                                line().p2().y() - line().p1().y());
    QRectF targetf = QRectF(LineStartPos.rx()-10,LineStartPos.ry()-10,
                            LineEndSize.width()+20,LineEndSize.height()+20);
    QPainterPath path;
    path.addRect(targetf);
    return path;
}

void DiagramFrameItem::updatePosition(const QPolygonF &rect, int startIndex, int endIndex)
{
    int count = rect.count();
    if(!(count == 4 && startIndex < count && endIndex < count))
    {
        IDE_TRACE();
        return;
    }
    QLineF line(rect.at(startIndex), rect.at(endIndex));
    int nextIndex = endIndex+1;
    if(nextIndex > count-1)
        nextIndex=0;
    QLineF nextLine(rect.at(endIndex), rect.at(nextIndex));
    myValidLen = nextLine.length();
    setLine(line);
}

void DiagramFrameItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //>@�����߶�
    painter->setPen(QPen(myColor, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line());
    if (isSelected())
    {
        painter->setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
    //>@����ʵ�ķ���
    painter->setBrush(myColor);
    painter->setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPointF topleftPoint = line().p1() - QPointF(VERTEXRADIUS, VERTEXRADIUS);
    QPointF bottomrightPoint = line().p1() + QPointF(VERTEXRADIUS, VERTEXRADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
    //>@����С����
    QPointF referPoint = line().pointAt(0.5);
    topleftPoint = referPoint - QPointF(SIDERADIUS, SIDERADIUS);
    bottomrightPoint = referPoint + QPointF(SIDERADIUS, SIDERADIUS);
    painter->drawRect(QRect(topleftPoint.toPoint(), bottomrightPoint.toPoint()));
}
