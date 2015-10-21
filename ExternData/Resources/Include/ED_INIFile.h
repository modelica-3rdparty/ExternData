/* ED_INIFile.c - INI functions header
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

#if !defined(ED_INIFILE_H)
#define ED_INIFILE_H

void* ED_createINI(const char* fileName);
void ED_destroyINI(void* _ini);
double ED_getDoubleFromINI(void* _ini, const char* varName, const char* section);
const char* ED_getStringFromINI(void* _ini, const char* varName, const char* section);
int ED_getIntFromINI(void* _ini, const char* varName, const char* section);

#endif
