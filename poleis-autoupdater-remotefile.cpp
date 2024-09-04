#include "poleis-autoupdater-remotefile.h"

PoleisAutoUpdaterRemoteFile::PoleisAutoUpdaterRemoteFile(std::string path, std::string url, std::string lastver, std::string md5, int size, bool needRestart)
{
    this->path = path;
    this->url = url;
    this->md5 = md5;
    this->lastver = lastver;
    this->size = size;
    this->needRestart = needRestart;
}

std::string PoleisAutoUpdaterRemoteFile::getPath()
{
    return this->path;
}

void PoleisAutoUpdaterRemoteFile::setPath(std::string path)
{
    this->path = path;
}

std::string PoleisAutoUpdaterRemoteFile::getUrl()
{
    return this->url;
}

void PoleisAutoUpdaterRemoteFile::setUrl(std::string url)
{
    this->url = url;
}

std::string PoleisAutoUpdaterRemoteFile::getLastver()
{
    return this->lastver;
}

void PoleisAutoUpdaterRemoteFile::setLastver(std::string lastver)
{
    this->lastver = lastver;
}

int PoleisAutoUpdaterRemoteFile::getSize()
{
    return this->size;
}

void PoleisAutoUpdaterRemoteFile::setSize(int size)
{
    this->size = size;
}

bool PoleisAutoUpdaterRemoteFile::getNeedRestart()
{   
    return this->needRestart;
}

void PoleisAutoUpdaterRemoteFile::setNeedRestart(bool needRestart)
{
    this->needRestart = needRestart;
}

std::string PoleisAutoUpdaterRemoteFile::getMd5()
{
    return this->md5;
}

void PoleisAutoUpdaterRemoteFile::setMd5(std::string md5)
{
    this->md5 = md5;
}