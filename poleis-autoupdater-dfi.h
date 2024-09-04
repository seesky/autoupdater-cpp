#pragma once
#include <string>

class PoleisAutoupdaterDownloadFileInfo{
public:
    PoleisAutoupdaterDownloadFileInfo(std::string url, std::string name, std::string ver, int size, std::string md5);
    std::string getDownloadUrl();
    void setDownloadUrl(std::string url);
    std::string getFileName();
    void setFileName(std::string fileName);
    std::string getMd5();
    void setMd5(std::string md5);
    std::string getLastver();
    void setLastver(std::string lastver);
    int getSize();
    void setSize(int size);

private:
    std::string downloadUrl;
    std::string fileName;
    std::string md5;
    std::string lastver;
    int size;
};