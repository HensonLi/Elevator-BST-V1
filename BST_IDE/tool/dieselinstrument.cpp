#include "dieselinstrument.h"

DieselInstrument::DieselInstrument(QWidget *parent) :
    QWidget(parent)
{
}

void DieselInstrument::init()
{
    mValue= 0;
    update();
}

void DieselInstrument::setValue(int newvalue)
{
    mValue= newvalue;
    update();
}

void DieselInstrument::paintEvent(QPaintEvent *event)
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
    QPainter tmPainter(this);
    tmPainter.setRenderHint(QPainter::Antialiasing, true);//�������
    tmPainter.setPen(Qt::black);
    tmPainter.setBrush(Qt::black);

    tmPainter.translate(width() / 2, height() / 2);       //������֮һ��͸���Ϊԭ��

    QRectF rect(-(side/2-6), -(side/2-6), side-12, side-12);
    int startAngle = -54 * 16;
    int spanAngle = (180+110) * 16;
    tmPainter.drawChord(rect, startAngle, spanAngle);

//    tmPainter.fillRect(rect(), QBrush(Qt::black));       //���ñ�����ɫ
    tmPainter.scale(side / 300.0, side / 300.0);          //����ͼƬ��С
    //����ָ������������ɫ
    tmPainter.setPen(Qt::NoPen);
    tmPainter.setBrush(pointer);
    //����ʱ�̶�
    tmPainter.save();
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.setPen(hourColor);
    for (int i = 0; i < 9; ++i)
    {
        tmPainter.drawLine(-53, 73, -62, 85);
        tmPainter.rotate(36.0);
    }
    tmPainter.restore();
    //���÷̶ֿ�
    tmPainter.save();
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.setPen(minuteColor);
    for (int j = 0; j < 8; ++j)
    {
        tmPainter.drawLine(-73, 53, -85, 62);
        tmPainter.rotate(36.0);
    }
    tmPainter.restore();
    //���ÿ̶�ֵ
    tmPainter.setPen(textColor);
    tmPainter.drawText(-61, 57,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(0));//�ӣ�x,y����ʼ,��Ϊ30,��Ϊ30
    tmPainter.drawText(-90, 15,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(10));
    tmPainter.drawText(-90, -35,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(20));
    tmPainter.drawText(-62, -71,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(30));
    tmPainter.drawText(-15, -90,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(40));
    tmPainter.drawText(32, -71,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(50));
    tmPainter.drawText(62, -31,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(60));
    tmPainter.drawText(60, 20,30, 30,Qt::AlignHCenter | Qt::AlignTop, QString::number(70));
    tmPainter.drawText(35, 55,30, 30,Qt::AlignHCenter | Qt::AlignTop, QString::number(80));
    //��Բ
    tmPainter.save();
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.setPen(QPen(Qt::white,2,Qt::SolidLine));
    tmPainter.setBrush(QBrush(Qt::NoBrush));
    tmPainter.drawEllipse(-20,-20,40,40);
    tmPainter.restore();
    //��ָ��
    tmPainter.save();
    tmPainter.setPen(Qt::NoPen);
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.rotate(180+36+mValue*288/80);
    tmPainter.drawConvexPolygon(hourHand, 8);
    tmPainter.restore();
    //������Բ��
    tmPainter.save();
    tmPainter.setPen(QPen(Qt::red,3,Qt::SolidLine));
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.drawArc(-105,-105,210,210,-54*16,72*16);
    tmPainter.restore();
    tmPainter.save();
    tmPainter.setPen(QPen(Qt::yellow,3,Qt::SolidLine));
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.drawArc(-105,-105,210,210,18*16,144*16);
    tmPainter.restore();
    tmPainter.save();
    tmPainter.setPen(QPen(Qt::white,3,Qt::SolidLine));
    tmPainter.setRenderHint(QPainter::Antialiasing,true);
    tmPainter.drawArc(-105,-105,210,210,162*16,72*16);
    tmPainter.restore();
    //��Բ������
    QFont font;
    font.setPointSize(12);
    setFont(font);

    tmPainter.drawText(-13, -11,30, 30,Qt::AlignHCenter | Qt::AlignTop,QString::number(mValue));
}


