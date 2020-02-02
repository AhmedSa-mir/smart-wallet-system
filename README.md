# Smart Wallet System
A simple QT C++ smart wallet system built on TCP sockets and MySQL

## Dependencies

* MySQL
* QT C++

## Server Installation

```
cd smart-wallet-server/
mkdir bin
mkdir build
cd build/
cmake -DMYSQL_LIBRARY=<path_to_libmysqlclient.so> ..
make
```

## Client Installation

```
cd smart-wallet-client/
qmake
make
```

## Usage

Run server
```
cd bin/
./server <port number>
```

Run client 
```
cd <build dir>
./smart-wallet-client 127.0.0.1 <port number>
```

PS:
You can change "127.0.0.1" (Localhost IP) to make it run on different machines
