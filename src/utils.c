#include <stdio.h>
#include "utils.h"
#include "time.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "crc16-ccitt-table.h"

uint32_t utils_timestamp_base2000_get() {
    uint32_t _ts = time(NULL);
    return _ts - 946656000; /* 时间戳-2000-01-01 00*/
	// return 771973200; /* 2024-06-17 21:00:00 */
}


uint16_t crc_calculate_algorithm(const void *data, const size_t len) {
	uint8_t *b = (uint8_t *)data;
	size_t i;
	uint16_t crc;

	crc = crc16_ccitt_init();

	/* Update the CRC using the data */
	for (i = 0; i < len; i++) {
		crc = crc16_ccitt_update(b[i], crc);
	}

	crc = crc16_ccitt_finalize(crc);

	return crc;
}



uint16_t crc_calculate_table(const void *data, const size_t len) {
	uint8_t *b = (uint8_t *)data;
	size_t i;
	uint16_t crc;

	crc = crc16_ccitt_init();

	/* Update the CRC using the data */
	for (i = 0; i < len; i++) {
		crc = crc16_ccitt_update(b[i], crc);
	}

	crc = crc16_ccitt_finalize(crc);

	return crc;
}

int utils_crc16_ccitt_calc(void) {
	int i;
	size_t datalen;
	uint16_t crc_algorithm;
	uint16_t crc_table;
	uint16_t crc_check_algorithm;
	uint16_t crc_check_table;
	/* Use the examples from
	 * http://srecord.sourceforge.net/crc16-ccitt.html */
	char *testdata[] = {
		/* Empty string */
		"",
		"A",
		"123456789",
		/* 256 x "A" */
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
		NULL
	};
	uint8_t temp_buffer[300];

	for (i = 0; testdata[i]; i++) {
		printf("Data: '%s'\n", testdata[i]);

		/* Calculate the CRCs */
		datalen = strlen(testdata[i]);
		crc_algorithm = crc_calculate_algorithm(testdata[i], datalen);
		printf("Algorithm-based CRC: 0x%04hX\n", crc_algorithm);
		crc_table = crc_calculate_table(testdata[i], datalen);
		printf("Table-based CRC:     0x%04hX\n", crc_table);

		/* Add the algorithm CRC after the data, note the big endian
		 * byte order */
		memcpy(temp_buffer, testdata[i], datalen);
		temp_buffer[datalen] = crc_algorithm >> 8;
		temp_buffer[datalen + 1] = crc_algorithm & 0xFF;
		/* Re-run the CRC check, but include the CRC */
		crc_check_algorithm = crc_calculate_algorithm(temp_buffer, datalen + 2);
		// printf("Algorithm-based CRC check: 0x%04hX\n", crc_check_algorithm);

		/* Repeat for the table-based CRC function, the first section
		 * of temp_buffer is still intact */
		temp_buffer[datalen] = crc_table >> 8;
		temp_buffer[datalen + 1] = crc_table & 0xFF;
		crc_check_table = crc_calculate_table(temp_buffer, datalen + 2);
		// printf("Table-based CRC check:     0x%04hX\n", crc_check_table);
		// printf("\n");
	}

	return 0;
}

bool utils_is_snbytes_equal(uint8_t srcSn[8], uint8_t desSn[8]) {
	#if 0
	for( int i = 0; i < 8; i ++ ) {
		if( srcSn[i] != desSn[i]) return false;
	}
	return true;
	#endif
	return (memcmp( srcSn, desSn, 8 )==0)?true:false;
}

bool utils_is_snstr_equal(char srcSn[16], char desSn[16]) {
	return (memcmp( srcSn, desSn, 16 )==0)?true:false;
}

/**************** 字符转整数 *************** */

int8_t utils_char2int( char c ) {
	if( (c>='a') && (c<='z') ) return (c-87);  /* a-z */
	if( (c>='A') && (c<='Z') ) return (c-55);  /* A-Z */	
	if( (c>=48) && (c<=57) ) return (c-48);    /* 0-9 */
	return -1;	
}

///////////////// 字符转整数 /////////////// 

/**************** 字符串 <-> HEX数组 *************** */

bool utils_str2bytes( char *pIn, size_t iLen, uint8_t *pOut, size_t *oLen ) {
	int8_t byteVal = 0x0;
	int8_t outLen = iLen/2;
	if( pIn == NULL || iLen < 1 || pOut == NULL || oLen == NULL ) {
		return false;
	}
	if(oLen) *oLen = 0;
	for(int i=0;i<outLen;i++) pOut[i]=0;
	for( int i = 0; i < iLen; i ++ ) {		
		if( byteVal = utils_char2int(pIn[i]), byteVal < 0 ) {
			return false;
		}
		if( i % 2 ) { // low 4bits
			pOut[i/2] |= (byteVal & 0x0F);
			if(oLen) *oLen += 1;
		} else {	  // high 4bits
			pOut[i/2] |= (byteVal << 4);
		}
	}
	return true;
}

bool utils_bytes2str( uint8_t *pIn, size_t iLen, char *pOut, size_t *oLen )
{
	if( pIn == NULL || iLen < 1 || pOut == NULL || oLen == NULL ) return false;
	for( int i = 0; i < iLen; i ++ ) { 
		sprintf( pOut + (2 * i), "%x%x", pIn[i] >> 4, pIn[i] & 0x0F);
		// pOut[ i*2 ] = pIn[i] >> 4;
		// pOut[i*2+1] = pIn[i] & 0x0F;
	}
	*oLen = iLen * 2;
	return true;
}

///////////////// 字符串 <-> HEX数组 /////////////// 

