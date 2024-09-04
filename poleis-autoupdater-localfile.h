#pragma once
#include <string>

class PoleisAutoUpdaterLocalFile{
public:
    PoleisAutoUpdaterLocalFile(std::string path, std::string ver, int size, std::string md5, std::string updateurl);
    PoleisAutoUpdaterLocalFile();

    std::string getUpdateUrl();
    void setUpdateUrl(std::string updateUrl);
    std::string getMd5();
    void setMd5(std::string md5);
    std::string getPath();
    void setPath(std::string path);
    std::string getLastVer();
    void setLastVer(std::string path);
    int getSize();
    void setSize(int size);


private:
    std::string path;
    std::string lastver;
    std::string updateurl;
    std::string md5;
    int size;
};