/*
 * SQLITE API LIKE WRAPPER 함수
 */
#include "sqliteIn.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int sqlite3_open (
  const char *zFilename, 
  sqlite3 **ppDb  
){
	**ppDb = open(zFilename, O_RDONLY); // fd

	if(**ppDb < 0) {
		return SQLITE_CANTOPEN;	
	} else 
		return SQLITE_OK;
}

int sqlite3_close (
  sqlite3 * db
){
	if (*db < 0) {
		return SQLITE_OK;
	} else {
		close(*db);
	}
}

char * columnName (SQLITE_RESULT * sqliteResult){
	char * ptr = sqliteResult -> result;
	char * ret = ptr;
	
	ptr = strtok(NULL, "|");
	sqliteResult->result = ptr;

	return ret;
}

char * sqlite3_column_text (sqlite3_stmt *pStmt, int N) {
	return pStmt->azCols[N];
}

char * sqlite3_column_name (sqlite3_stmt *pStmt, int N) {
	return pStmt->azColName[N];
}

int sqlite3_exec(
  sqlite3* db,
  char *zSql,
  sqlite3_callback xCallback,
  void *pArg,
  char **pzErrMsg  
) {
	int rc ;
	sqlite3_stmt * pStmt =0 ;	
	int nCol;
	char **azVals = 0;
	char **azCols = 0;
	int callbackIsInit ;
	int i ;
	if (zSql == 0) zSql = "" ;

	rc = sqlite3_prepare(db, zSql, -1, &pStmt, NULL);

	if(rc != SQLITE_OK)
		return rc;

	callbackIsInit =0;
	
	while(1) {
		rc = sqlite3_step(pStmt);
		
		if (xCallback && (SQLITE_ROW == rc || SQLITE_DONE == rc)) {
			if(!callbackIsInit) {
				nCol = sqlite3_column_count(pStmt);
				azCols = (char**) malloc((2*nCol+1)*sizeof(const char*));
				if(azCols == 0){
					sqlite3_finalize(pStmt);
					return -1; // TODO: change return value
				}

				for(i=0 ; i<nCol; i++) {
					azCols[i] = (char*) sqlite3_column_name(pStmt , i);
				}

				callbackIsInit = 1;
			}

			azVals = &azCols[nCol];
			for(i=0 ; i<nCol; i++) {
				azVals[i] = (char*)sqlite3_column_text(pStmt, i);
				if(!azVals[i]) {
					sqlite3_finalize(pStmt);
					return -1; //TODO: change return values
				}
			}
			azVals[i] = 0;
		
			if(xCallback(pArg, nCol, azVals, azCols)) {
				printf("ERROR\n");
				sqlite3_finalize(pStmt);
				return -1;
			}

		}

		if (rc != SQLITE_ROW ) {
			break;
		}

	}

	// finalize
	rc = sqlite3_finalize(pStmt);		
	free (azCols);
	return SQLITE_OK; /*SQLITE OK*/
}

int sqlite3_finalize(sqlite3_stmt * pStmt) {
	int rc ;

	if(pStmt == 0)
		return SQLITE_OK;

	free(pStmt);
}


int sqlite3_prepare (
  sqlite3 *db,
  char * zSql,
  int nBytes, 
  sqlite3_stmt ** ppStmt,
  const char ** pzTail  
) {
	char* result = (char*)malloc(sizeof(char) * IO_LEN); // NOTE: submit_ioctl 할때는 필요없음
	
	*ppStmt = (sqlite3_stmt*) malloc (sizeof(sqlite3_stmt));

	(*ppStmt)->colCnt = -1;
	(*ppStmt)->result = NULL;
	//submit_ioctl (*db, IO_NVM_WRITE, zSql, IO_LEN, MAGIC_NUM, 2, &result);
	//scanf("%s", result); // for debugging
	if(result){
		(*ppStmt)->result = result;
		get_header(*ppStmt);
	} else {
		printf("result is NULL\n");
	}

	return SQLITE_OK; /*SQLITE OK*/
}


int sqlite3_column_count(
  sqlite3_stmt * pStmt
) {
	return pStmt->colCnt;
}


int sqlite3_step (
  sqlite3_stmt *pStmt
) {

	for(int i=0 ; i<pStmt->colCnt ; i++){
		pStmt->azCols[i] = columnName(pStmt);
	}	

	if(pStmt->result)
		return SQLITE_ROW;

	return SQLITE_DONE;
}

