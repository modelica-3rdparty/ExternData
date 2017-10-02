#ifndef EXPAT_CONFIG_H
#define EXPAT_CONFIG_H

#if defined(_WIN32)

#define XML_NS 1
#define XML_DTD 1
#define XML_CONTEXT_BYTES 1024

/* we will assume all Windows platforms are little endian */
#define BYTEORDER 1234

/* Windows has memmove() available. */
#ifndef HAVE_MEMMOVE
#define HAVE_MEMMOVE
#endif

#endif /* defined(_WIN32) */

#endif /* ndef EXPAT_CONFIG_H */
