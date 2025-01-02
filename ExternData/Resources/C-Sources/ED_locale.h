/* ED_locale.h - C locale specific string to number conversions
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_LOCALE_H)
#define ED_LOCALE_H

#include <stdlib.h>
#include <locale.h>

enum {
	ED_OK = 0,
	ED_ERROR = 1,
	ED_OOM = 2
};

#define ED_STRICT (1)

#if defined(_MSC_VER) && _MSC_VER >= 1400
#define ED_LOCALE_TYPE _locale_t
#define ED_INIT_LOCALE _create_locale(LC_NUMERIC, "C")
#define ED_FREE_LOCALE(loc) _free_locale(loc)

static __inline int ED_strtod(char* token, ED_LOCALE_TYPE loc, double* val, int strict)
{
	int ret = ED_OK;
	char* endptr;
	*val = _strtod_l(token, &endptr, loc);
	if (ED_STRICT == strict && *endptr != 0) {
		*val = 0.;
		ret = ED_ERROR;
	}
	return ret;
}

static __inline int ED_strtol(char* token, ED_LOCALE_TYPE loc, long* val, int strict)
{
	int ret = ED_OK;
	char* endptr;
	*val = _strtol_l(token, &endptr, 10, loc);
	if (ED_STRICT == strict && *endptr != 0) {
		*val = 0;
		ret = ED_ERROR;
	}
	return ret;
}

#elif defined(__GLIBC__) && defined(__GLIBC_MINOR__) && ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= (2 << 16) + 3)
#define ED_LOCALE_TYPE locale_t
#define ED_INIT_LOCALE newlocale(LC_NUMERIC, "C", NULL)
#define ED_FREE_LOCALE(loc) freelocale(loc)

static inline int ED_strtod(char* token, ED_LOCALE_TYPE loc, double* val, int strict)
{
	int ret = ED_OK;
	char* endptr;
	*val = strtod_l(token, &endptr, loc);
	if (ED_STRICT == strict && *endptr != 0) {
		*val = 0.;
		ret = ED_ERROR;
	}
	return ret;
}

static inline int ED_strtol(char* token, ED_LOCALE_TYPE loc, long* val, int strict)
{
	int ret = ED_OK;
	char* endptr;
	*val = strtol_l(token, &endptr, 10, loc);
	if (ED_STRICT == strict && *endptr != 0) {
		*val = 0;
		ret = ED_ERROR;
	}
	return ret;
}

#else
#define ED_LOCALE_TYPE char*
#define ED_INIT_LOCALE localeconv()->decimal_point
#define ED_FREE_LOCALE(loc)

static int ED_strtod(char* token, ED_LOCALE_TYPE dec, double* val, int strict)
{
	int ret = ED_OK;
	char* endptr;
	if (*dec == '.') {
		*val = strtod(token, &endptr);
	}
	else if (NULL == strchr(token, '.')) {
		*val = strtod(token, &endptr);
	}
	else {
		char* token2 = malloc(
			(strlen(token) + 1)*sizeof(char));
		if (token2 != NULL) {
			char* p;
			strcpy(token2, token);
			p = strchr(token2, '.');
			*p = *dec;
			*val = strtod(token2, &endptr);
			if (ED_STRICT == strict && *endptr != 0) {
				*val = 0.;
				ret = ED_ERROR;
			}
			free(token2);
		}
		else {
			ret = ED_OOM;
		}
	}
	return ret;
}

static int ED_strtol(char* token, ED_LOCALE_TYPE loc, long* val, int strict)
{
	int ret = ED_OK;
	char* endptr;
	*val = strtol(token, &endptr, 10);
	if (ED_STRICT == strict && *endptr != 0) {
		*val = 0;
		ret = ED_ERROR;
	}
	return ret;
}

#endif

#endif
