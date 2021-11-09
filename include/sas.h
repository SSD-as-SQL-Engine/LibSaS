#pragma once

#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern "C" {
#include "nvme.h"
}

namespace sas {

class SaS {
  private:
     std::string device;
     int fd;
     SQLITE_STAT stat;

  public:
    SaS() = default;
    SaS(std::string device): device(device) {};

    void open(std::string);
    void close();
    void executeQuery(std::string query);
};

}; // namespace sas
