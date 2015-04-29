#if !defined(ED_INIFILE_H)
#define ED_INIFILE_H

void* ED_createINI(const char* fileName);
void ED_destroyINI(void* _ini);
double ED_getDoubleFromINI(void* _ini, const char* varName, const char* section);
const char* ED_getStringFromINI(void* _ini, const char* varName, const char* section);
int ED_getIntFromINI(void* _ini, const char* varName, const char* section);

#endif
