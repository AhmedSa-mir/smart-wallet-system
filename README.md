# Smart Wallet System
A simple C++ smart wallet system built on TCP sockets

## Installation

```
mkdir bin
mkdir build
cd build/
cmake ..
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
You can change "127.0.0.1 (Localhost IP)" to make it run on different machines
