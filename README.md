# LibSaS

C/C++ Library for SaS (SQL as SSD Engine) project
See details in our VLDB 2021 Vision Paper

[1] Jong-Hyeok Park, Soyee Choi, Gihwan Oh, and Sang-Won Lee. 2021. [SaS: SSD as SQL database system](https://www.vldb.org/pvldb/vol14/p1481-lee.pdf). 
Proc. VLDB Endow. 14, 9 (May 2021), 1481–1488.

## Prerequisite 

- Cosmos+ Board Hardware

![Cosmos+Board](https://www.crz-tech.com/uploads/uploads/2021/01/29/fnn45201781714615.jpg)

FPGA: Xilinx XC7Z045-3FFG900 Zynq-7000
- Dual ARM Cortex-A9 1GHz Core
- NEON DSP co-processor for each core
- 350K LCs

1GB DDR3
NAND Modules
- Cosmost : CFM-002 (up to 2)
- Cosmos : CFM-001 (only 1)

1 SD CARD Slot

Dual PCIe Gen2 x8 End-Points (Cabled PCIe Interface)

1 Gigabit Ethernet Interface

1 USB 2.0 (configurable to Host or Device)

JTAG and Debug Interface         


## Build

```bash
mkdir build
CC=gcc CXX=g++ cmake .. -DCMAKE_BUILD_TYPE=Release
cd build
```

## How to use `LibSaS` 

```bash
g++ test.c -I {PATH}/LibSaS/include -lLibSaS -L {PATH}/LibSaS/lib/
```

## Run (Test code)

```
cd bin
./TestLibSaS
```

## Usefule Links
- [Cosmos+ Board](https://www.crz-tech.com/crz/article/CosmosPlus/)
- [VLDB Lab](https://sites.google.com/site/skkuvldblab/)




