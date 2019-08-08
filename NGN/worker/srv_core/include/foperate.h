#ifndef __OW_FOPERATE_H__
#define __OW_FOPERATE_H__
/**
 * @brief 文件夹复制操作模式
 * @param OW_DIRCOPY_UPDATE 更新，如果目的文件已经存在则忽略
 * @param OW_DIRCOPY_COVERE 覆盖，覆盖所有文件
 */
enum {
    OW_DIRCOPY_UPDATE = 0x1, 
    OW_DIRCOPY_COVER  = 0x2,
    OW_DIRCOPY_MASK   = 0x3
};
/**
 * @file    foperate.h
 * @brief   常用文件操作函数
 * @author  niu.tao
 * @version v1.0
 * @date    2010-11-16
 */
 
/**
 * @brief  创建文件夹
 * @param dir 文件夹名
 * @return errno错误码
 */
int ow_mkdir(const char *dir);
/**
 * @brief  复制文件
 * @param fsrc 源文件
 * @param fdst 目标文件
 * @return errno错误码
 */
int ow_copy(const char *fsrc, const char *fdst);

/**
 * @brief  复制文件夹
 * @param dsrc 源文件夹
 * @param ddst 目标文件夹
 * @param mode 操作模式:见foperate.h
 * @return errno错误码
 */
int ow_dircopy(const char *dsrc, const char *ddst, int mode);

/**
 * @brief  移动文件
 * @param fsrc 源文件
 * @param fdst 目标文件
 * @return errno错误码
 */
int ow_move(const char *fsrc, const char *fdst);

/**
 * @brief  删除文件
 * @param fname 要删除的文件名
 * @return errno错误码
 */
int ow_remove(const char *fname);
/**
 * @brief 写一个字符串到文件中(一次写入的字符串的最大长度不能超过4096个字节)
 * @param fname 要写入的文件名
 * @param format 要写入的格式化字符串
 * @param ... 可变参数
 * @return >= 0 写入的字节数\n
 *         <  0 错误码
 */
int ow_fappend(const char *fname, const char *format, ...);
/**
 * @brief 创建一个空文件(如果文件已经存在，则被清空)
 * @param fname 要创建的文件名
 * @return = 0 创建成功\n
 *         < 0 错误码
 */
int ow_fcreate(const char *fname);
/**
 * @brief        读取文本文件的内容
 * @param fname  要读取的文本文件名
 * @param fout   输出文件描述符
 * @return       读取成功，返回读取的总长度\n
 *               失败返回负的错误码
 */
unsigned long ow_fread(const char *fname, int fout);
/**
 * @brief  从文件中搜索字符串是否存在
 * @param fname 要搜索的文件名
 * @param str 要搜索的字符串
 * @return  存在，返回 0\n
 *          不存在或者文件操作错误，返回负的错误码
 */
int ow_fstrstr(const char *fname, const char *str);

#endif
