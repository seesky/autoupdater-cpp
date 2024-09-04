#pragma once
#include <string>

class PoleisAutoupdaterEncryptHelper{
public:
    static std::string getFileMD5(std::string filePath);
    static std::string md5Encrypt(std::string beforeStr);
    static std::string hashEncrypt(std::string password);
    static std::string md5HashCodeEncrypt(std::string encryptPwd);
    static std::string hashCodeMD5Encrypt(std::string encryptPwd);
    static std::string hashMD5Encrypt(std::string encryptPwd);
    static std::string md5HashEncrypt(std::string encryptPwd);
};