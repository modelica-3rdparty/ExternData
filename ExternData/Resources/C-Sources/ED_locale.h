#if !defined(ED_LOCALE_H)
#define ED_LOCALE_H

#include <stdlib.h>
#include <locale.h>

#if defined(_MSC_VER) && _MSC_VER >= 1400
#define ED_LOCALE_TYPE _locale_t
#define ED_INIT_LOCALE _create_locale(LC_NUMERIC, "C")
#define ED_FREE_LOCALE(loc) _free_locale(loc)

int ED_strtod(char* token, ED_LOCALE_TYPE loc, double* val)
{
	int ret = 0; /* OK */
	char* endptr;
	*val = _strtod_l(token, &endptr, loc);
	if (*endptr != 0) {
		*val = 0.;
		ret = 1;
	}
	return ret;
}

int ED_strtoi(char* token, ED_LOCALE_TYPE loc, int* val)
{
	int ret = 0; /* OK */
	char* endptr;
	*val = (int)_strtol_l(token, &endptr, 10, loc);
	if (*endptr != 0) {
		*val = 0;
		ret = 1;
	}
	return ret;
}

#elif defined(__GLIBC__) && defined(__GLIBC_MINOR__) && ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= (2 << 16) + 3)
#define ED_LOCALE_TYPE locale_t
#define ED_INIT_LOCALE newlocale(LC_NUMERIC, "C", NULL)
#define ED_FREE_LOCALE(loc) free_locale(loc)

int ED_strtod(char* token, ED_LOCALE_TYPE loc, double* val)
{
	int ret = 0; /* OK */
	char* endptr;
	*val = strtod_l(token, &endptr, loc);
	if (*endptr != 0) {
		*val = 0.;
		ret = 1;
	}
	return ret;
}

int ED_strtoi(char* token, ED_LOCALE_TYPE loc, int* val)
{
	int ret = 0; /* OK */
	char* endptr;
	*val = (int)strtol_l(token, &endptr, 10, loc);
	if (*endptr != 0) {
		*val = 0;
		ret = 1;
	}
	return ret;
}

#else
#define ED_LOCALE_TYPE char*
#define ED_INIT_LOCALE localeconv()->decimal_point
#define ED_FREE_LOCALE(loc) *(loc) = '\0'
#endif

#endif
