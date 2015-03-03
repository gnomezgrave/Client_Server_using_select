# Client_Server
Simple Client-Server program using `select()`

## What is this ?

This C++ program is meant to imitate a simple client-server chat application. However, this is uses non-blocking IO calls, where the program does not hang for network activities. It uses `select()` to get notified if there are available, ready-to-use file descriptors.

## How to run

There are two main programs.

1. Server <br/>
2. Client<br/>

Helper.cpp and Helper.h is shared between both Client and Server. You can compile the files ass follows.

**Server**

``g++ RunServer.cpp Server.cpp Helper.cpp DataCenter.cpp -o server``

**Client**

``g++ Client.cpp Helper.cpp -o client``

To run the server, you need to provide a port. Or else it will use 5000 as the default port.

``./server 8888``

To run the client, you need to provide the host and the port of the server that it needs to connect. Or else it will use `localhost:5000` by default.

``./client 127.0.0.1 8888``

## What does it do?

This program will mimic a simple online store, which the client can send the item name and the amount which he wants to buy. For this intent, server keeps a data store, which was stored in the *data.csv* file on the server side.

For instance, client can request 10 Kg of Dhal from the server as follows.

``$ Dhal 10``

Then a message is sent to the server, and the server checks for the availability of the requested item. If it exists, server sends a message with the total price and reduces it's stock.

Client also can request for the  stock by simply entering `stock` at the terminal.

