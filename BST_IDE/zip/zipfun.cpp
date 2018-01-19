#include "zipfun.h"

bool ZipTask::Zip(QString pSrcPath, QString pDstPath)
{
    QString tmpString = GetDirectoryName(pSrcPath);
    if(pDstPath.endsWith('/') == false)
        pDstPath.append('/');
    return Zip(pSrcPath, pDstPath + tmpString, true);
}

bool ZipTask::Zip(QString pSrcPath, QString pZipFile, bool pCover)
{
    if(pZipFile.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    if(pZipFile.endsWith(".zip", Qt::CaseInsensitive) == false)
        pZipFile.append(".zip");
    QFile tmpFile(pZipFile);
    if(tmpFile.exists())
    {
        if(pCover)
        {
            IDE_TRACE();
            tmpFile.remove();
        }
        else
        {
            IDE_TRACE();
            return true;
        }
    }
    //>@ȷ��Ŀ��·�����ڡ�
    QString tmpDstPath = getFileDirectory(pZipFile);
    CreatPath(tmpDstPath);

    QFileInfo tmpInfo(pSrcPath);
    if(tmpInfo.isFile())
    {
        if(!tmpInfo.exists())
        {
            IDE_TRACE_STR(pSrcPath);
            return false;
        }
        if(pSrcPath.endsWith(".zip", Qt::CaseInsensitive)) //>@������ѹ��zip�ļ�
        {
            IDE_TRACE();
            return false;
        }
        //>@���ļ�׼����ʼѹ��
#ifdef GCC
        QuaZip zip(pZipFile);
        if(!zip.open(QuaZip::mdCreate))
        {
            IDE_TRACE();
            return false;
        }
        QuaZipFile outFile(&zip);
        QString tmpNameOfZip = getFileName(pSrcPath);//>@ѹ�����ڵĴ��ļ����ļ���
        QFile inFile(pSrcPath);
        if(!inFile.open(QIODevice::ReadOnly))
        {
            IDE_TRACE();
            zip.close();
            return false;
        }
        if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(tmpNameOfZip, inFile.fileName())))
        {
            IDE_TRACE();
            inFile.close();
            zip.close();
            return false;
        }
#endif
        //>@���Ϳ�ʼ�ź�
        emit start();
        //>@��ʼѹ��
#ifdef GCC
        quint64 count = inFile.size();
        quint64 i=0;
        char c;
        while(inFile.getChar(&c)&&outFile.putChar(c))
        {
            emit progress(i*100/count);
        }
        if(outFile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            inFile.close();
            outFile.close();
            zip.close();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            return false;
        }
        outFile.close();
        if(outFile.getZipError()!=UNZ_OK)
        {
            IDE_TRACE();
            inFile.close();
            zip.close();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            return false;
        }
        inFile.close();
        zip.close();
        if(zip.getZipError()!=UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            return false;
        }
#endif
        //>@���ͽ����ź�
        emit end();
        return true;
    }
    else if(tmpInfo.isDir())
    {
        if(!tmpInfo.exists())
        {
            IDE_TRACE_STR(pSrcPath);
            return false;
        }
        QStringList tmpFileList = ErgodicDirectory(pSrcPath);
        if (tmpFileList.size() > 0)
        {
#ifdef GCC
            QuaZip zip(pZipFile);
            if(!zip.open(QuaZip::mdCreate))
            {
                IDE_TRACE();
                return false;
            }
            QuaZipFile outFile(&zip);
#endif
            //>@���Ϳ�ʼ�ź�
            emit start();
            //>@��ʼѹ��
#ifdef GCC
            int totalf = tmpFileList.size();
            char c;
            QString tmpParentSrcDir = getParentDirectory(pSrcPath);//>@����pSrcDir�ĸ�·��
            if(tmpParentSrcDir.endsWith("/") == false)
                tmpParentSrcDir.append("/");
            for (int i=0; i<totalf; ++i)
            {
                QString tmpSrcPath = tmpFileList.at(i);
                QString tmpString = tmpSrcPath;
                QString tmpNameOfZip = tmpString.replace(tmpParentSrcDir,"");//>@ѹ�����ڵĴ��ļ����ļ���
                QFile inFile(tmpSrcPath);
                if(!inFile.open(QIODevice::ReadOnly))
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    continue;
                }
                if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(tmpNameOfZip, inFile.fileName())))
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    continue;
                }
                outFile.write(inFile.readAll());
                if(outFile.getZipError() != UNZ_OK)
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    continue;
                }
                outFile.close();
                if(outFile.getZipError()!=UNZ_OK)
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    continue;
                }
                inFile.close();
                //>@���㴫��İٷֱ�
                emit progress(i*100/totalf);
                emit message(QString("Zip %1 %%2 success").arg(tmpNameOfZip)
                                                          .arg(BiteorMega(inFile.size())));
            }
            zip.close();
            if(zip.getZipError()!=UNZ_OK)
            {
                IDE_TRACE();
                emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
                emit error();
                return false;
            }
#endif
            //>@���ͽ����ź�
            emit end();
            return true;
        }
        return false;
    }
    return false;
}

bool ZipTask::Unzip(QString pZipFile, QString pDstPath, bool pCover)
{
    if(pDstPath.endsWith("/") == false)
        pDstPath.append("/");
    CreatPath(pDstPath);

#ifdef GCC
    QuaZip zip(pZipFile);
    if(!zip.open(QuaZip::mdUnzip))
    {
        IDE_TRACE();
        return false;
    }
#endif
    //>@���Ϳ�ʼ�ź�
    emit start();
#ifdef GCC
    //>@����ѹ���ļ��е����ļ���
    int total=0;
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile())
    {
        total++;
    }
    int i=0;
    QuaZipFile infile(&zip);
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile())
    {
        QString tmpFileName = zip.getCurrentFileName();
        QString tmpString = pDstPath + tmpFileName;
        QFile outfile(tmpString);/* extract to path ....... */
        if(outfile.exists())
        {
            if(pCover)
            {
                IDE_TRACE();
                outfile.remove();
            }
            else
            {
                IDE_TRACE();
                continue;
            }
        }
        QFileInfo infofile(outfile);
        if(!CreatPath(infofile.absolutePath()))
        {
            IDE_TRACE_STR(infofile.absolutePath());
            continue;
        }
        if(!infile.open(QIODevice::ReadOnly))
        {
            IDE_TRACE_STR(tmpFileName);
            continue;
        }
        if(!outfile.open(QIODevice::WriteOnly))
        {
            IDE_TRACE_STR(tmpString);
            infile.close();
            continue;
        }
        outfile.write(infile.readAll());   /* write */
        if(infile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Unzip %1 error!").arg(tmpFileName));
            infile.close();
            outfile.close();
            continue;
        }
        outfile.close();
        infile.close();
        if(infile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Unzip %1 error!").arg(tmpFileName));
            continue;
        }
        //>@���㴫��İٷֱ�
        emit progress(++i*100/total);
        emit message(QString("Unzip %1 success!").arg(tmpFileName));
    }
    zip.close();
    if(zip.getZipError()!=UNZ_OK)
    {
        IDE_TRACE();
        emit message(QString("Unzip %1 to %2 error!").arg(pZipFile).arg(pDstPath));
        emit error();
        return false;
    }
#endif
    //>@���ͽ����ź�
    emit end();
    return true;
}

ZipTaskThread::ZipTaskThread(QObject *parent) :
    QThread(parent)
{
    m_ZipTask.moveToThread(this);
}

bool ZipTaskThread::Zip(QString pSrcPath, QString pZipFile, bool pCover)
{
    if(isRunning())
        return false;
    start(QThread::LowPriority);
    wait(10);
    return m_ZipTask.Zip(pSrcPath, pZipFile, pCover);
}

bool ZipTaskThread::Unzip(QString pZipFile, QString pDstPath, bool pCover)
{
    if(isRunning())
        return false;
    start(QThread::LowPriority);
    wait(10);
    return m_ZipTask.Unzip(pZipFile, pDstPath, pCover);
}
