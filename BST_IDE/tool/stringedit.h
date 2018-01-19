#ifndef STRINGEDIT_H
#define STRINGEDIT_H

#include "toolbase.h"
#include <QLineEdit>

#define ENETADDRMAXLEN 15

class StringEdit : public QLineEdit, public ToolBase
{
    Q_OBJECT
public:
    explicit StringEdit(QString pIP=QString(), QWidget *parent = 0);

    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent *e);

    void keyPressEvent(QKeyEvent *e);

    void contextMenuEvent(QContextMenuEvent * e);

    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

    //>@�麯����ʵ�����й���
    void ProcessMouseEvent(QMouseEvent* e);
    void ProcessKeyEvent(QKeyEvent *e);
    void ProcessWheelEvent(QWheelEvent *e);
    bool isFocus();
    void PaintBorder();

    void UpdateSelection(int pos);
    void UpdateSelectionData(Qt::Key pKey);

signals:
    void ShowKeyBoard(QPoint pos);
    void HideKeyBoard();

public slots:
    
};


#endif // ENETADDREDIT_H
