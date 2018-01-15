/** 
* @file                              error.h
* @brief                             标准库的错误
* @detail 用于异常处理,这是一个简单的实现,
* @par  依赖        assert.h
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/
#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

#include "except.h"

extern const Except_T EPERM; ///<   1   Operation not permitted 
extern const Except_T ENOENT; ///<   2   No such file or directory
extern const Except_T ESRCH; ///<   3  No such process 
extern const Except_T EINTR; ///<   4   Interrupted system call 
extern const Except_T EIO; ///<   5 I/O error
extern const Except_T ENXIO; ///< 6  No such device or address 
extern const Except_T E2BIG; ///< 7  Arg list too long 
extern const Except_T ENOEXEC; ///< 8  Exec format error 
extern const Except_T EBADF; ///< 9  Bad file number 
extern const Except_Te ECHILD; ///< 10  No child processes 
extern const Except_T EAGAIN; ///< 11  Try again 
extern const Except_T ENOMEM; ///< 12  Out of memory 
extern const Except_T EACCES; ///< 13  Permission denied 
extern const Except_T EFAULT; ///< 14  Bad address 
extern const Except_T ENOTBLK; ///< 15  Block device required 
extern const Except_T EBUSY; ///< 16  Device or resource busy 
extern const Except_T EEXIST; ///< 17  File exists 
extern const Except_T EXDEV; ///< 18  Cross-device link 
extern const Except_T ENODEV; ///< 19  No such device 
extern const Except_T ENOTDIR; ///< 20  Not a directory 
extern const Except_T EISDIR; ///< 21  Is a directory 
extern const Except_T EINVAL; ///< 22  Invalid argument 
extern const Except_T ENFILE; ///< 23  File table overflow 
extern const Except_T EMFILE; ///< 24  Too many open files 
extern const Except_T ENOTTY; ///< 25  Not a typewriter 
extern const Except_T ETXTBSY; ///< 26  Text file busy 
extern const Except_T EFBIG; ///< 27  File too large 
extern const Except_T ENOSPC; ///< 28  No space left on device 
extern const Except_T ESPIPE; ///< 29  Illegal seek 
extern const Except_T EROFS; ///< 30  Read-only file system 
extern const Except_T EMLINK; ///< 31  Too many links 
extern const Except_T EPIPE; ///< 32  Broken pipe 
extern const Except_T EDOM; ///< 33  Math argument out of domain of func 
extern const Except_T ERANGE; ///< 34  Math result not representable 
extern const Except_T EDEADLK; ///< 35  Resource deadlock would occur 
extern const Except_T ENAMETOOLONG; ///< 36  File name too long 
extern const Except_T ENOLCK; ///< 37  No record locks available 
extern const Except_T ENOSYS; ///< 38  Function not implemented 
extern const Except_T ENOTEMPTY; ///< 39  Directory not empty 
extern const Except_T ELOOP; ///< 40  Too many symbolic links encountered 
extern const Except_T EWOULDBLOCK; ///< EAGAIN  Operation would block 
extern const Except_T ENOMSG; ///< 42  No message of desired type 
extern const Except_T EIDRM; ///< 43  Identifier removed 
extern const Except_T ECHRNG; ///< 44  Channel number out of range 
extern const Except_T EL2NSYNC; ///< 45  Level 2 not synchronized 
extern const Except_T EL3HLT; ///< 46  Level 3 halted 
extern const Except_T EL3RST; ///< 47  Level 3 reset 
extern const Except_T ELNRNG; ///< 48  Link number out of range 
extern const Except_T EUNATCH; ///< 49  Protocol driver not attached 
extern const Except_T ENOCSI; ///< 50  No CSI structure available 
extern const Except_T EL2HLT; ///< 51  Level 2 halted 
extern const Except_T EBADE; ///< 52  Invalid exchange 
extern const Except_T EBADR; ///< 53  Invalid request descriptor 
extern const Except_T EXFULL; ///<  54  Exchange full 
extern const Except_T ENOANO; ///< 55  No anode 
extern const Except_T EBADRQC; ///< 56  Invalid request code 
extern const Except_T EBADSLT; ///< 57  Invalid slot 
extern const Except_T EDEADLOCK; ///< EDEADLK
extern const Except_T EBFONT; ///< 59  Bad font file format 
extern const Except_T ENOSTR; ///< 60  Device not a stream 
extern const Except_T ENODATA; ///< 61  No data available 
extern const Except_T ETIME; ///< 62  Timer expired 
extern const Except_T ENOSR; ///< 63  Out of streams resources 
extern const Except_T ENONET; ///< 64  Machine is not on the network 
extern const Except_T ENOPKG; ///< 65  Package not installed 
extern const Except_T EREMOTE; ///<  66  Object is remote 
extern const Except_T ENOLINK; ///< 67  Link has been severed 
extern const Except_T EADV; ///< 68  Advertise error 
extern const Except_T ESRMNT; ///< 69  Srmount error 
extern const Except_T ECOMM; ///< 70  Communication error on send 
extern const Except_T EPROTO; ///< 71  Protocol error 
extern const Except_T EMULTIHOP; ///< 72  Multihop attempted 
extern const Except_T EDOTDOT; ///< 73  RFS specific error 
extern const Except_T EBADMSG; ///< 74  Not a data message 
extern const Except_T EOVERFLOW; ///< 75  Value too large for defined data type 
extern const Except_T ENOTUNIQ; ///< 76  Name not unique on network 
extern const Except_T EBADFD; ///< 77  File descriptor in bad state 
extern const Except_T EREMCHG; ///< 78  Remote address changed 
extern const Except_T ELIBACC; ///< 79  Can not access a needed shared library 
extern const Except_T ELIBBAD; ///< 80  Accessing a corrupted shared library 
extern const Except_T ELIBSCN; ///<  81  .lib section in a.out corrupted 
extern const Except_T ELIBMAX; ///< 82  Attempting to link in too many shared libraries 
extern const Except_T ELIBEXEC; ///< 83  Cannot exec a shared library directly 
extern const Except_T EILSEQ; ///< 84  Illegal byte sequence 
extern const Except_T ERESTART; ///< 85  Interrupted system call should be restarted 
extern const Except_T ESTRPIPE; ///< 86  Streams pipe error 
extern const Except_T EUSERS; ///< 87  Too many users 
extern const Except_T ENOTSOCK; ///< 88  Socket operation on non-socket 
extern const Except_T EDESTADDRREQ; ///< 89  Destination address required 
extern const Except_T EMSGSIZE; ///< 90  Message too long 
extern const Except_T EPROTOTYPE; ///< 91  Protocol wrong type for socket 
extern const Except_T ENOPROTOOPT; ///< 92  Protocol not available 
extern const Except_T EPROTONOSUPPORT; ///< 93  Protocol not supported 
extern const Except_T ESOCKTNOSUPPORT; ///< 94  Socket type not supported 
extern const Except_T EOPNOTSUPP; ///< 95  Operation not supported on transport endpoint 
extern const Except_T EPFNOSUPPORT; ///< 96  Protocol family not supported 
extern const Except_T EAFNOSUPPORT; ///< 97  Address family not supported by protocol 
extern const Except_T EADDRINUSE; ///< 98  Address already in use 
extern const Except_T EADDRNOTAVAIL; ///< 99  Cannot assign requested address 
extern const Except_T ENETDOWN; ///< 100  Network is down 
extern const Except_T ENETUNREACH; ///< 101  Network is unreachable 
extern const Except_T ENETRESET; ///< 102  Network dropped connection because of reset 
extern const Except_T ECONNABORTED; ///< 103  Software caused connection abort 
extern const Except_T ECONNRESET; ///< 104  Connection reset by peer 
extern const Except_T ENOBUFS; ///< 105  No buffer space available 
extern const Except_T EISCONN; ///< 106  Transport endpoint is already connected 
extern const Except_T ENOTCONN; ///< 107  Transport endpoint is not connected 
extern const Except_T ESHUTDOWN; ///< 108  Cannot send after transport endpoint shutdown 
extern const Except_T ETOOMANYREFS; ///< 109  Too many references: cannot splice 
extern const Except_T ETIMEDOUT; ///< 110  Connection timed out 
extern const Except_T ECONNREFUSED; ///< 111  Connection refused 
extern const Except_T EHOSTDOWN; ///< 112  Host is down */
extern const Except_T EHOSTUNREACH; ///< 113  No route to host 
extern const Except_T EALREADY; ///< 114  Operation already in progress 
extern const Except_T EINPROGRESS; ///< 115  Operation now in progress 
extern const Except_T ESTALE; ///< 116  Stale NFS file handle 
extern const Except_T EUCLEAN; ///< 117  Structure needs cleaning 
extern const Except_T ENOTNAM; ///< 118  Not a XENIX named type file 
extern const Except_T ENAVAIL; ///< 119  No XENIX semaphores available 
extern const Except_T EISNAM; ///< 120  Is a named type file 
extern const Except_T EREMOTEIO; ///< 121  Remote I/O error 
extern const Except_T EDQUOT; ///< 122  Quota exceeded 
extern const Except_T ENOMEDIUM; ///< 123  No medium found 
extern const Except_T EMEDIUMTYPE; ///< 124  Wrong medium type 