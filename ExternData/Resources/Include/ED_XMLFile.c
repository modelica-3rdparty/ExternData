#if !defined(ED_XMLFILE_C)
#define ED_XMLFILE_C

typedef struct {
	char* fileName;
} XMLFile;

void* ED_createXML(const char* fileName) {
	XMLFile* xml = (XMLFile*)malloc(sizeof(XMLFile));
	if (xml) {
		xml->fileName = (char*)malloc((strlen(fileName) + 1)*sizeof(char));
		if (xml->fileName) {
			strcpy(xml->fileName, fileName);
		}
	}
	return xml;
}

void ED_destroyXML(void* _xml)
{
	XMLFile* xml = (XMLFile*)_xml;
	if (xml) {
		if (xml->fileName) {
			free(xml->fileName);
		}
		free(xml);
	}
}

double ED_getDoubleFromXML(void*_xml, const char* varName)
{
	XMLFile* xml = (XMLFile*)_xml;
	if (xml) {
		return 2.;
	}
	return -2.;
}

#endif
