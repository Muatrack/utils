#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <unistd.h>
#include <time.h>

#define T_INTERVAL_TMS  5000

void main() {
    uint32_t    _lastTms = 0;
    printf("[%s]\n", __func__);
    
    while( true ) {        
        if( utils_is_time_ms_escaped_in_day( &_lastTms, T_INTERVAL_TMS) == true ) {
            printf("[ UTILS_TMS-RING ] clock ts: %ld \n", time(NULL));
        }

        usleep(200000);
    }
}