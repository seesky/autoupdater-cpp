#include "poleis-autoupdater-localfile.h"

PoleisAutoUpdaterLocalFile::PoleisAutoUpdaterLocalFile(std::string path, std::string ver, int size, std::string md5, std::string updateurl)
{
    this->path = path;
    this->lastver = ver;
    this->size = size;
    this->md5 = md5;
    this->updateurl = updateurl;
}

PoleisAutoUpdaterLocalFile::PoleisAutoUpdaterLocalFile()
{

}

std::string PoleisAutoUpdaterLocalFile::getUpdateUrl()
{
    return this->updateurl;
}

void PoleisAutoUpdaterLocalFile::setUpdateUrl(std::string updateUrl)
{
    this->updateurl = updateurl;
}

std::string PoleisAutoUpdaterLocalFile::getMd5()
{
    return this->md5;
}

void PoleisAutoUpdaterLocalFile::setMd5(std::string md5)
{
    this->md5 = md5;
}

std::string PoleisAutoUpdaterLocalFile::getPath()
{
    return this->path;
}

void PoleisAutoUpdaterLocalFile::setPath(std::string path)
{
    this->path = path;
}

std::string PoleisAutoUpdaterLocalFile::getLastVer()
{
    return this->lastver;
}

void PoleisAutoUpdaterLocalFile::setLastVer(std::string path)
{
    this->path = path;
}

int PoleisAutoUpdaterLocalFile::getSize()
{
    return this->size;
}

void PoleisAutoUpdaterLocalFile::setSize(int size)
{
    this->size = size;
}