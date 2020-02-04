# Smart Wallet System
A multi-threaded QT C++ smart wallet system. Built on TCP sockets to handle multiple clients and MySQL DB

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

**P.S. Another way** You can create a qt project and add the files in smart-wallet-client dir

## Usage

Run server
```
Create DB and populate it first
mysql -h localhost -u root -p smartwallet < data.sql

cd smart-wallet-server/bin/
./server <port number> localhost <DB_user> <DB_password> smartwallet
```

Run client 
```
cd debug/
./smart-wallet-client 127.0.0.1 <port number>
```

**P.S.** You can change "127.0.0.1" (Localhost IP) to make it run on different machines
