#pragma once
#include "glib.h"
#include <gio/gio.h>
#include "poleis-autoupdater-config.h"
#include <QString>

class PoleisAutoUpdater{
public:
    PoleisAutoUpdater();
    bool Update();
    void RollBack();
    bool StartDownload(GList *downloadList);

private:
    PoleisAutoUpdaterConfig *config;
    bool bNeedRestart;
    bool bDownload;
    GList *downloadFileListTemp;

    bool checkUpdateFiles(std::string filename, std::string md5);
    void moveFolderToOld(QString oldPath, QString newPath);
    void startDownload(GList *downloadList); //DownloadFileInfo list
    void clearOld();
    GHashTable *parseRemoteJson(std::string json);

};