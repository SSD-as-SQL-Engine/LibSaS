#include <fcntl.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "sqliteIn.h"


#define QUERY_LEN 4096

SQLITE_STAT sqliteStat;

int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
     printf("%s = %s\t", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void get_header(SQLITE_RESULT * sqliteResult) {
	char * ptr = strtok(sqliteResult->result, "|");
	int colCnt;

	sscanf(ptr, "%d", &colCnt);
	printf("Column Number: %d\n", colCnt);

	sqliteResult->colCnt = colCnt;

	for(int i=0 ; i< colCnt ; i++) {
		ptr = strtok(NULL, "|");
		sqliteResult->azColName[i] = ptr;
	}

	ptr = strtok(NULL, "|");
	sqliteResult->result = ptr;
}

int query_test() {

	// query format:: [columnCount]|[col1Name]|[col2Name]|...|[record 1,1]|[record[1,2]]
	SQLITE_RESULT sqliteResult;
	char * result = (char*) malloc(sizeof(char) * IO_LEN);

	while (1) {
		scanf("%s", result);

		sqliteResult.result = result;

		printf("%s\n", sqliteResult.result);

		get_header(&sqliteResult);
		while(sqliteResult.result) {
			for(int i=0 ; i<sqliteResult.colCnt ; i++) {
				printf("%s\t" , sqlite3_column_text(&sqliteResult, i));
			}
			printf("\n");
		}
	}
}

int sql_wrapper_test() {
	SQLITE_RESULT * sqliteResult;
	int rc = 0;
/*
	sqlite3_prepare(0, NULL, 0, &sqliteResult, NULL);

	rc = sqlite3_step(sqliteResult);
	while (rc == SQLITE_ROW) {
		rc = sqlite3_step(sqliteResult);
	}

	sqlite3_finalize(sqliteResult);
*/
	sqlite3_exec(0, NULL, callback, NULL, NULL);
	return 0;
} 

int run_origin (int fd, FILE * query){
	char buffer[QUERY_LEN];
	int err;

	char *result;
	int colCnt;
	SQLITE_RESULT sqliteResult;

	while(1){
		fgets(buffer, QUERY_LEN, query);
		if(feof(query))
			break;
#ifdef DEBUG		
		printf("[DEBUG]  (%d) %s\n", sqliteStat.num_queries , buffer);
#endif
		err = submit_ioctl(fd, IO_NVM_WRITE, buffer, QUERY_LEN, MAGIC_NUM, 1, NULL);
		if (err !=0) {
			printf("[DEBUG] WRITE ERROR While submitting Query err: %d\n",err);
			return err;
		}
		sqliteStat.num_queries ++;
		
		continue;
	
		// TODO result select test	
		if(buffer[0]=='S'){
			err = submit_ioctl(fd, IO_NVM_READ, NULL, 4096, MAGIC_NUM, 1, &result);
			if (err !=0)
				printf("READ ERROR\n");

			/*printf("result(%ld):: %s\n",strlen(result), result);
			sqliteResult.result = result;
			printf("%s\n", sqliteResult.result);
			//sqliteResult.result = result;
			
			get_header(sqliteResult);
			while (sqliteResult.result)
				parse_result(sqliteResult);
			printf("\n");*/
			//free(result);
		}
		
	}

	// PRINT IO TIME --- (Check UART MSG)	
	err = submit_ioctl(fd, IO_NVM_PRINT_STAT, NULL , 4096, MAGIC_NUM, 0, NULL);
	if (err !=0) {
		printf("[DEBUG] IO ERROR While printing IO time\n");
	}
	printf("total: %d\n", sqliteStat.num_queries);

	return err;
}


int run_bind (int fd, FILE * query) {
	char buffer[QUERY_LEN];
	int err;

	char *result;
	int colCnt;
	SQLITE_RESULT * sqliteResult;

	while(1){
		fgets(buffer, QUERY_LEN, query);
		if(feof(query))
			break;
		//printf("[DEBUG]  (%d) %s\n", sqliteStat.num_queries , buffer);
		if((buffer[0] == 'C' && buffer[1] == 'R') 
			|| (buffer[0] =='b' && buffer[1] == 'e') || (buffer[0] == 'c' && buffer[1] == 'o') 
			|| (buffer[0] == 'I' && buffer[1] == 'N') || (buffer[0] == 'U' && buffer[1] == 'P')
			|| (buffer[0] == 'D' && buffer[3] == 'E') || (buffer[0] == 'S' && buffer[1] == 'E')
			|| (buffer[0] == 'p')) {
			// CREATE , begin, commit, INSERT, UPDATE, DELETE, SELECT, pragma
			err = submit_ioctl(fd, IO_NVM_WRITE, buffer, QUERY_LEN, MAGIC_NUM, 1, NULL);
		} else if (buffer[0] < '0' || buffer[0] > '9') {
			err = submit_ioctl(fd, IO_NVM_WRITE, buffer, QUERY_LEN, MAGIC_NUM, 2, NULL);
		} else {
			err = submit_ioctl(fd, IO_NVM_WRITE, buffer, QUERY_LEN, MAGIC_NUM, 3, NULL);
		}

		if (err !=0) {
			printf("[DEBUG] WRITE ERROR While submitting Query\n");
			return err;
		}
		sqliteStat.num_queries ++;
		
	}

	// PRINT IO TIME --- (Check UART MSG)	
	err = submit_ioctl(fd, IO_NVM_PRINT_STAT, NULL , 4096, MAGIC_NUM, 0, NULL);
	if (err !=0) {
		printf("[DEBUG] IO ERROR While printing IO time\n");
	}
	printf("total: %d\n", sqliteStat.num_queries);

	return err; 
}

int main (int argc, char **argv) {
	int fd, err;
	FILE * query;

	//sql_wrapper_test ();
	
	//query_test();


	sqliteStat.num_queries = 0;
	if(argc < 2) {
		fprintf(stderr, "Useage: %s FILE_NAME\n", argv[0]);
		return 1;
	}

	// open query log 
	query = fopen(argv[1], "r");
	if(!query){
		printf("file open error\n");
		return 0;
	}

	// open device
	err = open ("/dev/nvme1n1", O_RDONLY);
	if(err<0) {
		printf("Permission Error\n");
		return 0;
	}
	fd = err;
	
	init_ioctl();

	if(argc == 2) {
		run_origin (fd, query);
	} else {
		run_bind (fd, query);
	}

	fclose(query);
	return 0;
}

