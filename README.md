# Smart Wallet System
A simple C++ smart wallet system built on TCP sockets and MySQL

## Dependencies

mysql

```
sudo apt-get install mysql-server-5.7
``` 

## Installation

```
mkdir bin
mkdir build
cd build/
cmake -DMYSQL_LIBRARY=<path_to_libmysqlclient.so> ..
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
cd bin/
./client 127.0.0.1 <port number>
```

PS:
You can change "127.0.0.1" (Localhost IP) to make it run on different machines
