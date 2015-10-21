/* ED_JSONFile.c - JSON functions header
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

#if !defined(ED_JSONFILE_H)
#define ED_JSONFILE_H

void* ED_createJSON(const char* fileName);
void ED_destroyJSON(void* _json);
double ED_getDoubleFromJSON(void* _json, const char* varName);
const char* ED_getStringFromJSON(void* _json, const char* varName);
int ED_getIntFromJSON(void* _json, const char* varName);

#endif
