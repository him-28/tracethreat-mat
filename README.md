Tracethreat Malware Analysis Tools Core Framework
=================================================

Integrated Tools for analysis malware in real-time.

Core Features.
==============

* Pthread struct utility synchronizes with OpenCL.

* Scanning with TBB(Multithread and Parallelization tasks) and OpenCL.

* File Structure Support file type scanning type : PE

* Compiler server DB signatures support ClamAV  and Yara-project signature.

* Ansynchronous protocal-buffer communicates between server and client.


Table of content
============

* [Tracethreat-MAT Core Framework] (#tracethreat-mat-core)

* [Tracethreat-MAT Server](#tracethreat-server)

-------------------------------------------------------------------------- 

<a name="tracethreat-mat-core">TraceThreat-MAT Core Framework</a>
* Fast scanning with TBB and OpenCL concept.
* In memory scan infected file.
* Support PE and ELF file type scanning both Linux and Windows client.
* Shared memory send/received message to another module.

<a name="tracethreat-server">TraceThreat-MAT Sever</a>
* Internet Server/Client implement with ASIO boost library.
* Communication with Protocal Buffer(Protobuf).
* Multiple handle user in real-time scanning.
* Tracing message to analysis database.

[Update: 09:11 PM, 14/03/2015]
