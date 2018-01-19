#include "treeitemclass.h"
#include "tabpagemanager.h"
#include "uipage.h"
#include "devicemanageform.h"
#include "eventmapform.h"

FileTreeItem::FileTreeItem(QTreeWidgetItem *parent, QString pFilePath, int type) :
    QTreeWidgetItem(parent, type)
{
    mFileInfo = 0;
    SetActive(false);
    mDelFileFlag = false;
    ChangeFile(pFilePath);
}

FileTreeItem::~FileTreeItem()
{
    //>@ɾ���Ѿ��򿪵�ҳ��
    if(mFileInfo)
    {
        TabPageManager *tmpTabPageManager = VAR_CAST<TabPageManager*>(GlobalPara.m_TabPageManager);
        if(tmpTabPageManager)
        {
            tmpTabPageManager->DelTabPage(mFileInfo);
        }
        if(mDelFileFlag)
        {
            DelFile(mFileInfo->FileDir+mFileInfo->FileName);
        }
        delete mFileInfo;
    }
}

void FileTreeItem::ChangeFile(QString pFilePath)
{
    FILE_TYPE tmpType = getFileType(pFilePath);
    if(tmpType == TYPE_INVALID)
        return;
    if(mFileInfo)
        delete mFileInfo;
    SetActive(false);
    mDelFileFlag = false;
    mFileInfo = new PAGE_INFO;
    mFileInfo->FileType = tmpType;
    mFileInfo->FileDir = getFileDirectory(pFilePath);
    mFileInfo->FileName = getFileName(pFilePath);
    mFileInfo->TreeItem = this;
    switch(mFileInfo->FileType)
    {
        case TYPE_UI:
            mFileInfo->IconName = QString(":/Page/rc/Page/ui.png");
            break;
        case TYPE_MDP:
            mFileInfo->IconName = QString(":/Page/rc/Page/mdp.png");
            break;
        case TYPE_MAP:
            mFileInfo->IconName = QString(":/Page/rc/Page/map.png");
            break;
        case TYPE_STC:
            mFileInfo->IconName = QString(":/Page/rc/Page/stc.png");
            break;
        default:
            mFileInfo->IconName = QString(":/Menu/rc/Menu/file.png");
            break;
    }
    setIcon(0, QIcon(mFileInfo->IconName));
    setText(0, mFileInfo->FileName);
    //tmpTreeItem->setFlags(Qt::ItemIsEditable |Qt::ItemIsEnabled);
}

void FileTreeItem::SetActive(bool flag)
{
    if(flag)
        setFont(0, QFont("Arial", 9, QFont::Bold));
    else
        setFont(0, QFont("Arial", 9, QFont::Normal));
}

bool FileTreeItem::IsActive()
{
    QFont tmpFont = font(0);
    return tmpFont.bold();
}

bool FileTreeItem::Open()
{
    TabPageManager *tmpTabManager = VAR_CAST<TabPageManager *>(GlobalPara.m_TabPageManager);
    if(!tmpTabManager)
        return false;
    return tmpTabManager->OperatePageItem(mFileInfo, CodeInsert);
}

bool FileTreeItem::Close()
{
    TabPageManager *tmpTabManager = VAR_CAST<TabPageManager *>(GlobalPara.m_TabPageManager);
    if(!tmpTabManager)
        return false;
    return tmpTabManager->OperatePageItem(mFileInfo, CodeDelete);
}

QString FileTreeItem::GetProjectPath()
{
    ProjectTreeItem *tmpProItem = (ProjectTreeItem*)parent();
    if(!(tmpProItem && tmpProItem->mProManager))
        return QString();
    return tmpProItem->mProManager->m_FileDir;
}

xmlParse *FileTreeItem::GetFileParse()
{
    if(mFileInfo)
    {
        switch(mFileInfo->FileType)
        {
            case TYPE_UI:
            {
                uiPage *tmpPage = VAR_CAST<uiPage*>(mFileInfo->PageDialogPtr);
                if(!tmpPage)
                    break;
                return tmpPage->m_GraphicsView->scene->m_UiManager;
            }
            case TYPE_MDP:
            {
                DeviceManageForm *tmpPage = VAR_CAST<DeviceManageForm*>(mFileInfo->PageDialogPtr);
                if(!tmpPage)
                     break;
                if(!tmpPage->mDeviceScene)
                     break;
                return tmpPage->mDeviceScene->mMdpManager;
            }
            case TYPE_MAP:
            {
                EventMapForm *tmpPage = VAR_CAST<EventMapForm*>(mFileInfo->PageDialogPtr);
                if(!tmpPage)
                     break;
                return tmpPage->mMapManager;
            }
            case TYPE_STC:
            {
                stcPage *tmpPage = VAR_CAST<stcPage*>(mFileInfo->PageDialogPtr);
                if(!tmpPage)
                     break;
                return tmpPage->m_StcManager;
            }
            default:
                break;
        }
    }
    return (xmlParse *)0;
}

ProjectTreeItem::ProjectTreeItem(QTreeWidgetItem *parent, bstproManager *pManager, int type) :
    QTreeWidgetItem(parent, type)
{
    SetActive(false);
    SetRunning(false);
    mDelFileFlag = false;
    if(!pManager)
    {
        IDE_TRACE();
        return;
    }
    mProPath = pManager->m_FileDir+pManager->m_FileName;
    mProManager = pManager;
    setText(0, pManager->m_FileName);
    setIcon(0,QIcon(":/Menu/rc/Menu/solution.png"));
    Open();

    setExpanded(true);
}

ProjectTreeItem::ProjectTreeItem(QTreeWidgetItem *parent, QString pProFile, int type) :
    QTreeWidgetItem(parent, type)
{
    SetActive(false);
    SetRunning(false);
    mDelFileFlag = false;
    mProPath = pProFile;
    mProManager = 0;
    setText(0, getFileName(pProFile));
    setIcon(0,QIcon(":/Menu/rc/Menu/solution.png"));
    Open();

    setExpanded(true);
}

ProjectTreeItem::~ProjectTreeItem()
{
    Close();
    //>@ɾ��Դ�ļ�
    if(mDelFileFlag && !mProPath.isEmpty())
    {
        DelFolder(getFileDirectory(mProPath));
    }
}

bool ProjectTreeItem::isOpen()
{
    return IsActive();
}

bool ProjectTreeItem::Open()
{
    if(isOpen())
        return true;
    Close();
    if(mProPath.isEmpty())
        return false;
    mProManager = new bstproManager(mProPath);
    if(mProManager->OpenPro() == false)
    {
        delete mProManager;
        mProManager = 0;
        return false;
    }
    //>@��ʼ��TreeItem
    if(mProManager->GetItemElement("/FileList"))
    {
        QDomNodeList tmpList = mProManager->itemElement.childNodes();
        for(int i=0;i<tmpList.count();i++)
        {
            QDomElement tmpElement = tmpList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            AddFile(mProManager->m_FileDir + tmpElement.text());
        }
    }
    setExpanded(true);
    SetActive(true);
    return true;
}

void ProjectTreeItem::Close()  //>@�رչ��̣���ɾ�������е��ļ�
{
    if(mProManager)
    {
        for(int i=childCount()-1;i>=0;i--)
        {
            FileTreeItem *tmpItem = (FileTreeItem *)(child(i));
            RemoveFile(tmpItem, false);
        }
        delete mProManager;
        mProManager = 0;
    }
    SetActive(false);
}

void ProjectTreeItem::Clear()  //>@��չ��̣���ɾ�������е��ļ�
{
    if(mProManager)
    {
        for(int i=childCount()-1;i>=0;i--)
        {
            FileTreeItem *tmpItem = (FileTreeItem *)(child(i));
            DelFile(tmpItem, mDelFileFlag);
        }
    }
}

//>@�鿴��ǰ�������Ƿ��Ѿ��д��ļ������Ҳ���ͬһ�ļ����������ɾ����Ȼ���ٿ�����
//>@������Ե�ַ/�ļ���
QString ProjectTreeItem::CopyToHere(QString pFilePath)
{
    if(mProManager)
    {
        FILE_TYPE tmpFileType = getFileType(pFilePath);
        if(tmpFileType < TYPE_UI || tmpFileType > TYPE_STC)
            return QString();
        //>@�����ļ�
        QString tmpFileDir = getFileDirectory(pFilePath);
        QString tmpFileName = getFileName(pFilePath);
        //>@�Ƚ��ļ��Ƿ��ڵ�ǰ������
        if(tmpFileDir.compare(mProManager->m_FileDir, Qt::CaseInsensitive) == 0)
            return tmpFileName;
        QString tmpOldPath = mProManager->m_FileDir + tmpFileName;
        if(!CopyFile(pFilePath, tmpOldPath))
            return QString();
        //>@�����UI������Ҫ������Դ�ļ���
        if(tmpFileType == TYPE_UI)
        {
            QDir tmpSrcDir(tmpFileDir);
            QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (int i=0; i<folders.size(); ++i)
            {
                QString tmpSrcPath = tmpFileDir + folders[i];
                QString tmpDstPath = mProManager->m_FileDir + folders[i];
                if(!CopyFolder(tmpSrcPath, tmpDstPath, true))
                {
                    IDE_TRACE_STR(tmpSrcPath);
                }
            }
        }
        return tmpFileName;
    }
    return QString();
}

FileTreeItem *ProjectTreeItem::GetFileItem(FILE_TYPE pType)
{
    if(pType >= TYPE_UI && pType <= TYPE_STC)
    {
        for(int i=0;i<childCount();i++)
        {
            FileTreeItem *tmpItem = (FileTreeItem *)(child(i));
            if(!(tmpItem && tmpItem->mFileInfo))
                continue;
            if(tmpItem->mFileInfo->FileType == pType)
            {
                return tmpItem;
            }
        }
    }
    return (FileTreeItem *)0;
}

TabPage *ProjectTreeItem::GetPagePtr(FILE_TYPE pType)
{
    FileTreeItem *tmpFileItem = GetFileItem(pType);
    if(tmpFileItem)
    {
        return tmpFileItem->mFileInfo->PageDialogPtr;
    }
    return (TabPage *)0;
}

bool ProjectTreeItem::AddFile(QString pFilePath)
{
    if(!(mProManager && mProManager->m_Valid))
        return false;
    if(pFilePath.isEmpty())
        return false;
    QString tmpString = getFileSuffix(pFilePath);
    if(tmpString.isEmpty())
        return false;
    QString tmpFileName = getFileName(pFilePath);
    //>@��ֹ�ظ�����
    for(int i=childCount()-1;i>=0;i--)
    {
        FileTreeItem *tmpItem = (FileTreeItem *)(child(i));
        if(tmpItem == 0)
            continue;
        if(tmpItem->text(0).compare(tmpFileName) == 0)
        {
            int ret = QMessageBox::question(0, QString("Question"),
                                           QString("The same file already exists.\n"
                                                    "Do you want to replace?"),
                                           QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::No);
            if(ret == QMessageBox::Yes)
            {
                if(!DelFile(tmpItem, true))
                    return false;
            }
            else
            {
                return false;
            }
        }
    }
    //>@�鿴����ӵ��ļ��Ƿ��ڱ������ļ����ڣ����򿽱������ļ�����
    QString tmpNewFilePath = CopyToHere(pFilePath);
    if(!(!tmpNewFilePath.isEmpty() && mProManager->AddFile(tmpString, getFileName(tmpNewFilePath))))
        return false;
    FileTreeItem *tmpTreeItem = new FileTreeItem(this, mProManager->m_FileDir + tmpNewFilePath);
    addChild(tmpTreeItem);
    return true;
}

//>@����Ŀ��ɾ��ĳ���ļ���������bstpro�ļ���/�����ļ���/���������������ط�ɾ������
bool ProjectTreeItem::DelFile(FileTreeItem *&pFileItem, bool pDelFile)
{
    if(pFileItem == 0)
        return false;
    if(!(mProManager && mProManager->m_Valid))
        return false;
    QString tmpFileName = pFileItem->text(0);
    if(tmpFileName.isEmpty())
        return false;
    QString tmpString = getFileSuffix(tmpFileName);
    if(tmpString.isEmpty())
        return false;
    if(mProManager->DelFile(tmpString))
    {
        //>@ɾ��TreeItem
        RemoveFile(pFileItem, pDelFile);
        return true;
    }
    return false;
}

//>@�����ӽ������Ƴ�ĳ���pDelFileָʾɾ�������е�ԭʼ�ļ�
void ProjectTreeItem::RemoveFile(FileTreeItem *&pFileItem, bool pDelFile)
{
    pFileItem->mDelFileFlag = pDelFile;
    removeChild(pFileItem);
    delete pFileItem;
    pFileItem = 0;
}

void ProjectTreeItem::SetActive(bool flag)
{
    if(flag)
        setFont(0, QFont("Arial", 10, QFont::DemiBold, true));
    else
        setFont(0, QFont("Arial", 10, QFont::DemiBold, false));
}

bool ProjectTreeItem::IsActive()
{
    QFont tmpFont = font(0);
    return tmpFont.italic();
}

void ProjectTreeItem::SetRunning(bool flag)
{
    setData(0, ROLE_RUNNING, QVariant(flag));
    if(flag)
        setIcon(0,QIcon(":/Menu/rc/Menu/success.png"));
    else
        setIcon(0,QIcon(":/Menu/rc/Menu/solution.png"));
}

bool ProjectTreeItem::IsRunning()
{
    return data(0, ROLE_RUNNING).toBool();
}

//>@pDevName:ʵ�ʵ�ַ��������Ҫʱ����Ϊ�豸����
DeviceTreeItem::DeviceTreeItem(BaseDevice::DEVICETYPE pType, QString pAddr, QString pBufferPath, DEVICESTATE pState, int type) :
    QTreeWidgetItem(type)
{
    setFont(0, QFont("Arial", 10, QFont::Black));
    mDelFileFlag = false;
    mInterfaceDevice = CreateDevice(pType, pAddr, pBufferPath);
    mInterfaceDevice->SetDeviceTreeItem(this, pState);
    UpdateDeviceText();
    SetBuffer(pBufferPath);
    //>@ɨ�蹤��
    if(mInterfaceDevice)
    {
        mInterfaceDevice->Scan();
    }
}

DeviceTreeItem::DeviceTreeItem(BaseDevice *pDev, int type) :
    QTreeWidgetItem(type)
{
    setFont(0, QFont("Arial", 10, QFont::Black));
    mDelFileFlag = false;
    mInterfaceDevice = pDev;
    if(mInterfaceDevice)
    {
        mInterfaceDevice->SetDeviceTreeItem(this, mInterfaceDevice->mDevState);
        UpdateDeviceText();
        //>@ɨ�蹤��
        mInterfaceDevice->Scan();
    }
}

DeviceTreeItem::~DeviceTreeItem()
{
    Clear();
    if(mInterfaceDevice)
    {
        //>@ɾ��Դ�ļ�
        if(mDelFileFlag)
        {
            DelFolder(mInterfaceDevice->mBufferPath);
        }
        mInterfaceDevice->deleteLater();
        mInterfaceDevice = 0;
    }
}

void DeviceTreeItem::UpdateDeviceText()
{
    if(mInterfaceDevice)
    {
        if(!mInterfaceDevice->mDevNickname.isEmpty())
            setText(0, mInterfaceDevice->mDevNickname);
        else if(!mInterfaceDevice->mDevAddr.isEmpty())
            setText(0, mInterfaceDevice->mDevAddr);
        else if(!mInterfaceDevice->mBufferPath.isEmpty())
            setText(0, mInterfaceDevice->mBufferPath);
    }
}

void DeviceTreeItem::SetState(DEVICESTATE pState)
{
    if(mInterfaceDevice)
    {
        switch(pState)
        {
            case S_OFFLINE:
            {
                mInterfaceDevice->mDevState = pState;
                setIcon(0,QIcon(":/Menu/rc/Menu/notpass.png"));
                break;
            }
            case S_ONLINE:
            {
                mInterfaceDevice->mDevState = pState;
                setIcon(0,QIcon(":/Menu/rc/Menu/pass.png"));
                break;
            }
            case S_DEVSYNC:
            {
                mInterfaceDevice->mDevState = pState;
                setIcon(0,QIcon(":/Menu/rc/Menu/success.png"));
                break;
            }
            default:
                break;
        }
    }
    else
    {
        setIcon(0,QIcon(":/Menu/rc/Menu/notpass.png"));
    }
}

DEVICESTATE DeviceTreeItem::GetState()
{
    if(mInterfaceDevice)
        return mInterfaceDevice->mDevState;
    return S_OFFLINE;
}

void DeviceTreeItem::SetBuffer(QString pBufferPath)
{
    if(mInterfaceDevice)
        mInterfaceDevice->mBufferPath = pBufferPath;
}

QString DeviceTreeItem::GetBuffer()
{
    if(mInterfaceDevice)
        return mInterfaceDevice->mBufferPath;
    return QString();
}

//>@�����ļ��ľ���·����ֻ���ڹ��̲��ڻ�����ʱ����Ҫ��������
QString DeviceTreeItem::CopyToHere(QString pBstproFile)
{
    QString tmpSrcBstproDir = getFileDirectory(pBstproFile);
    QString tmpSrcDeviceDir = getParentDirectory(tmpSrcBstproDir);
    QString tmpBufferPath = GetBuffer();
    //>@�鿴�����������Ƿ��ڵ�ǰ�豸����ռ��У����������Ҫ����
    if(tmpBufferPath.compare(tmpSrcDeviceDir, Qt::CaseInsensitive) == 0)
        return pBstproFile;
    QString tmpBstproDirName = GetDirectoryName(tmpSrcBstproDir);
    QString tmpDstDir = tmpBufferPath + tmpBstproDirName;
    QString tmpBstproName = getFileName(pBstproFile);
    if(!tmpDstDir.endsWith('/'))
        tmpDstDir.append('/');
    QString tmpDstBstproPath = tmpDstDir + tmpBstproName;
    //>@���û�д��ļ������ڻ���������һ���µ�
    if(!isFileExist(pBstproFile))
    {
        CreatPath(tmpDstDir);
        bstproManager *tmpManager = new bstproManager(tmpDstBstproPath);
        tmpManager->NewPro();
        tmpManager->deleteLater();
    }
    //>@���û��Դ�ļ��У��򲻽��п���
    QDir tmpSrcDir(tmpSrcBstproDir);
    if(tmpSrcDir.exists())
    {
        if(CopyFolder(tmpSrcBstproDir, tmpDstDir, true))
            return tmpDstBstproPath;
    }
    else
    {
        return tmpDstBstproPath;
    }
    return QString();
}

ProjectTreeItem* DeviceTreeItem::GetProject(bstproManager *pManager)
{
    if(!pManager)
        return (ProjectTreeItem *)0;
    QString tmpFileName = pManager->m_FileName;
    //>@��ֹ�ظ�����
    for(int i=0;i<childCount();i++)
    {
        ProjectTreeItem *tmpItem = (ProjectTreeItem *)(child(i));
        if(tmpItem == 0)
            continue;
        if(tmpItem->text(0).compare(tmpFileName) == 0)
        {
            return tmpItem;
        }
    }
    return (ProjectTreeItem *)0;
}

ProjectTreeItem* DeviceTreeItem::GetProject(QString pBstproFile)
{
    if(pBstproFile.isEmpty())
        return (ProjectTreeItem *)0;
    QString tmpString = getFileSuffix(pBstproFile);
    if(tmpString.isEmpty())
        return (ProjectTreeItem *)0;
    QString tmpFileName = getFileName(pBstproFile);
    //>@��ֹ�ظ�����
    for(int i=0;i<childCount();i++)
    {
        ProjectTreeItem *tmpItem = (ProjectTreeItem *)(child(i));
        if(tmpItem == 0)
            continue;
        if(tmpItem->text(0).compare(tmpFileName) == 0)
        {
            return tmpItem;
        }
    }
    return (ProjectTreeItem *)0;
}

bool DeviceTreeItem::AddProject(QString pBstproFile)
{
    ProjectTreeItem *tmpItem = GetProject(pBstproFile);
    if(tmpItem)
    {
        int ret = QMessageBox::question(0, QString("Question"),
                                       QString("The same project already exists.\n"
                                          "Do you want to replace?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            if(!DelProject(tmpItem, true))
                return false;
        }
        else
        {
            return false;
        }
    }
    QString tmpNewProFile = CopyToHere(pBstproFile);
    if(tmpNewProFile.isEmpty())
        return false;
    ProjectTreeItem *tmpTreeItem = new ProjectTreeItem(this, tmpNewProFile);
    addChild(tmpTreeItem);
    return true;
}

bool DeviceTreeItem::AddProject(bstproManager *pManager)
{
    ProjectTreeItem *tmpItem = GetProject(pManager);
    if(tmpItem)
    {
        int ret = QMessageBox::question(0, QString("Question"),
                                       QString("The same project already exists.\n"
                                          "Do you want to replace?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            if(!DelProject(tmpItem, true))
                return false;
        }
        else
        {
            return false;
        }
    }
    QString tmpProFile = pManager->m_FileDir+pManager->m_FileName;
    QString tmpNewProFile = CopyToHere(tmpProFile);
    if(tmpNewProFile.isEmpty())
        return false;
    if(tmpNewProFile.compare(tmpProFile))  //>@�����˿�������Ҫ���½���Manager
    {
        pManager->deleteLater();
        pManager = new bstproManager(tmpNewProFile);
        pManager->OpenPro();
    }
    ProjectTreeItem *tmpTreeItem = new ProjectTreeItem(this, pManager);
    addChild(tmpTreeItem);
    return true;
}

bool DeviceTreeItem::DelProject(ProjectTreeItem *&pTreeItem, bool pDelFile)
{
    if(pTreeItem == 0)
        return false;
    pTreeItem->mDelFileFlag = pDelFile;
    removeChild(pTreeItem);
    delete pTreeItem;
    pTreeItem = 0;
    return true;
}

void DeviceTreeItem::Clear()
{
    for(int i=childCount()-1;i>=0;i--)
    {
        ProjectTreeItem *tmpItem = (ProjectTreeItem *)(child(i));
        if(tmpItem == 0)
            continue;
        DelProject(tmpItem, mDelFileFlag);
    }
}

void DeviceTreeItem::SetRunningProject(ProjectTreeItem *pProItem)
{
    if(!pProItem)
        return;
    for(int i=0;i<childCount();i++)
    {
        ProjectTreeItem *tmpItem = (ProjectTreeItem *)(child(i));
        if(tmpItem == 0)
            continue;
        if(pProItem == tmpItem)
        {
            tmpItem->SetRunning(true);
        }
        else
        {
            tmpItem->SetRunning(false);
        }
    }
}

void DeviceTreeItem::SetRunningProject(QString pBstproFile)
{
    if(pBstproFile.isEmpty())
        return;
    if(GetState() != S_DEVSYNC)
        return;
    //>@ͨ���豸�ӿڽ��豸�ϵĻ���̸ı�
    //>�����??????????????????????????????

    QString tmpBstproName = getFileName(pBstproFile);
    for(int i=0;i<childCount();i++)
    {
        ProjectTreeItem *tmpItem = (ProjectTreeItem *)(child(i));
        if(tmpItem == 0)
            continue;
        if(tmpItem->text(0).compare(tmpBstproName, Qt::CaseInsensitive) == 0)
        {
            tmpItem->SetRunning(true);
        }
        else
        {
            tmpItem->SetRunning(false);
        }
    }
}

ProjectTreeItem *DeviceTreeItem::GetRunningProject()
{
    for(int i=0;i<childCount();i++)
    {
        ProjectTreeItem *tmpItem = (ProjectTreeItem *)(child(i));
        if(tmpItem == 0)
            continue;
        if(tmpItem->IsRunning())
            return tmpItem;
    }
    return (ProjectTreeItem *)0;
}

