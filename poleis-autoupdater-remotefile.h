#pragma once
#include <string>

class PoleisAutoUpdaterRemoteFile{
public:
    PoleisAutoUpdaterRemoteFile(std::string path, std::string url, std::string lastver, std::string md5, int size, bool needRestart);
    std::string getPath();
    void setPath(std::string path);
    std::string getUrl();
    void setUrl(std::string url);
    std::string getLastver();
    void setLastver(std::string lastver);
    int getSize();
    void setSize(int size);
    bool getNeedRestart();
    void setNeedRestart(bool needRestart);
    std::string getMd5();
    void setMd5(std::string md5);

private:
    std::string path;
    std::string url;
    std::string lastver;
    std::string md5;
    int size;
    bool needRestart;
};