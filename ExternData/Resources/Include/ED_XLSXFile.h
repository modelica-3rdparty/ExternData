/* ED_XLSXFile.h - Excel XLSX functions header
 *
 * Copyright (C) 2015 tbeu
 *
 * This file is part of ExternData.
 * 
 * ExternData is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * ExternData is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ExternData; if not, see http://www.gnu.org/licenses
 *
 */

#if !defined(ED_XLSXFILE_H)
#define ED_XLSXFILE_H

void* ED_createXLSX(const char* fileName);
void ED_destroyXLSX(void* _xlsx);
double ED_getDoubleFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName);
const char* ED_getStringFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName);
int ED_getIntFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName);

#endif
