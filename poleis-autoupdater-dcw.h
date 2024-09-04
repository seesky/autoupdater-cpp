#pragma once
#include "qfluentwidgets-cpp/qfluentwidgets/components/dialog_box/dialog.h"
#include "qfluentwidgets-cpp/qfluentwidgets/components/widgets/table_view.h"
#include "glib.h"


class PoleisAutoupdaterDownloadConfirm : public FramelessDialog{
    Q_OBJECT
public:
    PoleisAutoupdaterDownloadConfirm(GList *downloadList, QString title, int updateWaitTime, QWidget *parent);
    void _setUpUi(GList *downloadList, QString title, QWidget *parent);
    void __initWidget();
    void __initLayout();
    void __setQss();
    void initTableView();
    void setContentCopyable(bool isCopyable);
    void setTitleBarVisible(bool isVisible);

    QLabel *windowTitleLabel;
    QLabel *timeLabel;
    QFrame *buttonGroup;
    PrimaryPushButton *yesButton;
    QPushButton *cancelButton;
    QVBoxLayout *vBoxLayout;
    QVBoxLayout *tableViewLayout;
    QHBoxLayout *buttonLayout;

    TableWidget *tableView;
    GList *downloadList;
    QTimer* timer;
    int secondsRemaining;

signals:
    void yesSignal();
    void cancelSignal();

public slots:
    void __onCancelButtonClicked();
    void __onYesButtonClicked();
    void updateCountdown();
};