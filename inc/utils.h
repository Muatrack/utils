#ifndef _UTILS_H_
#define _UTILS_H_

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdbool.h"
#include <math.h>
#include <sys/time.h>

__attribute__((weak)) uint32_t os_time_escape( uint32_t t_ms, uint32_t *cur_ms ) {
    struct timeval tNow = {0};
    uint32_t time_ms = 0;
    gettimeofday( &tNow, NULL );
    time_ms = tNow.tv_usec / 1000;
    if(*cur_ms){*cur_ms = time_ms; }
    printf("%s.%d[DEFAULT], cur tms:%ld\n", __func__, __LINE__, time_ms);
    return (time_ms-t_ms);
}


#define UTILS_SNSTR "%02X%02X%02X%02X%02X%02X%02X%02X"
#define UTILS_SN2STR(n) n[0],n[1],n[2],n[3],n[4],n[5],n[6],n[7]

#define UTILS_MD5STR "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
#define UTILS_MD52STR(n) n[0],n[1],n[2],n[3],n[4],n[5],n[6],n[7],n[8],n[9],n[10],n[11],n[12],n[13],n[14],n[15]

#define UTILS_MACSTR "%02X%02X%02X%02X%02X%02X"
#define UTILS_MAC2STR(n) n[0],n[1],n[2],n[3],n[4],n[5]

#define UTILS_CHECK_NULL(v,jump,msg) {  \
    if(v==NULL) {   \
        goto jump;  \
    }   \
}

/** 
 * 将寄存器(2字节)，字节顺序颠倒
 * 将输入的寄存器内字节颠倒并且将转换后的结果返回
 * @param x 寄存器(2字节的数据)地址
 */
#define UTILS_REG_SWAP(x) ({    \
    uint16_t _a = 0;    \
    uint8_t *_x = x;    \
    uint8_t _b = _x[0];   \
    _x[0] = _x[1];  \
    _x[1] = _b; \
    _a=*(uint16_t*)x;  \
    _a; \
})

#define UTILS_CHECK_NULL_GOTO(x,e,s)  \
    if(!x){ \
        s;  \
        goto e; \
    }

/**
 * 获取系统时间戳(s) 2000-01-01 00:00:00 至今
 * @return 返回秒数
 */
uint32_t utils_timestamp_base2000_get();

/**
 * 计算 crc16-ccitt 
*/
int utils_crc16_ccitt_calc(void);

/** 
 * 比较sn(字节序列)是否相同 
 * @param srcSn sn [uint8_t[8]]
 * @param desSn sn [uint8_t[8]]
 * @return 相同true, false
 */
bool utils_is_snbytes_equal(uint8_t srcSn[8], uint8_t desSn[8]);

/** 
 * 比较sn(字串)是否相同 
 * @param srcSn sn [char[16]]
 * @param desSn sn [char[16]]
 * @return 相同true, false
 */
bool utils_is_snstr_equal(char srcSn[16], char desSn[16]);

/**
 * 64bit id 初始化
 * id 取值 1-63
 */
#define UTILS_ID64_INIT() (~1UL)
/**
 * 64bi4 id 拿到一个唯一id
 * id 的取值范围 1-63
 * 每次调用后，如果存在可用id，返回一个id值。
 * 否则返回0. 
 * @param x utils_id64_init() 返回的64bits整数
 * @return 存在可用id返回id, 否则返回0
 */
#define UTILS_ID64_TAKE(x) ({  \
    uint8_t _id = 0;    \
    uint64_t val = x & (~(x - 1));    \
    if(val){    \
        _id = log(val)/log(2);  \
        x &= ~val; \
    }   \
    _id; \
})
/**
 * id 取值 1-63
 * 还回一个id值，每次执行utils_id64_take 后，
 * 被取走的id将不会再次返回，除非此id值被还回。
 * 执行此函数后， 此id有机会再次被使用
 * @param b 返回的64bits整数
 * @param x 需要还回的id值
 * @return None
 */
#define UTILS_ID64_GIVE(b,x) (b |= (1LLU << x))

/**
 * id 取值 1-63
 * 取出指定id值，
 * @param b 返回的64bits整数
 * @param x 需要取走的id值
 * @return None
 */
#define UTILS_ID64_SPEC(b,x) (b &= ~(1LLU << x))

#define UTILS_FUNC_LINE_STR  " %s.%d "
#define UTILS_FUNC_LINE_2STR __func__,__LINE__

/** 
 *  计算字节序列的CRC 
 *  @param data 字节序列
 *  @param len 字节序列长度
*/
uint16_t utils_crc_calculate_algorithm(const void *data, const size_t len);

/** 字符串 -> bytes 
 * @param pIn 输入字串
 * @param iLen 输入字串字节长度
 * @param pOut 输出字节数组
 * @param oLen NULL? 输出字节数组元素数量
 * @return 成功true, false
*/
bool utils_str2bytes( char *pIn, size_t iLen, uint8_t *pOut, size_t *oLen );
/** bytes -> 字符串 
 * @param pIn  输入字节数组
 * @param iLen 输入字节长度
 * @param pOut 输出字符串地址
 * @param oLen 输出字符数量-字符串长度
 * @return 成功true, false
*/
bool utils_bytes2str( uint8_t *pIn, size_t iLen, char *pOut, size_t *oLen );

/** 判断一个时间值(ms)与当前时间(ms)的差值是否超过 timout_ms 
 *  如last ts至今时差已超过 timeout_ms， 会更新 last ts
 * @param buf 为了实现重入， 调用者需要提供一个5字节的buf
 * @param vTsAddr 参与比较时间的变量地址
 * @param timout_ms 超时时长(ms)
 * @return 如果差值大于 timeout_ms返回true, 否则false
 */
#define utils_is_time_ms_escaped(buf,vTsAddr,timout_ms) ({    \
    bool *tOut = &buf[0];  \
    *tOut = false;  \
    uint32_t *__cur_ts = (uint32_t*)&buf[1]; \
    *__cur_ts = 0; \
    *tOut = ( os_time_escape( *vTsAddr, __cur_ts ) > timout_ms )?true:false;   \
    if(*tOut) {*vTsAddr = *__cur_ts;} \
    *tOut;   \
})

#define __GET_LINE(x,l) x##l
#define _GET_LINE(x,l) __GET_LINE(x,l)
#define NEW_VAL(x) _GET_LINE(x,__LINE__)
/** 判断一个时间值(ms)与当前时间(ms)的差值是否超过 timout_ms 
 *  
 * 如last ts至今时差已超过 timeout_ms， 会更新 last ts
 * @param vTsAddr   记录时间的变量地址
 * @param timout_ms 超时时长(ms)
 * @return 如果差值大于 timeout_ms返回true, 否则false
 */
#define utils_is_time_ms_escaped_v2(vTsAddr,timout_ms) ({    \
    bool NEW_VAL(_bRet) = false;  \
    uint32_t NEW_VAL(_cur_tms) = 0; \
    NEW_VAL(_bRet) = ( os_time_escape( *vTsAddr, &NEW_VAL(_cur_tms) ) > timout_ms )?true:false;   \
    if(NEW_VAL(_bRet)) {*vTsAddr = NEW_VAL(_cur_tms);} \
    NEW_VAL(_bRet);   \
})

/** 判断一个时间值(ms)与当前时间(ms)的差值是否超过 timout_ms 
 *  如last ts至今时差已超过 timeout_ms， 会更新 last ts
 * @param vTsAddr   记录时间的变量地址
 * @param timout_ms 超时时长(ms)
 * @return 如果差值大于 timeout_ms返回true, 否则false
 */
#define utils_is_time_ms_escaped_v2_without_update(vTsAddr,timout_ms) ({    \
    bool NEW_VAL(_bRet) = false;  \
    uint32_t NEW_VAL(_cur_tms) = 0; \
    NEW_VAL(_bRet) = ( os_time_escape( *vTsAddr, &NEW_VAL(_cur_tms) ) > timout_ms )?true:false;   \
    NEW_VAL(_bRet);   \
})

#define _TZ8_TS    28800
#define _DAY1_TMS_ 86400000 /* 小时 */

/** 
 *  判断一个时间值(ms)与当前时间(ms)的差值是否超过 timout_ms 
 *  如last ts至今时差已超过 timeout_ms， 会更新 last ts
 * @param vTsAddr       uint32_t*   记录时间的变量地址
 * @param timout_ms     uint32_t    期望超时时长(ms)
 * @return 如果差值大于 timeout_ms返回true, 否则false
 */
#define utils_is_time_ms_escaped_in_day(vTsAddr,timeout_ms) ({  \
    bool NEW_VAL(_bRet) = false;  \
    uint32_t NEW_VAL(_cur_tms) = 0; \
    struct timeval NEW_VAL(tNow) = {0}; \
    gettimeofday(&NEW_VAL(tNow), NULL);   \
    NEW_VAL(tNow).tv_sec += _TZ8_TS;    \
    NEW_VAL(_cur_tms) = ( NEW_VAL(tNow).tv_sec*1000 + NEW_VAL(tNow).tv_usec/1000 ) % _DAY1_TMS_;   \
    if(*vTsAddr==0) { *vTsAddr=NEW_VAL(_cur_tms); } \
    /* 当跨天发生后， _cur_tms 从0增长。会发生 cur_tms < refTms。 */   \
    if(NEW_VAL(_cur_tms)<*vTsAddr) { /* 跨天发生 */ \
        if((_DAY1_TMS_+NEW_VAL(_cur_tms)-*vTsAddr)>=timeout_ms) { \
            *vTsAddr=NEW_VAL(_cur_tms); \
            NEW_VAL(_bRet)=true; \
        } \
    }   \
    if ( (int32_t)(NEW_VAL(_cur_tms)-*vTsAddr)>=timeout_ms ){ \
        NEW_VAL(_bRet)=true; \
        *vTsAddr=NEW_VAL(_cur_tms); \
    } \
    NEW_VAL(_bRet);   \
})

/** 判断一个时间值(ms)与当前时间(ms)的差值是否超过 timout_ms 
 *  不会更新last ts
 * @param buf 为了实现重入， 调用者需要提供一个5字节的buf
 * @param vTs 参与比较时间的变量
 * @param timout_ms 超时时长(ms)
 * @return 如果差值大于 timeout_ms返回true, 否则false
 */
#define utils_is_time_ms_escaped_without_update(buf,vTs,tms) ({    \
    bool *tOut = &buf[0];  \
    *tOut = false;  \
    uint32_t *__cur_ts = (uint32_t*)&buf[1]; \
    *__cur_ts = 0; \
    *tOut = ( os_time_escape( vTs, __cur_ts ) > tms )?true:false;   \
    *tOut;   \
})

/**
 * 16进制检查,全长字串全是16进制则成功
 */
#define utils_is_all_char_hex(in,len) ({ \
    bool bRet=true;   \
    for(int i=0;i<len;i++) {    \
        if(in[i]>='a'&&in[i]<='f')continue; \
        if(in[i]>='A'&&in[i]<='F')continue; \
        if(in[i]>='0'&&in[i]<='9')continue; \
        bRet = false;   \
    }  \
    bRet;   \
})

/** 将全部输入字符转为大写，前提所有字符必须a-z|A-Z|0-9 */
#define utils_caps_str(in,len) { \
    for(int i=0;i<len;i++) {    \
        if(in[i]>='a'&&in[i]<='z') {    \
            in[i]=in[i]-32; \
        }   \
    }  \
}


/**
 * 分割字符串
 * @param       pIn        [!NULL]  输入待分割的字串
 * @param       delimiter  [!NULL]  分割字符,以字串形式传入
 * @param       pOutBuf    [!NULL]  输出分割字串的地址，传入可用内存空间
 * @param       outBufLen  [!0]     输出缓冲区的大小
 * @return      成功返回分割子串的数量, 失败返回-1
 */
int utils_split( char *pIn, char *delimiter, char *pOutBuf, size_t outBufLen );

#endif
