/**
 * @file    foperate.c
 * @brief   常用文件操作函数
 * @author  niu.tao
 * @version v1.0
 * @date    2010-11-16
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
#include "foperate.h"

/**
 * @brief  创建文件夹
 * @param dir 文件夹名
 * @return errno错误码
 */
int ow_mkdir(const char *dir)
{
    int len;
    struct stat st;
    char *p, *tmp;
    char logdir[512];

    if (dir == NULL)
        return -EINVAL;
    if (access(dir, F_OK | R_OK | W_OK) == 0)
        return 0;
    strncpy(logdir,dir ,512);
    len = strlen(logdir);
    if (len == 0)
        return -1;

    p = logdir + len - 1;
    while (*p == '/')
        *p-- = '\0';

    p = logdir;
    while (*p == '/')
        p++;

    while (p < (logdir + len)) {
        tmp = strchr(p, '/');
        if (tmp)
            *tmp = '\0';
        if (stat(logdir, &st) == -1) {
            if (errno == ENOENT)
                mkdir(logdir, 0755);
            else
                break;
        } else if (!S_ISDIR(st.st_mode)) {
            break;
        }

        if (tmp == NULL)
            break;

        *tmp = '/';
        p = tmp + 1;
    }
    return 0;
}

/**
 * @brief  复制文件
 * @param fsrc 源文件
 * @param fdst 目标文件
 * @return errno错误码
 */
int ow_copy(const char *fsrc, const char *fdst)
{
    unsigned char buffer[4096];
    FILE *fps, *fpd;
    int rv = 0, len;

    if (fsrc == NULL || fdst == NULL) {
        rv = -EINVAL;
        goto out;
    }

    if ((fps = fopen(fsrc, "r")) == NULL) {
        rv = -errno;
        goto out;
    }

    if ((fpd = fopen(fdst, "w")) == NULL) {
        rv = -errno;
        goto out1;
    }
    while (!feof(fps)) {
        len = fread(buffer, 1, sizeof(buffer), fps);
        if (fwrite(buffer, 1, len, fpd) != len) {
            rv = -errno;
            goto out2;
        }
    }
out2:
    fclose(fpd);
out1:
    fclose(fps);
out:
    return rv;
}

/**
 * @brief  复制文件夹
 * @param dsrc 源文件夹
 * @param ddst 目标文件夹
 * @param mode 操作模式:见foperate.h
 * @return errno错误码
 */
int ow_dircopy(const char *dsrc, const char *ddst, int mode)
{
    DIR *dir;
    struct dirent *direntp;
    char fsrc[512], fdst[512];
    int rv = 0;
    
    mode &= OW_DIRCOPY_MASK;
    if (dsrc == NULL || ddst == NULL || mode == 0) {
        rv = -EINVAL;
        goto out;
    }

    if (access(dsrc, F_OK | R_OK) == -1) {
        rv = -errno;
        goto out;
    }
    if ((dir = opendir(dsrc)) == NULL) {
        rv = -errno;
        goto out;
    }
    ow_mkdir(ddst);
    while ((direntp = readdir(dir)) != NULL) {
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
            continue;
        if (direntp->d_type == DT_DIR) {
            snprintf(fsrc, sizeof(fsrc), "%s/%s", dsrc, direntp->d_name);
            snprintf(fdst, sizeof(fdst), "%s/%s", ddst, direntp->d_name);
            ow_mkdir(fdst);
            ow_dircopy(fsrc, fdst, mode);
            continue;
        }
        snprintf(fdst, sizeof(fdst), "%s/%s", ddst, direntp->d_name);
        if ((mode & OW_DIRCOPY_UPDATE) && access(fdst, F_OK) == 0)
            continue;
        snprintf(fsrc, sizeof(fsrc), "%s/%s", dsrc, direntp->d_name);
        ow_copy(fsrc, fdst);
    }
    closedir(dir);
out:
    return rv;
}
/**
 * @brief  移动文件
 * @param fsrc 源文件
 * @param fdst 目标文件
 * @return errno错误码
 */
int ow_move(const char *fsrc, const char *fdst)
{
    int rv;

    if ((rv = ow_copy(fsrc, fdst)) == 0) {
        unlink(fsrc);
    }
    return rv;
}
/**
 * @brief  移动文件
 * @param fsrc 源文件
 * @param fdst 目标文件
 * @return errno错误码
 */
int ow_remove(const char *fname)
{
    if (fname == NULL)
        return -EINVAL;

    if (unlink(fname) == -1)
        return -errno;

    return 0;
}
/**
 * @brief 写一个字符串到文件中(一次写入的字符串的最大长度不能超过4096个字节)
 * @param fname 要写入的文件名
 * @param format 要写入的格式化字符串
 * @param ... 可变参数
 * @return >= 0 写入的字节数\n
 *         <  0 错误码
 */
int ow_fappend(const char *fname, const char *format, ...)
{
    va_list args;
    char buffer[4096];
    FILE *fp;
    int rv;

    if (fname == NULL)
        return -EINVAL;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if ((fp = fopen(fname, "a+")) == NULL)
        return -errno;

    rv = fprintf(fp, "%s", buffer);
    fclose(fp);

    return rv;
}

/**
 * @brief 创建一个空文件(如果文件已经存在，则被清空)
 * @param fname 要创建的文件名
 * @return = 0 创建成功\n
 *         < 0 错误码
 */
int ow_fcreate(const char *fname)
{
    FILE *fp;

    if (fname == NULL)
        return -EINVAL;

    if ((fp = fopen(fname, "w")) == NULL)
        return -errno;

    fclose(fp);

    return 0;
}
/**
 * @brief        读取文本文件的内容
 * @param fname 要读取的文本文件名
 * @param fout   输出文件描述符
 * @return       读取成功，返回读取的总长度\n
 *               失败返回负的错误码
 */
unsigned long ow_fread(const char *fname, int fout)
{
    FILE *fp;
    unsigned long rv = 0;
    char buffer[4096];
    int len;

    if (fname == NULL || fout < 0)
        return -EINVAL;

    if ((fp = fopen(fname, "r")) == NULL) {
        rv = -errno;
        goto out;
    }

    while (!feof(fp)) {
        len = fread(buffer, 1, sizeof(buffer), fp);
        len = write(fout, buffer, len);
        rv += len;
    }

    fclose(fp);
out:
    return rv;
}

/**
 * @brief  从文件中搜索字符串是否存在
 * @param fname 要搜索的文件名
 * @param str 要搜索的字符串
 * @return  存在，返回 0\n
 *          不存在或者文件操作错误，返回负的错误码
 */
int ow_fstrstr(const char *fname, const char *str)
{
    FILE *fp;
    int rv = -1;
    char buffer[4096];
    int len, l = 0;
	size_t r_len = 0;

    if (fname == NULL || str == NULL)
        return -EINVAL;
    
    len = strlen(str);
    if ((fp = fopen(fname, "r")) == NULL) {
        rv = -errno;
        goto out;
    }

    while (!feof(fp)) {
        r_len = fread(buffer + l, 1, sizeof(buffer) - l, fp);
        if (strstr(buffer, str) != NULL) {
            rv = 0;
            break;
        }
        memcpy(buffer, buffer + strlen(buffer) - len, len);
        buffer[len] = '\0';
        l = len;
    }

    fclose(fp);
out:
    return rv;

}
