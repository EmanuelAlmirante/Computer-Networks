# Computer Networks

This repository hosts the project of the course of Computer Networks.

## Table of Contents

- [Objetives]()
- [How It Works]()
- [Start Server and Client]()
- [Example]()
- [Compilation]()

## Objectives

This repository hosts a mini-project I did for the course of CN in college. It is a very simple project, just to introduce TCP and UDP communication between a client and a server programmed in C. 

I will try to improve this code over time, by adding some new functionalities and refactoring it.

## How It Works

Both the TCP and UDP communication work in the same way. There is a server , UDP or TCP, and a client, also UDP or TCP. The client will send a message to the server, which will display this message, as well as the size in bytes of that message. Then the client will receive an echo from the server, which is the message they sent.

The server stays up until it is stopped, while the client sends a message to the server and disconnects. 

## Start Server and Client

**UDP Server:**
- ./udpserver <port>
  - Example -> ./udpserver 8080
  
**UDP Client:**
- ./udpclient <host> <port>
  - Example -> ./udpclient localhost 8080
  
**TCP Server:**
- ./tcpserver <port>
  - Example -> ./tcpserver 8080
  
**TCP Client:**
- ./tcpclient <host> <port>
  - Example -> ./tcpclient localhost 8080
  
## Example

**Note:** The example is done in a UDP server and client, but is the same concept for the TCP server and client.

**Client:**
Please enter msg: This is a test message.
Echo from server: This is a test message.

**Server:**
server received datagram from localhost (127.0.0.1)
server received 24/24 bytes: This is a test message.

## Compilation

gcc -o udpserver udpserver.c

gcc -o udpclient udpclient.c

gcc -o tcpserver tcpserver.c

gcc -o tcpclient tcpclient.c

**Note:** There are some warnings when compiling, but can be safely ignored.
