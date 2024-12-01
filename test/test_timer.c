#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <unistd.h>
#include <time.h>

#define T_INTERVAL_TMS  5000

int main(char argv[], int argc) {
    uint32_t    _lastTms = 0;
    uint8_t     _testCounter = 0;

    printf("[%s] args count:%d\n", __func__, argc);
    
    while( true ) {        
        if( utils_is_time_ms_escaped_in_day( &_lastTms, T_INTERVAL_TMS) == true ) {
            printf("[ UTILS_TMS-RING ] clock ts: %ld \n", time(NULL));
        }

        if( _testCounter++ > 10 ) break;
        usleep(200000);
    }

    printf("[%s] -test done-\n", __func__);
    return 0;
}