#if !defined(ED_XLSXFILE_H)
#define ED_XLSXFILE_H

void* ED_createXLSX(const char* fileName);
void ED_destroyXLSX(void* _xlsx);
double ED_getDoubleFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName);
const char* ED_getStringFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName);
int ED_getIntFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName);

#endif
