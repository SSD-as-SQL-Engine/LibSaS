#include <iostream>
#include "sas.h"

int main() {

  // create SaSNetwork object (Port : 1990, IP addr: "192.168.1.10")
  sas::SaSNetwork sas_network(1990, "192.168.1.10");

  // connect SaSNetwork using TCP/IP socket
  sas_network.Connect();

  // execute query via network interface 
  sas_network.SendQuery("PRAGMA journal_mode;");

  return 0;
}
