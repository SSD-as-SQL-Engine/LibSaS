#include "sas.h"
#include "sqliteIn.h"
#include <cstring>

namespace sas {

void SaS::open(std::string device) {
  fd = ::open(device.c_str(), O_RDONLY);
  if (fd < 0) {
    std::cerr << "error !!!\n";
  }
  this->device = device;
}

void SaS::close() {
  // (xxx): actually not supported 
  fd = -1;
  device = "";
}

void SaS::executeQuery(std::string query) {
  int err = 0, colCnt = 0;
  char* result;
  SQLITE_RESULT sqliteResult;

#if (SAS_DEBUG == 1)
  std::cerr << "current SQL statement: " << query << std::endl;
#endif

  // (jhpark): make modulization !!!
  // init ioctl buffer
  init_ioctl();

  // execute query
  err = submit_ioctl(fd, IO_NVM_WRITE, query.c_str(), 4096, MAGIC_NUM, 1, NULL);
  if (err !=0) {
    printf("[DEBUG] WRITE ERROR While submitting Query err: %d\n",err);
  }
  stat.num_queries++;

  // print query result (select query)
  if (strstr(query.c_str(), "SELECT") != nullptr) {
    err = submit_ioctl(fd, IO_NVM_READ, NULL, 4096, MAGIC_NUM, 1, &result);
    sqliteResult.result = result;
    printf("%s\n", sqliteResult.result);
  }

#if (SAS_STAT_PRINT == 1)
  // PRINT IO TIME -- see UART print
  err = submit_ioctl(fd, IO_NVM_PRINT_STAT, NULL , 4096, MAGIC_NUM, 0, NULL);
  if (err !=0) {
    std::cerr << "[DEBUG] IO ERROR While printing IO time" << std::endl;
  }
  std::cerr << "[INFO] Total # of query: " << sqliteStat.num_queries << std::endl;
#endif

}

}; // namesapce sas
