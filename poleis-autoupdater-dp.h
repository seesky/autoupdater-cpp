#pragma once

#include "qfluentwidgets-cpp/qfluentwidgets/components/dialog_box/dialog.h"
#include "qfluentwidgets-cpp/qfluentwidgets/components/widgets/progress_bar.h"
// #include "libsoup/soup.h"
#include "glib.h"
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "poleis-autoupdater-cu.h"


class PoleisAutoupdaterDownloader : public QObject{
    Q_OBJECT
public:
    PoleisAutoupdaterDownloader(GList *downloadList, QObject *parent);
    

private:
    GList *downloadList;
    bool isFinished;
    GList *allFileList;
    long total;
    long nDownloadedTotal;
    PoleisAutoupdaterManualResetEvent *evtPerDonwload;
    QNetworkAccessManager *manager;

    void moveFolderToOld(QString oldPath, QString newPath);

signals:
    void currentDownloadFileName(QString filename);
    void currentDownloadFileSize(long size);
    void currentDownloadFileOffset(long offset);
    void currentDownloadTotalOffset(long offset);
    void currentDownloadTotalSize(long size);
    void downloadError();
    void downloadFinished();

public slots:
    void run();
};

class PoleisAutoupdaterDownLoadProgress : public FramelessDialog{
    Q_OBJECT
public:
    PoleisAutoupdaterDownLoadProgress(GList*downloadList, QString title, QWidget *parent);
    void _setUpUi(QString title, QWidget *parent);
    void __initWidget();
    void __initLayout();
    void __setQss();
    void setContentCopyable(bool isCopyable);
    void setTitleBarVisible(bool isVisible);

private:
    QLabel *windowTitleLabel;
    QFrame *buttonGroup;
    QPushButton *cancelButton;
    QVBoxLayout *vBoxLayout;
    QHBoxLayout *buttonLayout;

    QVBoxLayout *progressLayout;

    QLabel *subTitle;
    QLabel *currentDownloadFileLabel;
    ProgressBar *currentDownloadProgressBar;
    QLabel *totalDownloadProgressLabel;
    ProgressBar *totalDownloadProgressBar;

    GList *downloadFileList;
    

    QThread *downloadThread;
    PoleisAutoupdaterDownloader *downloader;
    

signals:
    void yesSignal();
    void cancelSignal();

public slots:
    void __onCancelButtonClicked();
    void currentDownloadFileName(QString filename);
    void currentDownloadFileSize(long size);
    void currentDownloadFileOffset(long offset);
    void currentDownloadTotalOffset(long offset);
    void currentDownloadTotalSize(long size);

    void downloadError();
    void downloadFinished();
};