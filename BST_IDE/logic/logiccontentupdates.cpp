#include "logiccontentupdates.h"
#ifdef IDE
    #include "devicescene.h"
    #include "toolbar.h"
    #include "simulatorform.h"
#elif DISPLAY
    #include "devicemanager.h"
#endif

LogicContentUpgrades::LogicContentUpgrades(QObject *parent)
    :LogicDevice(parent)
{
}

LogicContentUpgrades::~LogicContentUpgrades()
{
}

void LogicContentUpgrades::SetSrcPath(QString pPath)
{
    m_SrcPath =  pPath;
    if(!m_SrcPath.endsWith("/"))
        m_SrcPath.append("/");
}

void LogicContentUpgrades::SetDstPath(QString pPath)
{
    m_DstPath =  pPath;
    if(!m_DstPath.endsWith("/"))
        m_DstPath.append("/");
}

GraphicsScene *LogicContentUpgrades::GetThemeScene()
{
#ifdef IDE  //>@��Ҫ����ģ����
    ProjectBar* tmpProjectBar = VAR_CAST<ProjectBar*>(GlobalPara.m_ProjectBar);
    if(!(tmpProjectBar && tmpProjectBar->mSimulatorView))
        return (GraphicsScene*)0;
    return tmpProjectBar->mSimulatorView->m_SimulatorView->m_Scene;
#elif DISPLAY
    LogicThemeManager *tmpThemeLGDev = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
    if(!(tmpThemeLGDev && tmpThemeLGDev->m_UiView))
        return (GraphicsScene*)0;
    return tmpThemeLGDev->m_UiView->mScene;
#endif
}

quint32 LogicContentUpgrades::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("Backstage"))
        {
            tmpHBoxLayout = CreateBool(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("Device"))
        {
            QStringList tmpDevList = tmpElement.text().split(';');
            AppendPHDevices(tmpDevList);
            tmpHBoxLayout = CreateCheck(tmpName, tmpElement.attribute("List").split(';'), tmpDevList);
        }
        else
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    validcount++;
    return validcount;
}

void LogicContentUpgrades::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Backstage"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mBackStage, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!tmpName.compare("Device"))
    {
        CheckList *tmpComponent = VAR_CAST<CheckList *>(pObject);
        if(tmpComponent)
        {
            QStringList tmpDevList = tmpComponent->GetSelectList();
            QStringList tmpList = PHDevicesAdded(tmpDevList, tmpComponent);
            if(!ModifyCheck(tmpComponent, tmpElement, tmpComponent->m_SelectList, tmpList))
            {
            }
        }
    }
}

void LogicContentUpgrades::DeviceStateChanged()
{
    IDE_TRACE();
    //>@Ѱ�������豸
    PhysicalDevice *tmpUpgradePHDev = VAR_CAST<PhysicalDevice*>(sender());
    if(!tmpUpgradePHDev)
        return;
    IDE_TRACE();
    if(!mPHDevices.value(tmpUpgradePHDev->mDevType))
        return;
    IDE_TRACE();
    //>@MDisk
    if(tmpUpgradePHDev->mDevType == P_MDISK)
    {
        PhysicalMobileDisk *tmpDev = VAR_CAST<PhysicalMobileDisk*>(tmpUpgradePHDev);
        if(tmpDev && tmpDev->mExecutable)
            return;
    }
    IDE_TRACE();
    //>@����Դ��ַ
    if(tmpUpgradePHDev->mDevPath.isEmpty())
        return;
    IDE_TRACE();
    SetSrcPath(tmpUpgradePHDev->mDevPath);
    //>@����ĩ��ַ
    LogicThemeManager *tmpThemeLGDev = 0;
#ifdef DISPLAY
    tmpThemeLGDev = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
#endif
    if(!tmpThemeLGDev)
        return;
    IDE_TRACE();
    QList<PH_Type> tmpList = tmpThemeLGDev->mPHDevices.keys();
    if(tmpList.isEmpty())
        return;
    IDE_TRACE();
    PhysicalDevice *tmpThemePHDev = tmpThemeLGDev->mPHDevices.value(tmpList.first());
    if(!tmpThemePHDev)  //>@�����ǰ����û��ָ�������ŵ��豸���򲻽�������
        return;
    IDE_TRACE();
    SetDstPath(tmpThemePHDev->mDevPath);
    //>@���������豸�е�������������������������������û������ȴ��û��ε�U��
    STORAGESTATE tmpStorageState = StorageIsValid();
    bool success = false;
    switch(tmpStorageState)
    {
        case ExecCMD:
        {
            success = ExecCmdList(tmpUpgradePHDev, tmpThemePHDev);
            break;
        }
        case UpgradeExe:
        {
            success = UpgradeExeList(tmpUpgradePHDev, tmpThemePHDev,
                                     m_UpgradeContentList.values(EXE), m_DstPath + D_EXEDIR, true);
            break;
        }
        case UpgradeTheme:
        {
        IDE_TRACE();
            success = UpgradeThemeList(tmpUpgradePHDev, tmpThemePHDev,
                                       m_UpgradeContentList.values(THEME), m_DstPath + D_THEMEDIR, true);
            break;
        }
        case UpgradeArea:
        {
            //>@�鿴�Ƿ����������У�����У�����������ǰ����Ĵ��̣�Ȼ������������Զ��޸�ui�ļ�������Ĭ�ϴ��̲���������
            success = UpgradeAreaList(tmpUpgradePHDev, tmpThemePHDev,
                                      AREALOGO, m_UpgradeContentList.values(AREALOGO), true) &&
                      UpgradeAreaList(tmpUpgradePHDev, tmpThemePHDev,
                                      AREAVIDEO, m_UpgradeContentList.values(AREAVIDEO), true) &&
                      UpgradeAreaList(tmpUpgradePHDev, tmpThemePHDev,
                                      AREAIMAGE, m_UpgradeContentList.values(AREAIMAGE), true) &&
                      UpgradeAreaList(tmpUpgradePHDev, tmpThemePHDev,
                                      AREATEXT, m_UpgradeContentList.values(AREATEXT), true);
            break;
        }
        default:
            return;
    }
    if(success)
    {
        //emit sInfo(QString("Upgrade success"), 4000);
        SystemAudio(D_UpGradeSucess);
    }
    else
    {
        //emit sInfo(QString("Upgrade failed"), 4000);
        SystemAudio(D_UpGradeFailed);
    }
    system("reboot");
}

bool LogicContentUpgrades::copy(PhysicalDevice *pPHDev, QString pSrc, QString pDst)
{
    if(!pPHDev)
        return false;
    switch(pPHDev->mDevType)
    {
        case P_LAN:
        {
            break;
        }
        case P_MDISK:
        {
            PhysicalMobileDisk *tmpUpgradePHDev = VAR_CAST<PhysicalMobileDisk*>(pPHDev);
            if(!tmpUpgradePHDev)
                return false;
            return tmpUpgradePHDev->copy(pSrc, pDst);
        }
        case P_SodimasCAN:
        {
            break;
        }
        default:
            return false;
    }
    return false;
}

bool LogicContentUpgrades::cut(PhysicalDevice *pPHDev, QString pSrc, QString pDst)
{
    if(!pPHDev)
        return false;
    switch(pPHDev->mDevType)
    {
        case P_LAN:
        {
            break;
        }
        case P_MDISK:
        {
            PhysicalMobileDisk *tmpUpgradePHDev = VAR_CAST<PhysicalMobileDisk*>(pPHDev);
            if(!tmpUpgradePHDev)
                return false;
            return tmpUpgradePHDev->cut(pSrc, pDst);
        }
        case P_SodimasCAN:
        {
            break;
        }
        default:
            return false;
    }
    return false;
}

bool LogicContentUpgrades::del(PhysicalDevice *pPHDev, QString pPath)
{
    if(!pPHDev)
        return false;
    switch(pPHDev->mDevType)
    {
        case P_LAN:
        {
            break;
        }
        case P_MDISK:
        {
            PhysicalMobileDisk *tmpUpgradePHDev = VAR_CAST<PhysicalMobileDisk*>(pPHDev);
            if(!tmpUpgradePHDev)
                return false;
            return tmpUpgradePHDev->del(pPath);
        }
        case P_SodimasCAN:
        {
            break;
        }
        default:
            return false;
    }
    return false;
}

bool LogicContentUpgrades::mkdir(PhysicalDevice *pPHDev, QString pPath)
{
    if(!pPHDev)
        return false;
    switch(pPHDev->mDevType)
    {
        case P_LAN:
        {
            break;
        }
        case P_MDISK:
        {
            PhysicalMobileDisk *tmpUpgradePHDev = VAR_CAST<PhysicalMobileDisk*>(pPHDev);
            if(!tmpUpgradePHDev)
                return false;
            return tmpUpgradePHDev->mkdir(pPath);
        }
        case P_SodimasCAN:
        {
            break;
        }
        default:
            return false;
    }
    return false;
}

bool LogicContentUpgrades::copydir(PhysicalDevice *pPHDev, QString pSrc, QString pDst, bool subfolder)
{
    if(!pPHDev)
        return false;
    switch(pPHDev->mDevType)
    {
        case P_LAN:
        {
            break;
        }
        case P_MDISK:
        {
            PhysicalMobileDisk *tmpUpgradePHDev = VAR_CAST<PhysicalMobileDisk*>(pPHDev);
            if(!tmpUpgradePHDev)
                return false;
            return tmpUpgradePHDev->copydir(pSrc, pDst, subfolder);
        }
        case P_SodimasCAN:
        {
            break;
        }
        default:
            return false;
    }
    return true;
}

bool LogicContentUpgrades::deldir(PhysicalDevice *pPHDev, QString pDir)
{
    if(!pPHDev)
        return false;
    switch(pPHDev->mDevType)
    {
        case P_LAN:
        {
            break;
        }
        case P_MDISK:
        {
            PhysicalMobileDisk *tmpUpgradePHDev = VAR_CAST<PhysicalMobileDisk*>(pPHDev);
            if(!tmpUpgradePHDev)
                return false;
            return tmpUpgradePHDev->deldir(pDir);
        }
        case P_SodimasCAN:
        {
            break;
        }
        default:
            return false;
    }
    return false;
}

bool LogicContentUpgrades::cleardir(PhysicalDevice *pPHDev, QString pDir)
{
    if(!pPHDev)
        return false;
    switch(pPHDev->mDevType)
    {
        case P_LAN:
        {
            break;
        }
        case P_MDISK:
        {
            PhysicalMobileDisk *tmpUpgradePHDev = VAR_CAST<PhysicalMobileDisk*>(pPHDev);
            if(!tmpUpgradePHDev)
                return false;
            return tmpUpgradePHDev->cleardir(pDir);
        }
        case P_SodimasCAN:
        {
            break;
        }
        default:
            return false;
    }
    return false;
}

LogicContentUpgrades::STORAGESTATE LogicContentUpgrades::StorageIsValid()
{
    //>@1.�����Դ�б�
    m_UpgradeContentList.clear();
    //>@2.�βδ���
    if(m_SrcPath.isEmpty())
        return UpgradeNone;
    QDir tmpDir = QDir(m_SrcPath);
    if(!tmpDir.exists())
        return UpgradeNone;
    //>@3.�鿴�Ƿ���CMD�ļ�
    if(GetCmdList(m_SrcPath+D_UDISK_CMD))  //>@����п�ִ�е�CMD�ļ��������CMD�߼�����ģʽ
        return ExecCMD;
    //>@4.�鿴�Ƿ���������Ӧ�ó���
    if(GetContentList(m_SrcPath+D_UDISK_EXE, EXE, QStringList()<<".exe"))
        return UpgradeExe;
    //>@5.�鿴�Ƿ�������������
    if(GetContentList(m_SrcPath+D_UDISK_THEME, THEME, QStringList()<<".zip"))
        return UpgradeTheme;
    //>@6.�鿴RC�ļ����Ƿ�����������
    if(GetContentList(m_SrcPath+D_UDISK_RC_LOGO, AREALOGO, QStringList()<<".bmp"<<".jpg"<<".png"<<".svg"<<".gif") ||
       GetContentList(m_SrcPath+D_UDISK_RC_VIDEO, AREAVIDEO, QStringList()<<".avi"<<".mp4"<<".rmvb"<<".flv") ||
       GetContentList(m_SrcPath+D_UDISK_RC_IMAGE, AREAIMAGE, QStringList()<<".bmp"<<".jpg"<<".png"<<".svg"<<".gif") ||
       GetContentList(m_SrcPath+D_UDISK_RC_TEXT, AREATEXT, QStringList()<<".bmp"<<".jpg"<<".png"<<".svg"<<".font"))
        return UpgradeArea;
    return UpgradeNone;
}

bool LogicContentUpgrades::GetCmdList(QString pFile)
{
    cmdManager *tmpCmdManager = new cmdManager(pFile);
    if(!tmpCmdManager->OpenCMD())
    {
        tmpCmdManager->m_IsChanged = false;
        delete tmpCmdManager;
        return false;
    }
    bool flag = tmpCmdManager->GetItemElement("/CMD");
    if(flag == false)
    {
        tmpCmdManager->m_IsChanged = false;
        delete tmpCmdManager;
        return false;       //>@���DEV�ļ����ݲ��Ϸ������˳�
    }
    QString tmpString;
    QDomElement tmpElement;
    QDomNodeList tmpDeviceList = tmpCmdManager->itemElement.childNodes();
    for(int i=0;i<tmpDeviceList.count();i++)
    {
        tmpElement = tmpDeviceList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        tmpString = tmpElement.text();
        if(tmpString.isEmpty())
            continue;
        m_UpgradeContentList.insert(CMD, tmpString);
    }
    tmpCmdManager->m_IsChanged = false;
    delete tmpCmdManager;
    if(m_UpgradeContentList.values(CMD).count()>0)
        return true;
    return false;
}

bool LogicContentUpgrades::GetContentList(QString pDirPath, UPGRADECONTENT pContentType, QStringList pSuffix)
{
    QString tmpString;
    QStringList tmpPathList = getAllInFloders(pDirPath);
    if(tmpPathList.count()>0)
    {
        int count1 = tmpPathList.count();
        switch(pContentType)
        {
            case EXE:
            {
                for(int i=0;i<count1;i++)
                    m_UpgradeContentList.insert(EXE, tmpPathList.at(i));
                break;
            }
            case THEME:
            {
                for(int i=0;i<count1;i++)
                {
                    tmpString = tmpPathList.at(i);
                    QFileInfo tmpFileInfo(tmpString);
                    //>@zip���ļ��У��ں�ui����Ϊ����
                    if(tmpFileInfo.isFile() && tmpString.endsWith(".zip", Qt::CaseInsensitive))
                    {
                        m_UpgradeContentList.insert(THEME, tmpString);
                    }
                    else if(tmpFileInfo.isDir() && !SearchFiles(tmpString, "ui").isEmpty())
                    {
                        m_UpgradeContentList.insert(THEME, tmpString);
                    }
                }
                break;
            }
            default:
            {
                int count2 = pSuffix.count();
                for(int i=0;i<count1;i++)
                {
                    tmpString = tmpPathList.at(i);
                    for(int j=0;j<count2;j++)
                    {
                        if(tmpString.endsWith(pSuffix.at(j), Qt::CaseInsensitive))
                        {
                            m_UpgradeContentList.insert(pContentType, tmpString);
                            break;
                        }
                    }
                }
                break;
            }
        }
        if(m_UpgradeContentList.values(pContentType).count() > 0)
            return true;
    }
    return false;
}

bool LogicContentUpgrades::ExecCmdList(PhysicalDevice *pSrcPHDev, PhysicalDevice *pDstPHDev)
{
    QList<QString> tmpCmdList = m_UpgradeContentList.values(CMD);
    for(int i=0;i<tmpCmdList.count();i++)
    {
        QString tmpString = tmpCmdList.at(i);
        if(!ExecInstruc(tmpString))
            return false;
    }
    return true;
}

bool LogicContentUpgrades::ExecInstruc(QString pString)
{
    if(pString.isEmpty())
        return false;
    if(pString.contains(";"))
        pString.replace(";","&");
    system(pString.toLatin1().data());
    return true;
}

//>@pBackupΪFlaseʱ��ʾ�����ݵ������豸
bool LogicContentUpgrades::UpgradeExeList(PhysicalDevice *pSrcPHDev, PhysicalDevice *pDstPHDev, QStringList pExeList, QString pDstPath, bool pBackup)
{
    if(pExeList.isEmpty())
        return true;
    if(!pDstPHDev->mkdir(pDstPath))
    {
        //emit sInfo(QString("Can't create path on destination device!"), 10000);
        return false;
    }
    //>@��������
    QString tmpExePath, tmpExeName, tmpString;
    for(int i=0;i<pExeList.count();i++)
    {
        tmpExePath = pExeList.at(i);
        tmpExeName = getFileName(tmpExePath);
        tmpString = pDstPath + tmpExeName;
        //>@�����β��ж��Ƿ���Ҫ���ݵ�U��
        if(pBackup)
        {
            if(!cut(pSrcPHDev, tmpString, m_SrcPath + D_UDISK_BACKUP + tmpExeName))
            {
                //emit sInfo(QString("UpgradeExe#%1#BkErr").arg(tmpExeName), 10000);
                continue;
            }
        }
        if(!copy(pSrcPHDev, tmpExePath, tmpString))
        {
           // emit sInfo(QString("UpgradeExe#%1#CopyNewExeErr").arg(tmpExeName), 10000);
            continue;
        }
    }
    //emit sInfo(QString("UpgradeExe#AllFinished"), 10000);
    return true;
}

//>@pBackupΪFlaseʱ��ʾ�����ݵ�UDisk
bool LogicContentUpgrades::UpgradeThemeList(PhysicalDevice *pSrcPHDev, PhysicalDevice *pDstPHDev, QStringList pThemeList, QString pDstPath, bool pBackup)
{
    IDE_TRACE();
    if(pThemeList.isEmpty())
        return true;
    if(!pDstPath.endsWith('/'))
        pDstPath.append('/');
    if(!pDstPHDev->mDevPath.endsWith('/'))
        pDstPHDev->mDevPath.append('/');
    if(!pDstPHDev->mkdir(pDstPath))
    {
        //emit sInfo(tr("Can't create path in destination path!"), 1000);
        return false;
    }
    IDE_TRACE();
    QString tmpCurThemeName;
    GraphicsScene *tmpUiScene = GetThemeScene();
    if(tmpUiScene && tmpUiScene->m_UiManager)
        tmpCurThemeName = GetDirectoryName(tmpUiScene->m_UiManager->m_FileDir);
    //>@�������⣬��������������뵱ǰ�������е�����ͬ����������������
    //>@�����ļ��н�������ʱֱ�Ӹ��ǣ�zip����Ҳ��ֱ�Ӹ��ǣ�����ѡ���Ƿ񱸷�
    QString tmpThemePath, tmpThemeName, tmpThemeDir, tmpString;
    for(int i=0;i<pThemeList.count();i++)
    {
        tmpThemePath = pThemeList.at(i);
        QFileInfo tmpFileInfo(tmpThemePath);
        if(tmpFileInfo.isFile())
        {
            tmpThemeName = getFileName(tmpThemePath);
            if(!tmpThemeName.endsWith(".zip",Qt::CaseInsensitive))
                continue;
            QString tmpBufferZipName = pDstPHDev->mDevPath + D_TEMP_PATH + tmpThemeName;
            tmpThemeDir = tmpThemeName.remove(".zip", Qt::CaseInsensitive);
            tmpString = pDstPath + tmpThemeDir;
            //>@�ȿ�����TMPĿ¼
            if(copy(pSrcPHDev, tmpThemePath, tmpBufferZipName) == false)
            {
                //emit sInfo(QString("UpgradeTheme#%1#CopyToTempErr").arg(tmpThemePath), 1000);
                continue;
            }
            //>@ִ�б��ݲ���
            if(pBackup)
            {
                QDir tmpDir(tmpString);
                if(tmpDir.exists())   //>@���ϵͳ�����д����⣬��ѹ���������⵽U�̵�BK�ļ��У�������������
                {
                    ZipTask tmpZipTask;
                    if(tmpZipTask.Zip(tmpString, tmpBufferZipName, true) == false)
                    {
                        //emit sInfo(QString("UpgradeTheme#%1#ZipErr").arg(tmpBufferZipName), 1000);
                        continue;
                    }
                    if(cut(pSrcPHDev, tmpBufferZipName, m_SrcPath + D_UDISK_BACKUP + tmpThemeName) == false)
                    {
                        //emit sInfo(QString("UpgradeTheme#%1#BkErr").arg(tmpThemePath), 1000);
                        continue;
                    }
                }
            }
            //>@ִ�н�ѹ����
            ZipTask tmpZipTask;
            if(!(tmpZipTask.Unzip(tmpBufferZipName, pDstPath, true)))
            {
                //emit sInfo(QString("UpgradeTheme#%1#UnzipErr").arg(tmpThemePath), 10000);
                continue;
            }
            del(pSrcPHDev, tmpBufferZipName);
        }
        else if(tmpFileInfo.isDir())
        {
            IDE_TRACE();
            tmpThemeName = getFileName(tmpThemePath);
            QString tmpDstThemePath = pDstPath + tmpThemeName;
            //>@ִ�б��ݲ���
            if(pBackup)
            {
                IDE_TRACE();
                mkdir(pSrcPHDev, m_SrcPath + D_UDISK_BACKUP);
                copydir(pSrcPHDev, tmpDstThemePath, m_SrcPath + D_UDISK_BACKUP + tmpThemeName, true);
            }
            IDE_TRACE();
            //>@������ֱ�Ӹ���
            if(!copydir(pSrcPHDev, tmpThemePath, tmpDstThemePath, true))
            {
                //emit sInfo(QString("UpgradeTheme#%1#CopyErr").arg(tmpThemeName), 1000);
                continue;
            }
        }
    }
    //emit sInfo(QString("UpgradeTheme#AllFinished"), 10000);
    return true;
}

bool LogicContentUpgrades::UpgradeAreaList(PhysicalDevice *pSrcPHDev, PhysicalDevice *pDstPHDev, UPGRADECONTENT pContentType, QStringList pContentList, bool pBackup)
{
    if(pContentList.isEmpty())
        return true;
    //>@�鿴��UI�ļ����Ƿ���ڴ���������������ȡ������Ϣ�����������������
    GraphicsScene *tmpUiScene = GetThemeScene();
    if(!(tmpUiScene && tmpUiScene->m_UiManager))
        return false;
    GraphicsOperate *tmpComOperate = 0;
    QString tmpComRelPath;
    switch(pContentType)
    {
        case AREALOGO:
            tmpComOperate = tmpUiScene->m_ComDiagramGroup.value(COM_LOGO);
            tmpComRelPath = QString("/Resource/Image/");
            break;
        case AREAVIDEO:
            tmpComOperate = tmpUiScene->m_ComDiagramGroup.value(COM_VIDEO);
            tmpComRelPath = QString("/Resource/Film/");
            break;
        case AREAIMAGE:
            tmpComOperate = tmpUiScene->m_ComDiagramGroup.value(COM_IMAGE);
            tmpComRelPath = QString("/Resource/Image/");
            break;
        case AREATEXT:
            tmpComOperate = tmpUiScene->m_ComDiagramGroup.value(COM_TEXT);
            tmpComRelPath = QString("/Resource/Text/");
            break;
        default:
            return false;
    }
    if(!tmpComOperate)
        return false;
    //>@��ִ�й����������ɹ���ͨ���������ui�ļ��Լ���ǰ����
    QString tmpComRcPath = tmpUiScene->m_UiManager->m_FileDir + tmpComOperate->m_ComPath + tmpComRelPath;
    tmpComRcPath.replace("//", "/");
    QSize tmpSize = tmpComOperate->m_ComRect.size().toSize();
    QString tmpBufferPath = pDstPHDev->mDevPath + D_TEMP_PATH;
    QString tmpBkPath = m_SrcPath + D_UDISK_BACKUP;
    for(int i=0;i<pContentList.count();i++)
    {
        QString tmpFilePath = pContentList.at(i);
        //>@����
        QString tmpDstFile = regularFile(QString(), tmpFilePath, tmpBufferPath, tmpSize);
        if(tmpDstFile.isEmpty())
        {
           // emit sInfo(QString("reuglar file %1 error").arg(tmpFilePath), 10000);
            continue;
        }
        QString tmpFileName = getFileName(tmpDstFile);
        //>@ִ�б��ݲ���
        if(pBackup)
        {
            if(!cut(pSrcPHDev, tmpComRcPath + tmpFileName, tmpBkPath + tmpFileName))
            {
                //emit sInfo(QString("Back up %1 fail!").arg(tmpFileName), 10000);
                continue;
            }
        }
        if(!cut(pSrcPHDev, tmpFilePath, tmpComRcPath + tmpFileName))
        {
            //emit sInfo(QString("Upgrade %1 fail!").arg(tmpFileName), 10000);
            continue;
        }
    }
    return true;
}

