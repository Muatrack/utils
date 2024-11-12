#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * 将中文字符串，进行URL编码
 * @param   pIn 待编码的字符串地址
 * @param   iLen 待编码字符串长度
 * @param   pOut 编码后的字符串输出地址
 * @return  成功true, false
 */
bool url_encode( char *pIn, size_t iLen, char *pOut );