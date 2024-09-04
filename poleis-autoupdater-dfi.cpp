#include "poleis-autoupdater-dfi.h"


PoleisAutoupdaterDownloadFileInfo::PoleisAutoupdaterDownloadFileInfo(std::string url, std::string name, std::string ver, int size, std::string md5)
{
    this->downloadUrl = url;
    this->fileName = name;
    this->lastver = ver;
    this->size = size;
    this->md5 = md5;
}

std::string PoleisAutoupdaterDownloadFileInfo::getDownloadUrl()
{
    return this->downloadUrl;
}

void PoleisAutoupdaterDownloadFileInfo::setDownloadUrl(std::string url)
{
    this->downloadUrl = url;
}

std::string PoleisAutoupdaterDownloadFileInfo::getFileName()
{
    return this->fileName;
}

void PoleisAutoupdaterDownloadFileInfo::setFileName(std::string fileName)
{
    this->fileName = fileName;
}

std::string PoleisAutoupdaterDownloadFileInfo::getMd5()
{
    return this->md5;
}

void PoleisAutoupdaterDownloadFileInfo::setMd5(std::string md5)
{
    this->md5 = md5;
}

std::string PoleisAutoupdaterDownloadFileInfo::getLastver()
{
    return this->lastver;
}

void PoleisAutoupdaterDownloadFileInfo::setLastver(std::string lastver)
{
    this->lastver = lastver;
}

int PoleisAutoupdaterDownloadFileInfo::getSize()
{
    return this->size;
}

void PoleisAutoupdaterDownloadFileInfo::setSize(int size)
{
    this->size = size;
}