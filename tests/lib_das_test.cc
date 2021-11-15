#include <iostream>
#include "sas.h"

int main() {

  sas::SaS sas;

  // open database
  sas.open("/dev/nvme1n1");

  // execute query
  sas.executeQuery("PRAGMA journal_mode;");

  return 0;
}
