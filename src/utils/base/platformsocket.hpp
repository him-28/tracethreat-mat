
#ifndef BASE_TRANSPORT_PLATFORM_SOCKET_H_
#  define BASE_TRANSPORT_PLATFORM_SOCKET_H_

// support base_thread_from thrift
#ifdef _WIN32
#  define BASE_GET_SOCKET_ERROR ::WSAGetLastError()
#  define BASE_EINPROGRESS WSAEINPROGRESS
#  define BASE_EAGAIN WSAEWOULDBLOCK
#  define BASE_EINTR WSAEINTR
#  define BASE_ECONNRESET WSAECONNRESET
#  define BASE_ENOTCONN WSAENOTCONN
#  define BASE_ETIMEDOUT WSAETIMEDOUT
#  define BASE_EWOULDBLOCK WSAEWOULDBLOCK
#  define BASE_EPIPE WSAECONNRESET
#  define BASE_NO_SOCKET_CACHING SO_EXCLUSIVEADDRUSE
#  define BASE_SOCKET SOCKET
#  define BASE_INVALID_SOCKET INVALID_SOCKET
#  define BASE_SOCKETPAIR base_socketpair
#  define BASE_FCNTL base_fcntl
#  define BASE_O_NONBLOCK 1
#  define BASE_F_GETFL 0
#  define BASE_F_SETFL 1
#  define BASE_GETTIMEOFDAY base_gettimeofday
#  define BASE_CLOSESOCKET closesocket
#  define BASE_GAI_STRERROR gai_strerrorA
#  define BASE_SSIZET ptrdiff_t
#  define BASE_SNPRINTF _snprintf
#  define BASE_SLEEP_SEC base_sleep
#  define BASE_SLEEP_USEC base_usleep
#  define BASE_TIMESPEC base_timespec
#  define BASE_CTIME_R base_ctime_r
#  define BASE_POLL base_poll
#  if WINVER <= 0x0502 //XP, Server2003
#    define BASE_POLLFD  base_pollfd
#    define BASE_POLLIN  0x0300
#    define BASE_POLLOUT 0x0010
#  else //Vista, Win7...
#    define BASE_POLLFD  pollfd
#    define BASE_POLLIN  POLLIN
#    define BASE_POLLOUT POLLOUT
#  endif //WINVER
#  define BASE_SHUT_RDWR SD_BOTH
#else //not _WIN32
#  include <errno.h>
#  define BASE_GET_SOCKET_ERROR errno
#  define BASE_EINTR       EINTR
#  define BASE_EINPROGRESS EINPROGRESS
#  define BASE_ECONNRESET  ECONNRESET
#  define BASE_ENOTCONN    ENOTCONN
#  define BASE_ETIMEDOUT   ETIMEDOUT
#  define BASE_EWOULDBLOCK EWOULDBLOCK
#  define BASE_EAGAIN      EAGAIN
#  define BASE_EPIPE       EPIPE
#  define BASE_NO_SOCKET_CACHING SO_REUSEADDR
#  define BASE_SOCKET int
#  define BASE_INVALID_SOCKET (-1)
#  define BASE_SOCKETPAIR socketpair
#  define BASE_FCNTL fcntl
#  define BASE_O_NONBLOCK O_NONBLOCK
#  define BASE_F_GETFL F_GETFL
#  define BASE_F_SETFL F_SETFL
#  define BASE_GETTIMEOFDAY gettimeofday
#  define BASE_CLOSESOCKET close
#  define BASE_GAI_STRERROR gai_strerror
#  define BASE_SSIZET ssize_t
#  define BASE_SNPRINTF snprintf
#  define BASE_SLEEP_SEC sleep
#  define BASE_SLEEP_USEC usleep
#  define BASE_TIMESPEC timespec
#  define BASE_CTIME_R ctime_r
#  define BASE_POLL poll
#  define BASE_POLLFD  pollfd
#  define BASE_POLLIN  POLLIN
#  define BASE_POLLOUT POLLOUT
#  define BASE_SHUT_RDWR SHUT_RDWR
#endif

#endif // BASE_TRANSPORT_PLATFORM_SOCKET_H_
