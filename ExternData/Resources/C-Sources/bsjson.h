/* Simple JSON implementation
 *
 * Copyright (C) 2014 Borislav Sapundzhiev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or (at
 * your option) any later version.
 *
 */

#ifndef __BSJSON_H
#define __BSJSON_H

#include "array.h"
#include "bsstr.h"

#ifdef _WIN32
#define strdup _strdup
#endif

//#define DEBUG_JSON
#ifdef DEBUG_JSON
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

enum {JSON_NOK, JSON_OK };
enum {JSON_NONE, JSON_ROOT, JSON_OBJ, JSON_ARRAY } eNodeTypes;

#define NAME_ANON NULL

#define JSON_IS_OBJ(node)\
	(node->m_type == JSON_OBJ || node->m_type == JSON_ROOT)
#define JSON_IS_ARRAY(node)\
	(node->m_type == JSON_ARRAY)

struct JsonNode;
typedef char * String;
typedef struct JsonNode JsonNode;
typedef struct JsonParser JsonParser;
typedef struct JsonPair JsonPair;

struct JsonPair {
	String key;
	String value;
};

struct JsonNode {
	int m_type;
	String m_name;
	JsonNode * m_parent;
	cpo_array_t *m_pairs;
	cpo_array_t *m_childs;
};

struct JsonParser {
	JsonNode *m_root;
	cpo_array_t *m_nodeStack;

};

JsonNode *JsonParser_parse(JsonParser *parser, const char * json);
JsonNode *JsonParser_parseFile(JsonParser *parser, const char *fileName);

JsonNode *JsonNode_Create();
JsonNode *JsonNode_createChild(JsonNode *node, String name, int type);
JsonNode *JsonNode_createObject(JsonNode * node, String name);
JsonNode *JsonNode_createArray(JsonNode * node, String name);
void JsonNode_setPair(JsonNode *node, const String key, const String value );
String JsonNode_getPairValue(JsonNode *node, const String key);
void JsonNode_delete(JsonNode *node);
void JsonNode_deleteTree(JsonNode *root);
String JsonNode_getJSON(JsonNode *node);

#endif //__BSJSON_H