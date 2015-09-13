#if !defined(ED_XLSXFILE_C)
#define ED_XLSXFILE_C

#if defined(__gnu_linux__)
#define _GNU_SOURCE 1
#endif

#include <string.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include <ctype.h>
#include "ED_locale.h"
#include "bsxml.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_XLSXFile.h"
#include "unzip.h"

#define E_NO_MEMORY (11)
#define E_BAD_DATA (12)
#define E_EOPEN (15)
#define E_ECLOSE (17)
#define E_EREAD (18)
#define E_ELOCATE (30)
#define E_EGETFILEINFO (31)

typedef struct {
	char* fileName;
	ED_LOCALE_TYPE loc;
} XLSXFile;

static int parseXML(unzFile zfile, const char* fileName, XmlNodeRef* root)
{
	unz_file_info info;
	char* buf;
	int rc;
	XmlParser xmlParser;
	rc = unzLocateFile(zfile, fileName, 1);
	if (rc != UNZ_OK) {
		return E_ELOCATE;
	}
	rc = unzOpenCurrentFile(zfile);
	if (rc != UNZ_OK) {
		return E_EOPEN;
	}
	rc = unzGetCurrentFileInfo(zfile, &info, NULL, 0, NULL, 0, NULL, 0);
	if (rc != UNZ_OK) {
		return E_EGETFILEINFO;
	}
	buf = malloc(info.uncompressed_size + 1);
	if (buf == NULL) {
		return E_NO_MEMORY;
	}
	rc = unzReadCurrentFile(zfile, buf, info.uncompressed_size);
	if (rc < 0) {
		free(buf);
		return E_EREAD;
	}
	buf[info.uncompressed_size] = '\0';
	*root = XmlParser_parse(&xmlParser, buf);
	free(buf);
	if (*root == NULL) {
		return E_BAD_DATA;
	}

	return 0;
}

void* ED_createXLSX(const char* fileName, const char* encoding)
{
	XLSXFile* xlsx = NULL;
	int rc;
	XmlNodeRef wb_root;
	const char* wb = "xl/workbook.xml";
	unzFile zfile = unzOpen(fileName);
	if (zfile == NULL) {
		ModelicaFormatError("Cannot open file \"%s\"\n", fileName);
		return xlsx;
	}
	rc = parseXML(zfile, "xl/workbook.xml", &wb_root);
	if (rc != 0) {
		unzClose(zfile);
		switch (rc) {
			case: E_NO_MEMORY:
				ModelicaError("Memory allocation error\n");
				break;
			case E_ELOCATE:
				ModelicaFormatError("Cannot locate %s in file \"%s\"\n", wb, fileName);
				break;
			case E_EOPEN:
				ModelicaFormatError("Cannot open %s in file \"%s\"\n", wb, fileName);
				break;
			case E_EGETFILEINFO:
				ModelicaFormatError("Cannot get file info of %s in file \"%s\"\n", wb, fileName);
				break;
			case E_EREAD:
				ModelicaFormatError("Cannot read file %s in file \"%s\"\n", wb, fileName);
				break;
			case E_BAD_DATA:
				ModelicaFormatError("Cannot parse file %s of file \"%s\"\n", wb, fileName);
				break;
			default:
				break;
		}
		return xlsx;
	}
	xlsx = (XLSXFile*)malloc(sizeof(XLSXFile));
	if (xlsx != NULL) {
		xlsx->fileName = strdup(fileName);
		if (xlsx->fileName == NULL) {
			unzClose(zfile);
			XmlNode_deleteTree(wb_root);
			free(xlsx);
			ModelicaError("Memory allocation error\n");
			return NULL;
		}
		xlsx->loc = ED_INIT_LOCALE;
	}
	else {
		unzClose(zfile);
		XmlNode_deleteTree(wb_root);
		ModelicaError("Memory allocation error\n");
	}
	unzClose(zfile);
	return xlsx;
}

void ED_destroyXLSX(void* _xlsx)
{
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
}

double ED_getDoubleFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName)
{
	double ret = 0.;
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	if (xlsx != NULL) {
	}
	return ret;
}

const char* ED_getStringFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName)
{
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	if (xlsx != NULL) {
	}
	return "";
}

int ED_getIntFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName)
{
	int ret = 0;
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	if (xlsx != NULL) {
	}
	return ret;
}

#endif
