/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "pieinstrument.h"

PieInstrument::PieInstrument(QWidget *parent)
    : QWidget(parent)
{
}

PieInstrument::~PieInstrument()
{

}

void PieInstrument::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    // ȥ������
    painter.setPen(Qt::NoPen);
    // ���÷����
    painter.setRenderHint(QPainter::Antialiasing);
    //******************************************** Բ��ͼ *****************************************//
    if(1)
    {
        // ����Բ��
        QRectF rect_top(10.0, 20.0, 280.0, 160.0);
        // �ײ�Բ��
        QRectF rect_bottom(10.0, 60.0, 280.0, 160.0);
        // �м����
        QRectF rect_midd(10.0, 100.0, 280.0, 40.0);
        // ������ʼ�Ƕ�
        int startAngle = 230 * 16;
        // ���θ��Ƿ�Χ
        int spanAngle = 60 * 16;
        painter.setBrush(QColor(97,35,35,255));
        // ���Ƶײ�Բ��
        painter.drawEllipse(rect_bottom);
        // �����м����
        painter.drawRect(rect_midd);
        painter.setBrush(QColor(Qt::darkYellow));
        // ���Ƶײ�����
        painter.drawPie(rect_bottom, startAngle, spanAngle);
        // ���ε����뻡�Ľ���
        double pi = 3.1415926;
        double dx1 = rect_top.width() * 0.5 * cos(230 * pi / 180);
        double dy1 = rect_top.height() * 0.5 * sin(230 * pi / 180);
        double dx2 = rect_top.width() * 0.5 * cos(290 * pi / 180);
        double dy2 = rect_top.height() * 0.5 * sin(290 * pi / 180);
        // �󽻵������ֵ
        QPointF posBackCenter = QPointF(rect_top.center());
        double dX1 = posBackCenter.x() + dx1 + 0.5;
        double dY1 = posBackCenter.y() - dy1 + 0.5;
        double dX2 = posBackCenter.x() + dx2 + 0.5;
        double dY2 = posBackCenter.y() - dy2 + 0.5;
        // ��¼����
        QPointF topLeft = QPointF(dX1, dY1);
        QPointF bottomRight = QPointF(dX2, dY2) + QPointF(0,40);
        QPointF bottomLeft = topLeft + QPointF(0,40);
        painter.setBrush(QColor(Qt::darkYellow));
        // �����������ε�����
        QPolygonF path;
        path << topLeft << QPointF(dX2,dY2) << bottomRight << bottomLeft;
        painter.drawPolygon(path);
        // ���ƶ���Բ��
        painter.setBrush(QColor(156,56,56,255));
        painter.drawEllipse(rect_top);
        // ���ƶ�������
        painter.setBrush(QColor(Qt::yellow));
        painter.drawPie(rect_top, startAngle, spanAngle);
    }
    QWidget::paintEvent(e);
}
