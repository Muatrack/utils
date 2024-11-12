#include "utils/cycle_array.h"
#include "memory.h"

/** 
 * 初始化
 * @param buf 数组地址
 * @param cellSize 数组单元大小
 * @param cellCount 数组数量
 */
bool cycle_array_init( cycle_array_ctx_t *pCtx, uint8_t *buf, size_t cellSize, size_t cellCount ) {
    if( pCtx == NULL || buf == NULL || cellSize < 1 || cellCount < 1 ) goto excp;
    pCtx->_ptr = buf;
    pCtx->_endPos = -1;
    pCtx->_beginPos = -1;
    pCtx->_cellCount = cellCount;
    pCtx->_cellSize = cellSize;
    return true;
excp:
    return false;
}

/**
 * 向数组写数据. 写入原则，先跳再写
 * @param pCtx 循环数组的句柄
 * @return 成功true, false
 */
bool cycle_array_cell_write( cycle_array_ctx_t* pCtx, uint8_t *pIn ) {
    if( pCtx == NULL ) goto excp;

    if( pCtx->_endPos < 0 ) {
        pCtx->_endPos = 0;
        memcpy( pCtx->_ptr + (pCtx->_endPos*pCtx->_cellSize), pIn, pCtx->_cellSize );
        goto done;
    }
    if( pCtx->_endPos >= 0 && pCtx->_beginPos < 0 ) {
            pCtx->_endPos = (pCtx->_endPos + 1)%pCtx->_cellCount;
            memcpy( pCtx->_ptr + (pCtx->_endPos*pCtx->_cellSize), pIn, pCtx->_cellSize );
            pCtx->_beginPos = 0;
            goto done;
    }
    if( pCtx->_endPos > pCtx->_beginPos ) {
        pCtx->_endPos = (pCtx->_endPos + 1)%pCtx->_cellCount;
        if( pCtx->_endPos > pCtx->_beginPos ) {
            memcpy( pCtx->_ptr + (pCtx->_endPos*pCtx->_cellSize), pIn, pCtx->_cellSize );
            goto done;
        }  else {
            pCtx->_beginPos = (pCtx->_beginPos + 1)%pCtx->_cellCount;
            memcpy( pCtx->_ptr + (pCtx->_endPos*pCtx->_cellSize), pIn, pCtx->_cellSize );
            goto done;
        }
    } else {
        pCtx->_beginPos = (pCtx->_beginPos + 1)%pCtx->_cellCount;
        pCtx->_endPos = (pCtx->_endPos + 1)%pCtx->_cellCount;
        memcpy( pCtx->_ptr + (pCtx->_endPos*pCtx->_cellSize), pIn, pCtx->_cellSize );
        goto done;
    }
done:
    return true;
excp:
    return false;
}

/**
 * 获取新数据的地址
 * @param pCtx 句柄
 * @return 最新数据的地址/NULL
 */
uint8_t* cycle_array_front_cell_get( cycle_array_ctx_t* pCtx ) {
    if( pCtx == NULL ) goto excp;
    if( pCtx->_endPos < 0 ) goto excp;
    return pCtx->_ptr + (pCtx->_endPos * pCtx->_cellSize);
excp:
    return NULL;
}

/**
 * 获取旧数据的地址
 * @param pCtx 句柄
 * @return 最旧数据的地址/NULL
 */
uint8_t* cycle_array_rear_cell_get( cycle_array_ctx_t* pCtx ) {
    if( pCtx == NULL ) goto excp;
    if( pCtx->_beginPos < 0 && pCtx->_endPos < 0 ) goto excp;
    if( pCtx->_beginPos < 0 && pCtx->_endPos >= 0 ) return pCtx->_ptr;
    return pCtx->_ptr + (pCtx->_beginPos * pCtx->_cellSize);
excp:
    return NULL;
}

size_t cycle_array_valid_count( cycle_array_ctx_t* pCtx ) {
    if( pCtx == NULL ) goto excp;
    if( pCtx->_beginPos < 0 && pCtx->_endPos < 0 ) goto excp;
    if( pCtx->_beginPos < 0 && pCtx->_endPos >= 0 ) return 1;
    if( pCtx->_endPos > pCtx->_beginPos ) return (pCtx->_endPos - pCtx->_beginPos + 1);
    return pCtx->_cellCount;
excp:
    return 0;
}

#if 0 /* 屏蔽原因，已启用新版 foreach ，由宏定义实现*/
void cycle_array_foreach( cycle_array_ctx_t* pCtx, void(*cb)(uint8_t* pCell) ) {
    int8_t _idx = -1;
    int8_t _validCount = 0;

    if( pCtx == NULL || cb == NULL ) goto excp;
    if( _validCount = cycle_array_valid_count(pCtx), _validCount == 0 ) goto excp;
    // 仅有1个值
    if( pCtx->_beginPos < 0 ) {
         cb( pCtx->_ptr ); 
         return;
    }
    _idx = pCtx->_beginPos;
    for( ; _validCount-- ; ) {
    // while( _idx != pCtx->_beginPos ) {
        cb( pCtx->_ptr + (_idx * pCtx->_cellSize));
        _idx = (_idx + 1)%pCtx->_cellCount;
    }

excp:
    return;
}
#endif

void cycle_array_clear( cycle_array_ctx_t* pCtx ) {
    if(pCtx==NULL) return;
    pCtx->_beginPos = -1;
    pCtx->_endPos = -1;
}