#include "qfluentwidgets-cpp/qfluentwidgets/components/widgets/button.h"
#include "poleis-autoupdater-dcw.h"
#include "poleis-autoupdater-dfi.h"

PoleisAutoupdaterDownloadConfirm::PoleisAutoupdaterDownloadConfirm(GList *downloadList, QString title, int updateWaitTime, QWidget *parent) : FramelessDialog(parent)
{
    this->secondsRemaining = 5;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PoleisAutoupdaterDownloadConfirm::updateCountdown);

    this->_setUpUi(downloadList, title, this);
    this->windowTitleLabel = new QLabel(title, this);

    this->setResizeEnabled(false);
    this->resize(700, 300);
    this->titleBar->hide();

    this->vBoxLayout->insertWidget(0, this->windowTitleLabel, 0, Qt::AlignTop);
    this->windowTitleLabel->setObjectName(QString("windowTitleLabel"));
    FluentStyleSheet().apply(this, FluentStyleSheetMap.value("DIALOG"), Theme::AUTO);
    this->setFixedSize(this->size());

    
}

void PoleisAutoupdaterDownloadConfirm::_setUpUi(GList *downloadList, QString title, QWidget *parent)
{
    this->downloadList = downloadList;

    this->tableView = new TableWidget(parent);
    // this->tableViewLayout = new QVBoxLayout();

    

    this->buttonGroup = new QFrame(parent);
    this->timeLabel = new QLabel(this->buttonGroup);
    
    this->yesButton = new PrimaryPushButton(this->tr("Update"), this->buttonGroup, nullptr);
    this->cancelButton = new QPushButton(this->tr("Skip"), this->buttonGroup);

    this->vBoxLayout = new QVBoxLayout(parent);
    this->buttonLayout = new QHBoxLayout(this->buttonGroup);

    this->__initWidget();
}


void PoleisAutoupdaterDownloadConfirm::__initWidget()
{
    this->__setQss();
    this->__initLayout();
    this->initTableView();

    this->yesButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    this->cancelButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    this->yesButton->setFocus();
    this->buttonGroup->setFixedHeight(81);

    connect(this->yesButton, &PrimaryPushButton::clicked, this, &PoleisAutoupdaterDownloadConfirm::__onYesButtonClicked);
    connect(this->cancelButton, &QPushButton::clicked, this, &PoleisAutoupdaterDownloadConfirm::__onCancelButtonClicked);

    
}

void PoleisAutoupdaterDownloadConfirm::__setQss()
{
    this->buttonGroup->setObjectName(QString("buttonGroup"));
    this->cancelButton->setObjectName(QString("cancelButton"));

    FluentStyleSheet().apply(this, FluentStyleSheetMap.value("DIALOG"), Theme::AUTO);

    this->yesButton->adjustSize();
    this->cancelButton->adjustSize();
}

void PoleisAutoupdaterDownloadConfirm::__initLayout()
{
    this->vBoxLayout->setSpacing(0);
    this->vBoxLayout->setContentsMargins(2, 2, 2, 2);

    this->vBoxLayout->addWidget(this->tableView, 1);
    this->vBoxLayout->addWidget(this->buttonGroup, 0, Qt::AlignBottom);
    this->vBoxLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);


    this->buttonLayout->setSpacing(12);
    this->buttonLayout->setContentsMargins(24, 24, 24, 24);
    this->buttonLayout->addWidget(this->timeLabel, 1, Qt::AlignLeft);
    this->buttonLayout->addWidget(this->yesButton, 1, Qt::AlignVCenter);
    this->buttonLayout->addWidget(this->cancelButton, 1, Qt::AlignVCenter);
}


void PoleisAutoupdaterDownloadConfirm::__onCancelButtonClicked()
{
    this->reject();
    emit(this->cancelSignal());
}

void PoleisAutoupdaterDownloadConfirm::__onYesButtonClicked()
{
    this->accept();
    emit(this->yesSignal());
}

void PoleisAutoupdaterDownloadConfirm::setTitleBarVisible(bool isVisible)
{
    this->windowTitleLabel->setVisible(isVisible);
}

void PoleisAutoupdaterDownloadConfirm::initTableView()
{
    this->tableView->setBorderVisible(false);
    this->tableView->setWordWrap(false);

    
    
    this->tableView->setRowCount(g_list_length(this->downloadList));
    this->tableView->setColumnCount(4);

    QList<QStringList> fileInfos = QList<QStringList>();
    

    for (GList *df = this->downloadList; df != NULL; df = df->next)
    {
        PoleisAutoupdaterDownloadFileInfo *dfi = (PoleisAutoupdaterDownloadFileInfo*)df->data;
        fileInfos.append(QStringList() << QString::fromStdString(dfi->getFileName()) << QString::fromStdString(dfi->getLastver()) << QString::number(dfi->getSize()) << QString::fromStdString(dfi->getMd5()));
    }

    
    fileInfos += fileInfos;

    for(int i = 0; i < fileInfos.length(); i++){
        for(int n = 0; n < 4; n++){
            this->tableView->setItem(i, n, new QTableWidgetItem(fileInfos.at(i).at(n)));
        }
    }

    this->tableView->verticalHeader()->hide();
    this->tableView->setHorizontalHeaderLabels(QStringList() << this->tr("File Name") << this->tr("File Version") << this->tr("File Size") << this->tr("File DM5"));
    this->tableView->resizeRowsToContents();

    this->timer->start(1000);
}

void PoleisAutoupdaterDownloadConfirm::updateCountdown()
{
    if (secondsRemaining > 0) {
        this->timeLabel->setText(QString("将在%1秒后自动更新！").arg(secondsRemaining));
        secondsRemaining--;
    } else {
        this->timeLabel->setText("倒计时结束！");
        this->yesButton->clicked(true);
        timer->stop(); // 停止定时器
    }
}