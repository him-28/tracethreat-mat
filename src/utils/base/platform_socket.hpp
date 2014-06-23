/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef PLATFORM_SOCKET_HPP
#define PLATFORM_SOCKET_HPP

#ifdef _WIN32
#  define GET_SOCKET_ERROR ::WSAGetLastError()
#  define EINPROGRESS WSAEINPROGRESS
#  define EAGAIN WSAEWOULDBLOCK
#  define EINTR WSAEINTR
#  define ECONNRESET WSAECONNRESET
#  define ENOTCONN WSAENOTCONN
#  define ETIMEDOUT WSAETIMEDOUT
#  define EWOULDBLOCK WSAEWOULDBLOCK
#  define EPIPE WSAECONNRESET
#  define NO_SOCKET_CACHING SO_EXCLUSIVEADDRUSE
#  define SOCKET SOCKET
#  define INVALID_SOCKET INVALID_SOCKET
#  define SOCKETPAIR socketpair
#  define FCNTL fcntl
#  define O_NONBLOCK 1
#  define F_GETFL 0
#  define F_SETFL 1
#  define GETTIMEOFDAY gettimeofday
#  define CLOSESOCKET closesocket
#  define GAI_STRERROR gai_strerrorA
#  define SSIZET ptrdiff_t
#  define SNPRINTF _snprintf
#  define SLEEP_SEC sleep
#  define SLEEP_USEC usleep
#  define TIMESPEC timespec
#  define CTIME_R ctime_r
#  define POLL poll
#  if WINVER <= 0x0502 //XP, Server2003
#    define POLLFD  pollfd
#    define POLLIN  0x0300
#    define POLLOUT 0x0010
#  else //Vista, Win7...
#    define POLLFD  pollfd
#    define POLLIN  POLLIN
#    define POLLOUT POLLOUT
#  endif //WINVER
#  define SHUT_RDWR SD_BOTH
#else //not _WIN32
#  include <errno.h>
#  define GET_SOCKET_ERROR errno
#  define EINTR       EINTR
#  define EINPROGRESS EINPROGRESS
#  define ECONNRESET  ECONNRESET
#  define ENOTCONN    ENOTCONN
//#  define ETIMEDOUT   ETIMEDOUT
#  define EWOULDBLOCK EWOULDBLOCK
#  define EAGAIN      EAGAIN
#  define EPIPE       EPIPE
#  define NO_SOCKET_CACHING SO_REUSEADDR
#  define SOCKET int
#  define INVALID_SOCKET (-1)
#  define SOCKETPAIR socketpair
#  define FCNTL fcntl
#  define O_NONBLOCK O_NONBLOCK
#  define F_GETFL F_GETFL
#  define F_SETFL F_SETFL
#  define GETTIMEOFDAY gettimeofday
#  define CLOSESOCKET close
#  define GAI_STRERROR gai_strerror
#  define SSIZET ssize_t
#  define SNPRINTF snprintf
#  define SLEEP_SEC sleep
#  define SLEEP_USEC usleep
#  define TIMESPEC timespec
#  define CTIME_R ctime_r
#  define POLL poll
#  define POLLFD  pollfd
#  define POLLIN  POLLIN
#  define POLLOUT POLLOUT
#  define SHUT_RDWR SHUT_RDWR
#endif

#endif // _TRANSPORT_PLATFORM_SOCKET_H_
