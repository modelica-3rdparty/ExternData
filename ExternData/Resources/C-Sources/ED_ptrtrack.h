/* ED_ptrtrack.h - Simple pointer tracker
 *
 * Copyright (C) 2017-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_PTRTRACK_H)
#define ED_PTRTRACK_H

#if defined(_MSC_VER) && defined(_WINDLL)

#ifndef HASH_NONFATAL_OOM
#define HASH_NONFATAL_OOM 1
#endif

#include "uthash.h"
#include "g2constructor.h"

typedef struct {
	void* key;
	UT_hash_handle hh; /* Hashable structure */
} PtrTrack;

static PtrTrack* ptrTrack = NULL;

#if defined(G2_HAS_CONSTRUCTORS)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
static CRITICAL_SECTION cs;
#ifdef G2_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G2_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(G2_FUNCNAME(ED_initializeCS))
#endif
G2_DEFINE_CONSTRUCTOR(G2_FUNCNAME(ED_initializeCS))
static void G2_FUNCNAME(ED_initializeCS)(void) {
	InitializeCriticalSection(&cs);
}
#ifdef G2_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G2_DEFINE_DESTRUCTOR_PRAGMA_ARGS(G2_FUNCNAME(ED_cleanUp))
#endif
G2_DEFINE_DESTRUCTOR(G2_FUNCNAME(ED_cleanUp))
static void G2_FUNCNAME(ED_cleanUp)(void) {
	PtrTrack* iter;
	PtrTrack* tmp;
	DeleteCriticalSection(&cs);
	HASH_ITER(hh, ptrTrack, iter, tmp) {
		HASH_DEL(ptrTrack, iter);
		free(iter);
	}
}
#define MUTEX_LOCK() EnterCriticalSection(&cs)
#define MUTEX_UNLOCK() LeaveCriticalSection(&cs)
#else
#define MUTEX_LOCK()
#define MUTEX_UNLOCK()
#endif

/* Add a pointer to the tracker */
#define ED_PTR_ADD(_pt_ptr) \
do { \
	PtrTrack* _pt_iter = NULL; \
	MUTEX_LOCK(); \
	HASH_FIND_PTR(ptrTrack, &_pt_ptr, _pt_iter); \
	if (NULL == _pt_iter) { \
		_pt_iter = (PtrTrack*)malloc(sizeof(PtrTrack)); \
		if (NULL != _pt_iter) { \
			_pt_iter->key = (void*)_pt_ptr; \
			HASH_ADD_PTR(ptrTrack, key, _pt_iter); \
			if (NULL == _pt_iter->hh.tbl) { \
				free(_pt_iter); \
			} \
		} \
	} \
	MUTEX_UNLOCK(); \
} while (0)

/* Check if the pointer is tracked */
#define ED_PTR_CHECK(_pt_ptr) \
do { \
	PtrTrack* _pt_iter = NULL; \
	MUTEX_LOCK(); \
	HASH_FIND_PTR(ptrTrack, &_pt_ptr, _pt_iter); \
	if (NULL == _pt_iter) { \
		_pt_ptr = NULL; \
	} \
	MUTEX_UNLOCK(); \
} while (0)

/* Delete pointer from the tracker */
#define ED_PTR_DEL(_pt_ptr) \
do { \
	PtrTrack* _pt_iter = NULL; \
	MUTEX_LOCK(); \
	HASH_FIND_PTR(ptrTrack, &_pt_ptr, _pt_iter); \
	if (NULL != _pt_iter) { \
		HASH_DEL(ptrTrack, _pt_iter); \
		free(_pt_iter); \
	} \
	MUTEX_UNLOCK(); \
} while (0)

#else

#define ED_PTR_ADD(ptr)
#define ED_PTR_CHECK(ptr)
#define ED_PTR_DEL(ptr)

#endif

#endif
