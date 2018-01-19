#include "combobox.h"

ComboBox::ComboBox(QStringList pParaList, QString pSelectPara, QWidget *parent) :
    QComboBox(parent), ToolBase()
{
    setMinimumHeight(25);
    setMaximumHeight(25);
    setMinimumWidth(70);
    PaintBorder();
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    m_PressFlag = false;

    InitEnumList(pParaList, pSelectPara);
}

void ComboBox::InitEnumList(QStringList pList, QString pValue)
{
    clear();
    addItems(pList);
    setCurrentIndex(-1);
    if(pValue.isEmpty() == false)
    {
        for(int i=0;i<pList.count();i++)
        {
            if(pValue.compare(pList.at(i), Qt::CaseInsensitive) == 0)
            {
                setCurrentIndex(i);
                break;
            }
        }
    }
}

bool ComboBox::setCurrentText(QString pText)
{
    if(pText.isEmpty() == false)
    {
        for(int i=0;i<count();i++)
        {
            if(pText.compare(itemText(i), Qt::CaseInsensitive) == 0)
            {
                setCurrentIndex(i);
                return true;
            }
        }
    }
    return false;
}

void ComboBox::mousePressEvent(QMouseEvent* e)
{
    MouseEvent(e);
}

void ComboBox::mouseReleaseEvent(QMouseEvent* e)
{
    MouseEvent(e);
}

//>@��Ҫ����ȥ������ƶ�ѡ���ı��Ĺ���
void ComboBox::mouseMoveEvent(QMouseEvent* e)
{
    MouseEvent(e);
}

void ComboBox::mouseDoubleClickEvent(QMouseEvent* e)
{
    MouseEvent(e);
}

void ComboBox::wheelEvent(QWheelEvent *e)
{
    WheelEvent(e);
}

void ComboBox::contextMenuEvent(QContextMenuEvent * e)
{

}

void ComboBox::keyPressEvent( QKeyEvent *e )
{
    KeyEvent(e);
}

void ComboBox::focusInEvent(QFocusEvent *e)
{
    FocusInEvent();
}

void ComboBox::focusOutEvent(QFocusEvent *e)
{
    FocusOutEvent();
}

void ComboBox::ProcessMouseEvent(QMouseEvent* e)
{
    int index = currentIndex();
    if(e->type() == QEvent::MouseButtonPress ||
       e->type() == QEvent::MouseButtonDblClick)
    {
        //showPopup(); //>@showpopup��һ���������ĺ�����Ҳ���ǵ���showPopup�����ͨ�����������Enter��ʹ֮hidepopup�������һ��showPopup��
        ++index;
        if(index > count())
            index = 0;
        else if(index < 0)
            index = 0;
        setCurrentIndex(index);
        m_PressFlag = true;
    }
    //>@����showPopup��combox��ʧȥ���㣬��menupopup�õ����㣬���ǣ�release�����ź���combox�в����á���������QEvent::MouseButtonRelease�źŲ����á�
    else if(e->type() == QEvent::MouseButtonRelease)
    {
        QComboBox::mouseReleaseEvent(e);
    }
    else if(e->type() == QEvent::MouseMove)
    {
        QComboBox::mouseMoveEvent(e);
    }
}

void ComboBox::ProcessKeyEvent(QKeyEvent *e)
{
    int index = currentIndex();
    if(e->key() == Qt::Key_Enter)
    {
        ++index;
        if(index > count())
            index = 0;
        else if(index < 0)
            index = 0;
        setCurrentIndex(index);
        m_PressFlag = true;
    }
    else if(e->key() == Qt::Key_Escape)
    {
        setFocus();
    }
    else if(e->key() == Qt::Key_Up)
    {
        if(++index > count())
            return;
        setCurrentIndex(index);
    }
    else if(e->key() == Qt::Key_Down)
    {
        if(--index < 0)
            return;
        setCurrentIndex(index);
    }
}

void ComboBox::ProcessWheelEvent(QWheelEvent *e)
{
    //>@����һ�ι���15�ȣ�һ�εõ�deltaΪ120����˹����ľ��루Ҳ�����ֹ����ĵ�λ����Ϊdelta()/8/15
    int tmpDelta = e->delta();
    tmpDelta /= 8;
    tmpDelta /= 15;
    int index = currentIndex();
    if(tmpDelta > 0)
    {
        for(int i=0;i<tmpDelta;i++)
        {
            if(++index > count())
                break;
            setCurrentIndex(index);
        }
    }
    else if(tmpDelta < 0)
    {
        tmpDelta = -tmpDelta;
        for(int i=0;i<tmpDelta;i++)
        {
            if(--index < 0)
                break;
            setCurrentIndex(index);
        }
    }
}

bool ComboBox::isFocus()
{
    return hasFocus();
}

void ComboBox::PaintBorder()
{
    switch(m_LogicState)
    {
        case STATE_UNFOCUS: //>@���ñ���͸��
        {
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);border: 1px groove blue;");
            break;
        }
        case STATE_FOCUS:
        {
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);border: 2px groove magenta;");  //groove����  //>@solidʵ��
            break;
        }
        case STATE_EDITABLE:
        {
            setStyleSheet(QString("background-color: rgba(%1);color:rgba(%2);border: 2px groove magenta;").arg(D_EDITCOLOR).arg(D_TEXTCOLOR));
            break;
        }
        default:
        {
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);border: 1px groove blue;");
            break;
        }
    }
}

void ComboBox::currentIndexChanged(int index)
{
    if(m_PressFlag)
        m_LogicState = STATE_EDITABLE;
    PaintBorder();
    //>@��POPUP��ͨ��key����mouseѡ����һ��ѡ��󣬴˿ؼ�����Ϊ�Ѿ�ʧȥ���㣬������Ҫ�������ӵ�н���
    setFocus();
    emit sChanged(this);
}
