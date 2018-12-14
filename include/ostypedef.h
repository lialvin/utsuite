#ifndef _HEAD_OSTYPEDEF_3A24AE1F_5485D62D_32D4B53D_H
#define _HEAD_OSTYPEDEF_3A24AE1F_5485D62D_32D4B53D_H

#ifndef OS_WINDOWS
#if defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#endif
#endif


#include "osfdecl.h"
#include "osincl.h"

#ifdef OS_WINDOWS
#pragma warning(disable : 4786)
#endif

/* NOTE:
 * The corresponding system header files must be included is prerequisite
 * to make this file function correct.
 */

#if !defined(TRUE)
#define TRUE   1
#endif

#if !defined(FALSE)
#define FALSE  0
#endif

#if !defined(MAX_PATH)
#define MAX_PATH PATH_MAX
#endif

#ifndef mplat_max
#define mplat_max(a, b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef mplat_min
#define mplat_min(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#define LINUX_FILENAME_SEPERATOR       '/'
#define LINUX_FILENAME_SEPERATOR_STR   "/"
#define WIN_FILENAME_SEPERATOR	       '\\'
#define WIN_FILENAME_SEPERATOR_STR     "\\"

#define LINUX_LIB_FILE_SUFFIX_STR		".so"
#define WIN_LIB_FILE_SUFFIX_STR			".dll"

#if !defined(OS_WINDOWS)

/*
 * Sizes for buffers used by the _makepath() and _splitpath() functions.
 * note that the sizes include space for 0-terminator
 */
#ifndef _MAC
#define _MAX_PATH   260 /* max. length of full pathname */
#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */
#else   /* def _MAC */
#define _MAX_PATH   256 /* max. length of full pathname */
#define _MAX_DIR    32  /* max. length of path component */
#define _MAX_FNAME  64  /* max. length of file name component */
#endif  /* _MAC */

#define LIB_FILE_SUFFIX_STR			LINUX_LIB_FILE_SUFFIX_STR
#define FILENAME_SEPERATOR			LINUX_FILENAME_SEPERATOR
#define FILENAME_SEPERATOR_STR		LINUX_FILENAME_SEPERATOR_STR
#define IS_FILENAME_SEPERATOR(x)	((x)==LINUX_FILENAME_SEPERATOR)
#else
#define LIB_FILE_SUFFIX_STR			WIN_LIB_FILE_SUFFIX_STR
#define FILENAME_SEPERATOR			WIN_FILENAME_SEPERATOR
#define FILENAME_SEPERATOR_STR		WIN_FILENAME_SEPERATOR_STR
#define IS_FILENAME_SEPERATOR(x)	((x)==WIN_FILENAME_SEPERATOR||(x)==LINUX_FILENAME_SEPERATOR)
#endif


#if !defined(OS_WINDOWS) || !defined(BASETYPES)

/* These typedefs may or may not be defined even under Windows. It
 * depends on the macro BASETYPES.
 */

#if !defined(BASETYPES)
#define BASETYPES
#endif

#if !defined(OS_LINUX)
typedef int BOOL;
#endif

typedef char           *PSZ;
typedef unsigned char   UCHAR, *PUCHAR;
typedef unsigned short  USHORT, *PUSHORT;
typedef unsigned long   ULONG, *PULONG;

#endif  /* #if !defined(OS_WINDOWS) || !defined(BASETYPES) */


#if !defined(OS_WINDOWS)
typedef long long           LONGLONG;
typedef unsigned long long  ULONGLONG,DDWORD;
#define FILE_BEGIN	SEEK_SET
#define FILE_END	SEEK_END
#define FILE_CURRENT	SEEK_CUR

#define MAXDWORD    0xffffffff

#else

typedef __int64             LONGLONG;
typedef unsigned __int64    ULONGLONG, DDWORD;
typedef LONGLONG            off64_t;
typedef LONGLONG            loff_t;

#endif


#if !defined(OS_WINDOWS)

/* These typedefs are all predefined under Windows. We'd better
 * make a copy of them for other operating systems.
 */

typedef char   CHAR;
typedef short  SHORT;
typedef int    INT;
typedef long   LONG;
typedef long long  LLONG;

typedef unsigned char  BYTE, UBYTE;
typedef unsigned short WORD, UWORD;
typedef unsigned long  DWORD, UDWORD;
typedef unsigned long long  ULLONG;
typedef signed char    UINT8;
typedef signed char    SBYTE;
typedef signed short   SWORD;
typedef signed long    SDWORD;

typedef unsigned int   UINT;
typedef unsigned int   UINT32;
typedef unsigned int  *PUINT;

typedef float   FLOAT, *PFLOAT;

typedef void    VOID, *PVOID;

typedef int     BOOL;

typedef int near       *PINT;
typedef int far        *LPINT;
typedef long far       *LPLONG;
typedef BOOL near      *PBOOL;
typedef BOOL far       *LPBOOL;
typedef BYTE near      *PBYTE;
typedef BYTE far       *LPBYTE;
typedef WORD near      *PWORD;
typedef WORD far       *LPWORD;
typedef DWORD near     *PDWORD;
typedef DWORD far      *LPDWORD;
typedef VOID far       *LPVOID;
typedef const VOID far *LPCVOID;

typedef char        *LPSTR;
typedef const char  *LPCTSTR;
typedef const char  *LPCSTR;

typedef void   *HANDLE;
typedef HANDLE *PHANDLE;
typedef void   *HFILE;
typedef void   *HMODULE;
typedef void   *HINSTANCE;
typedef void   *WINHANDLE;

/* Types used for passing & returning polymorphic values */
typedef long           INT_PTR,  *PINT_PTR;
typedef unsigned long  UINT_PTR, *PUINT_PTR;
typedef long           LONG_PTR, *PLONG_PTR;

typedef UINT_PTR  WPARAM;
typedef LONG_PTR  LPARAM;
typedef LONG_PTR  LRESULT;

#ifdef STRICT
#define DECLARE_HANDLE(name) \
	struct name##__ { \
		int unused; \
	}; \
	typedef struct name##__ *name
#else
#define DECLARE_HANDLE(name) \
	typedef HANDLE name
#endif

//DECLARE_HANDLE(HWND); sybiq 冲突
DECLARE_HANDLE(HHOOK);
DECLARE_HANDLE(HEVENT);

typedef WORD ATOM;

typedef HANDLE NEAR *SPHANDLE;
typedef HANDLE FAR  *LPHANDLE;
typedef HANDLE       HGLOBAL;
typedef HANDLE       HLOCAL;
typedef HANDLE       GLOBALHANDLE;
typedef HANDLE       LOCALHANDLE;
#ifndef _MAC
#ifdef _WIN64
typedef INT_PTR (FAR WINAPI *FARPROC)();
typedef INT_PTR (NEAR WINAPI *NEARPROC)();
typedef INT_PTR (WINAPI *PROC)();
#else
typedef int (FAR WINAPI *FARPROC)();
typedef int (NEAR WINAPI *NEARPROC)();
typedef int (WINAPI *PROC)();
#endif  /* #ifdef _WIN64 */
#else
typedef int (CALLBACK *FARPROC)();
typedef int (CALLBACK *NEARPROC)();
typedef int (CALLBACK *PROC)();
#endif

typedef DWORD  COLORREF, *LPCOLORREF;

#define HFILE_ERROR ((HFILE)-1)

typedef struct tagRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECT, *PRECT, NEAR *NPRECT, FAR *LPRECT;

typedef const RECT FAR *LPCRECT;

typedef struct _RECTL
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECTL, *PRECTL, *LPRECTL;

typedef const RECTL FAR* LPCRECTL;

typedef struct tagPOINT
{
	LONG  x;
	LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;

typedef struct _POINTL
{
	LONG  x;
	LONG  y;
} POINTL, *PPOINTL;

typedef struct tagSIZE
{
	LONG        cx;
	LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef SIZE  SIZEL, *PSIZEL, *LPSIZEL;

typedef struct tagPOINTS
{
#ifndef _MAC
	SHORT   x;
	SHORT   y;
#else
	SHORT   y;
	SHORT   x;
#endif
} POINTS, *PPOINTS, *LPPOINTS;

/* mode selections for the device mode function */
#define DM_UPDATE           1
#define DM_COPY             2
#define DM_PROMPT           4
#define DM_MODIFY           8

#define DM_IN_BUFFER        DM_MODIFY
#define DM_IN_PROMPT        DM_PROMPT
#define DM_OUT_BUFFER       DM_COPY
#define DM_OUT_DEFAULT      DM_UPDATE

/* device capabilities indices */
#define DC_FIELDS           1
#define DC_PAPERS           2
#define DC_PAPERSIZE        3
#define DC_MINEXTENT        4
#define DC_MAXEXTENT        5
#define DC_BINS             6
#define DC_DUPLEX           7
#define DC_SIZE             8
#define DC_EXTRA            9
#define DC_VERSION          10
#define DC_DRIVER           11
#define DC_BINNAMES         12
#define DC_ENUMRESOLUTIONS  13
#define DC_FILEDEPENDENCIES 14
#define DC_TRUETYPE         15
#define DC_PAPERNAMES       16
#define DC_ORIENTATION      17
#define DC_COPIES           18

#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD)(a) & 0xff)) | ((WORD)((BYTE)((DWORD)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD)(w) >> 8))

#define MAKEWPARAM(l, h)      (WPARAM)MAKELONG(l, h)
#define MAKELPARAM(l, h)      (LPARAM)MAKELONG(l, h)
#define MAKELRESULT(l, h)     (LRESULT)MAKELONG(l, h)

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;

    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER {
    struct {
        DWORD LowPart;
        DWORD HighPart;
    };
    struct {
        DWORD LowPart;
        DWORD HighPart;
    } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef ULARGE_INTEGER *PULARGE_INTEGER;


#endif /* #if !defined(OS_WINDOWS) */


#define MAKELONGLONG(lowlong,uplong)	((((LONGLONG)(uplong))<<32)|((LONGLONG)(lowlong)))
#ifndef HIDWORD
#define HIDWORD(v)  			((DWORD)((v)>>32))
#endif
#define LODWORD(v)  			((DWORD)(v))
#define HILONG(ll)			((long)HIDWORD(ll))
#define LOLONG(ll)			LODWORD(ll)


#if !defined(OS_WINDOWS)

typedef pthread_mutex_t   	CRITICAL_SECTION_T;
typedef pthread_t			THREADT;
typedef int               	FILE_ID;
#define ZCTT_INVALID_HANDLE	(-1)

#else

typedef CRITICAL_SECTION  	CRITICAL_SECTION_T;
typedef DWORD				THREADT;
typedef HANDLE             	FILE_ID;
#define ZCTT_INVALID_HANDLE	INVALID_HANDLE_VALUE

#endif

typedef void *LOCATION;

#if defined(OS_WINDOWS)
#define _MAX_ULONGLONG  0xffffffffffffffff
#define _MIN_ULONGLONG  0x0
#else
#define _MAX_ULONGLONG  0xffffffffffffffffull
#define _MIN_ULONGLONG  0x0ull
#endif
#define _MAX_DWORD      0xffffffff
#define _MIN_DWORD      0x0
#define _MAX_WORD	0xffff
#define _MIN_WORD	0x0
#define _MAX_SHORT	0x7fff
#define _MIN_SHORT	(-0x7fff)
#define _MAX_LONG       0x7fffffff
#define _MIN_LONG       (-0x7fffffff)
#define _MAX_INT        0x7fffffff
#define _MIN_INT        (-0x7fffffff)

#ifndef _HCFG_PROJECT_DEFINE
#define _HCFG_PROJECT_DEFINE
typedef void * HCFG_PROJECT;
#endif//_HCFG_PROJECT_DEFINE

#ifndef _HMAP_PROJECT_DEFINE
#define _HMAP_PROJECT_DEFINE
typedef void * HMAP_PROJECT;
#endif//_HMAP_PROJECT_DEFINE

#ifndef _HMAP_FINDER_DEFINE
#define _HMAP_FINDER_DEFINE
typedef void * HMAP_FINDER;
#endif//_HMAP_FINDER_DEFINE


//.........
enum{
	MAC_ADDRESS_LEN = 6,
	IP_ADDRESS_LEN = 4,
	IPV6_ADDRESS_LEN = 16,
	IPX_ADDRESS_LEN = 10,

	MAC_SDADDRESS_LEN = MAC_ADDRESS_LEN*2,
	IP_SDADDRESS_LEN = IP_ADDRESS_LEN*2,
	IPV6_SDADDRESS_LEN = IPV6_ADDRESS_LEN*2,
	IPX_SDADDRESS_LEN = IPX_ADDRESS_LEN*2,

	MAC_ADDRESS_LEN_EX = 8,
	MAC_SDADDRESS_LEN_EX = MAC_ADDRESS_LEN_EX*2,
	IP_ADDRESS_LEN_EX = 4,
	IP_SDADDRESS_LEN_EX = IP_ADDRESS_LEN_EX*2,

	IPV6_ADDRESS_LEN_EX = 16,
	IPV6_SDADDRESS_LEN_EX = IPV6_ADDRESS_LEN_EX*2,

	IPX_ADDRESS_LEN_EX = 12,
	IPX_SDADDRESS_LEN_EX = IPX_ADDRESS_LEN_EX*2,

	PROTO_ADDRESS_MIN_LEN = 4,    //...........
	PROTO_SDADDRESS_MIN_LEN = 8,  //.............
	PROTO_ADDRESS_MAX_LEN = 12,   //............
	PROTO_SDADDRESS_MAX_LEN = PROTO_ADDRESS_MAX_LEN*2,	//.............

	ADDRESS_MAX_BYTES = PROTO_SDADDRESS_MAX_LEN,
};

#define MAX_PROTOOBJ_LEN	256
#define MAX_STACKLEVEL		256

typedef ULONGLONG		 MACADDR64;
typedef MACADDR64		 SDMACADDR64[2];
typedef SDMACADDR64		 DSMACADDR64;
typedef char			 MACADDR[MAC_ADDRESS_LEN_EX];
typedef MACADDR			 SDMACADDR[2];
typedef SDMACADDR		 DSMACADDR;
typedef DWORD			 IPv4ADDR;
typedef DWORD			 SDIPv4ADDR[2];
class IPv6ADDR;
typedef IPv6ADDR		 SDIPv6ADDR[2];
typedef IPv4ADDR		 IPADDR;
typedef SDIPv4ADDR		 SDIPADDR;
typedef SDIPv4ADDR		 DSIPv4ADDR;
typedef char			 IPXADDR[IPX_ADDRESS_LEN_EX];
typedef IPXADDR			 SDIPXADDR[2];
typedef SDIPXADDR		 DSIPXADDR;

typedef union
{
	BYTE byte[4];
	UINT value;
} ZCUINTSTR;

typedef union
{
	BYTE byte[2];
	WORD value;
} ZCWORDSTR;

typedef union _ULARGE_INT
{
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	} PART;
	ULONGLONG QuadPart;
} ULARGE_INT, *PULARGE_INT;

typedef union _LARGE_INT
{
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	} Part;
	LONGLONG QuadPart;
} LARGE_INT, *PLARGE_INT;

struct _OCTETS
{
	BYTE * stream;
	UINT   datalen;
	UINT   buflen;
	BOOL   dynamic;
};
typedef struct _OCTETS OCTETS;

typedef DWORD IPADDR;
typedef DWORD PORTID;

/* Address */
typedef struct _SOCKET_T
{
	IPADDR addr;
	PORTID port;
} SOCKET_T;

/* TCP Connection */
typedef struct _SOCKET_PAIR_T
{
	SOCKET_T sock1;	/* src, client */
	SOCKET_T sock2;	/* dst, server */
} SOCKET_PAIR_T;

/* Address pair */
typedef struct _ADDR_PAIR_T
{
	IPADDR addr1;
	IPADDR addr2;
} ADDR_PAIR_T;

/* Port pair */
typedef struct _PORT_PAIR_T
{
	PORTID port1;
	PORTID port2;
} PORT_PAIR_T;

#if defined(OS_LINUX)
typedef DWORD	TICK;
#endif
typedef UINT	TIME;
typedef int		POS;

/* Statistics */
typedef struct tagSTAT_SI_ITEM
{
	DDWORD ddFrame;
	DDWORD ddByte;

	tagSTAT_SI_ITEM()
	{
		ddFrame = 0;
		ddByte = 0;
	}
	void ComeAPacket(DWORD dwLen)
	{ ++ddFrame; ddByte += (DDWORD)dwLen;	}

} STAT_SI_ITEM;

typedef struct tagSTAT_BI_ITEM
{
	DDWORD ddSendFrame;
	DDWORD ddSendByte;
	DDWORD ddRecvFrame;
	DDWORD ddRecvByte;

	tagSTAT_BI_ITEM()
	{
		ddSendFrame = 0;
		ddSendByte = 0;
		ddRecvFrame = 0;
		ddRecvByte = 0;
	}

	void RecvAPacket(DWORD dwLen)
	{ ++ddRecvFrame; ddRecvByte += (DDWORD)dwLen;	}
	void SendAPacket(DWORD dwLen)
	{ ++ddSendFrame; ddSendByte += (DDWORD)dwLen;	}

	DDWORD TotalFrames() const
	{	return (ddSendFrame+ddRecvFrame);	}

	DDWORD TotalBytes() const
	{	return (ddSendByte+ddRecvByte);	}

	void CopyTo(tagSTAT_BI_ITEM *pDesc)
	{
		memcpy(pDesc, this, sizeof(*this));
	}
} STAT_BI_ITEM;

typedef struct
{
	DDWORD ddFrame;
} STAT_SI_FRAME;

typedef struct _STAT_BI_FRAME
{
	DDWORD ddSendFrame;
	DDWORD ddRecvFrame;

	_STAT_BI_FRAME()
	{
		ddSendFrame = 0;
		ddRecvFrame = 0;
	}

	DDWORD TotalFrames() const
	{	return (ddSendFrame+ddRecvFrame);	}

} STAT_BI_FRAME;

#define DEFAULT_HISTORY_INTERVAL 5

typedef union {
	LONGLONG			int64_Data;
	ULONGLONG			uint64_Data;
	int					int_Data;
	unsigned int		uint_Data;
	short				short_Data;
	unsigned short		ushort_Data;
	char				char_Data;
	unsigned char		uchar_Data;
} DATAUNION;

typedef enum {
	DATATYPE_INT64 = -81,
	DATATYPE_UINT64 = -82,
	DATATYPE_INT = -41,
	DATATYPE_UINT = -42,
	DATATYPE_SHORT = -21,
	DATATYPE_USHORT = -22,
	DATATYPE_CHAR = -11,
	DATATYPE_UCHAR = -12,
} DATATYPE;

#define IS_SAME_ADDR(addr1, addr2) \
	(((addr1) == 0) || ((addr1) == (addr2)))
#define IS_SAME_SRC2DST_ADDR(peer, sock) \
	(IS_SAME_ADDR((peer).srcAddr, (sock).sock1.addr) && \
	 IS_SAME_ADDR((peer).dstAddr, (sock).sock2.addr))
#define IS_SAME_DST2SRC_ADDR(peer, sock) \
	(IS_SAME_ADDR((peer).srcAddr, (sock).sock2.addr) && \
	 IS_SAME_ADDR((peer).dstAddr, (sock).sock1.addr))
#define IS_SAME_DOUBLE_ADDR(peer, sock) \
	(IS_SAME_SRC2DST_ADDR(peer, sock) || IS_SAME_DST2SRC_ADDR(peer, sock))

#define IS_SAME_PORT(port1, port2) \
	(((port1) == 0) || ((port1) == (port2)))
#define IS_SAME_SRC2DST_PORT(peer, sock) \
	(IS_SAME_PORT((peer).srcPort, (sock).sock1.port) && \
	 IS_SAME_ADDR((peer).dstPort, (sock).sock2.port))
#define IS_SAME_DST2SRC_PORT(peer, sock) \
	(IS_SAME_PORT((peer).srcPort, (sock).sock2.port) && \
	 IS_SAME_ADDR((peer).dstPort, (sock).sock1.port))
#define IS_SAME_DOUBLE_PORT(peer, sock) \
	(IS_SAME_SRC2DST_PORT(peer, sock) || IS_SAME_DST2SRC_PORT(peer, sock))

#define IsSameSocket(sock1, sock2) \
	(((sock1).addr == (sock2).addr) && ((sock1).port == (sock2).port))
#define IsSameSockPair12(sockpair1, sockpair2) \
	(IsSameSocket((sockpair1).sock1, (sockpair2).sock1) && \
	 IsSameSocket((sockpair1).sock2, (sockpair2).sock2))
#define IsSameSockPair21(sockpair1, sockpair2) \
	(IsSameSocket((sockpair1).sock1, (sockpair2).sock2) && \
	 IsSameSocket((sockpair1).sock2, (sockpair2).sock1))
#define IsSameSockPair(sockpair1, sockpair2) \
	(IsSameSockPair12((sockpair1), (sockpair2)) || \
	 IsSameSockPair21((sockpair1), (sockpair2)))

// 接口类型定义：定时器回调函数
typedef void (*LPFN_OnTimer)(DWORD dwTimerID,void * pParam);
typedef DWORD (*LPFN_RegisterTimer)(long ltimer,LPFN_OnTimer fnOnTimer,void * pParam);
typedef BOOL (*LPFN_UnRegisterTimer)(DWORD dwTimerId);
typedef void (*LPFN_SetTimerProc)(LPFN_RegisterTimer lpfnRegister,LPFN_UnRegisterTimer lpfnUnRegisterTimer);


#ifdef NET_ORDER_CPU

#define NTOHS(w)              (w)
#define HTONS(w)              NTOHS(w)

#define NTOHL(l)              (l)
#define HTONL(l)              NTOHL(l)

#define NTOHLL(ll)            (ll)
#define HTONLL(ll)            NTOHLL(ll)

#define BUF_MAKEWORD(p)       (*(WORD *)p)
#define BUF_MAKEDWORD(p)      (*(DWORD *)p)
#define BUF_MAKEULONGLONG(p)  (*(ULONGLONG *)p)

#else

#define NTOHS(w)              (WORD)((((DWORD)(w) & 0x00ff) << 8) | (((DWORD)(w) & 0xff00) >> 8))
#define HTONS(w)              NTOHS(w)

#define NTOHL(l)              ((((l) & 0x000000ff) << 24) | (((l) & 0x0000ff00) << 8) | (((l) & 0x00ff0000) >> 8) | (((l) & 0xff000000) >> 24))
#define HTONL(l)              NTOHL(l)

#define NTOHLL(ll)            ((ULONGLONG)(NTOHL((DWORD)(((ll) & 0xffffffff00000000) >> 32))) | (((ULONGLONG)NTOHL((DWORD)((ll) & 0x00000000ffffffff))) << 32))
#define HTONLL(ll)            NTOHLL(ll)

#define BUF_MAKEWORD(p)       MAKEWORD(((BYTE *)(p))[1], ((BYTE *)(p))[0])
#define BUF_MAKEDWORD(p)      ((((DWORD)((BYTE *)(p))[0]) << 24) | (((DWORD)((BYTE *)(p))[1]) << 16) | (((DWORD)((BYTE *)(p))[2]) << 8) | (((DWORD)((BYTE *)(p))[3])))
#define BUF_MAKEULONGLONG(p)  NTOHLL(*(ULONGLONG *)p)

#endif /* #ifdef NET_ORDER_CPU */

enum VARIANT_TYPE_T
{
    VARIANT_TYPE_INVALID        = 0,   //无效类型
    VARIANT_TYPE_BYTE           = 1,   //无字符
    VARIANT_TYPE_SHORT          = 2,   //有符号short
    VARIANT_TYPE_WORD           = 3,   //无符号short
    VARIANT_TYPE_INT            = 4,   //号整数
    VARIANT_TYPE_DWORD          = 5,   //无符号整数
    VARIANT_TYPE_FLOAT          = 6,   //浮点
    VARIANT_TYPE_DATETIME       = 7,   //时间
    VARIANT_TYPE_DOUBLE         = 8,   //浮点（双精度）
    VARIANT_TYPE_LONGLONG       = 9,   //longlong
    VARIANT_TYPE_STRING         = 10,  //字符串
    VARIANT_TYPE_BINARY         = 11,  //binary
    VARIANT_TYPE_CHAR           = 12,  //有字符
    VARIANT_TYPE_ULONGLONG      = 13,  //无符号longlong
    VARIANT_TYPE_FIX_BINARY     = 14,  //定长binary
    VARIANT_TYPE_BOOL           = 15,  //
    VARIANT_TYPE_SMALL_DATETIME = 16,  //
    VARIANT_TYPE_BIGINT         = 17,  // add by wangjj
    VARIANT_TYPE_VARSTRING      = 18,  //字符
    VARIANT_TYPE_BIT            = 19  //字符

};


enum VARIANT_TYPE_LEN
{
    VARIANT_TYPE_CHAR_LEN           = 1,  //字符
    VARIANT_TYPE_BOOL_LEN           = 1,  //
    VARIANT_TYPE_BYTE_LEN           = 1,  //字符
    VARIANT_TYPE_SHORT_LEN          = 2,
    VARIANT_TYPE_WORD_LEN           = 2,  //无符号short
    VARIANT_TYPE_INT_LEN            = 4,
    VARIANT_TYPE_DWORD_LEN          = 4,  //整数
    VARIANT_TYPE_FLOAT_LEN          = 4,  //浮点
    VARIANT_TYPE_DATETIME_LEN       = 8,  //时间
    VARIANT_TYPE_DOUBLE_LEN         = 8,  //浮点（双精度）
    VARIANT_TYPE_LONGLONG_LEN       = 8,  //longlong
    VARIANT_TYPE_ULONGLONG_LEN      = 8,  //无符号longlong

};
#endif /* #ifndef _HEAD_OSTYPEDEF_3A24AE1F_5485D62D_32D4B53D_H */
