#include "mainwindow.h"
#include "deviceinfo.h"
#include "eventmap.h"
#include "openfileform.h"
#include "projectmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GlobalPara.m_TabPageManager = ui->tabWidget;

    CreateSysTrayIcon();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(CreateContextMenu(const QPoint&)));

    //��QTDesigner��ʹ���һ�������->���������ô���Ϊդ�񲼾ֺ��Զ�����gridLayout��
    //���Զ�����ΪMarginΪ11����Ҫ�ڴ˴�����Ϊ0������TabWidget�����������кܴ�ķ�϶��
    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setContentsMargins(0, 0, 0, 0);

    InitUI();
}

MainWindow::~MainWindow()
{
    extendToolBar->deleteLater();
    helpToolBar->deleteLater();
    delete ui;
}

void MainWindow::InitUI()
{
    GlobalPara.m_ProjectBar = new ProjectBar(this);
    addToolBar(Qt::LeftToolBarArea, GlobalPara.m_ProjectBar);
    ProjectBar *tmpProjectBar = VAR_CAST<ProjectBar*>(GlobalPara.m_ProjectBar);
    //menuBar()->addMenu(GlobalPara.m_SqlToolBar->GenerateMenu());
    tmpProjectBar->mProjectManager->m_DevToolBar = new DeviceToolBar(this);
    addToolBar(Qt::LeftToolBarArea, tmpProjectBar->mProjectManager->m_DevToolBar);
    //menuBar()->addMenu(m_DevToolBar->GenerateMenu());

    GlobalPara.m_SqlToolBar = new SQLToolBar(this);
    addToolBar(Qt::RightToolBarArea, GlobalPara.m_SqlToolBar);
    //menuBar()->addMenu(GlobalPara.m_SqlToolBar->GenerateMenu());

    //>@��Ҫ��ProjectBar��ʼ��֮���ٽ�������Ϊ���¼���Toolbar����Ҫ����ProjectBar
    ui->tabWidget->fileToolBar = new FileToolBar(this);
    addToolBar(Qt::TopToolBarArea, ui->tabWidget->fileToolBar);
    menuBar()->addMenu(ui->tabWidget->fileToolBar->GenerateMenu());

    ui->tabWidget->editToolBar = new EditToolBar(this);
    addToolBar(Qt::TopToolBarArea, ui->tabWidget->editToolBar);
    menuBar()->addMenu(ui->tabWidget->editToolBar->GenerateMenu());

    ui->tabWidget->textToolBar = new TextToolBar(this);
    addToolBar(Qt::TopToolBarArea, ui->tabWidget->textToolBar);
    ui->tabWidget->textToolBar->setVisible(false);
    //menuBar()->addMenu(ui->tabWidget->textToolBar->GenerateMenu());

    GlobalPara.m_DockViewToolBar = new DockViewToolBar(this);
    addToolBar(Qt::TopToolBarArea, GlobalPara.m_DockViewToolBar);
    DockViewToolBar *tmpViewBar = VAR_CAST<DockViewToolBar*>(GlobalPara.m_DockViewToolBar);
    menuBar()->addMenu(tmpViewBar->GenerateMenu());
    ui->mainToolBar->addWidget(tmpViewBar->GenerateUserModeCombox());

    extendToolBar = new ExtendToolBar(this);
    addToolBar(Qt::RightToolBarArea, extendToolBar);
    menuBar()->addMenu(extendToolBar->GenerateMenu());

    helpToolBar = new HelpToolBar(this);
    addToolBar(Qt::TopToolBarArea, helpToolBar);
    menuBar()->addMenu(helpToolBar->GenerateMenu());

    //>@״̬��
    GlobalPara.statusBar = statusBar();
    GlobalPara.docEditStatus = new QLabel;
    GlobalPara.docEditStatus->setMinimumSize(100, 20);
    GlobalPara.docEditStatus->setFrameShadow(QFrame::Sunken);
    GlobalPara.docEditStatus->setFrameShape(QFrame::NoFrame);
    GlobalPara.statusBar->addWidget(GlobalPara.docEditStatus);
    //>@
    GlobalPara.keyboardStatus = new QLabel;
    GlobalPara.keyboardStatus->setMinimumSize(100, 20);
    GlobalPara.keyboardStatus->setFrameShadow(QFrame::Sunken);
    GlobalPara.keyboardStatus->setFrameShape(QFrame::NoFrame);
    GlobalPara.statusBar->addWidget(GlobalPara.keyboardStatus);
    //>@
    GlobalPara.searchStatus = new QLabel;
    GlobalPara.searchStatus->setAttribute(Qt::WA_DeleteOnClose);
    GlobalPara.searchStatus->setMinimumSize(100, 20);
    GlobalPara.searchStatus->setFrameShadow(QFrame::Sunken);
    GlobalPara.searchStatus->setFrameShape(QFrame::NoFrame);
    GlobalPara.statusBar->addWidget(GlobalPara.searchStatus);
    //>@
    GlobalPara.progressStatus = new QProgressBar;
    GlobalPara.progressStatus->setAttribute(Qt::WA_DeleteOnClose);
    GlobalPara.progressStatus->setMinimumSize(100, 20);
    GlobalPara.progressStatus->hide();
    GlobalPara.statusBar->addWidget(GlobalPara.progressStatus);

    //OpenStartPage();
}

void MainWindow::CreateSysTrayIcon()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    //����setTrayIconVisible�źſ�����ʾ������ϵͳͼ��
    connect(this, SIGNAL(setTrayIconVisible(bool)), trayIcon, SLOT(setVisible(bool)));
    //systrayicon����message���ں󣬵�����ڴ������¼�
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(TrayIconMessageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->setIcon(QIcon(":/Page/rc/Page/bstpro.png"));
    trayIcon->show();
    showTrayIconMessage();
}

//��ϵͳͼ�괦��ʾtip��Ϣ
void MainWindow::showTrayIconMessage()
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
    trayIcon->showMessage(QString::fromLocal8Bit("��ʾ"),
                          QString::fromLocal8Bit("Ӧ�ó���������"),
                          icon,
                          /*��ʱ*/1000);
}

//systrayicon����message���ں󣬵�����ڴ������¼�
void MainWindow::TrayIconMessageClicked()
{
    QMessageBox::information(0, QString::fromLocal8Bit("��ʾ"),
                                QString::fromLocal8Bit("Ӧ�ó���������"));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason pReason)
{

}

void MainWindow::OpenStartPage()
{
    ui->tabWidget->AddTabPage("StartPage", TYPE_START, ":/Menu/rc/Menu/new.png");
}

void MainWindow::OpenManufactureTool()    //>@��������
{
    ui->tabWidget->AddTabPage(QString("Manufacture"), TYPE_MANUFACTURE, ":/Page/rc/Page/manufacture.png");
}

void MainWindow::OpenUpgradeTool()    //>@��������
{
    ui->tabWidget->AddTabPage(QString("Upgrade"), TYPE_UPGRADE, ":/Menu/rc/Menu/undo.png");
}

void MainWindow::OpenNetworkManagerTool()    //>@���繤��
{
    ui->tabWidget->AddTabPage(QString("NetworkManager"), TYPE_NETWORKMANAGER, ":/Network/rc/Network/wlan.svg");
}

void MainWindow::OpenAsciiCodecTool()
{
    ui->tabWidget->AddTabPage(QString("TextCodec"), TYPE_ASCIICODEC, ":/Menu/rc/Menu/font.png");
}

void MainWindow::OpenFaceDetectTool()
{
    ui->tabWidget->AddTabPage(QString("FaceDetect"), TYPE_FACEDETECT, ":/Menu/rc/Menu/face.png");
}

void MainWindow::OpenRcManagerTool()
{
    ui->tabWidget->AddTabPage(QString("RcManager"), TYPE_RC, ":/Menu/rc/Menu/rcView.png");
}

void MainWindow::OpenFtpTool()
{
    ui->tabWidget->AddTabPage(QString("FtpTool"), TYPE_FTP, ":/Menu/rc/Menu/ftp.png");
}

void MainWindow::OpenChatForm()
{
    ui->tabWidget->AddTabPage(BROADCASTIP, TYPE_CHAT, ":/Menu/rc/Menu/chat.png");
}

void MainWindow::OpenMapsForm()
{
    ui->tabWidget->AddTabPage(QString("Maps"), TYPE_MAPS, ":/Menu/rc/Menu/map.png");
}

void MainWindow::OpenSmsForm()
{
    ui->tabWidget->AddTabPage(QString("StreamMedia"), TYPE_SMS, ":/Menu/rc/Menu/media.png");
}

void MainWindow::OpenSQLServerForm()
{
    if(GlobalPara.m_SqlDatabase.isOpen() == false)
    {
        SplashForm splash;
        int ret = splash.exec();
        if(ret == QDialog::Rejected)
        {
            return;
        }
    }
    ui->tabWidget->AddTabPage(QString("SQL Server"), TYPE_SQLSERVER, ":/Menu/rc/Menu/database.png");
}

void MainWindow::OpenMonitorForm()
{
    ui->tabWidget->AddTabPage(QString("MonitorForm"), Type_MONITOR, ":/Menu/rc/Menu/monitor.png");
}

void MainWindow::OpenImageEditor()
{
    ui->tabWidget->AddTabPage(QString("ImageEdiotr"), TYPE_IMAGEEDITOR, ":/Page/rc/Page/jpg.png");
}

void MainWindow::OpenAudioEditor()
{
    ui->tabWidget->AddTabPage(QString("AudioEdiotr"), TYPE_AUDIOEDITOR, ":/Page/rc/Page/mp3.png");
}

void MainWindow::OpenVideoEditor()
{
    ui->tabWidget->AddTabPage(QString("VideoEdiotr"), TYPE_VIDEOEDITOR, ":/Page/rc/Page/mp4.png");
}

void MainWindow::OpenFontEditor()
{
    ui->tabWidget->AddTabPage(QString("TextEdiotr"), TYPE_FONTEDITOR, ":/Page/rc/Page/txt.png");
}

void MainWindow::context()
{

}
void MainWindow::index()
{

}
void MainWindow::technicalSupport()
{

}
void MainWindow::reportBug()
{

}
void MainWindow::checkUpdate()
{

}
void MainWindow::aboutIDE()
{

}
void MainWindow::aboutBST()
{

}

void MainWindow::addNewTool()
{

}
