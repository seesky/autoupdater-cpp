#pragma once
#include "glib.h"
#include <gio/gio.h>
#include "poleis-autoupdater-config.h"

class PoleisAutoUpdater{
public:
    PoleisAutoUpdater();
    void Update();
    void RollBack();
    void StartDownload(GList *downloadList);

private:
    PoleisAutoUpdaterConfig *config;
    bool bNeedRestart;
    bool bDownload;
    GList *downloadFileListTemp;

    bool checkUpdateFiles(std::string filename, std::string md5);
    void moveFolderToOld(std::string oldPath, std::string newPath);
    void startDownload(GList *downloadList); //DownloadFileInfo list
    void clearOld();
    GHashTable *parseRemoteJson(std::string json);

};