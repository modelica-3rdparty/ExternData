#if !defined(ED_XLSFILE_H)
#define ED_XLSFILE_H

void* ED_createXLS(const char* fileName, const char* encoding);
void ED_destroyXLS(void* _xls);
double ED_getDoubleFromXLS(void* _xls, const char* cellAddress, const char* sheetName);
const char* ED_getStringFromXLS(void* _xls, const char* cellAddress, const char* sheetName);
int ED_getIntFromXLS(void* _xls, const char* cellAddress, const char* sheetName);

#endif
