#ifndef EXPAT_CONFIG_H
#define EXPAT_CONFIG_H

#define XML_GE 1
#define XML_NS 1
#define XML_DTD
#define XML_CONTEXT_BYTES 1024

/* 1234 = LIL_ENDIAN, 4321 = BIGENDIAN */
#if defined(__GLIBC__)
#if ( __BYTE_ORDER == __BIG_ENDIAN )
#define BYTEORDER 4321
#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
#define BYTEORDER 1234
#endif
#elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
#define BYTEORDER 4321
#elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#define BYTEORDER 1234
#elif defined(__sparc) || defined(__sparc__) || defined(_POWER) || defined(__powerpc__) || \
    defined(__ppc__) || defined(__hpux) || defined(_MIPSEB) || defined(_POWER) ||          \
    defined(__s390__)
#define BYTEORDER 4321
#elif defined(__i386__) || defined(__alpha__) || defined(__ia64) || defined(__ia64__) ||   \
    defined(_M_IX86) || defined(_M_IA64) || defined(_M_ALPHA) || defined(__amd64) ||       \
    defined(__amd64__) || defined(_M_AMD64) || defined(__x86_64) || defined(__x86_64__) || \
    defined(_M_X64) || defined(__bfin__) || defined(__loongarch64) || defined(__aarch64__)
#define BYTEORDER 1234
#endif

/* Windows has memmove() available. */
#ifndef HAVE_MEMMOVE
#define HAVE_MEMMOVE
#endif

#endif /* ndef EXPAT_CONFIG_H */
