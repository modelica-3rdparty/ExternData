AR = ar -ru
RM = rm -f

CFLAGS = -O3
CPPFLAGS = -DHAVE_UNISTD_H=1 -DHAVE_STDARG_H=1 -DHAVE_HIDDEN=1 -DHAVE_MEMCPY=1 -DHAVE_MEMMOVE=1 -DHAVE_ZLIB=1 -DHAVE_HDF5=1 -DNDEBUG -DXML_POOR_ENTROPY -DHAVE_EXPAT_CONFIG_H
INC = -I"bsxml-json" -I"expat/lib" -I"hdf5/include" -I"inih" -I"libxls/include" -I"minizip" -I"modelica" -I"zlib" -I"parson"

TARGETDIR = linux64

BS_OBJS = \
	bsxml-json/array.o \
	bsxml-json/bsjson.o \
	bsxml-json/bsxml.o

CSV_OBJS = \
	ED_CSVFile.o

INI_OBJS = \
	inih/ini.o \
	ED_INIFile.o

JSON_OBJS = \
	ED_JSONFile.o

MAT_OBJS = \
	ED_MATFile.o

XLS_OBJS = \
	libxls/src/endian.o \
	libxls/src/ole.o \
	libxls/src/xls.o \
	libxls/src/xlstool.o \
	ED_XLSFile.o

XLSX_OBJS = \
	minizip/ioapi.o \
	minizip/unzip.o \
	ED_XLSXFile.o

XML_OBJS = \
	ED_XMLFile.o

EXPAT_OBJS = \
	expat/lib/xmlparse.o \
	expat/lib/xmlrole.o \
	expat/lib/xmltok.o \
	expat/lib/xmltok_impl.o \
	expat/lib/xmltok_ns.o \
	expat/lib/loadlibrary.o

ZLIB_OBJS = \
	zlib/adler32.o \
	zlib/compress.o \
	zlib/crc32.o \
	zlib/deflate.o \
	zlib/gzclose.o \
	zlib/gzlib.o \
	zlib/gzread.o \
	zlib/gzwrite.o \
	zlib/infback.o \
	zlib/inffast.o \
	zlib/inflate.o \
	zlib/inftrees.o \
	zlib/trees.o \
	zlib/uncompr.o \
	zlib/zutil.o

PARSON_OBJS = \
	parson/parson.o

ALL_OBJS = $(BS_OBJS) $(CSV_OBJS) $(INI_OBJS) $(JSON_OBJS) $(MAT_OBJS) $(XLS_OBJS) $(XLSX_OBJS) $(XML_OBJS) $(EXPAT_OBJS) $(ZLIB_OBJS) $(PARSON_OBJS)

all: clean libs

libs: libbsxml-json.a libED_CSVFile.a libED_INIFile.a libED_JSONFile.a libED_MATFile.a libED_XLSFile.a libED_XLSXFile.a libED_XMLFile.a libexpat.a libzlib.a libparson.a
	cp $^ ../Library/$(TARGETDIR)

libbsxml-json.a: $(BS_OBJS)
	$(AR) $@ $(BS_OBJS)

libED_CSVFile.a: $(CSV_OBJS)
	$(AR) $@ $(CSV_OBJS)

libED_INIFile.a: $(INI_OBJS)
	$(AR) $@ $(INI_OBJS)

libED_JSONFile.a: $(JSON_OBJS)
	$(AR) $@ $(JSON_OBJS)

libED_MATFile.a: $(MAT_OBJS)
	$(AR) $@ $(MAT_OBJS)

libED_XLSFile.a: $(XLS_OBJS)
	$(AR) $@ $(XLS_OBJS)

libED_XLSXFile.a: $(XLSX_OBJS)
	$(AR) $@ $(XLSX_OBJS)

libED_XMLFile.a: $(XML_OBJS)
	$(AR) $@ $(XML_OBJS)

libexpat.a: $(EXPAT_OBJS)
	$(AR) $@ $(EXPAT_OBJS)

libzlib.a: $(ZLIB_OBJS)
	$(AR) $@ $(ZLIB_OBJS)

libparson.a: $(PARSON_OBJS)
	$(AR) $@ $(PARSON_OBJS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	$(RM) $(ALL_OBJS)
	$(RM) *.a
	$(RM) ../Library/$(TARGETDIR)/*.a
	$(RM) ../Library/$(TARGETDIR)/$(TARGETDIR).tar.xz
