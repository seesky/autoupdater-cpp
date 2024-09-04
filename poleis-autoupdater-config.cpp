#include "poleis-autoupdater-config.h"
#include "json.hpp"

static PoleisAutoUpdaterConfig* instance = nullptr;

PoleisAutoUpdaterConfig::PoleisAutoUpdaterConfig()
{
    this->enabled = true;
    this->serverUrl = "";
    this->updateFileList = NULL;
}

bool PoleisAutoUpdaterConfig::getEnabled()
{
    return this->enabled;
}

void PoleisAutoUpdaterConfig::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

std::string PoleisAutoUpdaterConfig::getServerUrl()
{
    return this->serverUrl;
}

void PoleisAutoUpdaterConfig::setServerUrl(std::string serverUrl)
{
    this->serverUrl = serverUrl;
}

GList *PoleisAutoUpdaterConfig::getUpdateFileList()
{
    return this->updateFileList;
}

void PoleisAutoUpdaterConfig::setUpdateFileList(GList *updateFileList)
{
    this->updateFileList = updateFileList;
}

PoleisAutoUpdaterConfig *PoleisAutoUpdaterConfig::loadConfig(const std::string file)
{
    if(instance == nullptr)
    {
        instance = new PoleisAutoUpdaterConfig();
        if(g_file_test(file.c_str(), G_FILE_TEST_EXISTS)){
            GError *error = NULL;
            gchar *content;
            gsize length;
            if(g_file_get_contents(file.c_str(), &content, &length, &error))
            {
                nlohmann::json configJson = nlohmann::json::parse(std::string(content));
                instance->setEnabled(configJson["enabled"]);
                instance->setServerUrl(configJson["serverurl"]);
            }else{
                instance->setEnabled(false);
                instance->setServerUrl(std::string());
            }
        }
    }
    return instance;
}

void PoleisAutoUpdaterConfig::saveConfig(std::string file)
{

}

void PoleisAutoUpdaterConfig::addAndEdit(PoleisAutoUpdaterLocalFile *file)
{
    GList *iterator = this->updateFileList;
    while (iterator != NULL) {
        GList *next = iterator->next;
        if(((PoleisAutoUpdaterLocalFile*)iterator)->getPath() == file->getPath())
        {
            this->updateFileList = g_list_remove(this->updateFileList, iterator);
        }
        iterator = next;
    }

    this->updateFileList = g_list_append(this->updateFileList, file);
}