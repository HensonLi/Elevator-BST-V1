#ifndef SWITCH_H
#define SWITCH_H

#include "toolbase.h"
#include <QPushButton>

class Switch : public QPushButton, public ToolBase
{
    Q_OBJECT
public:
    explicit Switch(QString pSwitch, QString pOnText, QString pOffText, QWidget *parent = 0);
    
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent *e);

    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);

    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

    //>@�麯����ʵ�����й���
    void ProcessMouseEvent(QMouseEvent* e);
    void ProcessKeyEvent(QKeyEvent *e);
    void ProcessWheelEvent(QWheelEvent *e);
    bool isFocus();
    void PaintBorder();
signals:
    void sChanged(QObject*);
public slots:
    void slot_Toggled(bool);
public:
    QString     m_OnText, m_OffText;
    QPixmap     m_CheckImage;
    QPixmap     m_NormalImage;
};

#endif // SWITCH_H
