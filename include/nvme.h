#pragma once

// this will be used as OPCODE 
#include <linux/types.h>


#define IO_NVM_PRINT_STAT	0x80
#define IO_NVM_SQLITE_QUERY	0x81
#define IO_NVM_SQLITE_RESULT	0x82
#define IO_NVM_WRITE		0x01
#define IO_NVM_READ		0x02

#define IO_LEN 4096
#define MAGIC_NUM 2621440

typedef struct _SQLITE_STAT {
	int num_queries;
} SQLITE_STAT;

typedef struct _SQLITE_RESULT {
	int colCnt;
	char * azCols[100];
	char * azColName[100];
	char * result;
} SQLITE_RESULT;

int init_ioctl();
int submit_ioctl(int fd, int opcode, const char* query, __u32 data_len, __u32 lba, int isQuery, char ** result);


// parsing
void get_header (SQLITE_RESULT* sqliteResult);

extern SQLITE_STAT sqliteStat;
