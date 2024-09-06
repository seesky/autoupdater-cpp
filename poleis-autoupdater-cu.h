#pragma once
#include <string>
#include "poleis-autoupdater-dfi.h"
#include <QWaitCondition>
#include <QMutex>

class PoleisAutoupdaterCommonUnitity{
public:
    PoleisAutoupdaterCommonUnitity();
    static void RestartApplication();
    static std::string getFolderUrl(PoleisAutoupdaterDownloadFileInfo *file);
private:
};


class PoleisAutoupdaterManualResetEvent {
public:

    PoleisAutoupdaterManualResetEvent(bool initialState = false) : signaled(initialState) {
        this->mutex = new QMutex();
    }
    // PoleisAutoupdaterManualResetEvent::PoleisAutoupdaterManualResetEvent(bool initialState = false) : signaled(initialState)
    // {
    //     this->mutex = new QMutex();
    // }

    // 设置事件为 signaled 状态，所有等待的线程都会继续执行
    void set();

    // 重置事件为非 signaled 状态，后续等待线程将阻塞
    void reset();
    
    // 等待直到事件被 set
    void wait();

private:
    QWaitCondition waitCondition;
    QMutex *mutex;
    bool signaled;
};