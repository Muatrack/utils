#include "utils.h"
#include "utils/url_code.h"

bool url_encode( char *pIn, size_t iLen, char *pOut ) {
    size_t tmpSize = 0;
    char c = '\0';
    if( pIn==NULL||iLen<1||pOut==NULL) goto excp;
    for(int i=0,j=0;i<iLen;i++) {
        c = pIn[i];
        
        if( (c>='a' && c<='z')||
            (c>='A'&&c<='Z')||
            (c>='0'&&c<='9')||
            c=='.'||c=='*'||c=='-'||c=='_'||c=='/'||c=='@'||c=='$'||
            c=='#'||c=='&'||c=='~'||c==';'||c==':'||c=='('||c==')'
        ) {
            pOut[j++] = c;
            continue;
        }
        if( c==' ' ) {
            pOut[j++]='+';
            continue;
        }
        pOut[j++]='%';
        sprintf( pOut+j, "%X%X", c >> 4, c & 0x0F);
        j+=2;
    }
    return true;
excp:
    return false;
}