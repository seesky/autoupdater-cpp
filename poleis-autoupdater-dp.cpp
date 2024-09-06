#include "poleis-autoupdater-dp.h"
#include "poleis-autoupdater-dfi.h"
#include "poleis-autoupdater-constfile.h"
#include <QtCore/QThread>




PoleisAutoupdaterDownloader::PoleisAutoupdaterDownloader(GList *downloadList, QObject *parent) : QObject(parent)
{
    this->evtPerDonwload = new PoleisAutoupdaterManualResetEvent(false);

    this->downloadList = downloadList;
    this->allFileList = NULL;
    this->total = 0;
    this->nDownloadedTotal = 0;
    this->isFinished = false;

    for (GList *df = this->downloadList; df != NULL; df = df->next)
    {
        PoleisAutoupdaterDownloadFileInfo *dfi = (PoleisAutoupdaterDownloadFileInfo*)df->data;
        this->allFileList = g_list_append(this->allFileList, dfi);    
    }
}

void PoleisAutoupdaterDownloader::run()
{
    QString tempFolderPath = QDir::currentPath() + "/" + QString::fromStdString(TEMPFOLDERNAME);

    qDebug() << tempFolderPath;
    
    QDir dir(tempFolderPath);
    if(!dir.exists()){
        dir.mkpath(tempFolderPath);
    }


    //计算所有文件的总大小
    for (GList *df = this->downloadList; df != NULL; df = df->next)
    {
        PoleisAutoupdaterDownloadFileInfo *dfi = (PoleisAutoupdaterDownloadFileInfo*)df->data;
        this->total += dfi->getSize();  
    }

    emit(this->currentDownloadTotalSize(this->total));

    while(!this->isFinished){
        if(g_list_length(this->downloadList) == 0){
            this->isFinished = true;
            break;
        }

        PoleisAutoupdaterDownloadFileInfo *file = (PoleisAutoupdaterDownloadFileInfo*)g_list_first(this->downloadList)->data;

        emit(this->currentDownloadFileName(this->tr("Downloading...   ") + QString::fromStdString(file->getFileName())));
        emit(this->currentDownloadFileSize(file->getSize()));

        // this->session = soup_session_new();

        QString tempFolderPath1 = QString::fromStdString(PoleisAutoupdaterCommonUnitity::getFolderUrl(file));
        if(!tempFolderPath1.isNull()){
            tempFolderPath = QDir::currentPath() + "/" + QString::fromStdString(TEMPFOLDERNAME);
            tempFolderPath += tempFolderPath1;
        }else{
            tempFolderPath = QDir::currentPath() + "/" + QString::fromStdString(TEMPFOLDERNAME);
        }

        qDebug() << tempFolderPath;

        if(QString::fromStdString(file->getFileName()).contains("\\\\")){
            QString tmpDir = tempFolderPath + QString::fromStdString(file->getFileName()).mid(0, QString::fromStdString(file->getFileName()).lastIndexOf("\\\\"));
            
            qDebug() << tmpDir;
            
            QDir dirTmpDir(tmpDir);
            if(!dirTmpDir.exists()){
                dir.mkpath(tmpDir);
            }
        }

        this->evtPerDonwload->reset();

        this->manager = new QNetworkAccessManager();
        QString urlstr = QString::fromStdString(file->getDownloadUrl());
        qDebug() << urlstr;
        QUrl url = QUrl(urlstr);
        QNetworkRequest *request = new QNetworkRequest(url);

        QEventLoop *eventLoop = new QEventLoop();

        QNetworkReply* reply = manager->get(*request);

        connect(reply, &QNetworkReply::downloadProgress, this, [this](qint64 bytesReceived, qint64 bytesTotal){
            // emit(this->currentDownloadFileSize(bytesTotal));
            this->nDownloadedTotal = this->nDownloadedTotal + bytesReceived;
            emit(this->currentDownloadTotalOffset(this->nDownloadedTotal));
            emit(this->currentDownloadFileOffset(bytesReceived));
        });

        QString savePath = tempFolderPath + "/" + QString::fromStdString(file->getFileName());
        connect(reply, &QNetworkReply::finished, this, [this, reply, savePath, eventLoop](){
            if(reply->error() == QNetworkReply::NoError){
                QFile file(savePath);
                qDebug() << savePath;
                if(file.open(QIODevice::WriteOnly)){
                    file.write(reply->readAll());
                    file.close();
                    delete reply;

                    eventLoop->quit();
                    this->evtPerDonwload->set();
                }else{
                    //TODO:这里应当发出错误信号，然后恢复应用
                    delete reply;
                    return;
                }
            }else{
                //TODO:这里应当发出错误信号，然后恢复应用
                delete reply;
            }
        });

        eventLoop->exec();

        this->evtPerDonwload->wait();


        delete manager;
        this->downloadList = g_list_remove(this->downloadList, file);

        // GFile *dfile = g_file_new_for_uri(file->getDownloadUrl().c_str());
        // GError *derror = NULL;
        // gchar *dcontents;
        // gsize length;

        // if(g_file_load_contents(dfile, NULL, &dcontents, &length, NULL, &derror))
        // {
            
        // }

    }

    if(g_list_length(this->downloadList) > 0){
        return;
    }

    for (GList *df = this->allFileList; df != NULL; df = df->next)
    {
        PoleisAutoupdaterDownloadFileInfo *file = (PoleisAutoupdaterDownloadFileInfo*)df->data;
        QString tempUrlPath = QString::fromStdString(PoleisAutoupdaterCommonUnitity::getFolderUrl(file));
        qDebug() << tempUrlPath;
        QString oldPath;
        QString newPath;

        try
        {
            if(!tempUrlPath.isEmpty()){
                oldPath = QDir::currentPath() + "/" +tempUrlPath.mid(1, -1) + "/" + QString::fromStdString(file->getFileName());
                newPath = QDir::currentPath() + "/" +QString::fromStdString(TEMPFOLDERNAME) + "/" + tempUrlPath + "/" +QString::fromStdString(file->getFileName());
            }else{
                oldPath = QDir::currentPath() + "/" +QString::fromStdString(file->getFileName());
                newPath = QDir::currentPath() + "/" +QString::fromStdString(TEMPFOLDERNAME) + "/" + QString::fromStdString(file->getFileName());
            }

            qDebug() << "oldPath:" << oldPath << "\n";
            qDebug() << "newPath:" << newPath << "\n";

            //just deal with the problem which the files EndsWith xml can not download
            // System.IO.FileInfo f = new FileInfo(newPath);
            // if (!file.Size.ToString().Equals(f.Length.ToString()) && !file.FileName.ToString().EndsWith(".xml"))
            // {
            //     ShowErrorAndRestartApplication();
            // }


            //Added for dealing with the config file download errors
            // string newfilepath = string.Empty;
            // if (newPath.Substring(newPath.LastIndexOf(".") + 1).Equals(ConstFile.CONFIGFILEKEY))
            // {
            //     if (System.IO.File.Exists(newPath))
            //     {
            //         if (newPath.EndsWith("_"))
            //         {
            //             newfilepath = newPath;
            //             newPath = newPath.Substring(0, newPath.Length - 1);
            //             oldPath = oldPath.Substring(0, oldPath.Length - 1);
            //         }
            //         File.Move(newfilepath, newPath);
            //     }
            // }
            // //End added

            if(QFile(oldPath).exists()){
                this->moveFolderToOld(oldPath, newPath);
            }else{
                if(!tempUrlPath.isNull()){
                    if(!QDir(QDir::currentPath() + tempUrlPath.mid(1, -1)).exists())
                    {
                        QDir().mkpath(QDir::currentPath() + tempUrlPath.mid(1, -1));
                        this->moveFolderToOld(oldPath, newPath);
                    }else{
                        this->moveFolderToOld(oldPath, newPath);
                    }
                }else{
                    this->moveFolderToOld(oldPath, newPath);
                }
            }


        }
        catch(const std::exception& e)
        {
            emit(this->downloadError());
            std::cerr << e.what() << '\n';
        }
        
    }

    g_list_free(this->allFileList);
    if(g_list_length(this->downloadList) == 0){
        emit(this->downloadFinished());
    }else{
        emit(this->downloadError());
    }

}


void PoleisAutoupdaterDownloader::moveFolderToOld(QString oldPath, QString newPath)
{
    if(QFile(oldPath + ".old").exists()){
        QFile(oldPath + ".old").remove();
    }

    if(QFile(oldPath).exists()){
        QFile(oldPath).rename(oldPath, newPath);
    }

    qDebug() << "oldPath:" << oldPath << "\n";
    qDebug() << "newPath:" << newPath << "\n";
    QFile(oldPath).rename(newPath, oldPath);
}


PoleisAutoupdaterDownLoadProgress::PoleisAutoupdaterDownLoadProgress(GList*downloadList, QString title, QWidget *parent) : FramelessDialog(parent)
{

    this->downloadFileList = downloadList;


    this->_setUpUi(title, this);
    this->windowTitleLabel = new QLabel(title, this);

    this->setResizeEnabled(false);
    this->resize(700, 300);
    this->titleBar->hide();

    this->vBoxLayout->insertWidget(0, this->windowTitleLabel, 0, Qt::AlignTop);
    this->windowTitleLabel->setObjectName(QString("windowTitleLabel"));
    FluentStyleSheet().apply(this, FluentStyleSheetMap.value("DIALOG"), Theme::AUTO);
    this->setFixedSize(this->size());


    this->downloadThread = new QThread();
    this->downloader = new PoleisAutoupdaterDownloader(this->downloadFileList, nullptr);
    connect(this->downloader, &PoleisAutoupdaterDownloader::currentDownloadFileName, this, &PoleisAutoupdaterDownLoadProgress::currentDownloadFileName, Qt::DirectConnection);
    connect(this->downloader, &PoleisAutoupdaterDownloader::currentDownloadFileSize, this, &PoleisAutoupdaterDownLoadProgress::currentDownloadFileSize, Qt::DirectConnection);
    connect(this->downloader, &PoleisAutoupdaterDownloader::currentDownloadFileOffset, this, &PoleisAutoupdaterDownLoadProgress::currentDownloadFileOffset, Qt::DirectConnection);
    connect(this->downloader, &PoleisAutoupdaterDownloader::currentDownloadTotalOffset, this, &PoleisAutoupdaterDownLoadProgress::currentDownloadTotalOffset, Qt::DirectConnection);
    connect(this->downloader, &PoleisAutoupdaterDownloader::currentDownloadTotalSize, this, &PoleisAutoupdaterDownLoadProgress::currentDownloadTotalSize, Qt::DirectConnection);
    connect(this->downloader, &PoleisAutoupdaterDownloader::downloadFinished, this, &PoleisAutoupdaterDownLoadProgress::downloadFinished, Qt::DirectConnection);
    this->downloader->moveToThread(this->downloadThread);
    connect(this->downloadThread, &QThread::started, this->downloader, &PoleisAutoupdaterDownloader::run);
    connect(this->downloadThread, &QThread::finished, this->downloader, &PoleisAutoupdaterDownloader::deleteLater);
    this->downloadThread->start();
}

void PoleisAutoupdaterDownLoadProgress::_setUpUi(QString title, QWidget *parent)
{

    this->buttonGroup = new QFrame(parent);
    
    this->cancelButton = new QPushButton(this->tr("Skip"), this->buttonGroup);

    this->vBoxLayout = new QVBoxLayout(parent);
    this->buttonLayout = new QHBoxLayout(this->buttonGroup);

    this->progressLayout = new QVBoxLayout(); 

    this->subTitle = new QLabel(this->tr("This step will take a few minutes and will install the latest version on your computer"));
    this->currentDownloadFileLabel = new QLabel(this->tr("Downloading..."));
    this->currentDownloadProgressBar = new ProgressBar(nullptr, true);
    this->totalDownloadProgressLabel = new QLabel(this->tr("Overall progress:"));
    this->totalDownloadProgressBar = new ProgressBar(nullptr, true);

    this->__initWidget();
}

void PoleisAutoupdaterDownLoadProgress::__initWidget()
{
    this->__setQss();
    this->__initLayout();

    this->cancelButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    this->cancelButton->setFocus();
    this->buttonGroup->setFixedHeight(81);

    connect(this->cancelButton, &QPushButton::clicked, this, &PoleisAutoupdaterDownLoadProgress::__onCancelButtonClicked);
}


void PoleisAutoupdaterDownLoadProgress::__setQss()
{
    this->buttonGroup->setObjectName(QString("buttonGroup"));
    this->cancelButton->setObjectName(QString("cancelButton"));

    FluentStyleSheet().apply(this, FluentStyleSheetMap.value("DIALOG"), Theme::AUTO);

    this->cancelButton->adjustSize();
}


void PoleisAutoupdaterDownLoadProgress::__initLayout()
{
    this->progressLayout->addWidget(this->subTitle, 0);
    this->progressLayout->addWidget(this->currentDownloadFileLabel, 1);
    this->progressLayout->addWidget(this->currentDownloadProgressBar, 2);
    this->progressLayout->addWidget(this->totalDownloadProgressLabel, 3);
    this->progressLayout->addWidget(this->totalDownloadProgressBar);

    this->progressLayout->setContentsMargins(10, 10, 10, 10);

    this->vBoxLayout->setSpacing(0);
    this->vBoxLayout->setContentsMargins(2, 2, 2, 2);

    this->vBoxLayout->addLayout(this->progressLayout, 1);
    this->vBoxLayout->addWidget(this->buttonGroup, 0, Qt::AlignBottom);
    this->vBoxLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);


    this->buttonLayout->setSpacing(12);
    this->buttonLayout->setContentsMargins(24, 24, 24, 24);
    this->buttonLayout->addWidget(this->cancelButton, 1, Qt::AlignRight);
}

void PoleisAutoupdaterDownLoadProgress::__onCancelButtonClicked()
{
    this->reject();
    emit(this->cancelSignal());
}

void PoleisAutoupdaterDownLoadProgress::setTitleBarVisible(bool isVisible)
{
    this->windowTitleLabel->setVisible(isVisible);
}


void PoleisAutoupdaterDownLoadProgress::currentDownloadFileName(QString filename)
{
    this->currentDownloadFileLabel->setText(filename);
}

void PoleisAutoupdaterDownLoadProgress::currentDownloadFileSize(long size)
{
    this->currentDownloadProgressBar->setMinimum(0);
    this->currentDownloadProgressBar->setMaximum(size);
}

void PoleisAutoupdaterDownLoadProgress::currentDownloadFileOffset(long offset)
{
    this->currentDownloadProgressBar->setVal(offset);
}

void PoleisAutoupdaterDownLoadProgress::currentDownloadTotalOffset(long offset)
{
    this->totalDownloadProgressBar->setVal(offset);
}

void PoleisAutoupdaterDownLoadProgress::currentDownloadTotalSize(long size)
{
    this->totalDownloadProgressBar->setMinimum(0);
    this->totalDownloadProgressBar->setMaximum(size);
}


void PoleisAutoupdaterDownLoadProgress::downloadError()
{
    this->reject();
}

void PoleisAutoupdaterDownLoadProgress::downloadFinished()
{
    this->accept();
}