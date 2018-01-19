#include "globalfun.h"

const QString xmlParse::XmlHead = "version=\"1.0\" encoding=\"UTF-8\"";
const QString xmlParse::XmlSuffix = ".xml";

int CalcPixelFromPoint(int pPoint)
{
    return (DPI * pPoint / 72);
}
int CalcPointFromPixel(int pPixel)
{
    return (pPixel * 72 / DPI);
}

qreal RegularAngle(qreal angle)
{
    while (angle < 0)
        angle += 2.0*PI;
    while (angle > 2.0*PI)
        angle -= 2.0*PI;
    return angle;
}

//>@���������270�ȵĲ�ֵ
qreal CalDiffBeginAngle(qreal angle)
{
    qreal tmpAngle = RegularAngle(angle);
    if(tmpAngle >= 0 && tmpAngle < PI/2.0)
        return PI/2.0 + tmpAngle;
    else if(tmpAngle >= PI/2.0 && tmpAngle < 3.0*PI/2.0)
        return 3.0*PI/2.0 - tmpAngle;
    else if(tmpAngle >= 3.0*PI/2.0 && tmpAngle < 2.0*PI)
        return tmpAngle - 3.0*PI/2.0;
    return 0;
}

//>@"Arial", pPrior��ʾ������Ȼ��߸߶�����
int CalcFontSizeFromSize(QString pFontType, QSize pSize, PRIOR pPrior)
{
    if(pFontType.isEmpty())
        return -1;
    if(pSize.isValid() == false)
        return -1;
    int fontsize=1;
    while(fontsize < 100) //>@ȷ��fontsize���Ϊ100����ֹ��ѭ��
    {
        QFont tmp(pFontType,fontsize);
        tmp.setPointSize(fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsWide = fm.width("8");
        qreal pixelsHigh = fm.height();
        if(pPrior == PRIOR_WIDTH)
        {
            if(pixelsWide>=pSize.width())
                break;
        }
        else if(pPrior == PRIOR_HEIGHT)
        {
            if(pixelsHigh >= pSize.height())
                break;
        }
        ++fontsize;
    }
    return fontsize;
}

int CalcFontSizeFromRect(QString pFontType, QSize pSize)
{
    if(pFontType.isEmpty())
        return -1;
    if(pSize.isValid() == false)
        return -1;
    int fontsize=1;
    while(fontsize < 100) //>@ȷ��fontsize���Ϊ100����ֹ��ѭ��
    {
        QFont tmp(pFontType,fontsize);
        tmp.setPointSize(fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsWide = fm.width("8");
        qreal pixelsHigh = fm.height();
        if(pixelsWide>=pSize.width() || pixelsHigh >= pSize.height())
            break;
        ++fontsize;
    }
    return fontsize;
}

QSize CalcSizeFromFontSize(QString pFontType, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width('8'), fm.height());
}

QSize CalcSizeFromFontSize(QString pFontType, QChar pChar, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width(pChar), fm.height());
}

QSize CalcSizeFromFontSize(QString pFontType, QString pText, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width(pText), fm.height());
}

void WriteText(QPainter *pPainter, QString pText, QRect pRect, QFont pFont, QColor pColor)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isEmpty() == false)
        return;
    pPainter->save();
    pPainter->setFont(pFont);
    pPainter->setPen(pColor);
    pPainter->drawText(pRect, pText);
    pPainter->restore();
}

//>@��������д��һ��
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType, QColor pColor, PRIOR pPrior)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isEmpty() == false)
        return;
    int fontsize = 1;
    int fontwidth = pRect.width()/num;
    int fontheight = pRect.height();
    fontsize = CalcFontSizeFromSize(pFontType, QSize(fontwidth, fontheight), pPrior);
    QFont font(pFontType, fontsize, QFont::DemiBold, false);
    font.setUnderline(false);
    WriteText(pPainter, pText, pRect, font, pColor);
}

/*
    QFontMetricsF fm(tmpFontInfo.mFont);
    textRect =  QRectF(fm.boundingRect(rect.toRect(), Qt::AlignLeft, tmpContent));
    p.drawText(rect, tmpContent);
*/

//>@���������С�Զ��������ֲ����л���
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType, QColor pColor, int pFontSize)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isValid() == false)
        return;
    QSize tmpSize = CalcSizeFromFontSize(pFontType, pFontSize);
    QFont font(pFontType, pFontSize, QFont::DemiBold, false);
    font.setUnderline(false);
    pPainter->save();
    pPainter->setFont(font);
    pPainter->setPen(pColor);
    quint32 tmpRowCharCount = (qreal)(pRect.width()) / (qreal)(tmpSize.width());
    if(tmpRowCharCount <= 0) //>@��ֹ��ѭ��bug
        tmpRowCharCount = 1;
    quint32 tmpColumnCount = (qreal)(pText.count()) / (qreal)(tmpRowCharCount) + 1;
    for(int i=0;i<tmpColumnCount;i++)
    {
        int y = pRect.y()+i*(tmpSize.height()+2);
        if(y >= pRect.y() + pRect.height() - tmpSize.height())
            break;
        pPainter->drawText(pRect.x(),
                           y,
                           pText.mid(tmpRowCharCount*i, tmpRowCharCount));
    }
    pPainter->restore();
}

qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += 2.0*PI;
    while (angle > 2.0*PI)
        angle -= 2.0*PI;
    return angle;
}

bool isFileExist(QString FileName)
{
    if(FileName.isEmpty())
        return false;
    QFile file(FileName);
    bool flag = file.exists();
    return flag;
}

bool delFile(QString FileName)
{
    if(FileName.isEmpty())
        return false;
    QFile file(FileName);
    bool flag = file.remove();
    return flag;
}

QStringList SplitString(QString pString, QString pKey)
{
    QStringList tmpStringList;
    tmpStringList.clear();
    if(pString.isEmpty())
        return tmpStringList;
    int index1 = 0, index2 = 0;
    int count = pString.length();
    while(index1<count)
    {
        index2 = pString.indexOf(pKey, index1);
        if(index2 < 0)
        {
            tmpStringList << pString.mid(index1);
            return tmpStringList;
        }
        tmpStringList << pString.mid(index1, index2-index1);
        index1 = index2 + 1;
    }
    return tmpStringList;
}

QString  BiteorMega(int peso)
{
    QString humanread;
    double canno = peso / 1024;
    int sale = canno;
    if (peso > 0)
    {
        if (canno < 1)
        {
           sale = 1;
        }
    }

   if (sale < 1025)
   {
       humanread = QString("%1.KB").arg(sale);
       return humanread;
   }

   float megad = sale / 1024;

   if (megad < 1025)
   {
       humanread = QString("%1.MB").arg(megad);
       return humanread;
   }
   else
   {
       humanread = QString("1.GB+");
   }
   return humanread;
}

bool is_file(QString fullFileName)
{
    if (!fullFileName.size() > 0) {
      return false;
    }
    QDir diro(fullFileName);
       if (diro.exists()) {
         return false;
       }
    QFile f( fullFileName );
    if ( f.exists(fullFileName) ) {
    return true;
    } else {
    return false;
    }
}

bool StringToInt(QString pString,int &pInt)
{
    bool ok;
    int dec = pString.toInt(&ok, 10);  //>@10����
    if(ok)
        pInt = dec;
    return ok;
}

QString getFileIconName(QString pFileName)
{
    QString tmpIconString;
    if (pFileName.isEmpty())
        return tmpIconString;
    int index = pFileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = pFileName.mid(index);
    else
        return tmpIconString;
    if(!QString::compare(fileSuffix, ".xml", Qt::CaseInsensitive))
    {
        tmpIconString = QString(":/");
    }
    return tmpIconString;
}

BASICUI_TYPE getBasicUiType(QString pName)
{
    if(pName.isEmpty())
        return BASICUI_UNKNOW;
    else if(pName.compare(QString("enum"), Qt::CaseInsensitive) == 0)
        return BASICUI_ENUM;
    else if(pName.compare(QString("stack"), Qt::CaseInsensitive) == 0)
        return BASICUI_STACK;
    else if(pName.compare(QString("string"), Qt::CaseInsensitive) == 0)
        return BASICUI_STRING;
    else if(pName.compare(QString("enetaddr"), Qt::CaseInsensitive) == 0)
        return BASICUI_ENETADDR;
    else if(pName.compare(QString("num"), Qt::CaseInsensitive) == 0)
        return BASICUI_NUM;
    else if(pName.compare(QString("check"), Qt::CaseInsensitive) == 0)
        return BASICUI_CHECK;
    else if(pName.compare(QString("button"), Qt::CaseInsensitive) == 0)
        return BASICUI_BUTTON;
    else if(pName.compare(QString("switch"), Qt::CaseInsensitive) == 0)
        return BASICUI_SWITCH;
    else if(pName.compare(QString("bool"), Qt::CaseInsensitive) == 0)
        return BASICUI_BOOL;
    else if(pName.compare(QString("label"), Qt::CaseInsensitive) == 0)
        return BASICUI_LABEL;
    else if(pName.compare(QString("map"), Qt::CaseInsensitive) == 0)
        return BASICUI_MAP;
    else if(pName.compare(QString("file"), Qt::CaseInsensitive) == 0)
        return BASICUI_FILE;
    return BASICUI_UNKNOW;
}

QString getBasicUiString(BASICUI_TYPE pType)
{
    if(BASICUI_ENUM == pType)
        return QString("enum");
    else if(BASICUI_STACK == pType)
        return QString("stack");
    else if(BASICUI_STRING == pType)
        return QString("string");
    else if(BASICUI_ENETADDR == pType)
        return QString("enetaddr");
    else if(BASICUI_NUM == pType)
        return QString("num");
    else if(BASICUI_CHECK == pType)
        return QString("check");
    else if(BASICUI_BUTTON == pType)
        return QString("button");
    else if(BASICUI_SWITCH == pType)
        return QString("switch");
    else if(BASICUI_BOOL == pType)
        return QString("bool");
    else if(BASICUI_LABEL == pType)
        return QString("label");
    else if(BASICUI_MAP == pType)
        return QString("map");
    else if(BASICUI_FILE == pType)
        return QString("file");
    return QString();
}

FILE_TYPE getFileType(QString FileName)
{
    FILE_TYPE tmpFileType = TYPE_INVALID;
    if (FileName.isEmpty())
        return tmpFileType;
    int index = FileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = FileName.mid(index);
    else
        return TYPE_INVALID;
    if(!QString::compare(fileSuffix, ".bstpro", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_PRO;
    }
    else if(!QString::compare(fileSuffix, ".ui", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_UI;
    }
    else if(!QString::compare(fileSuffix, ".mdp", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_MDP;
    }
    else if(!QString::compare(fileSuffix, ".stc", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_STC;
    }
    else if(!QString::compare(fileSuffix, ".sto", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_STO;
    }
    else if(!QString::compare(fileSuffix, ".rc", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_RC;
    }
    else if(!QString::compare(fileSuffix, ".xml", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_XML;
    }
    else if(!QString::compare(fileSuffix, ".devinfo", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_DEVINFO;
    }
    else if(!QString::compare(fileSuffix, ".map", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_MAP;
    }
    else if(fileSuffix.contains(".avi", Qt::CaseInsensitive) ||
            fileSuffix.contains(".mp4", Qt::CaseInsensitive) ||
            fileSuffix.contains(".flv", Qt::CaseInsensitive) ||
            fileSuffix.contains(".rmvb", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_VIDEOEDITOR;
    }
    else if(fileSuffix.contains(".png", Qt::CaseInsensitive) ||
            fileSuffix.contains(".jpg", Qt::CaseInsensitive) ||
            fileSuffix.contains(".svg", Qt::CaseInsensitive) ||
            fileSuffix.contains(".gif", Qt::CaseInsensitive) ||
            fileSuffix.contains(".bmp", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_IMAGEEDITOR;
    }
    else if(fileSuffix.contains(".wav", Qt::CaseInsensitive) ||
            fileSuffix.contains(".mp3", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_AUDIOEDITOR;
    }
    return tmpFileType;
}

FILE_TYPE getFileType(FILE_INFO *pFileInfo)
{
    if(pFileInfo->CategoryType.compare(QString("Solution"), Qt::CaseInsensitive)==0)
    {
        return TYPE_PRO;
    }
    else if(pFileInfo->CategoryType.compare(QString("File"), Qt::CaseInsensitive)==0)
    {
        return getFileType(pFileInfo->ItemType);
    }
    return TYPE_INVALID;
}

QString getFileSuffix(FILE_TYPE pFileType)
{
    QString tmpString;
    switch(pFileType)
    {
        case TYPE_START:
        {
            break;
        }
        case TYPE_MANUFACTURE:
        {
            tmpString.append("mft");
            break;
        }
        case TYPE_VIDEOEDITOR:
        {
            tmpString.append("veditor");
            break;
        }
        case TYPE_IMAGEEDITOR:
        {
            tmpString.append("ieditor");
            break;
        }
        case TYPE_AUDIOEDITOR:
        {
            tmpString.append("aeditor");
            break;
        }
        case TYPE_PRO:
        {
            tmpString.append("bstpro");
            break;
        }
        case TYPE_UI:
        {
            tmpString.append("ui");
            break;
        }
        case TYPE_MDP:
        {
            tmpString.append("mdp");
            break;
        }
        case TYPE_STC:
        {
            tmpString.append("stc");
            break;
        }
        case TYPE_MAP:
        {
            tmpString.append("map");
            break;
        }
        case TYPE_DEVINFO:
        {
            tmpString.append("devinfo");
            break;
        }
        case TYPE_STO:
        {
            tmpString.append("sto");
            break;
        }
        case TYPE_LOG:
        {
            tmpString.append("log");
            break;
        }
        case TYPE_RC:
        {
            tmpString.append("rc");
            break;
        }
        default:
            break;
    }
    return tmpString;
}

QString getNewFileName(FILE_TYPE pType)
{
    return QString("New.") + getFileSuffix(pType);
}

RC_TYPE getRcType(QString FileName)
{
    if (FileName.isEmpty())
        return RC_INVALID;
    if(FileName.endsWith(".bmp",Qt::CaseInsensitive))
    {
        return RC_BMP;
    }
    else if(FileName.endsWith(".jpg",Qt::CaseInsensitive))
    {
        return RC_JPG;
    }
    else if(FileName.endsWith(".png",Qt::CaseInsensitive))
    {
        return RC_PNG;
    }
    else if(FileName.endsWith(".gif",Qt::CaseInsensitive))
    {
        return RC_GIF;
    }
    else if(FileName.endsWith(".svg",Qt::CaseInsensitive))
    {
        return RC_SVG;
    }
    else if(FileName.endsWith(".qml",Qt::CaseInsensitive))
    {
        return RC_QML;
    }
    else if(FileName.endsWith(".avi",Qt::CaseInsensitive))
    {
        return RC_AVI;
    }
    else if(FileName.endsWith(".mp4",Qt::CaseInsensitive))
    {
        return RC_MP4;
    }
    else if(FileName.endsWith(".flv",Qt::CaseInsensitive))
    {
        return RC_FLV;
    }
    else if(FileName.endsWith(".rmvb",Qt::CaseInsensitive))
    {
        return RC_RMVB;
    }
    else if(FileName.endsWith(".font",Qt::CaseInsensitive))
    {
        return RC_TXT;
    }
    return RC_INVALID;
}

int GetKeyType(QString pKey)
{
    if(pKey.length() == 1)
        return (int)(pKey.toLatin1().at(0));
    else if(pKey.compare("Esc") == 0)
        return (QTASCII)?(Qt::Key_Escape):(0x1b);
    else if(pKey.compare("Tab") == 0)
        return (QTASCII)?(Qt::Key_Tab):(0x0b);
    else if(pKey.compare("Backspace") == 0)
        return (QTASCII)?(Qt::Key_Backspace):(0x08);
    else if(pKey.compare("Enter") == 0)
        return (QTASCII)?(Qt::Key_Enter):(0x0d);
    else if(pKey.compare("Del") == 0)
        return (QTASCII)?(Qt::Key_Delete):(0x7f);
    else if(pKey.compare("Home") == 0)
        return (QTASCII)?(Qt::Key_Home):(0x02);
    else if(pKey.compare("End") == 0)
        return (QTASCII)?(Qt::Key_End):(0x03);
    else if(pKey.compare("Left") == 0)
        return Qt::Key_Left;
    else if(pKey.compare("Up") == 0)
        return Qt::Key_Up;
    else if(pKey.compare("Right") == 0)
        return Qt::Key_Right;
    else if(pKey.compare("Down") == 0)
        return Qt::Key_Down;
    else if(pKey.compare("Shift") == 0)
        return (QTASCII)?(Qt::Key_Shift):(0x0f);
    else if(pKey.compare("Ctrl") == 0)
        return (QTASCII)?(Qt::Key_Control):(0x11);
    else if(pKey.compare("Alt") == 0)
        return (QTASCII)?(Qt::Key_Alt):(0x10);
    else if(pKey.compare("Caps") == 0)
        return Qt::Key_CapsLock;
    else if(pKey.compare("Space") == 0)
        return Qt::Key_Space;
    else if(pKey.compare("Disablity") == 0)
        return RCDISABILITY;
    return 0;
}

MDPConfigParaInfo::MDPConfigParaInfo(const QDomElement &pElement)
{
    if(pElement.isNull())
        return;
    mName = pElement.tagName();
    //>@��ȡȨ��
    QString tmpString = pElement.attribute("Permission");
    if(!tmpString.compare("RO", Qt::CaseInsensitive))
        mPermission = RO;
    else if(!tmpString.compare("EDIT", Qt::CaseInsensitive))
        mPermission = EDIT;
    else if(!tmpString.compare("RW", Qt::CaseInsensitive))
        mPermission = RW;
    mParaType = getBasicUiType(pElement.attribute("Para"));
    mParas = pElement.attribute("List");
    if(mParaType != BASICUI_STACK)
    {
        mValue = pElement.text();
    }
    else
    {
        mValue = pElement.attribute("Active");
        QDomNodeList tmpList = pElement.childNodes();
        for(int i=0;i<tmpList.count();i++)
        {
            QDomElement tmpElement = tmpList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            SubConfigParaInfo *tmpParaInfo = new SubConfigParaInfo;
            tmpParaInfo->Name = tmpElement.tagName();
            QDomNodeList tmpSubList = tmpElement.childNodes();
            for(int i=0;i<tmpSubList.count();i++)
            {
                QDomElement tmpSubElement = tmpSubList.at(i).toElement();
                if(tmpSubElement.isNull())
                    continue;
                tmpParaInfo->ParaInfoList.append(new MDPConfigParaInfo(tmpSubElement));
            }
            mSubInfos.append(tmpParaInfo);
        }
    }
}

MDPConfigParaInfo::~MDPConfigParaInfo()
{
    if(mSubInfos.count() <= 0)
        return;
    for(int i=mSubInfos.count()-1;i>=0;i--)
    {
        SubConfigParaInfo* tmpParaInfo = mSubInfos.takeAt(i);
        if(tmpParaInfo)
        {
            if(tmpParaInfo->ParaInfoList.count() <= 0)
                continue;
            for(int j=tmpParaInfo->ParaInfoList.count()-1;j>=0;j--)
            {
                MDPConfigParaInfo *tmpInfo = tmpParaInfo->ParaInfoList.takeAt(j);
                if(tmpInfo)
                    delete tmpInfo;
            }
            delete tmpParaInfo;
        }
    }
}

bool MDPConfigParaInfo::IsNull()
{
    if((mName.isEmpty())||(mParaType == BASICUI_UNKNOW))
        return true;
    return false;
}

MDPConfigInfo::MDPConfigInfo(const QDomElement &pElement)
{
    mDrag = false;
    mZValue = 0;
    if(pElement.isNull())
        return;
    mName = pElement.tagName();
    mRc = pElement.attribute("img");
    QDomNodeList nodeList = pElement.childNodes();
    for(int i=0;i<nodeList.count();i++)
    {
        QDomElement cldElement = nodeList.at(i).toElement();
        if(cldElement.isNull())
            continue;
        QString cldName = cldElement.tagName();
        if(!cldName.compare("Drag", Qt::CaseInsensitive))
        {
            mDrag =  (!cldElement.text().compare("True", Qt::CaseInsensitive))?true:false;
        }
        else if(!cldName.compare("geometry", Qt::CaseInsensitive))
        {
            QString text = cldElement.text();
            QStringList list = text.split(",", QString::SkipEmptyParts);
            if(list.count()!=4)
                continue;
            mRectF = QRectF(list.at(0).toDouble(),list.at(1).toDouble(),list.at(2).toDouble(),list.at(3).toDouble()).normalized();
        }
        else if(!cldName.compare("ZValue", Qt::CaseInsensitive))
        {
            mZValue = cldElement.text().toInt();
        }
        else if(!cldName.compare("Parameter", Qt::CaseInsensitive))
        {
            QDomNodeList cldParaNodeList = cldElement.childNodes();
            for(int i=0;i<cldParaNodeList.count();i++)
            {
                QDomElement cldParaElement = cldParaNodeList.at(i).toElement();
                if(cldParaElement.isNull())
                    continue;
                mParaInfoList.append(new MDPConfigParaInfo(cldParaElement));
            }
        }
    }
}
MDPConfigInfo::~MDPConfigInfo()
{
    mName = QString();
    mDrag = false;
    mRc = QString();
    mRectF = QRectF();
    mZValue = 0;
    if(mParaInfoList.count() > 0)
    {
        for(int i=mParaInfoList.count();i>=0;i--)
        {
            MDPConfigParaInfo *tmpParaInfo = mParaInfoList.takeAt(i);
            if(tmpParaInfo)
                delete tmpParaInfo;
        }
    }
}
bool MDPConfigInfo::IsNull()
{
    if((mName.isEmpty())||
       (mRc.isEmpty())||
       (!mRectF.isValid()))
        return true;
    return false;
}

COM_TYPE getComponetType(QString ComponentName)
{
    COM_TYPE tmpComType = COM_INVALID;
    if (ComponentName.isNull())
        return tmpComType;
    else if (ComponentName.isEmpty() ||
             ComponentName.compare(QString("Desktop"), Qt::CaseInsensitive) == 0 ||
             ComponentName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_DESKTOP;

    }
    else if(ComponentName.compare(QString("SetBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_SETBAR;
    }
    else if(ComponentName.compare(QString("MessageBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_MESSAGEBAR;
    }
    else if(ComponentName.compare(QString("ProgressBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_PROGRESSBAR;
    }
    else if(!ComponentName.compare(QString("Animal"), Qt::CaseInsensitive))
    {
        tmpComType = COM_ANIMAL;
    }
    else if(!ComponentName.compare(QString("FuncInfo"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FUNCINFO;
    }
    else if(!ComponentName.compare(QString("Floor"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOOR;
    }
    else if(!ComponentName.compare(QString("Arrow"), Qt::CaseInsensitive))
    {
        tmpComType = COM_ARROW;
    }
    else if(!ComponentName.compare(QString("Function"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FUNCTION;
    }
    else if(!ComponentName.compare(QString("MultiMedia"), Qt::CaseInsensitive))
    {
        tmpComType = COM_MULTIMEDIA;
    }
    else if(!ComponentName.compare(QString("Video"), Qt::CaseInsensitive))
    {
        tmpComType = COM_VIDEO;
    }
    else if(!ComponentName.compare(QString("Image"), Qt::CaseInsensitive))
    {
        tmpComType = COM_IMAGE;
    }
    else if(!ComponentName.compare(QString("WebView"), Qt::CaseInsensitive))
    {
        tmpComType = COM_WEBVIEW;
    }
    else if(!ComponentName.compare(QString("QML"), Qt::CaseInsensitive))
    {
        tmpComType = COM_QML;
    }
    else if(!ComponentName.compare(QString("OpenGL"), Qt::CaseInsensitive))
    {
        tmpComType = COM_OPENGL;
    }
    else if(!ComponentName.compare(QString("Audio"), Qt::CaseInsensitive))
    {
        tmpComType = COM_AUDIO;
    }
    else if(!ComponentName.compare(QString("Phone"), Qt::CaseInsensitive))
    {
        tmpComType = COM_PHONE;
    }
    else if(!ComponentName.compare(QString("TextBulletin"), Qt::CaseInsensitive))
    {
        tmpComType = COM_TEXTBULLETIN;
    }
    else if(!ComponentName.compare(QString("Text"), Qt::CaseInsensitive))
    {
        tmpComType = COM_TEXT;
    }
    else if(!ComponentName.compare(QString("IntegratedInfo"), Qt::CaseInsensitive))
    {
        tmpComType = COM_INTEGRATEDINFO;
    }
    else if(!ComponentName.compare(QString("Logo"), Qt::CaseInsensitive))
    {
        tmpComType = COM_LOGO;
    }
    else if(!ComponentName.compare(QString("Weather"), Qt::CaseInsensitive))
    {
        tmpComType = COM_WEATHER;
    }
    else if(!ComponentName.compare(QString("Time"), Qt::CaseInsensitive))
    {
        tmpComType = COM_TIME;
    }
    else if(!ComponentName.compare(QString("NamePlate"), Qt::CaseInsensitive))
    {
        tmpComType = COM_NAMEPLATE;
    }
    else if(!ComponentName.compare(QString("ButtonOperate"), Qt::CaseInsensitive))
    {
        tmpComType = COM_BUTTONOPERATE;
    }
    else if(!ComponentName.compare(QString("FloorLCD"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOORLCD;
    }
    else if(!ComponentName.compare(QString("FloorBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOORBTN;
    }
    else if(!ComponentName.compare(QString("KeyboardBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_KEYBOARDBTN;
    }
    else if(!ComponentName.compare(QString("PhoneBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_PHONEBTN;
    }
    else if(!ComponentName.compare(QString("EmergencyBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_EMERGENCYBTN;
    }
    else if(!ComponentName.compare(QString("OpenBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_OPENBTN;
    }
    else if(!ComponentName.compare(QString("CloseBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_CLOSEBTN;
    }
    else if(!ComponentName.compare(QString("SetBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_SETBTN;
    }
    else if(!ComponentName.compare(QString("DisabilityBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_DISABILITYBTN;
    }
    return tmpComType;
}

QString getComponetName(COM_TYPE ComponentType)
{
    switch(ComponentType)
    {
    case COM_DESKTOP:
        return QString("Desktop");
    //>@���ô���
    case COM_SETBAR:
        return QString("SetBar");
    case COM_MESSAGEBAR:
        return QString("MessageBar");
    case COM_PROGRESSBAR:
        return QString("ProgressBar");
    case COM_ANIMAL:
        return QString("Animal");
    //>@Function Info Reginon
    case COM_FUNCINFO:
        return QString("FuncInfo");
    case COM_FLOOR:
        return QString("Floor");
    case COM_ARROW:
        return QString("Arrow");
    case COM_FUNCTION:
        return QString("Function");
    //>@MultiMedia Reginon
    case COM_MULTIMEDIA:
        return QString("MultiMedia");
    case COM_VIDEO:
        return QString("Video");
    case COM_IMAGE:
        return QString("Image");
    case COM_WEBVIEW:
        return QString("WebView");
    case COM_QML:
        return QString("QML");
    case COM_OPENGL:
        return QString("OpenGL");
    case COM_AUDIO:
        return QString("Audio");
    case COM_PHONE:
        return QString("Phone");
    //>@Text Bulletin Reginon
    case COM_TEXTBULLETIN:
        return QString("TextBulletin");
    case COM_TEXT:
        return QString("Text");
    //>@Integrated Info Reginon
    case COM_INTEGRATEDINFO:
        return QString("IntegratedInfo");
    case COM_LOGO:
        return QString("Logo");
    case COM_WEATHER:
        return QString("Weather");
    case COM_TIME:
        return QString("Time");
    case COM_NAMEPLATE:
        return QString("NamePlate");
    //>@Button Operate Reginon
    case COM_BUTTONOPERATE:
        return QString("ButtonOperate");
    case COM_FLOORLCD:
        return QString("FloorLCD");
    case COM_FLOORBTN:
        return QString("FloorBtn");
    case COM_KEYBOARDBTN:
        return QString("KeyboardBtn");
    case COM_PHONEBTN:
        return QString("PhoneBtn");
    case COM_EMERGENCYBTN:
        return QString("EmergencyBtn");
    case COM_OPENBTN:
        return QString("OpenBtn");
    case COM_CLOSEBTN:
        return QString("CloseBtn");
    case COM_SETBTN:
        return QString("SetBtn");
    case COM_DISABILITYBTN:
        return QString("DisabilityBtn");
    default:
        return QString();
    }
    return QString();
}

QString getComponetPath(COM_TYPE pType)
{
    switch(pType)
    {
    case COM_DESKTOP:
        return QString("Desktop");
    //>@���ô���
    case COM_SETBAR:
        return QString("/SetBar");
    case COM_MESSAGEBAR:
        return QString("/MessageBar");
    case COM_PROGRESSBAR:
        return QString("/ProgressBar");
    case COM_ANIMAL:
        return QString("/Animal");
    //>@Function Info Reginon
    case COM_FUNCINFO:
        return QString("/FuncInfo");
    case COM_FLOOR:
        return QString("/FuncInfo/Floor");
    case COM_ARROW:
        return QString("/FuncInfo/Arrow");
    case COM_FUNCTION:
        return QString("/FuncInfo/Function");
    //>@MultiMedia Reginon
    case COM_MULTIMEDIA:
        return QString("/MultiMedia");
    case COM_VIDEO:
        return QString("/MultiMedia/Video");
    case COM_IMAGE:
        return QString("/MultiMedia/Image");
    case COM_WEBVIEW:
        return QString("/MultiMedia/WebView");
    case COM_QML:
        return QString("/MultiMedia/QML");
    case COM_OPENGL:
        return QString("/MultiMedia/OpenGL");
    case COM_AUDIO:
        return QString("/MultiMedia/Audio");
    case COM_PHONE:
        return QString("/MultiMedia/Phone");
    //>@Text Bulletin Reginon
    case COM_TEXTBULLETIN:
        return QString("/TextBulletin");
    case COM_TEXT:
        return QString("/TextBulletin/Text");
    //>@Integrated Info Reginon
    case COM_INTEGRATEDINFO:
        return QString("/TextBulletin/IntegratedInfo");
    case COM_LOGO:
        return QString("/TextBulletin/Logo");
    case COM_WEATHER:
        return QString("/TextBulletin/Weather");
    case COM_TIME:
        return QString("/TextBulletin/Time");
    case COM_NAMEPLATE:
        return QString("/TextBulletin/NamePlate");
    //>@Button Operate Reginon
    case COM_BUTTONOPERATE:
        return QString("/ButtonOperate");
    case COM_FLOORLCD:
        return QString("/ButtonOperate/FloorLCD");
    case COM_FLOORBTN:
        return QString("/ButtonOperate/FloorBtn");
    case COM_KEYBOARDBTN:
        return QString("/ButtonOperate/KeyboardBtn");
    case COM_PHONEBTN:
        return QString("/ButtonOperate/PhoneBtn");
    case COM_EMERGENCYBTN:
        return QString("/ButtonOperate/EmergencyBtn");
    case COM_OPENBTN:
        return QString("/ButtonOperate/OpenBtn");
    case COM_CLOSEBTN:
        return QString("/ButtonOperate/CloseBtn");
    case COM_SETBTN:
        return QString("/ButtonOperate/SetBtn");
    case COM_DISABILITYBTN:
        return QString("/ButtonOperate/DisabilityBtn");
    default:
        return QString();
    }
    return QString();
}

COM_TYPE getParentComType(COM_TYPE pType)
{
    switch(pType)
    {
    case COM_DESKTOP:
        return COM_NULL;
    //>@���ô���
    case COM_SETBAR:
        return COM_DESKTOP;
    case COM_MESSAGEBAR:
    case COM_PROGRESSBAR:
    case COM_ANIMAL:
        return COM_DESKTOP;   //>@����
    //>@Function Info Reginon
    case COM_FUNCINFO:
        return COM_DESKTOP;
    case COM_FLOOR:
    case COM_ARROW:
    case COM_FUNCTION:
        return COM_FUNCINFO;
    //>@MultiMedia Reginon
    case COM_MULTIMEDIA:
        return COM_DESKTOP;
    case COM_VIDEO:
    case COM_IMAGE:
    case COM_WEBVIEW:
    case COM_QML:
    case COM_OPENGL:
    case COM_AUDIO:
    case COM_PHONE:
        return COM_MULTIMEDIA;
    //>@Text Bulletin Reginon
    case COM_TEXTBULLETIN:
        return COM_DESKTOP;
    case COM_TEXT:
        return COM_TEXTBULLETIN;
    //>@Integrated Info Reginon
    case COM_INTEGRATEDINFO:
        return COM_DESKTOP;
    case COM_LOGO:
    case COM_WEATHER:
    case COM_TIME:
    case COM_NAMEPLATE:
        return COM_INTEGRATEDINFO;
    //>@Button Operate Reginon
    case COM_BUTTONOPERATE:
        return COM_DESKTOP;
    case COM_FLOORBTN:
    case COM_FLOORLCD:
    case COM_KEYBOARDBTN:
    case COM_PHONEBTN:
    case COM_EMERGENCYBTN:
    case COM_OPENBTN:
    case COM_CLOSEBTN:
    case COM_SETBTN:
    case COM_DISABILITYBTN:
        return COM_BUTTONOPERATE;
    default:
        return COM_NULL;
    }
    return COM_NULL;
}

COM_TYPE getParentComType(QString ComPath)
{
    if(ComPath.isEmpty())
        return COM_INVALID;
    int index1,index2;
    QString tmpString;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return COM_INVALID;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0) //>@desktop and rect
    {
        tmpString = ComPath.mid(index1);
        if(tmpString.length() == 1)
        {
            return COM_NULL;
        }
        return COM_DESKTOP;
    }
    else
    {
        tmpString = ComPath.mid(index1+1, index2-index1-1);
    }
    return getComponetType(tmpString);
}

QString getComPath(QString ComName)
{
    QString tmpString;
    if (ComName.isNull())
        return tmpString;
    else if (ComName.isEmpty() ||
             ComName.compare(QString("Desktop"), Qt::CaseInsensitive) == 0 ||
             ComName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        return QString("/");
    }
    else if(!ComName.compare(QString("SetBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/SetBar");
    }
    else if(!ComName.compare(QString("MessageBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MessageBar");
    }
    else if(!ComName.compare(QString("ProgressBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ProgressBar");
    }
    else if(!ComName.compare(QString("FuncInfo"), Qt::CaseInsensitive))
    {
        tmpString = QString("/FuncInfo");
    }
    else if(!ComName.compare(QString("Floor"), Qt::CaseInsensitive))
    {
        tmpString = QString("/FuncInfo/Floor");
    }
    else if(!ComName.compare(QString("Arrow"), Qt::CaseInsensitive))
    {
        tmpString = QString("/FuncInfo/Arrow");
    }
    else if(!ComName.compare(QString("Function"), Qt::CaseInsensitive))
    {
        tmpString = QString("/FuncInfo/Function");
    }
    else if(!ComName.compare(QString("MultiMedia"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MultiMedia");
    }
    else if(!ComName.compare(QString("Video"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MultiMedia/Video");
    }
    else if(!ComName.compare(QString("Image"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MultiMedia/Image");
    }
    else if(!ComName.compare(QString("WebView"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MultiMedia/WebView");
    }
    else if(!ComName.compare(QString("QML"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MultiMedia/QML");
    }
    else if(!ComName.compare(QString("OpenGL"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MultiMedia/OpenGL");
    }
    else if(!ComName.compare(QString("Audio"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MultiMedia/Audio");
    }
    else if(!ComName.compare(QString("Phone"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MultiMedia/Phone");
    }
    else if(!ComName.compare(QString("TextBulletin"), Qt::CaseInsensitive))
    {
        tmpString = QString("/TextBulletin");
    }
    else if(!ComName.compare(QString("Text"), Qt::CaseInsensitive))
    {
        tmpString = QString("/TextBulletin/Text");
    }
    else if(!ComName.compare(QString("IntegratedInfo"), Qt::CaseInsensitive))
    {
        tmpString = QString("/IntegratedInfo");
    }
    else if(!ComName.compare(QString("Logo"), Qt::CaseInsensitive))
    {
        tmpString = QString("/IntegratedInfo/Logo");
    }
    else if(!ComName.compare(QString("Weather"), Qt::CaseInsensitive))
    {
        tmpString = QString("/IntegratedInfo/Weather");
    }
    else if(!ComName.compare(QString("Time"), Qt::CaseInsensitive))
    {
        tmpString = QString("/IntegratedInfo/Time");
    }
    else if(!ComName.compare(QString("NamePlate"), Qt::CaseInsensitive))
    {
        tmpString = QString("/IntegratedInfo/NamePlate");
    }
    else if(!ComName.compare(QString("ButtonOperate"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ButtonOperate");
    }
    else if(!ComName.compare(QString("FloorBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ButtonOperate/FloorBtn");
    }
    else if(!ComName.compare(QString("PhoneBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ButtonOperate/PhoneBtn");
    }
    else if(!ComName.compare(QString("EmergencyBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ButtonOperate/EmergencyBtn");
    }
    else if(!ComName.compare(QString("OpenBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ButtonOperate/OpenBtn");
    }
    else if(!ComName.compare(QString("CloseBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ButtonOperate/CloseBtn");
    }
    else if(!ComName.compare(QString("SetBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ButtonOperate/SetBtn");
    }
    else if(!ComName.compare(QString("DisabilityBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ButtonOperate/DisabilityBtn");
    }
    return tmpString;
}

QString getParentComPath(QString ComPath)
{
    int index1,index2;
    QString tmpString;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return tmpString;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1);
        if(tmpString.length() == 1)
            return QString("");     //>@Empty��ʾ��ǰ�ؼ�ΪDesktop
        else
            return QString("/");
    }
    else
    {
        tmpString = ComPath.mid(index1, index2-index1);
    }
    return tmpString;
}

QString getCurComName(QString ComPath)
{
    if(ComPath.isEmpty())
        return QString();
    int index1,index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        if(ComPath.compare(QString("Desktop"), Qt::CaseInsensitive) == 0)
            return QString("Desktop");
        else
            return QString();
    }
    QString tmpString;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
            return QString("Desktop");     //>@Empty��ʾDesktop
        else
            return tmpString;
    }
    else
    {
        tmpString = ComPath.mid(index2+1);
    }
    return tmpString;
}

COM_TYPE getCurComType(QString ComPath)
{
    QString tmpString;
    int index1,index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        return getComponetType(ComPath);
    }
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
            return COM_DESKTOP;     //>@Empty��ʾDesktop
    }
    else
    {
        tmpString = ComPath.mid(index2+1);
    }
    return getComponetType(tmpString);
}

void COM_INFO::UpdateGeometry()
{
    if(ComponentPtr)
    {
        MdyGeometry(ComponentPtr->geometry());
    }
}
QRectF COM_INFO::GetChildGroupRect()
{
    if(ChildComListPtr.count() <= 0)
        return QRectF(0,0,0,0);
    for(int ilist=0;ChildRectList.count()<ChildComListPtr.count();ilist++)
        ChildRectList.append(QRectF(0,0,0,0));
    QRectF retRect;
    QGraphicsItemGroup *m_ItemGroup = new QGraphicsItemGroup;
    for(int i = 0;i<ChildComListPtr.count();i++)
    {
        QGraphicsWidget *tmpWidget = ChildComListPtr.at(i)->ComponentPtr;
        if(tmpWidget == 0)
            continue;
        ChildRectList.replace(i, tmpWidget->sceneBoundingRect());
        m_ItemGroup->addToGroup(tmpWidget);
    }
    retRect = m_ItemGroup->sceneBoundingRect();
    for(int i=0;i<ChildComListPtr.count();i++)
    {
        QGraphicsWidget *tmpWidget = ChildComListPtr.at(i)->ComponentPtr;
        if(tmpWidget == 0)
            continue;
        m_ItemGroup->removeFromGroup(tmpWidget);
        QRectF rect = ComponentPtr->mapRectFromScene(ChildRectList.at(i));
        tmpWidget->setPos(rect.topLeft());
        tmpWidget->setParentItem(ComponentPtr);
    }
    return retRect;
}
//>@�ָ������ӽڵ�ĸ�
void COM_INFO::RestoreParent()
{
    for(int i = 0;i<ChildComListPtr.count();i++)
    {
        QGraphicsWidget *tmpWidget = ChildComListPtr.at(i)->ComponentPtr;
        if(tmpWidget == 0)
            continue;
        tmpWidget->setParentItem(ComponentPtr);
        QRectF rect = ComponentPtr->mapRectFromScene(ChildRectList.at(i));
        tmpWidget->setPos(rect.topLeft());
    }
}
//>@�������ӽڵ�ĸ��ı�Ϊ����
bool COM_INFO::ReleaseParent(QGraphicsWidget* pWidget)
{
    if(pWidget == 0)
        return false;
    for(int ilist=0;ChildRectList.count()<ChildComListPtr.count();ilist++)
        ChildRectList.append(QRectF(0,0,0,0));
    for(int i = 0;i<ChildComListPtr.count();i++)
    {
        QGraphicsWidget *tmpWidget = ChildComListPtr.at(i)->ComponentPtr;
        if(tmpWidget == 0)
            continue;
        QRectF tmpRect = tmpWidget->sceneBoundingRect();
        ChildRectList.replace(i, tmpRect);
        tmpWidget->setParentItem(pWidget);
        QRectF rect = pWidget->mapRectFromScene(tmpRect);
        tmpWidget->setPos(rect.topLeft());
    }
    return true;
}
QList<STATE_INFO*> COM_INFO::BackupStatemachine()
{
    QList<STATE_INFO*> tmpList;
    for(int i=0;i<StateMachine.count();i++)
    {
        STATE_INFO *tmpState = StateMachine.at(i);
        if(tmpState)
        {
            STATE_INFO* tmpStateInfo = new STATE_INFO;
            tmpStateInfo->StateName = tmpState->StateName;
            tmpStateInfo->Effect = tmpState->Effect;
            tmpList.append(tmpStateInfo);
        }
    }
    return tmpList;
}
bool COM_INFO::CopyFromOther(COM_INFO *s, bool pFull)  //>@pFull=true��ʾ��״̬������ȫ��������
{
    if(s == 0)
        return false;
    ComponentPtr = s->ComponentPtr;
    ComPath = s->ComPath;
    ComRect = s->ComRect;
    if(pFull)
    {
        for(int i=0;i<s->StateMachine.count();i++)
        {
            STATE_INFO *tmpState = s->StateMachine.at(i);
            if(tmpState)
                AddState(tmpState->StateName, tmpState->Effect);
        }
    }
    else
    {
        StateMachine = s->StateMachine;
    }
    RectChanged = s->RectChanged;
    ParentComPtr = s->ParentComPtr;
    ChildComListPtr = s->ChildComListPtr;
    mUpdateLevel = s->mUpdateLevel;
    return true;
}

QString getFileName(QString FilePath)
{
    QString tmpFileName;
    if (FilePath.isEmpty())
        return tmpFileName;
    int index = FilePath.lastIndexOf("/");
    if(index >= 0)
        tmpFileName =  FilePath.mid(index + 1);
    else
        tmpFileName = FilePath;
    return tmpFileName;
}

QString getFileDirectory(QString FilePath)   //>@FilePath�б����к�׺�����������Ϊ��·��
{
    QString tmpFileName;
    if (FilePath.isEmpty())
        return tmpFileName;
    int index1 = FilePath.lastIndexOf("/");
    int index2 = FilePath.lastIndexOf(".");
    if(index1 < 0)
        return tmpFileName;
    if(index2 < 0)
        return FilePath;
    if(index1 < index2)
        tmpFileName =  FilePath.mid(0, index1 + 1);
    else
        return FilePath;
    return tmpFileName;
}

//>@ e:/xx/xx/
QString getParentDirectory(QString DirPath)
{
    QString tmpDirPath;
    if (DirPath.isEmpty())
        return tmpDirPath;
    if(!DirPath.endsWith("/"))
        DirPath.append("/");
    int index1 = DirPath.lastIndexOf("/");
    if(index1 < 0)
        return tmpDirPath;
    int index2 = DirPath.lastIndexOf("/", index1-1);
    if(index2 < 0)
        return tmpDirPath;
    tmpDirPath =  DirPath.mid(0, index2 + 1);
    return tmpDirPath;
}

QString GetDirectoryName(QString DirPath)
{
    QString tmpDirName;
    if (DirPath.isEmpty())
        return tmpDirName;
    if(!DirPath.endsWith("/"))
        DirPath.append("/");
    int index1 = DirPath.lastIndexOf("/");
    if(index1 < 0)
        return tmpDirName;
    int index2 = DirPath.lastIndexOf("/", index1-1);
    if(index2 < 0)
        return tmpDirName;
    tmpDirName =  DirPath.mid(index2 + 1, index1 - index2 - 1);
    return tmpDirName;
}

QRect GetRectFromString(QString pRectString)
{
    QStringList tmpList = pRectString.split(",");
    if(tmpList.count() != 4)
    {
        return QRect();
    }
    int tmpX = tmpList.at(0).toInt();
    int tmpY = tmpList.at(1).toInt();
    int tmpW = tmpList.at(2).toInt();
    int tmpH = tmpList.at(3).toInt();
    if(tmpW<=0 || tmpH<=0)
    {
        return QRect();
    }
    return QRect(tmpX, tmpY, tmpW, tmpH);
}

QRectF GetRotateRect(QSizeF pParentSize, QRectF pRect, ROTATE_ANGLE pAngle)
{
    if(pParentSize.isValid() == false || pRect.isValid() == false)
        return QRectF();
    qreal ParWidth = pParentSize.width();
    qreal ParHeight = pParentSize.height();
    QRectF tmpRect;
    int value;
    switch(pAngle)
    {
        case ROTATE_90: //>@��Ϊ˳ʱ����ת
        {
            //>@��ȡ���½Ƕ�������ڸ��������½Ƕ���ľ���
            value = ParHeight - pRect.top() - pRect.height();
            tmpRect.setLeft(value);
            value = pRect.left();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.height());
            tmpRect.setHeight(pRect.width());
            break;
        }
        case ROTATE_180:
        {
            //>@��ȡ���½Ƕ���Ϊ���
            value = ParHeight - pRect.top() - pRect.height();
            tmpRect.setLeft(value);
            value = ParWidth - pRect.left() - pRect.width();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.width());
            tmpRect.setHeight(pRect.height());
            break;
        }
        case ROTATE_270:
        {
            //>@��ȡ���ϽǶ���Ϊ���
            value = pRect.top();
            tmpRect.setLeft(value);
            value = ParWidth - pRect.left() - pRect.width();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.height());
            tmpRect.setHeight(pRect.width());
            break;
        }
        default:
            return QRectF();
    }
    return tmpRect;
}

CONSTRAINT GetRotateConstraint(CONSTRAINT pConstraint, ROTATE_ANGLE pAngle)
{
    CONSTRAINT tmpConstraint;
    switch(pAngle)
    {
        case ROTATE_90: //>@��Ϊ˳ʱ����ת
        {
            tmpConstraint.mDirection = (DIRECTION)((pConstraint.mDirection + 1) % 2);
            tmpConstraint.mOrientation = (ORIENTATION)((pConstraint.mOrientation + 1) % 2);
            break;
        }
        case ROTATE_180:
        {
            tmpConstraint.mDirection = (DIRECTION)(pConstraint.mDirection);
            tmpConstraint.mOrientation = (ORIENTATION)((pConstraint.mOrientation + 1) % 2);
            break;
        }
        case ROTATE_270:
        {
            tmpConstraint.mDirection = (DIRECTION)((pConstraint.mDirection + 1) % 2);
            tmpConstraint.mOrientation = (ORIENTATION)(pConstraint.mOrientation);
            break;
        }
        default:
            return pConstraint;
    }
    return tmpConstraint;
}

//>@�����ļ��ľ���·��
QStringList findFiles(const QDir &dir,
                      const QString &fileName,
                      const QString &txt,
                      Qt::CaseSensitivity sensitive)
{
    QString tmpDirPath = dir.path();
    int index = tmpDirPath.lastIndexOf('/');
    if(index < tmpDirPath.length()-1)
        tmpDirPath.append('/');
    QStringList foundFiles;
    QStringList files;
    if(fileName.isEmpty())
        files = dir.entryList(QDir::Files | QDir::NoSymLinks);
    else
        files = dir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    if(txt.isEmpty())
    {
        for(int i=0;i<files.count();i++)
        {
            foundFiles.append(tmpDirPath + files.at(i));
        }
    }
    else
    {
        for (int i = 0; i < files.size(); ++i)
        {
            QFile file(dir.absoluteFilePath(files[i]));
            if (file.open(QIODevice::ReadOnly))
            {
                QString line;
                QTextStream in(&file);
                while (!in.atEnd())
                {
                    line = in.readLine();
                    if (line.contains(txt, sensitive))
                    {
                        foundFiles.append(tmpDirPath + files[i]);
                        break;
                    }
                }
            }
        }
    }
    return foundFiles;
}

QStringList findFilesInFloder(QString DirName, QString fileName, bool subfolder)
{
    int index = DirName.lastIndexOf('/');
    if(index < DirName.length()-1)
        DirName.append('/');
    QDir dir = QDir(DirName);
    QStringList findFileList;
    if(subfolder)
    {
        QStringList folders;
        folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(folders.size() > 0)
        {
            for (int i = 0; i < folders.size(); ++i)
            {
                findFileList << findFilesInFloder(DirName + folders[i], fileName, true);
            }
        }
    }
    findFileList << findFiles(dir, fileName, QString(""), Qt::CaseSensitive);
    return findFileList;
}

//>@����ĳ�����͵��ļ�
QStringList SearchFiles(QString pDirPath, QString pSuffix, bool pSubfolder)
{
    if(pDirPath.endsWith('/')==false)
        pDirPath.append('/');
    QDir dir = QDir(pDirPath);
    QStringList findFileList;
    if(pSubfolder)
    {
        QStringList folders;
        folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(folders.size() > 0)
        {
            for (int i = 0; i < folders.size(); ++i)
            {
                findFileList << SearchFiles(pDirPath + folders[i], pSuffix, pSubfolder);
            }
        }
    }

    QStringList tmpFileList = dir.entryList(QStringList(QString("*.%1").arg(pSuffix)), QDir::Files | QDir::NoSymLinks);
    for(int i=0;i<tmpFileList.count();i++)
    {
        IDE_TRACE_STR(pDirPath + tmpFileList.at(i));
        findFileList.append(pDirPath + tmpFileList.at(i));
    }
    return findFileList;
}

QStringList ErgodicDirectory(const QString d)
{
    QStringList tmpFileList;
    QDir dir(d);
    if (dir.exists())
    {
        const QFileInfoList list = dir.entryInfoList();
        QFileInfo fi;
        for (int l = 0; l < list.size(); l++)
        {
            fi = list.at(l);
            if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
            {
                tmpFileList << ErgodicDirectory(fi.absoluteFilePath());
            }
            else if(fi.isFile())
            {
                tmpFileList.append(fi.absoluteFilePath());
            }
        }
    }
    return tmpFileList;
}

bool CutFile(QString srcFile, QString dstFile)
{
    if(!CopyFile(srcFile, dstFile))
        return false;
    return DelFile(srcFile);
}

//>@ע�⣺srcFile��dstFile���붼���ļ��� �������ͬ���ļ��򸲸�
bool CopyFile(QString srcFile, QString dstFile)
{
    //>@���·����ͬ���򲻽��п���
    if(srcFile.compare(dstFile, Qt::CaseInsensitive) == 0)
        return true;
    if(QFile::exists(srcFile) == false)
    {
        IDE_TRACE_STR(srcFile);
        return false;
    }
    QString tmpDstString = getFileDirectory(dstFile);
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstString))
    {
        IDE_TRACE_STR(tmpDstString);
        return false;
    }
    if(!DelFile(dstFile))
    {
        IDE_TRACE_STR(dstFile);
        return false;
    }
    return QFile::copy(srcFile, dstFile);
}

bool DelFile(QString pPath)
{
    QFile file(pPath);
    if(!file.exists())
        return true;
    if(!file.setPermissions(QFile::WriteOther))
        return false;
    if(!file.remove())
        return false;
    return true;
}

bool CreatFile(QString pPath)
{
    QFile file(pPath);
    if(file.exists())
        return true;
    CreatPath(getFileDirectory(pPath));
    if(!file.open(QFile::WriteOnly))
        return false;
    file.close();
    return true;
}

bool CreatPath(QString pPath)
{
    QDir dir(pPath);
    if(!dir.mkpath(pPath))
    {
        dir.cdUp();
        return false;
    }
    dir.cdUp();
    return true;
}

QStringList getAllInFloders(QString pDir)
{
    if(!pDir.endsWith("/"))
        pDir.append("/");
    QDir dir = QDir(pDir);
    QStringList getAllList,tmpList;
    QString tmpString;
    getAllList.clear();
    tmpList.clear();
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList(QDir::Dirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pDir).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        getAllList.append(tmpString);
    }
    return getAllList;
}

bool CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder, bool pCopyFile)
{
    if(!pSrcDir.endsWith("/"))
        pSrcDir.append("/");
    if(!pDstDir.endsWith("/"))
        pDstDir.append("/");
    //>@���·����ͬ���򲻽��п���
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
    QDir tmpSrcDir(pSrcDir);
    if(!tmpSrcDir.exists())
        return false;
    if(!CreatPath(pDstDir))
        return false;
    if(pSubfolder)
    {
        QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < folders.size(); ++i)
        {
            QString tmpSrcPath = QString("%1%2").arg(pSrcDir).arg(folders[i]);
            QString tmpDstPath = QString("%1%2").arg(pDstDir).arg(folders[i]);
            if(!CopyFolder(tmpSrcPath, tmpDstPath, pSubfolder))
            {
                IDE_TRACE_STR(tmpSrcPath);
            }
        }
    }
    if(pCopyFile)
    {
        QStringList files = findFiles(tmpSrcDir, QString(), QString(), Qt::CaseInsensitive);
        for(int i = 0; i < files.size(); ++i)
        {
            QString tmpDstFilePath = QString("%1%2").arg(pDstDir).arg(getFileName(files[i]));
            if(!CopyFile(files[i], tmpDstFilePath))
            {
                IDE_TRACE_STR(files[i]);
            }
        }
    }
    return true;
}

//>@����ļ������ļ�
bool ClearFolder(QString pDir)
{
    if(!DelFolder(pDir))
        return false;
    return CreatPath(pDir);
}

bool DelFolder(QString pDir)
{
    if(pDir.isEmpty())
        return false;
    if(!pDir.endsWith("/"))
        pDir.append("/");
    QDir dir(pDir);
    if(!dir.exists())
        return true;
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir())
        {
            QString tmpdir = pDir + fileInfo.fileName();
            if(!DelFolder(tmpdir))
            {
                IDE_TRACE_STR(tmpdir);
                return false;
            }
        }
        else if(fileInfo.isFile())
        {
            if(!DelFile(fileInfo.filePath()))
            {
                IDE_TRACE_STR(fileInfo.filePath());
                return false;
            }
        }
    }
    if(!dir.cdUp())            /**< �����ϼ�Ŀ¼����Ϊֻ�з����ϼ�Ŀ¼���ſ���ɾ�����Ŀ¼ */
    {
        IDE_TRACE_STR("cdUp#Failed");
        return false;
    }
    if(dir.exists(pDir))
    {
        if(!dir.rmdir(pDir))
        {
            IDE_TRACE_STR(pDir);
            return false;
        }
    }
    return true;
}

bool MoveFolder(QString pSrcDir, QString pDstDir)
{
    if(!pSrcDir.endsWith("/"))
        pSrcDir.append("/");
    if(!pDstDir.endsWith("/"))
        pDstDir.append("/");
    //>@���·����ͬ���򲻽��п���
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
    QDir tmpDir(pSrcDir);
    if(!tmpDir.exists())
        return false;
    if(!CreatPath(pDstDir))
        return false;
    //>@���ƶ����ļ���
    QDir tmpSrcDir(pSrcDir);
    QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpSrcPath = QString("%1%2").arg(pSrcDir).arg(folders[i]);
        QString tmpDstPath = QString("%1%2").arg(pDstDir).arg(folders[i]);
        if(!MoveFolder(tmpSrcPath, tmpDstPath))
        {
            IDE_TRACE_STR(tmpSrcPath);
        }
    }
    //>@���ƶ��ļ�
    QStringList files = findFiles(tmpSrcDir, QString(), QString(), Qt::CaseInsensitive);
    for(int i = 0; i < files.size(); ++i)
    {
        QString tmpString = QString("%1%2").arg(pDstDir).arg(getFileName(files[i]));
        if(!CutFile(files[i], tmpString))
        {
            IDE_TRACE_STR(files[i]);
        }
    }
    return true;
}

//>@pValidRect:Ϊ0��ʾȫ������  pResolution:Ŀ��ͼƬ�ֱ���  �Ȳü�ԭͼ����ת��ΪĿ��ֱ��ʡ�
//>@ConvertImage(QString("d:/a.jpg"), QRect(0,0,640,480), QString("d:/b.bmp"), QSize(320,240));
bool ConvertImage(QString pSrcFileName,
                  QRect pValidRect,
                  QString pDstFileName,
                  QSize pResolution)
{
    if(pSrcFileName.isEmpty() || pSrcFileName.indexOf(QRegExp(".")) < 0)
    {
        IDE_TRACE();
        return false;
    }
    if(pDstFileName.isEmpty() || pDstFileName.indexOf(QRegExp(".")) < 0)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = false;
    QImage tmpImage;
    tmpImage.load(pSrcFileName);
    QRect tmpRect = tmpImage.rect();
    if(pValidRect.left() != tmpRect.left() ||
       pValidRect.right() != tmpRect.right() ||
       pValidRect.top() != tmpRect.top() ||
       pValidRect.bottom() != tmpRect.bottom())
    {
        QImage newImage = tmpImage.copy(pValidRect);
        QImage dstImage = newImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        flag = dstImage.save(pDstFileName);
    }
    else
    {
        QImage dstImage = tmpImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        flag = dstImage.save(pDstFileName);
    }
    return flag;
}

QString getFileSuffix(QString pFileName)
{
    int index = pFileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = pFileName.mid(index+1);
    else
        return fileSuffix;
    if(fileSuffix.length() < 2)
        return QString();
    return fileSuffix;
}

//>@���ݲ����ж�ͼƬ���Ŵ�С
bool ConvertImage(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QFile tmpFile(pFileName);
    if(tmpFile.exists() == false)
    {
        IDE_TRACE();
        return false;
    }
    QImage tmpImage, tmpNewImage;
    if(tmpImage.load(pFileName) == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPrior == PRIOR_WIDTH)
    {
        tmpNewImage = tmpImage.scaledToWidth(pSize.width(), Qt::SmoothTransformation);
    }
    else
    {
        tmpNewImage = tmpImage.scaledToHeight(pSize.height(), Qt::SmoothTransformation);
    }
    if(tmpFile.remove() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(tmpNewImage.save(pFileName) == false)
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

QImage GetScaledImage(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QFile tmpFile(pFileName);
    if(tmpFile.exists() == false)
    {
        IDE_TRACE();
        return QImage();
    }
    QImage tmpImage, tmpNewImage;
    if(tmpImage.load(pFileName) == false)
    {
        IDE_TRACE();
        return QImage();
    }
    if(pPrior == PRIOR_WIDTH)
    {
        tmpNewImage = tmpImage.scaledToWidth(pSize.width(), Qt::SmoothTransformation);
    }
    else
    {
        tmpNewImage = tmpImage.scaledToHeight(pSize.height(), Qt::SmoothTransformation);
    }
    return tmpNewImage;
}

QSize GetScaledImageSize(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QImage tmpImage= GetScaledImage(pFileName, pSize, pPrior);
    if(tmpImage.isNull())
    {
        IDE_TRACE();
        return QSize();
    }
    return tmpImage.size();
}

//>@���pResolution��Ч��������
bool ConvertImage(QString pSrcFileName,
                  QString pDstFileName,
                  QSize pResolution)
{
    QFile tmpSrcFile(pSrcFileName);
    if(tmpSrcFile.exists() == false)
    {
        return false;
    }
    QImage tmpImage;
    if(!tmpImage.load(pSrcFileName))
    {
        IDE_TRACE();
        return false;
    }
    if(pDstFileName.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QString suffix = getFileSuffix(pDstFileName);
    if(suffix.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QString tmpDstString;
    QFile tmpDstFile(pDstFileName);
    if(tmpDstFile.exists())
    {
        QFile::Permissions tmpPermissions = tmpDstFile.permissions(pDstFileName);
        if(!(tmpPermissions & QFile::WriteUser))
            tmpDstFile.setPermissions(tmpPermissions | QFile::WriteUser);
        tmpDstFile.remove();
    }
    else
    {
        tmpDstString = getFileDirectory(pDstFileName);
        QDir tmpDir;
        tmpDir.mkpath(tmpDstString);
    }

    QImage dstImage;
    //>@���Ŀ��ͼƬ�ֱ����Ƿ���������С��Ч���ߴ���1920x1920���򱣳�ԭ��С���䡣
    if(pResolution.isEmpty() || pResolution.isValid()==false ||
       pResolution.width() > 1920 ||
       pResolution.height() > 1920)
    {
        dstImage = tmpImage;
    }
    else
    {
        dstImage = tmpImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    if(!dstImage.save(pDstFileName, suffix.toLatin1().data()))
    {
        IDE_TRACE_STR(pDstFileName);
        return false;
    }
    return true;
}

QString GetStrBehindKey
    (QString pString, QString pKey,
     quint8 pStart, quint8 pLenth,
     Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    if((pLenth==0)||(pString.isEmpty()))
        return tmpString;
    int tmpDex = pString.indexOf(pKey,pCaps);
    if(tmpDex==-1)
        return tmpString;
    if((tmpDex+pKey.length()+pStart,pLenth) > pString.length())
        return tmpString;

    tmpString = pString.mid(tmpDex+pKey.length()+pStart,pLenth);
    return tmpString;
}

QString GetStrBetweenPreAndAfter
    (QString pString,
     QString pPre, QString pAfter,
     int pPreindent, int pAfterindent,
     Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    int tmpPreDex = pString.indexOf(pPre,pCaps);
    if(tmpPreDex==-1)
        return tmpString;
    int tmpAfterDex = pString.indexOf(pAfter,pCaps);
    if(tmpAfterDex==-1)
        return tmpString;

    tmpString = pString.mid(tmpPreDex+pPre.length()+pPreindent,
                            tmpAfterDex-tmpPreDex-pPre.length()-pAfterindent);
    return tmpString;
}

/************************************************************************
//>@
************************************************************************/
MULTIMEDIA_CONVERT::MULTIMEDIA_CONVERT(QObject *parent) :
    QObject(parent)
{
    m_Process = 0;
    mMutex = new QMutex(QMutex::Recursive);
}

MULTIMEDIA_CONVERT::~MULTIMEDIA_CONVERT()
{
    KillProcess();
}

bool MULTIMEDIA_CONVERT::GetFileInfo(QString pSrc, int pTimeout)
{
    QFile file(pSrc);
    if(!file.exists())
    {
        IDE_TRACE_STR("SrcUnexist");
        return false;
    }
    m_FileInfo.mPath = pSrc;
    m_FileInfo.mName = getFileName(m_FileInfo.mPath);
    m_FileInfo.mSize = file.size();
    QString cmd = QString("%1 -i %2").arg((QString)D_FFMPEG_PATH).arg(pSrc);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertAudio(QString pSrc, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
        return false;
//    ./ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -y test.wav
    QString cmd = QString("%1 -y -i %2 -ac 1 -ar 44100 -y %3").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(pDst);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertVideo(QString pSrc, QSize pResolution, int pFrame, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
        return false;
    if(!pResolution.isValid())
        return false;
//    ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -qscale 6 -s 640x480 -r 29.97 -y test.wav
    QString resolution = SizeToStr(pResolution);
    QString cmd = QString("%1 -y -i %2 -vcodec mpeg4 -s %3 -r %4 -y %5").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(resolution).
                         arg(pFrame).
                         arg(pDst);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertVideo(QString pSrc, QRect pRect, QSize pSize, int pFrame, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
        return false;
    if(!pSize.isValid())
        return false;
    if(!GetFileInfo(pSrc, pTimeout/10))
        return false;
    QtSleep(100);
    QSize srcResolution = m_FileInfo.mResolution;
    IDE_TRACE_INT(srcResolution.width());
    IDE_TRACE_INT(srcResolution.height());
    IDE_TRACE_INT(pRect.left());
    IDE_TRACE_INT(pRect.top());
    IDE_TRACE_INT(pRect.width());
    IDE_TRACE_INT(pRect.height());
    quint16 left =0,top=0,right=0,bottom=0;
    left = pRect.left();
    top = pRect.top();
    right = srcResolution.width() - pRect.width() - left;
    bottom = srcResolution.height() - pRect.height() - top;
    if(((left + right)>=srcResolution.width())||((top + bottom)>=srcResolution.height()))
        return false;
//    ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -qscale 6 -croptop 100 -cropbottom 100 -cropleft 100 -cropright 100 -s 640x480 -r 29.97 -y test.wav
    QString tempDst = QString("%1.%2").arg(pSrc).arg(getFileSuffix(pSrc));
    QString cutCmd = QString("%1 -y -i %2 -croptop %3 -cropbottom %4 -cropleft %5 -cropright %6 -r %7 -y %8").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(top).arg(bottom).arg(left).arg(right).
                         arg(pFrame).
                         arg(tempDst);
    if(!StartProcess(cutCmd, pTimeout*3/10))
       return false;
    if(!PreHandel(tempDst, pDst))
        return false;
    QString resolution = SizeToStr(pSize);
    QString cmd = QString("%1 -y -i %2 -vcodec mpeg4 -s %3 -y %4").
                         arg((QString)D_FFMPEG_PATH).
                         arg(tempDst).
                         arg(resolution).
                         arg(pDst);
    if(!StartProcess(cmd, pTimeout*3/5))
       return false;
    QFile tempFile(tempDst);
    if(tempFile.exists())
    {
        tempFile.setPermissions(QFile::WriteOther);
        if(!tempFile.remove())
        {
            IDE_TRACE_STR("TempFileLock");
            return false;
        }
    }
    return true;
}
//>@----------------------------------------------------------------
void MULTIMEDIA_CONVERT::slot_CatchStandardOutput()
{
    quint8 tmpCnt = 0;
    QString tmpString,tmpInfo,tmpKeyInfo,tmpKey0,tmpKey1;
    QStringList tmpStrList;
    tmpStrList.clear();
    if(m_Process != 0)
    {
        if(m_Process->state())
            tmpInfo = m_Process->readAllStandardOutput();
    }
    qDebug()<<"---"<<tmpInfo;
//>@Type
    tmpKey0 = QString("Input #");
    tmpKey1 = QString(", from '");
    tmpKeyInfo = GetStrBetweenPreAndAfter(tmpInfo,tmpKey0,tmpKey1,0,0,Qt::CaseInsensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        IDE_TRACE_STR(tmpKeyInfo);
        m_FileInfo.mType = GetSrcType(tmpKeyInfo);
    }
//>@Lenth
    tmpKey0 = QString("Duration:");
    tmpKeyInfo = GetStrBehindKey(tmpInfo,tmpKey0,1,8,Qt::CaseSensitive);
    if(!tmpKeyInfo.isEmpty())
    {
//        IDE_TRACE_STR(tmpKeyInfo);
        m_FileInfo.mLenth = GetLenth(tmpKeyInfo);
    }
//>@Resolution
    tmpKey0 = QString("Stream #0.0");
    tmpKey1 = QString("[PAR");
    tmpKeyInfo = GetStrBetweenPreAndAfter(tmpInfo,tmpKey0,tmpKey1,0,0,Qt::CaseInsensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        IDE_TRACE_STR(tmpKeyInfo);
        m_FileInfo.mResolution = GetResolution(tmpKeyInfo);
    }
//>@Progress
    tmpKey0 = QString("time=");
    tmpKey1 = QString("bitrate=");
    tmpStrList = tmpInfo.split(tmpKey0, QString::KeepEmptyParts, Qt::CaseInsensitive);
    tmpCnt = tmpStrList.count();
    for(int i=1;i<tmpCnt;i++)
    {
        tmpString = QString(" time=%1").arg(tmpStrList.at(i));
        IDE_TRACE_STR(tmpString);
        tmpKeyInfo = GetStrBetweenPreAndAfter(tmpString,tmpKey0,tmpKey1,0,1,Qt::CaseSensitive);
        if(!tmpKeyInfo.isEmpty())
        {
            IDE_TRACE_STR(tmpKeyInfo);
            emit sProgress(GetProgress(tmpKeyInfo,m_FileInfo.mLenth));
        }
    }
}

void MULTIMEDIA_CONVERT::slot_Finished(int pID, QProcess::ExitStatus pState)
{
    IDE_TRACE_INT(pID);
    switch(pState)
    {
        case 0:
            IDE_TRACE_STR("Sucess");
            mTaskState = Success;
            break;
        case 1:
            IDE_TRACE_STR("Falied");
            mTaskState = Failed;
            break;
    }
}

QPixmap GenerateSpectrum(QString pMediaName, QRectF pRect, QPixmap pBgPixmap)
{
    QRect tmpRect;
    if(pBgPixmap.isNull() == false)
        tmpRect = pBgPixmap.rect();
    else
        tmpRect = pRect.toRect();
    QImage tmpImage(tmpRect.width(), tmpRect.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return QPixmap();
    tmpImage.fill(Qt::transparent); //>@����͸��ͼ��
    QPainter painter(&tmpImage);
    if(pBgPixmap.isNull() == false)
        painter.drawPixmap(0,0,pBgPixmap);
    MULTIMEDIA_CONVERT tmpMediaCtrl;
    if(tmpMediaCtrl.GetFileInfo(pMediaName, 500))
    {
        qreal tmpDiffHeight = pRect.height() / 6.0;
        qreal tmpDiffWidth = tmpDiffHeight * 2.0 /3.0;

        QString tmpString;
        if(tmpMediaCtrl.m_FileInfo.mType == RC_INVALID)
        {
            tmpString= QString("[MediaType] : INVALID");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::red, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_MP3)
        {
            tmpString= QString("[MediaType] : MP3");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_WAV)
        {
            tmpString= QString("[MediaType] : WAV");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_MP4)
        {
            tmpString= QString("[MediaType] : MP4");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_AVI)
        {
            tmpString= QString("[MediaType] : AVI");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_RMVB)
        {
            tmpString= QString("[MediaType] : RMVB");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_FLV)
        {
            tmpString= QString("[MediaType] : FLV");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        tmpString = QString("[Path] : %1").arg(tmpMediaCtrl.m_FileInfo.mPath);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Name] : %1").arg(tmpMediaCtrl.m_FileInfo.mName);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*2, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[TotalSize] : %1").arg(tmpMediaCtrl.m_FileInfo.mSize);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*3, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Resolution] : %1x%2").arg(tmpMediaCtrl.m_FileInfo.mResolution.width()).arg(tmpMediaCtrl.m_FileInfo.mResolution.height());
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*4, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[TotalLength] : %1").arg(tmpMediaCtrl.m_FileInfo.mLenth);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*5, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
    }
    painter.end();
    return QPixmap::fromImage(tmpImage);
}

bool ConvertAudio(QString pSrcFileName, QString pDstFileName)
{
    QString tmpString = getFileSuffix(pSrcFileName);
    if(tmpString.isEmpty())
        return false;
    if(pDstFileName.endsWith(".wav", Qt::CaseInsensitive) == false)
        pDstFileName.append(".wav");
    if(tmpString.compare("wav", Qt::CaseInsensitive) == 0)
    {
        return CopyFile(pSrcFileName, pDstFileName);
    }
    else if(tmpString.compare("mp3", Qt::CaseInsensitive) == 0)
    {
        MULTIMEDIA_CONVERT tmpAudioConvert;
        return tmpAudioConvert.ConvertAudio(pSrcFileName, pDstFileName, 100000);
    }
    return false;
}

bool ConvertVideo(QString pSrcFileName, QString pDstFileName, QSize pResolution)
{
    return ConvertVideo(pSrcFileName, QRect(), pDstFileName, pResolution);
}

bool ConvertVideo(QString pSrcFileName, QRect pValidRect, QString pDstFileName, QSize pResolution)
{
    QString tmpString = getFileSuffix(pSrcFileName);
    if(tmpString.isEmpty())
        return false;
    if(pDstFileName.endsWith(".mp4", Qt::CaseInsensitive) == false)
        pDstFileName.append(".mp4");
    MULTIMEDIA_CONVERT tmpVideoConvert;
    return tmpVideoConvert.ConvertVideo(pSrcFileName, pResolution, 10, pDstFileName, 1000000);
}

/****************************************************************************************************
            ��ʽ�������������
***************************************************************************************************/
/************************************************************
_scanfĿǰ��֧�֣�
��֧���ַ����ַ������������ĸ�ʽ�����룻
һ��������MAX_SCANF_COUNT�����ݣ�
fmt��û�зָ���ʱĬ��ʹ�ÿո���Ʊ����
�ָ�����ʹ�����ⵥ�ֽ����ݣ�
�Ի�����Ϊ����Ľ�������
exp��
_scanf((pInputFun)myFun,"%d,%s,%c",&a,&b,&c) ����123,jinwenjie,A\n
************************************************************/

#ifdef WINDOWS

size_t _Scanf(pInputCharFun InputFun, const char* fmt,...)
{
    //����fmt
    if(fmt == NULL)
        return 0;
    size_t count=0;
    char tmpBuf[MAX_SCANF_COUNT][2]={{0}};  //���֧��һ�ζ���10����������
    size_t __tmp = strlen(fmt);
    for(size_t tmp=0;tmp<__tmp;tmp++)
    {
        if(fmt[tmp] == '%')
        {
            //��ֹ���fmt��Χ
            if(tmp+1 >= __tmp)
                return 0;
            if((fmt[tmp+1]=='c')||(fmt[tmp+1]=='s'||(fmt[tmp+1]=='d')))  //Ŀǰ��֧��%d,%s,%c
            {
                tmpBuf[count][0]	= fmt[++tmp];
                //printf("%c\t",tmpBuf[count][0]);
                //��ֹ���fmt��Χ
                if(tmp+1 >= __tmp+1)
                    return 0;
                if((fmt[tmp+1]=='%')) //�ָ���֧�ֳ�%����������ַ�����ֻ�����ǵ��ַ�
                {
                    tmpBuf[count][1]	= 0;
                }
                else if(fmt[tmp+1]=='\0')  //fmt����
                {
                    tmpBuf[count][1]	= 1;
                }
                else
                {
                    tmpBuf[count][1]	= fmt[tmp+1];
                }
                count++;
                if(count == MAX_SCANF_COUNT)
                    break;
            }
            else
                return 0;//��ʽ���Ϸ�
        }
    }
    //�˴�countֵΪfmt���ж������������
    //printf(" total : %d\r\n",count);

    char key;
    char * p_arg_c = 0;
    char * p_arg_s = 0;
    int * p_arg_d = 0;
    va_list argbuf;
    va_start(argbuf, fmt);
    size_t i = 0;  //����ָʾʵ�ʴ����������
    bool InReading = false;	//ָʾ��ȡһ�����������Ƿ������true���ڶ�ȡ
    size_t StrCount=0; //��ȡ���ַ�������
    while(1)
    {
        if( i == count) //����Ѿ���ȡ��ָ�����������͸�������ֱ���˳�
        {
            goto exit;
        }
        key = InputFun();
        //���û�зָ���Ĭ���Կո��\t���зָ�������ָ���ַ����зָע���������������ָ���������Ĭ�Ͽ���һ���ָ���
        if((((tmpBuf[i][1]==0)||(tmpBuf[i][1]==1))?((key != ' ')&&(key != '\t')):(key != tmpBuf[i][1]))&&(key != '\r')&&(key != '\n'))   //ע������������ַ���Ϊ������ֹ��������Ҫ����key != '\r' key != '\n'
        {
            switch(tmpBuf[i][0])
            {
            case 'c':  //ֻ����һ���ַ�
                if((key >= 'a' && key <= 'z')||(key >= 'A' && key <= 'Z'))  //ֻȡ��ĸ��������Ϊ���Ϸ��������������ҪҪ�޸Ĵ˴�
                {
                    p_arg_c= va_arg(argbuf, char*);
                    *p_arg_c = key;
                    InReading = true;
                }
                else
                {
                    goto exit;
                }
                break;
            case 's':
                if((key >= 'a' && key <= 'z')||(key >= 'A' && key <= 'Z'))  //ֻȡ��ĸ��������Ϊ���Ϸ��������������ҪҪ�޸Ĵ˴�
                {
                    if(InReading == false)
                    {
                        p_arg_s = va_arg(argbuf, char*);
                        *p_arg_s = key;
                        StrCount++;
                        InReading = true;
                    }
                    else
                    {
                        *(p_arg_s+StrCount++) = key;   //�˴�û�м��ж��Ƿ�������ж�
                    }
                }
                else
                {
                    goto exit;
                }
                break;
            case 'd':
                if(key <= '9' && key >= '0')
                {
                    if(InReading == false)
                    {
                        p_arg_d = va_arg(argbuf, int*);
                        *p_arg_d = key - '0';
                        InReading = true;
                    }
                    else
                    {
                        *p_arg_d *= 10;
                        *p_arg_d += key - '0';
                    }
                }
                else
                {
                    goto exit;
                }
                break;
            }
        }
        else
        {
            if(InReading == true)  //ȷ�ϳɹ���ȡ��һ�����ݽṹʱ���Լ�1.
                i++;
            InReading = false;  //�������ָ���ʱ����ʾ���ݽṹȡֵ����
            if(StrCount>0)  //������ַ������������������\0
            {
                *(p_arg_s+StrCount++) = '\0';
                StrCount = 0;
            }
            if(key == '\n')	//�Ի��з���Ϊ�������
            {
                goto exit;
            }
        }
    }
exit:
    va_end(argbuf);
    return i;
}


size_t _Printf(pOutputStrFun OutputStrFun, const char* fmt,...)
{
    char *tmpBuf = new char[1024];
    size_t len;
    va_list argptr;
    va_start(argptr, fmt);
    vsprintf(tmpBuf, fmt, argptr);
    va_end(argptr);
    len = OutputStrFun(tmpBuf, strlen(tmpBuf));
    delete []tmpBuf;
    return len;
}

size_t _Printf(pOutputCharFun OutputCharFun, const char* fmt,...)
{
    char *tmpBuf = new char[1024];
    size_t len;
    va_list argptr;
    va_start(argptr, fmt);
    vsprintf(tmpBuf, fmt, argptr);
    va_end(argptr);
    for( len=0;len<strlen(tmpBuf);len++ )
    {
        if(OutputCharFun(tmpBuf[len]) == false)
            break;
    }
    return len;
}
#endif

bool InitLogPath(QString pLogPath, QString pLogName, int pMaxLogFileNum)
{
    if(pLogName.isEmpty())
        return false;
    if(pLogPath.endsWith("/") == false)
        pLogPath.append("/");
    QDir tmpDir(pLogPath);
    if(tmpDir.exists() == false)
        tmpDir.mkpath(pLogPath);
    QString FilePath = QString("%1%2").arg(pLogPath).arg(pLogName);
    QString tmppath;
    int i;
    for(i=1;i<=pMaxLogFileNum;i++)
    {
        tmppath = QString("%1.%2").arg(FilePath).arg(i);
        QFile file(tmppath);
        if(file.exists() == true)
            continue;
        else
            break;
    }
    QString srcFile;
    QString dstFile;
    if(i == pMaxLogFileNum+1)  //>@��־�ļ�����
    {
        for(int j=1;j<=pMaxLogFileNum;j++)
        {
            srcFile = QString("%1.%2").arg(FilePath).arg(j);
            dstFile = QString("%1.%2").arg(FilePath).arg(j-1);
            CopyFile(srcFile,dstFile);
        }
    }
    srcFile = FilePath;
    dstFile = QString("%1.%2").arg(FilePath).arg(i);
    CopyFile(srcFile,dstFile);
    return true;
}
bool COM_INFO::AddState(QString pStateName, EFFECT_INFO pEffect)
{
    if(pStateName.isEmpty())
        return false;
    for(int i=StateMachine.count()-1;i>=0;i--)
    {
        STATE_INFO* tmpState = StateMachine.at(i);
        if(tmpState == 0)
        {
            IDE_TRACE();
            StateMachine.removeAt(i);
            SetUpdateLevel(UPDATESTATE);
            continue;
        }
        if(tmpState->StateName == pStateName)
            return true;
    }
    STATE_INFO* tmpStateInfo = new STATE_INFO;
    tmpStateInfo->StateName = pStateName;
    tmpStateInfo->Effect = pEffect;
    return AddState(tmpStateInfo);
}

bool COM_INFO::AddState(STATE_INFO* pState)
{
    StateMachine.append(pState);
    SetUpdateLevel(UPDATESTATE);
    return true;
}

bool COM_INFO::DelState(QString pStateName, bool pFree)
{
    if(pStateName.isEmpty())
        return false;
    for(int i=StateMachine.count()-1;i>=0;i--)
    {
        STATE_INFO* tmpState = StateMachine.at(i);
        if(tmpState == 0)
        {
            IDE_TRACE();
            StateMachine.removeAt(i);
            SetUpdateLevel(UPDATESTATE);
            continue;
        }
        if(tmpState->StateName == pStateName)
        {
            return DelState(tmpState, pFree);
        }
    }
    return true;
}

bool COM_INFO::DelState(STATE_INFO* pState, bool pFree)
{
    if(pState == 0)
        return false;
    if(pFree)
        delete pState;
    SetUpdateLevel(UPDATESTATE);
    return StateMachine.removeOne(pState);
}

void COM_INFO::DelStateList(bool pFree)
{
    for(int i=StateMachine.count()-1;i>=0;i--)
    {
        STATE_INFO* tmpState = StateMachine.at(i);
        if(tmpState == 0)
        {
            IDE_TRACE();
            StateMachine.removeAt(i);
            SetUpdateLevel(UPDATESTATE);
            continue;
        }
        DelState(tmpState, pFree);
    }
}

bool COM_INFO::MdyState(QString pStateName, EFFECT_INFO pEffect)
{
    if(pStateName.isEmpty())
        return false;
    for(int i=StateMachine.count()-1;i>=0;i--)
    {
        STATE_INFO* tmpState = StateMachine.at(i);
        if(tmpState == 0)
        {
            IDE_TRACE();
            StateMachine.removeAt(i);
            SetUpdateLevel(UPDATESTATE);
            continue;
        }
        if(tmpState->StateName == pStateName)
        {
            delete tmpState;
            STATE_INFO* tmpStateInfo = new STATE_INFO;
            tmpStateInfo->StateName = pStateName;
            tmpStateInfo->Effect = pEffect;
            StateMachine.replace(i, tmpStateInfo);
            SetUpdateLevel(UPDATESTATE);
            return true;
        }
    }
    return AddState(pStateName, pEffect);
}

bool COM_INFO::MdyGeometry(QRectF pRect)
{
    if(pRect.left() < 0 || pRect.top() < 0 || !(pRect.isValid()))
        return false;
    if(ComRect == pRect)
        return false;
    if(ComRect.size() != pRect.size())
        SetUpdateLevel(UPDATESTATE);
    ComRect = pRect;
    return true;
}

bool COM_INFO::MdyGeometry(QString pRectString)
{
    if(pRectString.isEmpty())
        return false;
    QStringList tmpList = pRectString.split(',');
    if(tmpList.count() != 4)
        return false;
    return MdyGeometry(QRectF(tmpList.at(0).toInt(),
                              tmpList.at(1).toInt(),
                              tmpList.at(2).toInt(),
                              tmpList.at(3).toInt()));
}

void COM_INFO::DelChildList()
{
    for(int i=ChildComListPtr.count()-1;i>=0;i--)
    {
        COM_INFO* tmpComInfo = ChildComListPtr.at(i);
        if(tmpComInfo==0)
            continue;
        delete tmpComInfo;
        ChildComListPtr.removeAt(i);
    }
}

void COM_INFO::SetUpdateLevel(UPDATELEVEL pLevel)
{
    switch(pLevel)
    {
        case UPDATENONE:
        {
            mUpdateLevel = UPDATENONE;
            break;
        }
        case UPDATEGEOMETRY:
        {
            if(mUpdateLevel == UPDATENONE)
                mUpdateLevel = UPDATEGEOMETRY;
            else if(mUpdateLevel == UPDATESTATE)
                mUpdateLevel = UPDATEALL;
            break;
        }
        case UPDATESTATE:
        {
            if(mUpdateLevel == UPDATENONE)
                mUpdateLevel = UPDATESTATE;
            else if(mUpdateLevel == UPDATEGEOMETRY)
                mUpdateLevel = UPDATEALL;
            break;
        }
        case UPDATEALL:
        {
            mUpdateLevel = UPDATEALL;
            break;
        }
        default:
            return;
    }
}

void COM_INFO::ClrUpdateLevel(UPDATELEVEL pLevel)
{
    switch(pLevel)
    {
        case UPDATEGEOMETRY:
        {
            if(mUpdateLevel == UPDATEGEOMETRY)
                mUpdateLevel = UPDATENONE;
            else if(mUpdateLevel == UPDATEALL)
                mUpdateLevel = UPDATESTATE;
            break;
        }
        case UPDATESTATE:
        {
            if(mUpdateLevel == UPDATESTATE)
                mUpdateLevel = UPDATENONE;
            else if(mUpdateLevel == UPDATEALL)
                mUpdateLevel = UPDATEGEOMETRY;
            break;
        }
        case UPDATEALL:
        {
            mUpdateLevel = UPDATENONE;
            break;
        }
        default:
            return;
    }
}

#if(QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
void SetFuzzy(QGraphicsItem *pItem, qreal pBlurRadius)
{
    if(!pItem) return;
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
    effect->setBlurRadius(pBlurRadius);
    pItem->setGraphicsEffect(effect);
}
void SetColor(QGraphicsItem *pItem, QColor pColor)
{
    if(!pItem) return;
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect;
    effect->setColor(pColor);
    pItem->setGraphicsEffect(effect);
}
void SetShadow(QGraphicsItem *pItem, QColor pColor, qreal pBlurRadius, QPointF pPointF)
{
    if(!pItem) return;
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setColor(pColor);
    effect->setBlurRadius(pBlurRadius);
    effect->setOffset(pPointF);
    pItem->setGraphicsEffect(effect);
}
void SetOpacity(QGraphicsItem *pItem, qreal pOpacity)
{
    if(!pItem) return;
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacity(pOpacity);
    pItem->setGraphicsEffect(effect);
}
void ClearFuzzy(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsBlurEffect* effect = VAR_CAST<QGraphicsBlurEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearColor(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsColorizeEffect* effect = VAR_CAST<QGraphicsColorizeEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearShadow(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsDropShadowEffect* effect = VAR_CAST<QGraphicsDropShadowEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearOpacity(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsOpacityEffect* effect = VAR_CAST<QGraphicsOpacityEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}

void Effect_Fuzzy(QGraphicsItem *Item,qreal n)
{
    if(!Item) return;
    QGraphicsBlurEffect *uEffect = new QGraphicsBlurEffect();
    uEffect->setBlurRadius(n);
    Item->setGraphicsEffect(uEffect);
}

void Effect_color(QGraphicsItem *Item,int r, int g, int b)
{
    if(!Item) return;
    if(r==-1 || g==-1 || b==-1)
    {
        Item->setGraphicsEffect(0);
    }
    else
    {
        QGraphicsColorizeEffect *uEffect = new QGraphicsColorizeEffect();
        uEffect->setColor(QColor(r,g,b));
        Item->setGraphicsEffect(uEffect);
    }
}

void Effect_Shadow(QGraphicsItem *Item,qreal dx, qreal dy)
{
    if(!Item) return;
    QGraphicsDropShadowEffect *uEffect = new QGraphicsDropShadowEffect();
    uEffect->setOffset(dx,dy);
    Item->setGraphicsEffect(uEffect);
}
void Effect_Transparent(QGraphicsItem *Item,qreal opacity)
{
    if(!Item) return;
    QGraphicsOpacityEffect *uEffect = new QGraphicsOpacityEffect();
    uEffect->setOpacity(opacity);
    Item->setGraphicsEffect(uEffect);
}
#endif

GraphicsSceneBase::GraphicsSceneBase(QObject *parent, QRect pRect) :
    QGraphicsScene(parent)
{
    m_Rect = pRect;
    setSceneRect(pRect);
}

GraphicsViewBase::GraphicsViewBase(QWidget *parent)
{
    QRectF rect = this->geometry();
    m_Scene = new GraphicsSceneBase(0, QRect(0,0,rect.width(),180));
    setScene(m_Scene);

    setRenderHint(QPainter::Antialiasing);
    //setBackgroundBrush(QPixmap("d:\\bg.png"));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(true);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

GraphicsViewBase::GraphicsViewBase(QWidget *parent, QRect pRect):
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setGeometry(pRect);

    m_Scene = new GraphicsSceneBase(0, pRect);
    setScene(m_Scene);

    setRenderHint(QPainter::Antialiasing);
    //setBackgroundBrush(QPixmap("d:\\bg.png"));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::RubberBandDrag);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(true);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

xmlParse::xmlParse() :
    QObject(), QDomDocument()
{
    m_Valid = false;
    m_IsChanged = true;
}

xmlParse::xmlParse(QString fileName) :
    QObject(), QDomDocument()
{
    m_Valid = false;
    m_IsChanged = true;
    int index = fileName.lastIndexOf('/');
    if(index < 0)
    {
        m_FileName = fileName;
        m_FileDir = "";
    }
    else
    {
        m_FileName = fileName.mid(index+1);
        m_FileDir = fileName.mid(0,index+1);
        if(m_FileDir.endsWith('/') == false)
            m_FileDir.append('/');
    }
}

xmlParse::~xmlParse()
{
    if(m_IsChanged)
        SaveAs(m_FileDir + m_FileName);
    clear();
}

bool xmlParse::GetItemElement(QString itemPath)
{
    bool nodeFlag = true;          //>@�Ƿ�Ϊ/test/subtest/���͵�·��
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = documentElement();
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
        indexbk = index;
    }
    if(false == nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}

bool xmlParse::GetItemElement(QDomElement itemDomElement, QString itemPath)
{
    bool nodeFlag = true;          //>@�Ƿ�Ϊ/test/subtest/���͵�·��
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        IDE_TRACE_STR(itemPath);
        return false;
    }
    tmpElement = itemDomElement;
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE_STR(tmpString);
            return false;
        }
        tmpElement = newElement;
        indexbk = index;
    }
    if(false == nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE_STR(tmpString);
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}

QString xmlParse::GetItemElementValue()
{
    QDomNode tmpNode = itemElement.firstChild();
    if(tmpNode.isNull() == false)
       return itemElement.text();
    return QString("");
}

QString xmlParse::GetItemElementValue(QString itemPath)
{
    if(GetItemElement(itemPath))
    {
        return GetItemElementValue();
    }
    return QString("");
}

QList<QDomAttr> xmlParse::GetItemElementAttrs(QDomElement itemDomElement)
{
    QList<QDomAttr> tmpAttrs;
    QDomNamedNodeMap tmpNodesMap = itemDomElement.attributes();
    for(int i=0;i<tmpNodesMap.count();i++)
        tmpAttrs.append(tmpNodesMap.item(i).toAttr());
    return tmpAttrs;
}

QList<QDomAttr> xmlParse::GetItemElementAttrs()
{
    return GetItemElementAttrs(itemElement);
}

QList<QDomAttr> xmlParse::GetItemElementAttrs(QString itemPath)
{
    QList<QDomAttr> tmpAttrs;
    if(GetItemElement(itemPath))
        return GetItemElementAttrs();
    return tmpAttrs;
}

bool xmlParse::SearchItemElement(QString itemName)
{
    QDomElement docElem = documentElement();
    QDomNodeList nodes=docElem.elementsByTagName(itemName);
    if(nodes.size()>0)
    {
        QDomElement tmpElement = nodes.at(0).toElement();
        if(tmpElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        itemElement = tmpElement;
        return true;
    }
    return false;
}

bool xmlParse::CreateItemElement(QString itemPath)
{
    //>@��ȡ��������Ԫ��·����������·���Ƿ���ڣ�����������򴴽�
    bool nodeFlag = true;
    QString tmpString;
    QDomElement tmpElement, tmpElement_bk, newElement;
    QDomNode newNode;

    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = documentElement();
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        tmpElement = tmpElement.firstChildElement(tmpString);
        if(tmpElement.isNull())
        {
            newElement = createElement(tmpString);
            newNode = tmpElement_bk.appendChild(newElement);
            m_IsChanged = true;
            tmpElement = newNode.toElement();
            if(tmpElement.isNull())
            {
                IDE_TRACE();
                return false;
            }
        }
        indexbk = index;
        tmpElement_bk = tmpElement;
    }
    if(!nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        tmpElement = tmpElement.firstChildElement(tmpString);
        if(tmpElement.isNull())
        {
            newElement = createElement(tmpString);
            newNode = tmpElement_bk.appendChild(newElement);
            m_IsChanged = true;
            tmpElement = newNode.toElement();
            if(tmpElement.isNull())
            {
                IDE_TRACE();
                return false;
            }
        }
    }
    itemElement = tmpElement;
    m_IsChanged = true;
    return true;
}

bool xmlParse::CreateItemElement(QDomElement pParElement, QString pName, QString pValue)
{
    QDomElement newElement = createElement(pName);
    QDomText newTitleText = createTextNode(pValue);
    newElement.appendChild(newTitleText);
    pParElement.appendChild(newElement);
    return true;
}

bool xmlParse::SaveItemElement(QDomElement itemDomElement, QString& pContent)
{
    if(itemDomElement.isNull())
        return false;
    QTextStream out(&pContent);
    itemDomElement.save(out, 0);
    return true;
}

bool xmlParse::SaveItemElement(QString& pContent)
{
    return SaveItemElement(itemElement, pContent);
}

bool xmlParse::SaveItemElement(QDomElement itemDomElement, QIODevice *pDevice)
{
    if(itemDomElement.isNull() || !pDevice)
        return false;
    if(!pDevice->isOpen())
        return false;
    QTextStream out(pDevice);
    itemDomElement.save(out, 0);
    return true;
}

bool xmlParse::SaveItemElement(QIODevice *pDevice)
{
    return SaveItemElement(itemElement, pDevice);
}

bool xmlParse::ModifyItemElement(QString itemPath, QString value)
{
    bool flag = GetItemElement(itemPath);
    if(flag == false)
    {
        flag = CreateItemElement(itemPath);
        if(flag == false)
        {
            IDE_TRACE();
            return false;
        }
    }
    return ModifyItemElement(itemElement, value);
}

bool xmlParse::ModifyItemElement(QDomElement itemDomElement, QString value)
{
    if(itemDomElement.isElement() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(value.isNull())
    {
        IDE_TRACE();
        return false;
    }
    QDomText newTitleText = createTextNode(value);
    QDomNode tmpNode = itemDomElement.firstChild();
    if(tmpNode.isNull())
    {
        QDomNode tmpNewNode = itemDomElement.appendChild(newTitleText);
        m_IsChanged = true;
        if(tmpNewNode.isNull())
        {
            IDE_TRACE();
            return false;
        }
    }
    else
    {
        QDomNode tmpNewNode = itemDomElement.replaceChild(newTitleText, tmpNode);
        m_IsChanged = true;
        if(tmpNewNode.isNull())
        {
            IDE_TRACE();
            return false;
        }
    }
    return true;
}

bool xmlParse::SetText(QDomElement itemDomElement, QString value)
{
    if(itemDomElement.isNull())
        return false;
    itemDomElement.removeChild(itemDomElement.firstChild());
    if(!value.isEmpty())
        itemDomElement.appendChild(createTextNode(value));
    m_IsChanged = true;
    return true;
}

bool xmlParse::write(QIODevice *device)
{
    const int IndentSize = 4;
    if(device->isOpen() == false)
    {
        IDE_TRACE();
        return false;
    }
    QTextStream out(device);
    save(out, IndentSize);
    return true;
}

bool xmlParse::read(QIODevice *device)
{
    if(device->isOpen() == false)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = setContent(device, false, &errorStr, &errorLine, &errorColumn);
    if (flag == false)
    {
        QString tmpString = QString("Line %1,Column %2 : %3").arg(errorLine).arg(errorColumn).arg(errorStr);
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return true;
}

bool xmlParse::Open()
{
    if(!m_FileDir.endsWith("/"))
        m_FileDir.append("/");
    QFile file(m_FileDir + m_FileName);
    bool flag = file.exists();
    if(flag == true)
    {
        flag = file.open(QFile::ReadOnly | QFile::Text);
        if (flag == false)
        {
            IDE_TRACE();
            return false;
        }
        flag = read(&file);
        if(flag == false)
        {
            file.close();
            IDE_TRACE();
            return false;
        }
    }
    else
    {
        IDE_TRACE_STR(m_FileDir + m_FileName);
        return false;
    }
    file.close();
    return true;
}

bool xmlParse::New()
{
    QDir dir(m_FileDir);
    dir.mkpath(m_FileDir);

    QFile file(m_FileDir + m_FileName);       //<@�˴�ֻҪ�½�һ�����ļ����ɡ�
    bool flag = file.exists();
    if(flag == true)
    {
        IDE_TRACE();
        return false;
    }
    flag = file.open(QFile::WriteOnly | QFile::Text);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    file.close();
    return true;
}

bool xmlParse::Save()
{
    if(m_IsChanged)
    {
        //����ǰ�ļ�����
        QString tmpString( QString("%1%2").arg(m_FileName).arg("_bk") );
        bool flag = Rename(tmpString);
        if(flag == false)
        {
            IDE_TRACE_STR(tmpString);
            return false;
        }
        flag = SaveAs(m_FileDir+m_FileName);
        if(flag == false)
        {
            QFile bkFile(m_FileDir+tmpString);
            bkFile.rename(m_FileDir+m_FileName);
            IDE_TRACE();
            return false;
        }
        //ɾ�������ļ�
        QFile delFile(m_FileDir+tmpString);
        delFile.remove();
        m_IsChanged = false;
    }
    return true;
}

bool xmlParse::Rename(QString fileName)
{
#if 1
    QFile file(m_FileDir + m_FileName);
    file.setPermissions(QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);
    bool flag = file.exists();
    if(flag == false)  //>@����ļ������ڣ��������������ɹ���
    {
        IDE_TRACE_STR("File is not exist");
        return true;
    }
    flag = file.rename(m_FileDir + fileName);
    return flag;
#else
    QFile file(m_FileDir + m_FileName);
    file.copy(m_FileDir + fileName);
    file.remove();
#endif
}

bool xmlParse::SaveAs(QString fileName)
{
    QFile f(fileName);
    bool flag = f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    flag = write(&f);
    if(flag == false)
    {
        f.close();
        IDE_TRACE();
        return false;
    }
    f.close();
    m_IsChanged = false;
    return true;
}

iniParse::iniParse(QString pfileName) :
    QSettings(pfileName, QSettings::IniFormat)
{

}

bool iniParse::getProperty(QString pkey, QString &pValue)
{
    QVariant tmpVariant = value(pkey);
    if(tmpVariant.isNull())
        return false;
    pValue = tmpVariant.toString();
    return true;
}

int  iniParse::getProperty(QString pkey, int &pValue)
{
    QVariant tmpVariant = value(pkey);
    if(tmpVariant.isNull())
        return false;
    bool ok = false;
    int tmpValue = tmpVariant.toInt(&ok);
    if(ok == false)
        return false;
    pValue = tmpValue;
    return true;
}
