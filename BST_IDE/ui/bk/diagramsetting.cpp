#include "diagramsetting.h"
#include "ui_diagramsetting.h"

EffectSetting::EffectSetting(QWidget *parent, QString pControl, QString pOldControl) :
    QDialog(parent)
{
    m_MainLayout = new QVBoxLayout;
    this->setLayout(m_MainLayout);

    this->setWindowTitle(tr("EffectParameter Setting"));

    //>@
    m_StringList = SplitString(pControl, ";");
    if(m_StringList.count() <= 0)
    {
        deleteLater();
        return;
    }

    QStringList tmpParaList = SplitString(pOldControl, ";");
    QString initValue;
    for(int i=0;i<m_StringList.count();i++)
    {
        QStringList tmpList = SplitString(m_StringList.at(i), ":");
        if(tmpList.count() <= 0)
            continue;
        for(int j=0;j<tmpParaList.count();j++)
        {
            QString tmpString = tmpParaList.at(j);
            if(tmpString.contains(tmpList.at(0)))
            {
                QStringList tmpLastList = SplitString(tmpString, ":");
                if(tmpLastList.count()==2)
                    initValue = tmpLastList.at(1);
                break;
            }
        }
        CreateUi(this, m_MainLayout, tmpList, initValue);
    }

    //>@
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    m_ButtonBox = new QDialogButtonBox(this);
    m_ButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(m_ButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
    tmpHBoxLayout->addWidget(m_ButtonBox);
    m_MainLayout->addItem(tmpHBoxLayout);
}

void EffectSetting::accept()
{
    m_EffectPara.clear();
    for(int i=0;i<m_StringList.count();i++)
    {
        QString EffectString = m_StringList.at(i);
        QStringList tmpList = SplitString(EffectString, ":");
        QString tmpString = tmpList.at(0);
        if(tmpString.isEmpty())
            continue;
        QString tmpParaString;
        tmpParaString.clear();
        if(tmpString.compare(PARA_DIRECTION, Qt::CaseInsensitive) == 0 ||
            tmpString.compare(PARA_ORIENTATION, Qt::CaseInsensitive) == 0 ||
            tmpString.compare(PARA_CURVESHAPE, Qt::CaseInsensitive) == 0 ||
            tmpString.compare(PARA_STYLE, Qt::CaseInsensitive) == 0 ||
            tmpString.compare(PARA_PATTERN, Qt::CaseInsensitive) == 0 ||
            tmpString.compare(PARA_ALIGN, Qt::CaseInsensitive) == 0)
        {
            QComboBox *tmpComboBox = findChild<QComboBox *>(tmpString);
            if(tmpComboBox->currentText().isEmpty()==false)
                tmpParaString = QString("%1:%2").arg(tmpString).arg(tmpComboBox->currentText());
        }
        else if(tmpString.compare(PARA_AUTOPLAY, Qt::CaseInsensitive) == 0)  //>@�Զ�����
        {
            QString para;
            QCheckBox *tmpCheckBox = findChild<QCheckBox *>(tmpString);
            Qt::CheckState tmpCheck = tmpCheckBox->checkState();
            if(tmpCheck == Qt::Unchecked)
                para = "N";
            else if(tmpCheck == Qt::Checked)
                para = "Y";
            tmpParaString = QString("%1:%2").arg(tmpString).arg(para);
        }
        else        //>@Ĭ�ϵľ�Ϊ���ֱ༭
        {
            QSpinBox *tmpSpinBox = findChild<QSpinBox *>(tmpString);
            if(tmpSpinBox && tmpSpinBox->text().isEmpty()==false)
                tmpParaString = QString("%1:%2").arg(tmpString).arg(tmpSpinBox->text());
        }

        if(tmpParaString.isEmpty()==false)
        {
            m_EffectPara.append(tmpParaString);
            if(i < m_StringList.count()-1)
            {
                m_EffectPara.append(";");
            }
        }
    }
    QDialog::accept();
}

void EffectSetting::reject()
{
    QDialog::reject();
}

bool EffectSetting::CreateUi(QWidget *pParentWidget, QLayout *pLayout, QStringList pUiPara, QString pInitValue)
{
    if(pParentWidget == 0 || pLayout == 0 || pUiPara.count() <= 0)
    {
        return false;
    }

    QString tmpString = pUiPara.at(0);
    if(tmpString.isEmpty())
        return false;
    if(tmpString.compare(PARA_DIRECTION, Qt::CaseInsensitive) == 0 ||
       tmpString.compare(PARA_ORIENTATION, Qt::CaseInsensitive) == 0 ||
       tmpString.compare(PARA_CURVESHAPE, Qt::CaseInsensitive) == 0 ||
       tmpString.compare(PARA_STYLE, Qt::CaseInsensitive) == 0 ||
       tmpString.compare(PARA_PATTERN, Qt::CaseInsensitive) == 0 ||
       tmpString.compare(PARA_ALIGN, Qt::CaseInsensitive) == 0)
    {
        //>@Combobox
        QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
        QLabel *tmpLabel = new QLabel(tmpString, pParentWidget);
        tmpLabel->setMinimumWidth(100);
        tmpLabel->setMaximumWidth(100);

        QComboBox *tmpComboBox = new QComboBox(pParentWidget);
        tmpComboBox->setObjectName(tmpString);
        tmpComboBox->setMinimumWidth(200);
        tmpComboBox->setMaximumWidth(200);

        tmpString = pUiPara.at(1);
        if(tmpString.isEmpty())
            return false;
        QStringList tmpTextLsit = SplitString(tmpString, "/");
        tmpComboBox->addItems(tmpTextLsit);

        if(pInitValue.isEmpty() == false)
        {
            for(int i=0;i<tmpTextLsit.count();i++)
            {
                if(pInitValue == tmpTextLsit.at(i))
                    tmpComboBox->setCurrentIndex(i);
            }
        }

        tmpHBoxLayout->addWidget(tmpLabel);
        tmpHBoxLayout->addWidget(tmpComboBox);
        pLayout->addItem(tmpHBoxLayout);
    }
    else if(tmpString.compare(PARA_AUTOPLAY, Qt::CaseInsensitive) == 0)  //>@�Զ�����
    {
        //>@checkbox
        QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
        QCheckBox *tmpCheckBox = new QCheckBox(tmpString, pParentWidget);
        tmpCheckBox->setObjectName(tmpString);
        tmpCheckBox->setMinimumWidth(200);

        if(pInitValue.isEmpty() == false)
        {
            if(pInitValue == QString("N"))
                tmpCheckBox->setCheckState(Qt::Unchecked);
            else if(pInitValue == QString("Y"))
                tmpCheckBox->setCheckState(Qt::Checked);
        }

        tmpHBoxLayout->addWidget(tmpCheckBox);
        pLayout->addItem(tmpHBoxLayout);
    }
    else
    {
        //>@SpinBox
        QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
        QLabel *tmpLabel = new QLabel(tmpString, pParentWidget);
        tmpLabel->setMinimumWidth(100);
        tmpLabel->setMaximumWidth(100);

        QSpinBox *tmpSpinBox = new QSpinBox(pParentWidget);
        tmpSpinBox->setObjectName(tmpString);
        tmpSpinBox->setMinimum(0);
        tmpSpinBox->setMaximum(99999);
        tmpSpinBox->setMinimumWidth(200);
        tmpSpinBox->setMaximumWidth(200);

        if(pInitValue.isEmpty() == false)
        {
            tmpSpinBox->setValue(pInitValue.toInt());
        }
        else
        {
            tmpSpinBox->setValue(100);
        }

        tmpHBoxLayout->addWidget(tmpLabel);
        tmpHBoxLayout->addWidget(tmpSpinBox);
        pLayout->addItem(tmpHBoxLayout);
    }
    return true;
}

DiagramSetting::DiagramSetting(uiManager *pManager, COM_INFO *pComInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagramSetting)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/Menu/rc/Menu/setting.png"));
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    m_CurState = 0;
    m_CurSelectItem = 0;
    m_IsValid = false;
    m_CurStateIndex = -1;   //>@״̬����Ͽ�ǰ��ʾ�������
    m_CurEffectIndex = -1;  //>@��Ч��Ͽ�ǰ��ʾ�������

    m_Manager = pManager;
    m_ComInfo = pComInfo;

    if(m_Manager)
        m_CurRcDir = m_Manager->m_FileDir;        //>@��ǰ�򿪵��ļ����ڵ�Ŀ¼��Ĭ����ui�ļ�����Ŀ¼

    ui->EffectEditTree->clear();
    ui->EffectEditTree->setColumnWidth(0, 150);

    if(m_ComInfo == 0)
    {
        IDE_TRACE();
        m_IsValid = false;
        return;
    }
    m_ComCategory = GlobalPara.m_ComCategoryGroup.value(m_ComInfo->ComPath);
    if(m_ComCategory == 0)
    {
        IDE_TRACE();
        m_IsValid = false;
        return;
    }
    setWindowTitle(tr("%1 Settings").arg(m_ComInfo->ComPath));
    m_IsValid = true;

    InitUI();
}

DiagramSetting::~DiagramSetting()
{
    delete ui;
}

void DiagramSetting::InitUI()
{
    if(m_IsValid == false)
        return;
    ui->StateMachineType->clear();
    for(int i=0;i<m_ComCategory->StateMachines.count();i++)
    {
        STATEMACHINE* tmpState = m_ComCategory->StateMachines.at(i);
        ui->StateMachineType->addItem(tmpState->StateName);
    }
    ui->StateMachineType->setCurrentIndex(0);

    ui->EditRcPath->clear();
    ui->EffectPara->clear();

    UpdateTree(m_ComInfo);
}

void DiagramSetting::DelTreeAllItem(QTreeWidgetItem *parentItem)
{
    QList<QTreeWidgetItem*> ItemList = parentItem->takeChildren();
    QTreeWidgetItem* tmpItem;
    for(int i=0;i<ItemList.count();i++)
    {
        tmpItem = ItemList.at(i);
        parentItem->removeChild(tmpItem);
    }
}

//>@���ظ�item
QTreeWidgetItem *DiagramSetting::DelTreeItem(QTreeWidgetItem *pItem)
{
    QTreeWidgetItem* tmpParentItem = pItem->parent();
    if(tmpParentItem == 0)
        return 0;
    tmpParentItem->removeChild(pItem);
    return tmpParentItem;
}

//>@������ӵ�Item
QTreeWidgetItem *DiagramSetting::AddTreeItem(QTreeWidgetItem *parentItem, QStringList contentList, bool Editable)
{
    static int i = 0;
    QTreeWidgetItem *pNewItem = new QTreeWidgetItem(parentItem, contentList);
    pNewItem->setTextAlignment(0, Qt::AlignLeft || Qt::AlignVCenter);
    pNewItem->setTextAlignment(1, Qt::AlignLeft || Qt::AlignVCenter);
    pNewItem->setBackgroundColor(0, (i%2)?QColor(254,235,208):QColor(255,250,179));
    pNewItem->setBackgroundColor(1, (i%2)?QColor(254,235,208):QColor(255,250,179));
    pNewItem->setFont(0, QFont());
    if(Editable)
        pNewItem->setFlags(Qt::ItemIsEditable |Qt::ItemIsEnabled);
    parentItem->addChild(pNewItem);
    i++;
    return pNewItem;
}

//>@���ĳ��״̬��Ч��Դ
QTreeWidgetItem *DiagramSetting::AddTreeItem(QTreeWidgetItem *parentItem, STATE_INFO* pState)
{
    if(pState ==0 || pState->StateName.isEmpty())
        return 0;
    QTreeWidgetItem *pNewItem = AddTreeItem(parentItem, QStringList()<<pState->StateName, false);
    QTreeWidgetItem *pNewSubItem = AddTreeItem(pNewItem, QStringList()<<QString("EffectType")<<pState->Effect.EffectType, false);
    for(int i=0;i<pState->Effect.RcFiles.count();i++)
    {
        RESOURCE_INFO tmpRc = pState->Effect.RcFiles.at(i);
        AddTreeItem(pNewSubItem, QStringList()<<tmpRc.RcName<<tmpRc.RcFile, false);
    }
    return pNewItem;
}

void DiagramSetting::UpdateTree(COM_INFO *pComInfo)
{
    ui->EffectEditTree->clear();
    m_RootTreeItem = new QTreeWidgetItem(ui->EffectEditTree, QStringList(QString("StateMachine")));
    m_RootTreeItem->setBackground(0, QColor(185,185,185));
    m_RootTreeItem->setBackground(1, QColor(185,185,185));
    for(int i=0;i<pComInfo->StateMachine.count();i++)
    {
        STATE_INFO *tmpState = pComInfo->StateMachine.at(i);
        AddTreeItem(m_RootTreeItem, tmpState);
    }
    ui->EffectEditTree->expandAll();
}

void DiagramSetting::on_StateMachineType_currentIndexChanged(const QString &arg1)
{
    for(int i=0;i<m_ComCategory->StateMachines.count();i++)
    {
        STATEMACHINE* tmpState = m_ComCategory->StateMachines.at(i);
        if(arg1 == tmpState->StateName)
        {
            //>@���浱ǰѡ�е�״̬���±�
            m_CurStateIndex = i;
            m_CurState = tmpState;
            ui->EffectType->clear();
            ui->RcName->clear();
            for(int j=0;j<tmpState->Effects.count();j++)
            {
                EFFECT tmpEffect = tmpState->Effects.at(j);
                ui->EffectType->addItem(tmpEffect.EffectName);
            }
            ui->EffectType->setCurrentIndex(0);
            break;
        }
    }
}

void DiagramSetting::on_EffectType_currentIndexChanged(const QString &arg1)
{
    if(m_CurState == 0)
        return;
    for(int j=0;j<m_CurState->Effects.count();j++)
    {
        EFFECT tmpEffect = m_CurState->Effects.at(j);
        if(tmpEffect.EffectName == arg1)
        {
            m_CurEffectPara = tmpEffect.EffectPara;
            m_CurEffectIndex = j;
            ui->RcName->clear();
            for(int k=0;k<tmpEffect.Resources.count();k++)
            {
                RESOURCE tmpRc = tmpEffect.Resources.at(k);
                ui->RcName->addItem(tmpRc.RcName);
                if(tmpRc.RcName == QString("Invalid"))
                {
                    ui->EditRcPath->setText("*");
                }
            }
            SetRcNameCurrentIndex(0);
            break;
        }
    }
}

void DiagramSetting::on_RcName_currentIndexChanged(int index)
{
    if(m_CurEffectIndex < m_CurState->Effects.count() && m_CurEffectIndex>=0)
    {
        EFFECT tmpEffect = m_CurState->Effects.at(m_CurEffectIndex);
        if(tmpEffect.EffectName != ui->EffectType->currentText())
        {
            return;
        }
        if(index < ui->RcName->count() && index >= 0)
        {
            QString tmpString = ui->RcName->itemText(index);
            if(index < tmpEffect.Resources.count() && index >= 0)
            {
                QString tmpRcName = tmpEffect.Resources.at(index).RcName;
                QString tmpNickName = tmpEffect.Resources.at(index).NickName;
                if(tmpString == tmpRcName)
                {
                    ui->NickName->setText(tmpNickName);
                }
            }
        }
    }
}

void DiagramSetting::SetRcNameCurrentIndex(int pIndex)
{
    ui->RcName->setCurrentIndex(pIndex);
}

void DiagramSetting::on_BtnRcBrowser_clicked()
{
    //��ȡ��ǰѡ�е���Դ,Ȼ��������Դ����
    if(m_CurState == 0)
        return;
    for(int j=0;j<m_CurState->Effects.count();j++)
    {
        EFFECT tmpEffect = m_CurState->Effects.at(j);
        if(tmpEffect.EffectName == ui->EffectType->currentText())
        {
            for(int k=0;k<tmpEffect.Resources.count();k++)
            {
                RESOURCE tmpRc = tmpEffect.Resources.at(k);
                if(tmpRc.RcName == ui->RcName->currentText())
                {
                    int i;
                    QString tmpString("Resources (*.");
                    for(i=0;i<tmpRc.RcTypes.count()-1;i++)
                    {
                        tmpString.append(tmpRc.RcTypes.at(i));
                        tmpString.append(QString(" *."));
                    }
                    tmpString.append(tmpRc.RcTypes.at(i));
                    tmpString.append(")");
                    QString fileName = QFileDialog::getOpenFileName(this,
                                                                    tr("Select File"),
                                                                    m_CurRcDir,
                                                                    tmpString);
                    if (fileName.isEmpty())
                        return;
                    m_CurRcDir = getFileDirectory(fileName);
                    ui->EditRcPath->setText(fileName);
                    return;
                }
            }
        }
    }
}

//>@��Դ����Ͽ��Զ���1
void DiagramSetting::updateRcNameCombo()
{
    int index = ui->RcName->currentIndex();
    if(index + 1 < ui->RcName->count())
        SetRcNameCurrentIndex(index+1);
}

//>@ֻ����������Դʱ�������޸�ĳһ����Դ�ļ�·����������ӵ��ļ�����ͬ����·�����У��������·�����Լ��½�ĳ����Դ����Դ����Ͽ�Ż��Լ�1��
void DiagramSetting::on_BtnAdd_clicked()
{
    QString StateMachineString, EffectTypeString, RcNameString, RcPathString;
    StateMachineString = ui->StateMachineType->currentText();
    //>@���״̬���Ƿ�ѡ��
    if(StateMachineString.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Add Error"),
                             tr("Please select a StateMachine!"),
                             QMessageBox::Close);
        return;
    }
    else
    {
        //>@�����Ч�Ƿ�ѡ��
        EffectTypeString = ui->EffectType->currentText();
        if(EffectTypeString.isEmpty())
        {
            QMessageBox::warning(this,
                                 tr("Add Error"),
                                 tr("Please select a Effect!"),
                                 QMessageBox::Close);
            return;
        }
        else
        {
            //>@�����Դ���Ƿ�ѡ��
            RcNameString = ui->RcName->currentText();
            if(RcNameString.isEmpty())
            {
                QMessageBox::warning(this,
                                     tr("Add Error"),
                                     tr("Please select a resource name!"),
                                     QMessageBox::Close);
                return;
            }
            else
            {
                RcPathString = ui->EditRcPath->text();
                if(RcPathString.isEmpty())
                {
                    QMessageBox::warning(this,
                                         tr("Add Error"),
                                         tr("Please select a resource file!"),
                                         QMessageBox::Close);
                    return;
                }
                else
                {
                    //>@ִ����ӣ������޸�COM,Ȼ��ͨ��COM����Tree
                    //>@�鿴�Ƿ��ڵ�ǰ״̬�����޸ģ�����ǣ����޸ģ����򴴽���
                    for(int i=0;i<m_ComInfo->StateMachine.count();i++)
                    {
                        STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);
                        if(tmpState->StateName == StateMachineString)
                        {
                            //>@һ��״̬��ֻ��һ����Ч���˴�Ϊ�޸���Ч
                            if(tmpState->Effect.EffectType != EffectTypeString)
                            {
                                tmpState->Effect.EffectPara = ui->EffectPara->text();//>@�޸���Ч����
                                tmpState->Effect.EffectType = EffectTypeString;
                                tmpState->Effect.RcFiles.clear(); //>@�����Դ�б�
                                RESOURCE_INFO tmpRcInfo;
                                tmpRcInfo.RcName = RcNameString;
                                tmpRcInfo.RcFile = RcPathString;
                                tmpState->Effect.RcFiles.append(tmpRcInfo);
                                UpdateTree(m_ComInfo);

                                //>@��Դ����Ͽ��Զ���1
                                updateRcNameCombo();
                                return;
                            }
                            else
                            {
                                //>@������ڵ�ǰ��Ч�£���ֻ��鿴��Դ�ļ����Ƿ�����ͬ����Դ��������У����滻��û���򴴽�
                                for(int j=0;j<tmpState->Effect.RcFiles.count();j++)
                                {
                                    RESOURCE_INFO tmpRcInfo = tmpState->Effect.RcFiles.at(j);
                                    //>@��Դ����ͬ��·������ͬ���滻
                                    if(tmpRcInfo.RcName == RcNameString)
                                    {
                                        if(tmpRcInfo.RcFile != RcPathString)
                                        {
                                            tmpState->Effect.RcFiles.removeAt(j);
                                            RESOURCE_INFO RcInfo;
                                            RcInfo.RcName = RcNameString;
                                            RcInfo.RcFile = RcPathString;
                                            tmpState->Effect.RcFiles.append(RcInfo);
                                            UpdateTree(m_ComInfo);

                                            //>@��Դ����Ͽ��Զ���1
                                            updateRcNameCombo();
                                        }
                                        return;
                                    }
                                }
                                //>@�����ǰ��Ч��û����ͬ��Դ�����ļ����򴴽���ͬʱ��Դ����Ͽ��Զ���1
                                RESOURCE_INFO RcInfo;
                                RcInfo.RcName = RcNameString;
                                RcInfo.RcFile = RcPathString;
                                tmpState->Effect.RcFiles.append(RcInfo);
                                UpdateTree(m_ComInfo);

                                //>@��Դ����Ͽ��Զ���1
                                updateRcNameCombo();
                                return;
                            }
                            return;
                        }
                    }
                    //>@���һ��״̬��
                    STATE_INFO* State = new STATE_INFO;
                    State->StateName = StateMachineString;
                    State->Effect.EffectPara = ui->EffectPara->text();//>@�޸���Ч����
                    State->Effect.EffectType = EffectTypeString;
                    RESOURCE_INFO tmpRcInfo;
                    tmpRcInfo.RcName = RcNameString;
                    tmpRcInfo.RcFile = RcPathString;
                    State->Effect.RcFiles.append(tmpRcInfo);
                    m_ComInfo->StateMachine.append(State);
                    UpdateTree(m_ComInfo);

                    //>@��Դ����Ͽ��Զ���1
                    updateRcNameCombo();
                }
            }
        }
    }
}

void DiagramSetting::on_BtnModify_clicked()
{
    if(m_CurSelectItem == 0)
    {
        QMessageBox::warning(this,
                             tr("Modify Error"),
                             tr("Please select resource \nin the ResourcesTree \non the left!"),
                             QMessageBox::Close);
        return;
    }
    QTreeWidgetItem* tmpParentItem = m_CurSelectItem->parent();
    if(tmpParentItem)
    {
        //>@ͬ��ComInfo����
        QString curName = m_CurSelectItem->text(0);
        QString curRc = m_CurSelectItem->text(1);
        QString parentName = tmpParentItem->text(0);
        QString parentRc = tmpParentItem->text(1);
        if(curRc.isEmpty()) //>@��ǰѡ�е���״̬�У����˳�
        {
            m_CurSelectItem = 0;
            return;
        }
        else if(curName == QString("EffectType") || parentRc.isEmpty()) //>@��ǰѡ�е�����Ч�У����Ը�����Ч����
        {
            //>@��ComInfo�в��ҵ����޸ĵ���Ч
            for(int i=0;i<m_ComInfo->StateMachine.count();i++)
            {
                STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);
                if(tmpState->Effect.EffectType == curRc)
                {
                    QString tmpEffectPara = ui->EffectPara->text();
                    //>@�鿴�Ƿ�����в�����ͬ
                    if(tmpState->Effect.EffectPara != tmpEffectPara)
                    {
                        //>@ѯ���Ƿ��޸���Ч����
                        int ret = QMessageBox::warning(this,
                                             tr("Effect Parameter"),
                                             tr("Do you want to modify parameter[%1] to parameter[%2]")
                                             .arg(tmpState->Effect.EffectPara).arg(tmpEffectPara),
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No);
                        if(ret == QMessageBox::Yes)
                        {
                            tmpState->Effect.EffectPara = tmpEffectPara;
                        }
                    }
                    break;
                }
            }
        }
        else if(parentName == QString("EffectType")) //>@��ǰѡ�е�����Դ�У��������޸���Դ�ļ�·��
        {
            for(int i=0;i<m_ComInfo->StateMachine.count();i++)
            {
                STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);
                for(int j=0;j<tmpState->Effect.RcFiles.count();j++)
                {
                    RESOURCE_INFO tmpRc = tmpState->Effect.RcFiles.at(j);
                    if(tmpRc.RcFile == curRc && tmpRc.RcName == curName)
                    {
                        //>@���ҵ�ComInfo�е���Դ��
                        QString tmpString = ui->EditRcPath->text();
                        if(tmpRc.RcFile != tmpString)
                        {
                            RESOURCE_INFO tmpNewRc;
                            tmpNewRc.RcName = tmpRc.RcName;
                            tmpNewRc.RcFile = tmpString;
                            tmpState->Effect.RcFiles.removeAt(j);
                            tmpState->Effect.RcFiles.append(tmpNewRc);
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            m_CurSelectItem = 0;
            return;
        }
        UpdateTree(m_ComInfo);
    }
    m_CurSelectItem = 0;
}

void DiagramSetting::on_BtnDelete_clicked()
{
    if(m_CurSelectItem == 0)
    {
        QMessageBox::warning(this,
                             tr("Delete Error"),
                             tr("Please select resource \nin the ResourcesTree \non the left!"),
                             QMessageBox::Close);
        return;
    }
    QTreeWidgetItem* tmpParentItem = m_CurSelectItem->parent();
    if(tmpParentItem)
    {
        //tmpParentItem->removeChild(m_CurSelectItem);
        //>@ͬ��ComInfo����
        QString curName = m_CurSelectItem->text(0);
        QString curRc = m_CurSelectItem->text(1);
        QString parentName = tmpParentItem->text(0);
        QString parentRc = tmpParentItem->text(1);
        if(curRc.isEmpty()) //>@��ǰѡ�е���״̬��
        {
            for(int i=0;i<m_ComInfo->StateMachine.count();i++)
            {
                STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);

                if(tmpState->StateName == curName)
                {
                    ui->StateMachineType->clear();
                    ui->StateMachineType->addItem(tmpState->StateName);

                    m_ComInfo->StateMachine.removeAt(i);
                    break;
                }
            }
        }
        else if(curName == QString("EffectType") || parentRc.isEmpty()) //>@��ǰѡ�е�����Ч�У��Ὣ״̬��Ҳɾ������Ϊ״̬������һ����Ч����ô��ʱ״̬��������
        {
            for(int i=0;i<m_ComInfo->StateMachine.count();i++)
            {
                STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);
                if(tmpState->Effect.EffectType == curRc)
                {
                    ui->StateMachineType->clear();
                    ui->StateMachineType->addItem(parentName);
                    for(int j=0;j<ui->EffectType->count();j++)
                    {
                        QString tmpString = ui->EffectType->itemText(j);
                        if(tmpString == curRc)
                        {
                            ui->EffectType->setCurrentIndex(j);
                            break;
                        }
                    }

                    m_ComInfo->StateMachine.removeAt(i);
                    break;
                }
            }
        }
        else if(parentName == QString("EffectType")) //>@��ǰѡ�е�����Դ��
        {
            for(int i=0;i<m_ComInfo->StateMachine.count();i++)
            {
                STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);
                for(int j=0;j<tmpState->Effect.RcFiles.count();j++)
                {
                    RESOURCE_INFO tmpRc = tmpState->Effect.RcFiles.at(j);
                    if(tmpRc.RcFile == curRc && tmpRc.RcName == curName)
                    {
                        tmpState->Effect.RcFiles.removeAt(j);
                    }
                }
            }
        }
        else
        {
            m_CurSelectItem = 0;
            return;
        }
        on_BtnRefresh_clicked();
    }
    m_CurSelectItem = 0;
}

void DiagramSetting::on_BtnClear_clicked()
{
    m_ComInfo->StateMachine.clear();
    on_BtnRefresh_clicked();
}

void DiagramSetting::on_EffectEditTree_itemSelectionChanged()
{
    QList<QTreeWidgetItem*> tmpList = ui->EffectEditTree->selectedItems();
    if(tmpList.count() <= 0)
        return;
    m_CurSelectItem = tmpList.at(0);
    if(m_CurSelectItem == 0)
        return;
    QTreeWidgetItem* tmpParentItem = m_CurSelectItem->parent();
    if(tmpParentItem == 0)  //>@��ǰѡ�е��Ƕ������ʱ�����±ߵ�״̬���ָ���Ĭ�ϼ���
    {
        ui->StateMachineType->clear();
        for(int i=0;i<m_ComCategory->StateMachines.count();i++)
        {
            STATEMACHINE* tmpState = m_ComCategory->StateMachines.at(i);
            ui->StateMachineType->addItem(tmpState->StateName);
        }
        ui->StateMachineType->setCurrentIndex(0);
    }
    else
    {
        //tmpParentItem->removeChild(m_CurSelectItem);
        //>@ͬ��ComInfo����
        QString curName = m_CurSelectItem->text(0);
        QString curRc = m_CurSelectItem->text(1);
        QString parentName = tmpParentItem->text(0);
        QString parentRc = tmpParentItem->text(1);
        if(curRc.isEmpty()) //>@��ǰѡ�е���״̬��
        {
            for(int i=0;i<m_ComInfo->StateMachine.count();i++)
            {
                STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);
                if(!tmpState)
                    continue;
                if(tmpState->StateName == curName)
                {
                    for(int j=0;j<ui->StateMachineType->count();j++)
                    {
                        QString tmpString = ui->StateMachineType->itemText(j);
                        if(tmpString == curName)
                        {
                            ui->StateMachineType->setCurrentIndex(j);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        else if(curName == QString("EffectType") || parentRc.isEmpty()) //>@��ǰѡ�е�����Ч�У��Ὣ״̬��Ҳɾ������Ϊ״̬������һ����Ч����ô��ʱ״̬��������
        {
            for(int i=0;i<m_ComInfo->StateMachine.count();i++)
            {
                STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);
                if(tmpState->Effect.EffectType == curRc)
                {
                    for(int j=0;j<ui->StateMachineType->count();j++)
                    {
                        QString tmpString = ui->StateMachineType->itemText(j);
                        if(tmpString == parentName)
                        {
                            ui->StateMachineType->setCurrentIndex(j);
                            break;
                        }
                    }
                    ui->EffectPara->update();
                    ui->EffectPara->setText(tmpState->Effect.EffectPara);
                    ui->EffectType->update();
                    for(int j=0;j<ui->EffectType->count();j++)
                    {
                        QString tmpString = ui->EffectType->itemText(j);
                        if(tmpString == curRc)
                        {
                            ui->EffectType->setCurrentIndex(j);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        else if(parentName == QString("EffectType")) //>@��ǰѡ�е�����Դ��
        {
            for(int i=0;i<m_ComInfo->StateMachine.count();i++)
            {
                STATE_INFO* tmpState = m_ComInfo->StateMachine.at(i);
                for(int j=0;j<tmpState->Effect.RcFiles.count();j++)
                {
                    RESOURCE_INFO tmpRc = tmpState->Effect.RcFiles.at(j);
                    if(tmpRc.RcFile == curRc && tmpRc.RcName == curName)
                    {
                        for(int j=0;j<ui->StateMachineType->count();j++)
                        {
                            QString tmpString = ui->StateMachineType->itemText(j);
                            if(tmpString == tmpParentItem->parent()->text(0))
                            {
                                ui->StateMachineType->setCurrentIndex(j);
                                break;
                            }
                        }

                        ui->EffectType->update();
                        for(int j=0;j<ui->EffectType->count();j++)
                        {
                            QString tmpString = ui->EffectType->itemText(j);
                            if(tmpString == parentRc)
                            {
                                ui->EffectType->setCurrentIndex(j);
                                break;
                            }
                        }
                        ui->RcName->update();
                        for(int j=0;j<ui->RcName->count();j++)
                        {
                            QString tmpString = ui->RcName->itemText(j);
                            if(tmpString == curName)
                            {
                                SetRcNameCurrentIndex(j);
                                ui->EditRcPath->setText(curRc);
                                break;
                            }
                        }
                    }
                }
            }
            //>@Ԥ������Դ
            PreviewRC(curRc);
        }
    }
}

void DiagramSetting::PreviewRC(QString pFile)
{
    //>@Ԥ������Դ
    FILEPATH_TYPE tmpFileType = RegulatRcFile(pFile, m_Manager);
    if(tmpFileType != PATH_TYPENONE)
    {
        RC_TYPE tmpRcType = getRcType(pFile);
        switch(tmpRcType)
        {
            case RC_BMP:
            case RC_JPG:
            case RC_PNG:
            {
                ui->LabelPreview->ShowPixmap(pFile);
                break;
            }
            case RC_GIF:
            {
                ui->LabelPreview->ShowGIF(pFile);
                break;
            }
            case RC_SVG:
            {
                ui->LabelPreview->ShowSVG(pFile);
                break;
            }
            case RC_QML:
            {
                ui->LabelPreview->ShowQML(pFile);
                break;
            }
            case RC_TXT:
            {
                break;
            }
            case RC_MP3:
            case RC_WAV:
            case RC_MP4:
            case RC_AVI:
            case RC_RMVB:
            case RC_FLV:
            {
                ui->LabelPreview->PlayMedia(pFile);
                break;
            }
            default:
                break;
        }
    }
}

void DiagramSetting::on_BtnEffectParaEdit_clicked()
{
    m_CurStateIndex = ui->StateMachineType->currentIndex();
    m_CurEffectIndex = ui->EffectType->currentIndex();
    STATEMACHINE *tmpState = m_ComCategory->StateMachines.at(m_CurStateIndex);
    if(!tmpState)
        return;
    EFFECT tmpEffect = tmpState->Effects.at(m_CurEffectIndex);
    if(tmpEffect.EffectName != ui->EffectType->currentText())
    {
        IDE_TRACE_STR(tmpEffect.EffectName);
        IDE_TRACE_STR(ui->EffectType->currentText());
        return;
    }
    m_CurEffectPara = tmpEffect.EffectPara;
    if(m_CurEffectPara.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("The current effect parameter is invalid,\nPlease correct!"),
                             QMessageBox::Close);
        return;
    }

    //>@���Ӵ�ui->EffectPara�л�ȡ�����õ�ֵ
    QString tmpLastPara = ui->EffectPara->text();

    EffectSetting myEffectSetting(0, m_CurEffectPara, tmpLastPara);
    int ret = myEffectSetting.exec();
    if(ret == QDialog::Accepted)
    {
        if(myEffectSetting.m_EffectPara.isEmpty()==false)
            ui->EffectPara->setText(myEffectSetting.m_EffectPara);
    }

    m_CurEffectPara.clear();
}

void DiagramSetting::on_BtnRefresh_clicked()
{
    InitUI();
}

