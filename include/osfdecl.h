#ifndef _HEAD_OSFDECL_4F019B3F_072741AA_55743EB8_H
#define _HEAD_OSFDECL_4F019B3F_072741AA_55743EB8_H


/******************************************************************************/
#ifndef OS_WINDOWS

#define __stdcall
#define _stdcall
#define __fastcall
#define _fastcall
#define __cdecl
#define _cdecl

#undef pascal
#if (!defined(_MAC)) && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
#define pascal __stdcall
#else
#define pascal
#endif

#if defined(DOSWIN32) || defined(_MAC)
#define cdecl _cdecl
#ifndef CDECL
#define CDECL _cdecl
#endif
#else
#define cdecl
#ifndef CDECL
#define CDECL
#endif
#endif

#ifdef _MAC
#define CALLBACK    PASCAL
#define WINAPI      CDECL
#define WINAPIV     CDECL
#define APIENTRY    WINAPI
#define APIPRIVATE  CDECL
#ifdef _68K_
#define PASCAL      __pascal  /* Who defined __pascal here? */
#else
#define PASCAL
#endif
#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define WINAPIV     __cdecl
#define APIENTRY    WINAPI
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#else
#define CALLBACK
#define WINAPI
#define WINAPIV
#define APIENTRY    WINAPI
#define APIPRIVATE
#define PASCAL      pascal
#endif

#undef  far
#undef  near
#define far
#define near

#undef  FAR
#undef  NEAR
#define FAR     far
#define NEAR    near

#ifndef CONST
#define CONST   const
#endif

#ifndef STATIC
#define STATIC  static
#endif

#ifndef STATIC_ASSERT
#define STATIC_ASSERT(bexp) typedef _[(bexp) ? 1 : 0];
#endif

#endif


/******************************************************************************/

#if !defined(IN)
#define IN
#endif

#if !defined(OUT)
#define OUT
#endif

#if !defined(OPTIONAL)
#define OPTIONAL
#endif

#ifdef OS_WINDOWS
#define XTRY __try
#define EXCEPT __except
#else
#define XTRY try
#define EXCEPT	except
#endif

#endif /* #ifndef _HEAD_OSFDECL_4F019B3F_072741AA_55743EB8_H */
