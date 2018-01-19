#include "speedinstrument.h"
#include <QtGui>

SpeedInstrument::SpeedInstrument(QWidget *parent) :
    QWidget(parent)
{
    m_DragFlag = false;
}

void SpeedInstrument::init()                    //初始化指�?
{
    setValue(0);
}

void SpeedInstrument::mousePressEvent(QMouseEvent *e)
{
    m_DragFlag = true;
}

void SpeedInstrument::mouseMoveEvent(QMouseEvent *e)
{
    if(m_DragFlag)
    {
        if(mValue < 288)
            setValue(mValue+1);
    }
}

void SpeedInstrument::mouseReleaseEvent(QMouseEvent *e)
{
    m_DragFlag = false;
}

void SpeedInstrument::focusOutEvent(QFocusEvent *e)
{
    m_DragFlag = false;
}

void SpeedInstrument::paintEvent(QPaintEvent *)
{
    //����ָ��Ĺؼ���
    static const QPoint hourHand[8] =
    {
        QPoint(4, -20),
        QPoint(4, -60),
        QPoint(1, -70),
        QPoint(1, -80),
        QPoint(-1, -80),
        QPoint(-1, -70),
        QPoint(-4, -60),
        QPoint(-4,-20)
    };
    // ������ɫ
    QColor pointer(200,0,0);
    QColor hourColor(255, 255, 255);
    QColor minuteColor(255,122,29);
    QColor textColor(255, 255, 255);

    int side = qMin(width(), height());                //ȡ��Сֵ
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);//�������
    painter.setPen(Qt::black);
    //painter.setBrush(Qt::black);

    painter.translate(width() / 2, height() / 2);       //������֮һ��͸���Ϊԭ��

    QConicalGradient conicalGradient(0,0,-54);
    //������һ��QConicalGradient����ʵ���������ֱ�Ϊ��������ͳ�ʼ�Ƕ�
    conicalGradient.setColorAt(0,Qt::darkRed);
    conicalGradient.setColorAt(0.2,Qt::red);
    conicalGradient.setColorAt(0.4,Qt::magenta);
    conicalGradient.setColorAt(0.6,Qt::darkGray);
    conicalGradient.setColorAt(0.8,Qt::black);//���ý������ɫ��·������
    painter.setBrush(QBrush(conicalGradient));

    QRectF rect(-(side/2-6), -(side/2-6), side-12, side-12);
    int startAngle = -54 * 16;
    int spanAngle = (180+110) * 16;
    painter.drawChord(rect, startAngle, spanAngle);

//    painter.fillRect(rect(), QBrush(Qt::black));       //���ñ�����ɫ
    painter.scale(side / 300.0, side / 300.0);          //����ͼƬ��С
    //����ָ������������ɫ
    painter.setPen(Qt::NoPen);
    painter.setBrush(pointer);
    //����ʱ�̶�
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(hourColor);
    for (int i = 0; i < 9; ++i)
    {
        painter.drawLine(-53, 73, -62, 85);
        painter.rotate(36.0);
    }
    painter.restore();
    //���÷̶ֿ�
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(minuteColor);
    for (int j = 0; j < 8; ++j)
    {
        painter.drawLine(-73, 53, -85, 62);
        painter.rotate(36.0);
    }
    painter.restore();
    //���ÿ̶�ֵ
    painter.setPen(textColor);
    painter.drawText(-61, 57,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(0));//�ӣ�x,y����ʼ,��Ϊ30,��Ϊ30
    painter.drawText(-90, 15,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(10));
    painter.drawText(-90, -35,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(20));
    painter.drawText(-62, -71,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(30));
    painter.drawText(-15, -90,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(40));
    painter.drawText(32, -71,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(50));
    painter.drawText(62, -31,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(60));
    painter.drawText(60, 20,30, 30,Qt::AlignHCenter | Qt::AlignTop, QString::number(70));
    painter.drawText(35, 55,30, 30,Qt::AlignHCenter | Qt::AlignTop, QString::number(80));
    //��Բ
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::white,2,Qt::SolidLine));
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawEllipse(-20,-20,40,40);
    painter.restore();
    //��ָ��
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.rotate(180+36+mValue*288/80);
    painter.drawConvexPolygon(hourHand, 8);
    painter.restore();
    //������Բ��
    painter.save();
    painter.setPen(QPen(Qt::red,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,-54*16,72*16);
    painter.restore();
    painter.save();
    painter.setPen(QPen(Qt::yellow,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,18*16,144*16);
    painter.restore();
    painter.save();
    painter.setPen(QPen(Qt::white,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,162*16,72*16);
    painter.restore();
    //��Բ������
    QFont font;
    font.setPointSize(12);
    setFont(font);

    painter.drawText(-13, -11,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(mValue));
 }

void SpeedInstrument::setValue(int newvalue)
{
    mValue= newvalue;
    update();
}

