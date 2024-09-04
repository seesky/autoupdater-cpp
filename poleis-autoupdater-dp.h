#pragma once
#include "qfluentwidgets-cpp/qfluentwidgets/components/dialog_box/dialog.h"
#include "glib.h"

class PoleisAutoupdaterDownLoadProgress : public FramelessDialog{
    Q_OBJECT
public:
    PoleisAutoupdaterDownLoadProgress(GList*downloadList, QString title, QWidget *parent);
    void _setUpUi(GList *downloadList, QString title, QWidget *parent);
    void __initWidget();
    void __initLayout();
    void __setQss();
    void setContentCopyable(bool isCopyable);
    void setTitleBarVisible(bool isVisible);

private:
    QLabel *windowTitleLabel;
    QFrame *buttonGroup;
    PrimaryPushButton *yesButton;
    QPushButton *cancelButton;
    QVBoxLayout *vBoxLayout;
    QVBoxLayout *progressLayout;
    QHBoxLayout *buttonLayout;

    GList *downloadList;

signals:
    void yesSignal();
    void cancelSignal();

public slots:
    void __onCancelButtonClicked();
    void __onYesButtonClicked();

};