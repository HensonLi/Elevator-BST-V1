/*
    1����ִ��focusin����ִ��mousepressevent
    2��
*/

#include "toolbase.h"

ToolBase::ToolBase()
{
    m_IsChanged = false;
    //>@��ʵ����һ���ؼ�ʱ�����ڹ��캯��֮ǰ�õ�focus�����ͻ���ɿؼ�һ��ʼ���ͻ�ý����bug����˴˴���Ӳ��Ϊ��ʼ��û�л�ý��㡣
    m_LogicState = STATE_UNFOCUS;
}

void ToolBase::FocusInEvent()
{
    if(m_LogicState == STATE_UNFOCUS)
    {
        m_LogicState = STATE_FOCUS;
    }
    PaintBorder();
}

void ToolBase::FocusOutEvent()
{
    if(m_LogicState != STATE_UNFOCUS)
    {
        m_LogicState = STATE_UNFOCUS;
    }
    PaintBorder();
}

void ToolBase::MouseEvent(QMouseEvent* e)
{
    QEvent::Type type = e->type();
    switch(m_LogicState)
    {
        //>@��û�еõ��������뽹������µĴ�����
        case STATE_UNFOCUS:
        {
            if(type == QEvent::MouseButtonPress)       //>@���뽹��ģʽ
            {
#ifdef IDE
                m_LogicState = STATE_EDITABLE;
#else
                m_LogicState = STATE_FOCUS;
#endif
                PaintBorder();
            }
            else if(type == QEvent::MouseButtonDblClick)       //>@ֱ�ӽ���ɱ༭ģʽ
            {
                m_LogicState = STATE_EDITABLE;
                PaintBorder();
            }
            break;
        }
        //>@�ڵõ��������뽹�������
        case STATE_FOCUS:
        {
            if(type == QEvent::MouseButtonDblClick)       //>@˫������ɱ༭ģʽ
            {
                m_LogicState = STATE_EDITABLE;
                PaintBorder();
            }
#ifdef IDE
            else if(type == QEvent::MouseButtonPress)       //>@���뽹��ģʽ
            {
                m_LogicState = STATE_EDITABLE;
                PaintBorder();
            }
#endif
            break;
        }
        //>@�ڿɱ༭״̬�µĴ�����
        case STATE_EDITABLE:
        {
            ProcessMouseEvent(e);
            break;
        }
        default:
            break;
    }
}

//>@���ݱ����m_LogicState״̬��������ذ�������
void ToolBase::KeyEvent(QKeyEvent *e)
{
    switch(m_LogicState)
    {
        //>@��û�еõ��������뽹������µĴ�����
        case STATE_UNFOCUS:
        {
            //>@do nothing
            break;
        }
        //>@�ڵõ��������뽹������£������Enter���ɽ���ɱ༭״̬
        case STATE_FOCUS:
        {
            if(e->key() == Qt::Key_Enter)
            {
                m_LogicState = STATE_EDITABLE;
                PaintBorder();
            }
            break;
        }
        //>@�ڿɱ༭״̬�µĴ������������ESC�����˳��ɱ༭״̬�����뽹��״̬
        case STATE_EDITABLE:
        {
            if(e->key() == Qt::Key_Escape)
            {
                m_LogicState = STATE_FOCUS;
                PaintBorder();
            }
            else
                ProcessKeyEvent(e);
            break;
        }
        default:
            break;
    }
    IDE_TRACE_INT(m_LogicState);
}

void ToolBase::WheelEvent(QWheelEvent* e)
{
    switch(m_LogicState)
    {
        //>@��û�еõ��������뽹������µĴ�����
        case STATE_UNFOCUS:
        {
            //>@do nothing
            break;
        }
        //>@�ڵõ��������뽹�������
        case STATE_FOCUS:
        {
            //>@do nothing
            break;
        }
        //>@�ڿɱ༭״̬�µĴ�����
        case STATE_EDITABLE:
        {
            ProcessWheelEvent(e);
            break;
        }
        default:
            break;
    }
}

void ToolBase::ProcessMouseEvent(QMouseEvent* e)
{

}

void ToolBase::ProcessKeyEvent(QKeyEvent *e)
{

}

void ToolBase::ProcessWheelEvent(QWheelEvent* e)
{

}

bool ToolBase::isFocus()
{
    return false;
}

//>@����״̬�����Ʋ�ͬ״̬�µ�StyleSheet
void ToolBase::PaintBorder()
{

}

