#include "glib.h"
#include "poleis-autoupdater-cu.h"
#include "poleis-autoupdater-constfile.h"


PoleisAutoupdaterCommonUnitity::PoleisAutoupdaterCommonUnitity()
{

}

std::string PoleisAutoupdaterCommonUnitity::getFolderUrl(PoleisAutoupdaterDownloadFileInfo *file)
{

    gchar *folderPathUrl = "";
    
    const gchar *downloadUrl = g_strdup(file->getDownloadUrl().c_str());

    g_warning("%s", downloadUrl);

    const gchar *substring = g_strstr_len(downloadUrl + 15, -1, "/");
    int folderPathPoint = substring - downloadUrl + 1;
    gchar *filepathstring = g_strdup(downloadUrl + folderPathPoint);

    int folderPathPoint1;
    const gchar *substring1 = g_strstr_len(filepathstring, -1, "/");
    if(substring1 == NULL){
        int folderPathPoint1 = 0;
    }else{
        int folderPathPoint1 = substring1 - filepathstring;
    }
    
    gchar *filepathstring1 = g_strdup(filepathstring + folderPathPoint1);

    if (g_strrstr(filepathstring1, "/") != NULL) {
        // 使用 g_strsplit() 按 "/" 分割字符串
        gchar **exeGroup = g_strsplit(filepathstring1, "/", -1);
        int i = 0;

        // 拼接文件夹路径，最后一个元素不包含在内
        while (exeGroup[i] != NULL && exeGroup[i + 1] != NULL) {
            gchar *temp = folderPathUrl;
            folderPathUrl = g_strconcat(folderPathUrl, "\\", exeGroup[i], NULL);
            g_free(temp);
            i++;
        }

        // 构建完整的文件夹路径
        gchar *fullFolderPath = g_strconcat(g_get_current_dir(), TEMPFOLDERNAME, folderPathUrl, NULL);

        // 如果路径不存在，创建路径
        if (!g_file_test(fullFolderPath, G_FILE_TEST_IS_DIR)) {
            g_mkdir_with_parents(fullFolderPath, 0) == 0;  // 创建目录
        }

        g_free(filepathstring);
        g_free(filepathstring1);
        g_free(fullFolderPath);
        g_strfreev(exeGroup);

        
    }

    std::string rstr = std::string(folderPathUrl);
    return rstr;
}


// PoleisAutoupdaterManualResetEvent::PoleisAutoupdaterManualResetEvent(bool initialState = false) : signaled(initialState)
// {
//     this->mutex = new QMutex();
// }


void PoleisAutoupdaterManualResetEvent::set() {
    QMutexLocker locker(mutex);
    signaled = true;
    waitCondition.wakeAll();  // 唤醒所有等待线程
}

void PoleisAutoupdaterManualResetEvent::reset() {
    QMutexLocker locker(mutex);
    signaled = false;
}

void PoleisAutoupdaterManualResetEvent::wait() {
    QMutexLocker locker(mutex);
    while (!signaled) {
        waitCondition.wait(mutex);
    }
}