Tracethreat Malware Analysis Tools Core Framework
=================================================

Integrated Tools for analysis malware in real-time.

Core Features.
==============

* High performance scanning in Thread Building Block and OpenCL mode.

* Multithread synchronous utility synchronizes with OpenCL.

* File structure supports type of file such PE (Plan ELF, Processes in system).

* Asynchronous protocol-buffer message communicates between server and client.


####Table of content
============
#####Overview
* [Tracethreat-MAT Core Framework](#tracethreat-mat-core)

* [Tracethreat-MAT Server](#tracethreat-server)

#####Core Network scanning
* [Server message detail](#server-message-detail)

* [Client message detail](#client-message-detail)
-------------------------------------------------------------------------- 

<a name="tracethreat-mat-core">TraceThreat-MAT Core Framework</a>
* Fast scanning with TBB and OpenCL concept.
* In memory scan infected file.
* API supports PE,ELF and Process.
* Shared memory send/received message to another module.

<a name="tracethreat-server">TraceThreat-MAT Sever</a>
* Internet Server/Client implements with ASIO boost library.
* Communication with Protocol Buffers(Protobuf).
* Multiple handle user in real-time scanning.
* Tracing infected message to analysis on database.

<a name="server-message-detail">Server message detail</a>
* Communication message. 

```c++

package message_scan; 

message RequestScan{
...//Message Request from/to server scan file.
}

message ResponseScan{
...//Message Response from/to server scan file.
}

```
Protocol buffer declares package message object is message_scan which use for
Server/Client communicate detail of infected file scanning. Most details,It's
critical information from client such type, binary detail, status and name of file.
Client not send only detail of file, but send IP and UUID of source to server
because database in cluster threat source in order to separate group of 
infected file.

* Messsage detail on package.

```C++
        required RequestType  type         = 1; 
        required bytes         uuid     = 2; 
        required bytes      timestamp     = 3; 
        optional bytes      key         = 4; 
        optional bytes      iv          = 5; 
        required bytes      ip          = 6; 

        required bytes      conn_ip     = 7; 
        required bytes      conn_uuid   = 8; 
        
        message GetBinaryValue{
            optional bytes binary         = 1;   
            optional bytes file_name      = 2;   
            optional uint64 file_size      = 3;
            optional EncodeType  scan_type = 4; 
            optional FileType    file_type = 5; 
            optional FileStatus  file_status = 6;
            optional bytes       uuid        = 7;
            optional bytes       ip          = 8;
            optional bytes       machine_name = 9;
        }
```

Type of messages are standard communicate between server/client. Programs will not 
send sensitive data to server which message declare above are encrypted.



[Update: 09:11 PM, 14/03/2015]



