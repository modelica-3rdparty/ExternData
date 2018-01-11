/* ED_ptrtrack.h - Simple pointer tracker
 *
 * Copyright (C) 2017-2018, tbeu
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(ED_PTRTRACK_H)
#define ED_PTRTRACK_H

#if defined(_MSC_VER) && defined(_WINDLL)

#ifndef HASH_NONFATAL_OOM
#define HASH_NONFATAL_OOM 1
#endif

#include "uthash.h"
#include "gconstructor.h"

typedef struct {
	void* key;
	UT_hash_handle hh; /* Hashable structure */
} PtrTrack;

static PtrTrack* ptrTrack = NULL;

#if defined(G_HAS_CONSTRUCTORS)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
static CRITICAL_SECTION cs;
#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(ED_initializeCS)
#endif
G_DEFINE_CONSTRUCTOR(ED_initializeCS)
static void ED_initializeCS(void) {
	InitializeCriticalSection(&cs);
}
#ifdef G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(ED_cleanUp)
#endif
G_DEFINE_DESTRUCTOR(ED_cleanUp)
static void ED_cleanUp(void) {
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
