#include "utils/json_compress.h"
#include <string.h>
#include <stdio.h>

bool utils_json_compress( char *pIn, char *pOut, size_t outBufLen ) {
    if( pIn == NULL || pOut == NULL || outBufLen <= strlen(pIn) ) goto excp;
    char c = 0;
    size_t strSize = strlen(pIn);
    if( strSize >= outBufLen ) {
        printf("%s.%d, input str len [%d] max than output buf size[%d]\n", 
                                    __func__, __LINE__, strSize, outBufLen);
        goto excp;
    }
    memset(pOut,0,outBufLen);
    for( int i=0,j=0;i<strSize;i++) {
        c = pIn[i];
        if( c==' ' || c=='\r' || c=='\t' || c=='\n' ) continue;
        pOut[j++] = c;
    }

    return true;
excp:
    return false;
}