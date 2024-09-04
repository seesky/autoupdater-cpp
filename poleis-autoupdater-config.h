#pragma once

#include <glib.h>
#include <gio/gio.h>
#include <string>
#include "poleis-autoupdater-localfile.h"
#include "poleis-autoupdater-constfile.h"

class PoleisAutoUpdaterConfig{
public:
    bool getEnabled();
    void setEnabled(bool enabled);
    std::string getServerUrl();
    void setServerUrl(std::string serverUrl);
    GList *getUpdateFileList();
    void setUpdateFileList(GList *updateFileList);

    static PoleisAutoUpdaterConfig *loadConfig(const std::string file);
    void saveConfig(std::string file);
    void addAndEdit(PoleisAutoUpdaterLocalFile *file);

private:
    PoleisAutoUpdaterConfig();
    PoleisAutoUpdaterConfig(const PoleisAutoUpdaterConfig&) = delete;
    PoleisAutoUpdaterConfig& operator=(const PoleisAutoUpdaterConfig&) = delete;
    // static PoleisAutoUpdaterConfig* instance;

    bool enabled;
    std::string serverUrl;
    GList *updateFileList;
};

static PoleisAutoUpdaterConfig *autoUpdaterConfig = PoleisAutoUpdaterConfig::loadConfig(FILENAME);