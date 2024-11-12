
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

/**
 *  压缩json字串，减少占用空间
 *  @param  pIn 待压缩的字串
 *  @param  pOut 字串压缩后的输出地址
 *  @param  outBufLen 压缩字串缓冲区的大小
 */
bool utils_json_compress( char *pIn, char *pOut, size_t outBufLen );