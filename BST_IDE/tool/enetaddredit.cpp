/*
lineEdit��ʵ���ı���text)��"192.168.0.1"��
������ʾ�Ͳ������ı�(displayText)Ϊ"192.168.0  .1  "��
Ҳ����˵QLineEdit����ʾ�ı���ʱ������InputMask��ʵ���ı�������䲹λ�����displayText�ĳ��ȴ���text�ĳ��ȣ�
��setSelection����text�ĳ���ȥ���start�Ƿ�Ϸ���
���start��ֵ���ڻ����text�ĳ��ȣ� ����ʾInvalid start position��
ͬʱ�� ��ѡ��displayText��startָ���ı��� ����displayText�����Ĳ�����Զ��ѡ�в��ˣ�
��Ӧ����QT��һ��BUG

����������
����mousePressEvent��mouseMoveEvent��mouseDoubleClickEvent��keyPressEvent��contextMenuEvent.
ΪʲôҪ����?:
    mousePressEvent:��ֹ�û��������ʾ����ʱcursor���ƶ�������ʾ�������֡�
    mouseMoveEvent:��ֹ�û��������קѡ��ʱѡ�С���ʾ�������ֵ��ı���
    mouseDoubleClickEvent:��ֹ�û�˫���ı������ʱѡ�����������ʾ��ȫ���ı���
    contextMenuEvent:QLineEdit���Ҽ��˵�����һ��С�Select All�������ǵ÷ϵ������ҸɵıȽϾ���ֱ�Ӳ����Ҽ��˵�����~
    keyPressEvent:����ͱȽ϶��ˡ�
        (1)�����Ǽ��̵��������Backspace�п��ܻ���cursor�ƶ�������ʾ�������֡�
        (2)Ȼ��������˵�����Ǹ���Select All�������⣬���и���Ӧ�Ŀ�ݼ���Ctrl + A������Ҳ���޸����������á�
        (3)�پ���Home���Ĺ��ܣ�����Ǻ����ױ����Եġ����ǿ���ѡ���Home�����²�����Ӧ
*/

#include "enetaddredit.h"

ENetAddrEdit::ENetAddrEdit(QString pIP, QWidget *parent) :
    QLineEdit(parent), ToolBase()
{
    setMinimumHeight(25);
    setMaximumHeight(25);
    setText(pIP);
    //>@��ΪToolBase()ִ��ʱ��ENetAddrEdit��δ��ʼ�������ToolBase()��PaintBorder��Ч�����ȥ���˴���PaintBorder��������ı����ʼʱδ��ʼ����
    PaintBorder();
    //>@�������뷶Χ�͸�ʽ
    //QRegExp rx("^(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])$");
    //setValidator(new QRegExpValidator(rx, this));
    setInputMask("000.000.000.000");
    setText("000.000.000.000");
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(slot_TextChanged(QString)));
}

void ENetAddrEdit::slot_TextChanged(QString)
{
    emit sChanged(this);
}

void ENetAddrEdit::mousePressEvent(QMouseEvent* e)
{
    MouseEvent(e);
}

//>@��Ҫ����ȥ������ƶ�ѡ���ı��Ĺ���
void ENetAddrEdit::mouseMoveEvent(QMouseEvent* e)
{

}

void ENetAddrEdit::mouseDoubleClickEvent(QMouseEvent* e)
{
    MouseEvent(e);
}

void ENetAddrEdit::wheelEvent(QWheelEvent *e)
{
    WheelEvent(e);
}

void ENetAddrEdit::keyPressEvent( QKeyEvent *e )
{
    KeyEvent(e);
}

void ENetAddrEdit::contextMenuEvent( QContextMenuEvent* e)
{
    //Do nothing
}

void ENetAddrEdit::focusInEvent(QFocusEvent *e)
{
    FocusInEvent();
    emit ShowKeyBoard(geometry().bottomLeft());
}

void ENetAddrEdit::focusOutEvent(QFocusEvent *e)
{
    FocusOutEvent();
    emit HideKeyBoard();
}

void ENetAddrEdit::ProcessMouseEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress ||
       e->type() == QEvent::MouseButtonDblClick)
    {
        int index = cursorPositionAt(e->pos());
        UpdateSelection(index);
    }
}

void ENetAddrEdit::ProcessKeyEvent(QKeyEvent *e)
{
    int index = cursorPosition();
    if(e->key() == Qt::Key_Left)
    {
        UpdateSelection(index-4);
    }
    else if(e->key() == Qt::Key_Right)
    {
        UpdateSelection(index+4);
    }
    //>@����ı�ѡ�����ֵİ����ź�
    else
    {
        UpdateSelectionData((Qt::Key)(e->key()));
    }
    //>@Ȼ����������ź�
    //>@����ȫѡ�ź�
    //if(event->matches(QKeySequence::SelectAll) ||      //>@��Щϵͳ�л����ȫѡ�Ŀ�ݼ�������ʹ��QKeySequence::SelectAll��ɿ���
    //   (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A))
    //{
    //    //Do nothing
    //}
}

void ENetAddrEdit::ProcessWheelEvent(QWheelEvent *e)
{
    //>@����һ�ι���15�ȣ�һ�εõ�deltaΪ120����˹����ľ��루Ҳ�����ֹ����ĵ�λ����Ϊdelta()/8/15
    int tmpDelta = e->delta();
    tmpDelta /= 8;
    tmpDelta /= 15;
    if(tmpDelta > 0)
    {
        for(int i=0;i<tmpDelta;i++)
            UpdateSelectionData(Qt::Key_Up);
    }
    else if(tmpDelta < 0)
    {
        for(int i=0;i<qAbs(tmpDelta);i++)
            UpdateSelectionData(Qt::Key_Down);
    }
}

bool ENetAddrEdit::isFocus()
{
    return hasFocus();
}

void ENetAddrEdit::PaintBorder()
{
    switch(m_LogicState)
    {
        case STATE_UNFOCUS: //>@���ñ���͸��
        {
            UpdateSelection(-1);
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);");
            break;
        }
        case STATE_FOCUS:
        {
            UpdateSelection(-1);
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);border: 2px groove magenta;");
            break;
        }
        case STATE_EDITABLE:
        {
            UpdateSelection(1);
            setStyleSheet("background-color: rgba(240,240,240,255);color:rgba(255,10,10,255);border: 2px groove magenta;");
            break;
        }
        default:
        {
            UpdateSelection(-1);
            setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(255,10,10,255);");
            break;
        }
    }
}

void ENetAddrEdit::UpdateSelection(int pos)
{
    if(pos <= 3)
    {
        setCursorPosition(0);   //>@�˴��ɼӿɲ��ӣ�ʹ�ô��д�����޸Ĺ��λ�ã���ʹ֮ǰѡ�е��ַ�����Ϊ��ѡ��
        setSelection(0, 3);
    }
    else if(pos > 3 && pos <= 7)
    {
        setCursorPosition(4);
        setSelection(4, 3);
    }
    else if(pos > 7 && pos <= 11)
    {
        setCursorPosition(8);
        setSelection(8, 3);
    }
    else if(pos > 11)
    {
        setCursorPosition(12);
        setSelection(12, 3);
    }
}

//>@���ұ߲���/ɾ��һ�����֣�����ͨ�����¼����Ӽ�������
void ENetAddrEdit::UpdateSelectionData(Qt::Key pKey)
{
    if(hasSelectedText())
    {
        int start = selectionStart();
        QString tmpData = selectedText();
        if(pKey == Qt::Key_Backspace)
        {
            tmpData.remove(2,1);
            tmpData.insert(0,'0');
        }
        else if(pKey == Qt::Key_Up)
        {
            int tmpInt = tmpData.toInt();
            if(tmpInt < 255)
            {
                tmpInt++;
                tmpData = QString("%1%2%3").arg(tmpInt/100)
                                           .arg(tmpInt/10%10)
                                           .arg(tmpInt%10);
            }
            else
                return;
        }
        else if(pKey == Qt::Key_Down)
        {
            int tmpInt = tmpData.toInt();
            if(tmpInt > 0)
            {
                tmpInt--;
                tmpData = QString("%1%2%3").arg(tmpInt/100)
                                           .arg(tmpInt/10%10)
                                           .arg(tmpInt%10);
            }
            else
                return;
        }
        else if(pKey >= Qt::Key_0 &&
                pKey <= Qt::Key_9)
        {
            tmpData.remove(0,1);
            tmpData.append((char)pKey);
        }
        else
        {
            return;
        }
        insert(tmpData);
        setSelection(start, 3);
        update();
    }
}
