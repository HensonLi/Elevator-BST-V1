#ifndef ANALYSISFORM_H
#define ANALYSISFORM_H

#include "global.h"
#include "interfaceinc.h"
#include "serialport.h"
#include "logicnetmanager.h"

namespace Ui {
class AnalysisForm;
}

class AnalysisForm : public QWidget
{
    Q_OBJECT
public:
    explicit AnalysisForm(QWidget *parent = 0);
    AnalysisForm(QString pProtocol, QWidget *parent = 0);
    ~AnalysisForm();
    
    bool AddParseUI(QString pProtocol, QDomElement pElement);
    bool DelParseUI(QString pProtocol);

    void ClearLog();
    bool SaveLog();
    bool LoadLog();

    InterfaceBase* GetInterface();
    HostNetManager* GetNetManager();

private slots:
    void on_InterfaceTypeCombo_currentIndexChanged(const QString &arg1);
    void on_ProtocolTypeCombo_currentIndexChanged(const QString &arg1);
    void on_ParseTab_currentChanged(int index);
    void on_BtnOpen_clicked();
    void on_BtnSearch_clicked();
    void on_BtnRefresh_clicked();
    void on_BtnSave_clicked();
    void on_BtnClear_clicked();
    void on_BtnConnect_clicked(bool checked);

    void slot_ReadUartFrame();

public:
    SerialPort      *m_UartPort;        //>@�����շ���֡����
    HostNetManager  *m_SocketPort;      //>@��ǰ���ӵ��豸HostNetManager���йؽӿ�֡������

    InterfaceBase   *m_Interface;

private:
    Ui::AnalysisForm *ui;
};

#endif // ANALYSISFORM_H
