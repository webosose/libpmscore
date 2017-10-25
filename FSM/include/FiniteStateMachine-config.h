#ifndef _FiniteStateMachine_config_h_
#define _FiniteStateMachine_config_h_

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
#undef size_t

/* Define if you have the ANSI C header files.  */
#undef STDC_HEADERS

/* Define if you have the ANSI # stringizing operator in cpp. */
#undef HAVE_STRINGIZE

/* Define if you have namespaces support */
#undef HAVE_NAMESPACES

/* Define if you have no native bool type */
#undef NO_BOOL

/* Define if you have posix facilities */
#undef _POSIX_C_SOURCE

/* Define if you use sproc threads */
#undef USE_SPROC

/* Define if you use windows threads */
#undef WIN32

/* Define for additional checks */
#undef _DEBUG

/* Define if you have hash_map  */
#undef HAVE_HASH_MAP

/* Define if you have iError library  */
#undef HAVE_LIBIERROR

/* Define if you have iWrapper library  */
#undef HAVE_LIBIWRAPPER

/* Define if you have XERCES library  */
#undef HAVE_LIBXERCES

#ifdef NO_BOOL
    typedef int bool;
    #define true 1
    #define false 0
#endif

#ifdef HAVE_NAMESPACES
    namespace std {};
#endif

#ifdef __GNUC__
#    ifndef _GNU_SOURCE
#        define _GNU_SOURCE
#    endif
#endif

#ifdef _POSIX_C_SOURCE
    #ifndef _XOPEN_SOURCE
        #define _XOPEN_SOURCE 500
        #define _XOPEN_SOURCE_EXTENDED 1
    #endif
#endif

#ifdef USE_SPROC
   #define _SGI_MP_SOURCE
#endif

#ifdef _POSIX_C_SOURCE
#ifdef USE_SPROC
#error cannot use POSIX and SPROC standards simultaneously
#endif
#endif

#ifdef HAVE_STRINGIZE
    #define __concat(a,b) a##b
    #define __quote(a) #a
#else
    #define __concat(a,b) a/**/b
    #define __quote(a) "a"
#endif

#endif
