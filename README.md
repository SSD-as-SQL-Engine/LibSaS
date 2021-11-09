# LibSaS

C/C++ Library for SaS (SQL as SSD Engine) project
See details in our VLDB 2021 Vision Paper

[1] Jong-Hyeok Park, Soyee Choi, Gihwan Oh, and Sang-Won Lee. 2021. [SaS: SSD as SQL database system](https://www.vldb.org/pvldb/vol14/p1481-lee.pdf). 
Proc. VLDB Endow. 14, 9 (May 2021), 1481–1488.

## Prerequisite 

- Cosmos+ Board (SaS firmware)
- 


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





