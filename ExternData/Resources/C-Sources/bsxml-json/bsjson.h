/* Simple JSON implementation
 *
 * Copyright (C) 2014-2017, Borislav Sapundzhiev
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BSJSON_H
#define __BSJSON_H

#include "array.h"

#if defined(_MSC_VER)
#undef strdup
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

#define String char*

typedef struct JsonNode * JsonNodeRef;
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
    String m_errorString;
    int m_errorLine;
    int m_errorLineSet;
};

JsonNode *JsonParser_parse(JsonParser *parser, const char * json);
JsonNode *JsonParser_parseFile(JsonParser *parser, const char *fileName);
String JsonParser_getErrorString(JsonParser *parser);
int JsonParser_getErrorLine(JsonParser *parser);
int JsonParser_getErrorLineSet(JsonParser *parser);

//Create root node
JsonNode *JsonNode_Create();
JsonNode *JsonNode_createChild(JsonNode *node, String name, int type);
JsonNode *JsonNode_createObject(JsonNode * node, String name);
JsonNode *JsonNode_createArray(JsonNode * node, String name);
JsonNode * JsonNode_findChild(JsonNode *node, const String name, int type);
JsonPair * JsonNode_findPair(JsonNode *node, const String key);
void JsonNode_setPair(JsonNode *node, const String key, const String value );
asize_t JsonNode_getChildCount(JsonNode * node);
asize_t JsonNode_getPairCount(JsonNode *node);
JsonNode * JsonNode_getChild(JsonNode *node, asize_t index);
JsonPair * JsonNode_getPair(JsonNode *node, asize_t index);
String JsonNode_getPairValue(JsonNode *node, const String key);
int JsonNode_getPairValueInt(JsonNode *node, const String key);
void JsonNode_delete(JsonNode *node);
void JsonNode_deleteTree(JsonNode *root);
String JsonNode_getJSON(JsonNode *node);

#endif //__BSJSON_H
