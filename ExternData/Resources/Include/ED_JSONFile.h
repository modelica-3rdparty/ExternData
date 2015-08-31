#if !defined(ED_JSONFILE_H)
#define ED_JSONFILE_H

void* ED_createJSON(const char* fileName);
void ED_destroyJSON(void* _json);
double ED_getDoubleFromJSON(void* _json, const char* varName);
const char* ED_getStringFromJSON(void* _json, const char* varName);
int ED_getIntFromJSON(void* _json, const char* varName);

#endif
