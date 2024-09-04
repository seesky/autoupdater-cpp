#include "poleis-autoupdater.h"
#include "poleis-autoupdater-constfile.h"
#include "poleis-autoupdater-remotefile.h"
#include "poleis-autoupdater-eh.h"
#include "poleis-autoupdater-dfi.h"
#include "json.hpp"
#include "poleis-autoupdater-dcw.h"

PoleisAutoUpdater::PoleisAutoUpdater()
{
    this->config = autoUpdaterConfig;
}


void PoleisAutoUpdater::Update()
{
    //不启用更新
    if(!this->config->getEnabled())
    {
        return;
    }

    this->clearOld();

    GHashTable *listRemoteFile = this->parseRemoteJson(this->config->getServerUrl());

    GList *downloadList = NULL;

    for (GList *l = config->getUpdateFileList(); l != NULL; l = l->next)
    {
        // if(l != NULL){
            PoleisAutoUpdaterLocalFile *lf = (PoleisAutoUpdaterLocalFile *)l;
            if(!this->checkUpdateFiles(lf->getPath(), lf->getMd5()))
            {
                downloadList = g_list_remove(downloadList, lf);
                downloadList = g_list_append(downloadList, new PoleisAutoupdaterDownloadFileInfo(lf->getUpdateUrl(), lf->getPath(), lf->getLastVer(), lf->getSize(), lf->getMd5()));
                this->bDownload = true;
            }
        // }
    }

    g_list_free_full(config->getUpdateFileList(), g_free);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, listRemoteFile);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {   
        PoleisAutoUpdaterRemoteFile *uf = (PoleisAutoUpdaterRemoteFile*)value;
        config->addAndEdit(new PoleisAutoUpdaterLocalFile(uf->getPath(), uf->getLastver(), uf->getSize(), uf->getMd5(), uf->getUrl()));

        if(!this->checkUpdateFiles(uf->getPath(), uf->getMd5()))
        {
            for (GList *df = downloadList; df != NULL; df = df->next)
            {
                PoleisAutoupdaterDownloadFileInfo *dfi = (PoleisAutoupdaterDownloadFileInfo*)df->data;
                if(dfi->getFileName() == uf->getPath())
                {
                    downloadList = g_list_remove(downloadList, dfi);
                }

                
            }

            downloadList = g_list_append(downloadList, new PoleisAutoupdaterDownloadFileInfo(uf->getUrl(), uf->getPath(), uf->getLastver(), uf->getSize(), uf->getMd5()));
            this->bDownload = true;

            //config.SaveConfig(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, ConstFile.FILENAME)); //暂时保存

            if(uf->getNeedRestart())
                this->bNeedRestart = true;
        }
    }

    downloadFileListTemp = downloadList;

    if(this->bDownload)
    {
        PoleisAutoupdaterDownloadConfirm *dc = new PoleisAutoupdaterDownloadConfirm(downloadList, QString("发现新版本"), 10, nullptr);
        if(dc->exec()){
            dc->close();
            qDebug() << "Yes button is pressed";
        }else{
            dc->close();
            qDebug() << "Cancel button is pressed";
        }
    }
}   

void PoleisAutoUpdater::clearOld()
{
    // 获取当前应用程序的基础目录
    gchar *base_dir = g_strdup(g_get_current_dir());

    // 构建文件匹配模式 "*.old"
    gchar *pattern = g_strdup_printf("%s/*.old", base_dir);

    // 使用 GPatternSpec 进行模式匹配
    GPatternSpec *pspec = g_pattern_spec_new("*.old");
    GDir *dir = g_dir_open(base_dir, 0, NULL);
    const gchar *filename;

    while ((filename = g_dir_read_name(dir)) != NULL)
    {
        // 检查文件是否匹配模式
        if (g_pattern_match_string(pspec, filename))
        {
            gchar *file_path = g_build_filename(base_dir, filename, NULL);

            // 尝试删除文件
            if (remove(file_path) != 0)
            {
                // 错误处理
            }

            g_free(file_path);
        }
    }

    // 清理资源
    g_dir_close(dir);
    g_pattern_spec_free(pspec);
    g_free(pattern);
    g_free(base_dir);
}


GHashTable *PoleisAutoUpdater::parseRemoteJson(std::string json)
{

    GFile *file = g_file_new_for_uri(json.c_str());
    GError *error = NULL;
    gchar *contents;
    gsize length;
    GHashTable *map = g_hash_table_new(g_str_hash, g_str_equal);

    if(g_file_load_contents(file, NULL, &contents, &length, NULL, &error))
    {
        std::string file_content(contents, length);
        nlohmann::json configJson = nlohmann::json::parse(file_content);

        for(int i = 0; i < configJson.size(); i++)
        {
            std::string path = configJson[i]["path"];
            std::string url = configJson[i]["url"];
            std::string lastver = configJson[i]["lastver"];
            int size = configJson[i]["size"];
            std::string md5 = configJson[i]["md5"];
            int needRestart = configJson[i]["needRestart"];

            PoleisAutoUpdaterRemoteFile *f = new PoleisAutoUpdaterRemoteFile(
                path,
                url,
                lastver,
                md5,
                size,
                needRestart
            );

            g_hash_table_insert(map, g_strdup(path.c_str()), f);
        }

        return map;

    }else{
        g_error_free(error);
        return nullptr;
    }

    g_object_unref(file);
    
}   


bool PoleisAutoUpdater::checkUpdateFiles(std::string filename, std::string md5)
{
    return PoleisAutoupdaterEncryptHelper::getFileMD5(filename) == md5 ? true : false;
}


void PoleisAutoUpdater::StartDownload(GList *downloadList)
{
    
}