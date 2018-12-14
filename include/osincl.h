#ifndef _HEAD_OSINCL_7769794A_4443FF8E_285F0F92_H
#define _HEAD_OSINCL_7769794A_4443FF8E_285F0F92_H

/* Checking the operating system */

#ifndef OS_LINUX
#ifdef __linux
#define OS_LINUX __linux
#endif
#endif

#ifndef OS_SUN
#ifdef __sparc
#define OS_SUN __sparc
#elif defined(__sun)
#define OS_SUN __sun
#endif
#endif

#ifndef OS_WINDOWS
#if defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
//#include <inaddr.h>
#include <WS2TcpIp.h>

#endif
#endif


#if !defined(OS_LINUX) && !defined(OS_SUN) && !defined(OS_WINDOWS)
#error Unknow operating system.
#endif


#if !defined(OS_WINDOWS)

/* For Linux specific system headers. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/vfs.h>
#include <sys/procfs.h>
#include <sys/param.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>  
#include <dirent.h>
#include <dlfcn.h>
#include <strings.h>
#include <termios.h>
#include <netdb.h>
#include <pthread.h>
#ifdef OS_LINUX
//#include <asm/page.h>
#include <linux/unistd.h>
#endif
#include <fstream>
#include <unistd.h>
#ifdef OS_SUN
#include <atomic.h>
#endif

#else //!defined(OS_WINDOWS)
/* For Windows specific system headers. */
#define _CRT_SECURE_NO_DEPRECATE
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>
#include <memory.h>
#include <stdarg.h>
#include <time.h>
#include <crtdbg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <process.h>
#include <windows.h>
#include <fstream>
#include <io.h>
#include <share.h>
#include <direct.h>
#include <assert.h>
#include <errno.h>
#include <tchar.h>
#include <ShellApi.h>


#ifndef NT_INCLUDED
#include <winnt.h>
#endif

#endif

#endif /* #ifndef _HEAD_OSINCL_7769794A_4443FF8E_285F0F92_H */
