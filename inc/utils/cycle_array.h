#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

/** 
 * 循环数组对一个有限长度的线性数组以循环的方式写数据。
 * 循环数组的使用
 *  1 调用函数 cycle_array_init 初始化预设数组
 *  2 调用函数 cycle_array_cell_write 向数组中写入数据
 *    > 调用函数 cycle_array_front_cell_get 获取数组中最后写入的数据单元
 *    > 调用函数 cycle_array_rear_cell_get 获取数组中最老的数据单元
 *    > 函数 cycle_array_valid_count 可获得已写入数据的单元数量， 例如新建数组 [100], 已写入20个数组， 函数返回 20
 */

typedef struct {
    uint8_t _cellSize;          //数组单元大小
    uint8_t _cellCount;         // 数组单元个数, 此值初始化时固定，即便数组清零时此值不变
    int8_t _endPos;            // 有效数据的尾部索引, 空闲位置或者可覆盖位置
    int8_t _beginPos;          // 有效数据的起始索引
    uint8_t *_ptr;              // 数组的地址
}cycle_array_ctx_t;

/** 
 * 初始化
 * @param pCtx 预分配的循环数组实例地址
 * @param buf 预分配的数组地址
 * @param cellSize 数组单元大小
 * @param cellCount 数组数量
 */
bool cycle_array_init( cycle_array_ctx_t *pCtx, uint8_t *buf, size_t cellSize, size_t cellCount );

/**
 * 写数据
 * @param pCtx 循环数组的句柄
 * @param pIn  待写数据的地址
 * @return 返回下一个写数据的单元的地址 (>=0 )
 */
bool cycle_array_cell_write( cycle_array_ctx_t* pCtx, uint8_t *pIn );

/**
 * 获取数组数据的数量
 * @param pCtx 句柄
 * @param 返回有效数据的数量 (>=0)
 */
size_t cycle_array_valid_count( cycle_array_ctx_t* pCtx );

/**
 * 获取新数据的地址
 * @param pCtx 句柄
 * @return 最新数据的地址/NULL
 */
uint8_t* cycle_array_front_cell_get( cycle_array_ctx_t* pCtx );

/**
 * 获取旧数据的地址
 * @param pCtx 句柄
 * @return 最旧数据的地址/NULL
 */
uint8_t* cycle_array_rear_cell_get( cycle_array_ctx_t* pCtx );

#if 0 /* 屏蔽原因，已启用新版 foreach ，由宏定义实现*/
/**
 * 遍历数组
 * @param pCtx 句柄
 * @param pCell 遍历到的元素指针，遍历顺序，从最旧到最新
 * @return 最旧数据的地址/NULL
 */
void cycle_array_foreach( cycle_array_ctx_t* pCtx, void(*cb)(uint8_t* pCell) );
#endif

/**
 * 清空循环数组, 清空后的数组指针不变
 * @param pCtx 循环数组的会话地址
 */
void cycle_array_clear( cycle_array_ctx_t* pCtx );

/** 定义 cycle array 的遍历宏函数 */
#define cycle_array_foreach(pNode,pCtx)\
    for( int16_t _validCount=cycle_array_valid_count((pCtx)), _idx=((pCtx)->_beginPos<0)?0:(pCtx)->_beginPos; *(pNode)=(pCtx)->_ptr + (_idx * (pCtx)->_cellSize), _idx = (_idx + 1)%(pCtx)->_cellCount, _validCount>0; _validCount-- )
