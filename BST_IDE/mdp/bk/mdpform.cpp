#include "mdpform.h"
#include "mainwindowclass.h"
#include "ui_mdpform.h"

MDPForm::MDPForm(QString pPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MDPForm)
{
    ui->setupUi(this);
    InitPara();
    SycncUIFromMDP(pPath);
}

MDPForm::~MDPForm()
{
    delete ui;
}
/**
//>@Sycnc*****************************************************
*/
void MDPForm::SycncUIFromMDP(QString pPath)
{
    mMDPPath = pPath;
    if(!OpenBootMdp(mMDPPath))
    {
        if(!CreatBootMdp(mMDPPath))
            return;
    }
    QObjectList OBList;
    if(!SearchOB(OBList, ui->PHInfoPage, 7))
        return;
    if(!PreMapToUI(OBList))
        return;
    QStringList labelList = mMdpManager->m_CfgparaElementGroup.keys();
    for(int i=0;i<labelList.count();i++)
    {
        QString label = labelList.at(i);
        QDomElement parElement =  mMdpManager->m_CfgparaElementGroup.value(label);
        if(parElement.isNull())
            continue;
        QDomNodeList nodeList = parElement.childNodes();
        for(int j=0;j<nodeList.count();j++)
        {
            QDomNode node = nodeList.at(j);
            if(node.isNull())
                continue;
            QDomElement element = node.toElement();
            if(element.isNull())
                continue;
            QStringList listLabel = label.split("/", QString::SkipEmptyParts);
            if(listLabel.count() != 2)
                continue;
            QString key = QString("%1_%2_%3_%4_").
                    arg(listLabel.at(0)).
                    arg(listLabel.at(1)).
                    arg(element.tagName()).
                    arg(element.attribute("Para"));
            QStringList list = element.attribute("List").split(";", QString::SkipEmptyParts);
            QString vaule = element.text();
            for(int i=0;i<OBList.count();i++)
            {
                QObject* OB = OBList.at(i);   
                MapToUI(key, list, vaule, OB);
            }
        }
    }
}

void MDPForm::SycncMDPFromUI()
{
    if(mMdpManager == 0)
        return;
    mMdpManager->ClrMDP();
    DomInfo domInfo;
    QObjectList OBList;
    if(!SearchOB(OBList, ui->PHInfoPage, 6))
        return;
    for(int i=0;i<OBList.count();i++)
    {
        QObject* OB = OBList.at(i);
        if(MapToDom(domInfo, OB))
        {
            mMdpManager->AddDomNewNode(domInfo);
        }
    }
    SaveBootMdp();
}
/**
//>@UICtrl*****************************************************
*/
void MDPForm::on_PHTypeList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->PHInfoPage->setCurrentIndex(ui->PHTypeList->row(current));
//    ui->HardWareIcon->setPixmap(current->icon().pixmap(ui->HardWareIcon->size()));
}

void MDPForm::on_Sreen_LCD_Resolution_Enum_Vaule_valueChanged(int value)
{
    QString paras = ui->Sreen_LCD_Resolution_Enum_Vaule->toolTip();
    QStringList paraList = paras.split(";", QString::SkipEmptyParts);
    if(value >= paraList.count())
        return;
    ui->LcdResolutionVaule->setText(paraList.at(value));
}

void MDPForm::on_Sreen_LCD_luminance_Num_Vaule_valueChanged(int value)
{
    ui->LcdLuminanceVaule->setText(QString("%1").arg(value));
}

void MDPForm::on_Sreen_LCD_Sound_Num_Vaule_valueChanged(int value)
{
    ui->LcdSoundVaule->setText(QString("%1").arg(value));
}

void MDPForm::on_Sreen_USBCAM_Resolution_Enum_Vaule_valueChanged(int value)
{
    QString paras = ui->Sreen_USBCAM_Resolution_Enum_Vaule->toolTip();
    QStringList paraList = paras.split(";", QString::SkipEmptyParts);
    if(value >= paraList.count())
        return;
    ui->USBCamResolutionVaule->setText(paraList.at(value));
}

void MDPForm::on_SaveBtn_clicked()
{
    SycncMDPFromUI();
}

void MDPForm::on_RefreshBtn_clicked()
{
    SycncUIFromMDP(mMDPPath);
}

void MDPForm::on_AddThemeBtn_clicked()
{
    QString themeZipSrc = QFileDialog::getOpenFileName
            (this,
             tr("Open File"),
             QDir::currentPath(),
             tr("Zip (*.zip)"));
    if(themeZipSrc.isEmpty())
        return;
    QString zipName = getFileName(themeZipSrc);
    QString Dst = QString("%1%2").arg(getFileDirectory(mMDPPath)).arg(zipName);
//    IDE_TRACE_STR(Dst);
    QFile dstTheme(Dst);
    if(dstTheme.exists())
    {
        dstTheme.setPermissions(QFile::WriteOther);
        if(!dstTheme.remove())
            return;
    }
    QFile themeZip(themeZipSrc);
    themeZip.setPermissions(QFile::ReadOther);
    if(!themeZip.copy(Dst))
        return;
    zipName.replace(".zip", "");
    for(int i=0;i<ui->Storage_TFlash_Theme_Enum_Item->count();i++)
    {
        QListWidgetItem* item = ui->Storage_TFlash_Theme_Enum_Item->item(i);
        if(!item->text().compare(zipName, Qt::CaseInsensitive))
            return;
    }
    QListWidgetItem* newItem = new QListWidgetItem;
    newItem->setCheckState(Qt::Unchecked);
    newItem->setIcon(mThemeListIcon);
    newItem->setText(zipName);
    ui->Storage_TFlash_Theme_Enum_Item->addItem(newItem);
}

void MDPForm::on_DelThemeBtn_clicked()
{
    QListWidgetItem* curItem = ui->Storage_TFlash_Theme_Enum_Item->currentItem();
    QString themeName = curItem->text();
    DeviceToolBar* toolBar = qobject_cast<DeviceToolBar*>(GlobalPara.m_DeviceToolBar);
    if(toolBar != 0)
    {
        if(toolBar->mDevListFrom  != 0)
        {
            if(toolBar->mDevListFrom->mNFDHostApp != 0)
            {
                QString message = QString("/TFlash/Theme;Del;%1").arg(themeName);
                toolBar->mDevListFrom->mNFDHostApp->SendNews(message);
            }
        }
    }
    QString Dst = QString("%1%2.zip").arg(getFileDirectory(mMDPPath)).arg(themeName);
//    IDE_TRACE_STR(Dst);
    QFile dstTheme(Dst);
    if(dstTheme.exists())
    {
        dstTheme.setPermissions(QFile::WriteOther);
        dstTheme.remove();
    }
    ui->Storage_TFlash_Theme_Enum_Item->takeItem(ui->Storage_TFlash_Theme_Enum_Item->currentRow());
}
