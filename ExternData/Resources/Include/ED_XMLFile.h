#if !defined(ED_XMLFILE_H)
#define ED_XMLFILE_H

void* ED_createXML(const char* fileName);
void ED_destroyXML(void* _xml);
double ED_getDoubleFromXML(void* _xml, const char* varName);
const char* ED_getStringFromXML(void* _xml, const char* varName);
int ED_getIntFromXML(void* _xml, const char* varName);
void ED_getDoubleArray1DFromXML(void* _xml, const char* varName, double* a, size_t n);
void ED_getDoubleArray2DFromXML(void* _xml, const char* varName, double* a, size_t m, size_t n);

#endif
