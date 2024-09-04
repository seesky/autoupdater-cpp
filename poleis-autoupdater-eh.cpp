#include "poleis-autoupdater-eh.h"
#include "glib.h"
#include "gio/gio.h"
#include <sstream>
#include <iomanip>

std::string PoleisAutoupdaterEncryptHelper::getFileMD5(std::string filePath)
{
    GError *error = NULL;
    GFile *file = g_file_new_for_path(filePath.c_str());
    GFileInputStream *input_stream = g_file_read(file, NULL, &error);

    if(error != NULL)
    {
        g_printerr("Error opening file: %s\n", error->message);
        g_error_free(error);
        return "";
    }

    GChecksum *checksum = g_checksum_new(G_CHECKSUM_MD5);

    gsize bytes_read;
    gchar buffer[4096];
    while ((bytes_read = g_input_stream_read(G_INPUT_STREAM(input_stream), buffer, sizeof(buffer), NULL, &error)) > 0)
    {
        g_checksum_update(checksum, (const guchar *)buffer, bytes_read);
    }

    if (error != NULL)
    {
        g_printerr("Error reading file: %s\n", error->message);
        g_error_free(error);
        g_checksum_free(checksum);
        g_object_unref(input_stream);
        g_object_unref(file);
        return "";
    }

    gchar *md5_string = g_strdup(g_checksum_get_string(checksum));

    g_checksum_free(checksum);
    g_object_unref(input_stream);
    g_object_unref(file);

    std::string md5s = std::string(md5_string);
    g_free(md5_string);
    return md5s;
}


std::string PoleisAutoupdaterEncryptHelper::md5Encrypt(std::string beforeStr)
{
    std::string afterString;

    // 创建GChecksum对象用于MD5加密
    GChecksum *checksum = g_checksum_new(G_CHECKSUM_MD5);
    g_checksum_update(checksum, reinterpret_cast<const guchar*>(beforeStr.c_str()), beforeStr.length());

    // 获取哈希值
    const gchar *hash_str = g_checksum_get_string(checksum);

    // 将哈希值转换为小写字母形式的字符串
    afterString = std::string(hash_str);

    // 释放GChecksum对象
    g_checksum_free(checksum);

    return afterString;
}

std::string PoleisAutoupdaterEncryptHelper::hashEncrypt(std::string password)
{
    // 创建GChecksum对象用于MD5加密
    GChecksum *checksum = g_checksum_new(G_CHECKSUM_MD5);
    g_checksum_update(checksum, reinterpret_cast<const guchar*>(password.c_str()), password.length());

    // 获取哈希值
    guint8 hash_bytes[16]; // MD5 produces a 16-byte (128-bit) hash
    gsize digest_len = sizeof(hash_bytes);
    g_checksum_get_digest(checksum, hash_bytes, &digest_len);

    // 将哈希值转换为格式化字符串
    std::ostringstream oss;
    for (int i = 0; i < digest_len; ++i)
    {
        if (i > 0)
            oss << "-";
        oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash_bytes[i]);
    }

    // 释放GChecksum对象
    g_checksum_free(checksum);

    return oss.str();
}

std::string PoleisAutoupdaterEncryptHelper::md5HashCodeEncrypt(std::string encryptPwd)
{
    return md5Encrypt(hashEncrypt(encryptPwd));
}

std::string PoleisAutoupdaterEncryptHelper::hashCodeMD5Encrypt(std::string encryptPwd)
{
    return hashEncrypt(md5Encrypt(encryptPwd));
}

std::string PoleisAutoupdaterEncryptHelper::hashMD5Encrypt(std::string encryptPwd)
{
    return hashCodeMD5Encrypt(hashCodeMD5Encrypt(encryptPwd));
}

std::string PoleisAutoupdaterEncryptHelper::md5HashEncrypt(std::string encryptPwd)
{
    return md5HashCodeEncrypt(md5HashCodeEncrypt(encryptPwd));
}