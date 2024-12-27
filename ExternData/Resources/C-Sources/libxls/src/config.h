/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the `asprintf' function. */
#if defined(__CYGWIN__) || defined(__gnu_linux__)
#define HAVE_ASPRINTF 1
#else
#undef HAVE_ASPRINTF
#endif

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if you have the iconv() function and it works. */
/* #undef HAVE_ICONV */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
   and to 0 otherwise. */
#define HAVE_REALLOC 1

/* Define to 1 if you have the <stdint.h> header file. */
#if defined (_WIN32)
#if defined(_MSC_VER) && _MSC_VER >= 1600
#define HAVE_STDINT_H 1
#elif defined(__WATCOMC__)
#define HAVE_STDINT_H 1
#else
#undef HAVE_STDINT_H
#endif
#else
#define HAVE_STDINT_H 1
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <wchar.h> header file. */
#define HAVE_WCHAR_H 1

/* Define as const if the declaration of iconv() needs const. */
/* #undef ICONV_CONST */

/* Major version */
#define LIBXLS_MAJOR_VERSION 1

/* Micro version */
#define LIBXLS_MICRO_VERSION 3

/* Minor version */
#define LIBXLS_MINOR_VERSION 6

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "libxls"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "emmiller@gmail.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libxls"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libxls 1.6.3"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libxls"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://github.com/libxls/libxls"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.6.3"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Enable GNU extensions on systems that have them.  */
#if defined(__gnu_linux__)
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
#endif

/* Version number of package */
#define VERSION "1.6.3"

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define to rpl_realloc if the replacement function should be used. */
/* #undef realloc */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

#ifdef _WIN32
# ifdef _MSC_VER
/* On Microsoft compilers define inline to __inline on all others use inline */
#  undef inline
#  define inline __inline

# endif // _MSC_VER
#endif /* _WIN32 */
