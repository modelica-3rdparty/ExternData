/* bsjson.c - Simple JSON implementation
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#define oom() break
#include "utstring.h"
#include "bsjson.h"

#define JSON_STACK_SIZE 32

#define isNullorEmpty(str) \
    (str == NULL || str[0] == '\0')

#define utstring_addchr(s, c) \
    do {\
        UT_string tmp; \
        utstring_init(&tmp); \
        utstring_printf(&tmp, "%c", c); \
        utstring_concat(s, &tmp); \
        utstring_done(&tmp); \
    } while(0)

enum eElemType {
    JSON_OBJ_B, JSON_OBJ_E, JSON_ARR_B, JSON_ARR_E,
    JSON_COLON, JSON_COMMA, JSON_QUOTE, JSON_LEFT,
    JSON_RIGHT, JSON_BEGIN, JSON_FORMFEED, JSON_LF,
    JSON_CR, JSON_TAB, JSON_HEX, JSON_INVALID
};

static const char jsonElems [] = {
    '{','}','[', ']', ':',',','\"','\\',
    '/','\b','\f', '\n', '\r', '\t', 'u'
};

static char Json_elem(enum eElemType type)
{
    return jsonElems[type];
}

static enum eElemType Json_typeOfElem(const char c)
{
    enum eElemType type = JSON_INVALID;
    for (type = JSON_OBJ_B; type != JSON_INVALID; ++type) {
        if (c == jsonElems[type]) break;
    }

    return type;
}

JsonNode * JsonNode_Create()
{
    JsonNode *node = (JsonNode *) malloc( sizeof(JsonNode) );
    if (!node) return NULL;

    node->m_type = JSON_ROOT;
    node->m_name = NULL;
    node->m_parent = NULL;
    node->m_pairs = cpo_array_create(4, sizeof(JsonPair));
    node->m_childs = cpo_array_create(4, sizeof(JsonNode));
    return node;
}

JsonNode * JsonNode_createChild(JsonNode * node, String name, int type)
{
    JsonNode * child = (JsonNode *)cpo_array_push(node->m_childs);
    child->m_type = type;
    child->m_parent = node;
    child->m_name = (name != NULL) ? strdup(name) : NULL;
    child->m_pairs = cpo_array_create(4, sizeof(JsonPair));
    child->m_childs = cpo_array_create(4, sizeof(JsonNode));
    return child;
}

JsonNode * JsonNode_createObject(JsonNode * node, String name)
{
    return  JsonNode_createChild(node, name, JSON_OBJ);
}

JsonNode * JsonNode_createArray(JsonNode * node, String name)
{
    return  JsonNode_createChild(node, name, JSON_ARRAY);
}

void JsonNode_setPair(JsonNode * node, const String key, const String value )
{
    JsonPair *a = (JsonPair*)cpo_array_push( node->m_pairs );
    a->key = strdup(key);
    a->value = strdup(value);
}

static int JsonPair_comparer(const void *a, const void *b)
{
    return strcmp(((JsonPair *) a)->key, ((JsonPair *) b)->key);
}

JsonPair * JsonNode_findPair(JsonNode *node, const String key)
{
    JsonPair p = { (String)key, NULL };
    JsonPair *ret = (JsonPair*)cpo_array_bsearch(node->m_pairs, &p, JsonPair_comparer);
    return ret;
}

String JsonNode_getPairValue(JsonNode *node, const String key)
{
    String value  = NULL;
    JsonPair *pair = JsonNode_findPair(node,  key);
    if(pair) {
        value = pair->value;
    }
    return value;
}

int JsonNode_getPairValueInt(JsonNode *node, const String key)
{
    String jsonVal = JsonNode_getPairValue(node, key);
    if(jsonVal) {
        return atoi(jsonVal);
    }
    return 0;
}

double JsonNode_getPairValueFloat(JsonNode *node, const String key)
{
    String jsonVal = JsonNode_getPairValue(node, key);
    if(jsonVal) {
        return atof(jsonVal);
    }
    return 0;
}

static int JsonNode_comparer(const void *a, const void *b)
{
    return strcmp(((JsonNode *) a)->m_name, ((JsonNode *) b)->m_name);
}

JsonNode * JsonNode_findChild(JsonNode *node, const String name, int type)
{
    JsonNode tmpNode = { type, (String)name };
    JsonNode *ret = (JsonNode*)cpo_array_bsearch(node->m_childs, &tmpNode, JsonNode_comparer);
    return ret;
}

asize_t JsonNode_getChildCount(JsonNode *node)
{
    return node->m_childs->num;
}

asize_t JsonNode_getPairCount(JsonNode *node)
{
    return node->m_pairs->num;
}

JsonNode * JsonNode_getChild(JsonNode *node, asize_t index)
{
    return (JsonNode *)cpo_array_get_at(node->m_childs, index);
}

JsonPair * JsonNode_getPair(JsonNode *node, asize_t index)
{
    return (JsonPair *)cpo_array_get_at(node->m_pairs, index);
}

void JsonNode_delete(JsonNode *node)
{
    asize_t i;
    if (!node) return;
    for (i=0; i < JsonNode_getPairCount(node); i++) {
        JsonPair *pair = JsonNode_getPair(node, i);
        free(pair->key);
        free(pair->value);
    }

    if (node->m_childs) {
        cpo_array_destroy(node->m_childs);
    }

    if (node->m_pairs) {
        cpo_array_destroy(node->m_pairs);
    }

    if (node->m_name)
        free(node->m_name);
}

void JsonNode_deleteTree(JsonNode *root)
{
    asize_t i;
    if (!root) return;
    for (i=0 ; i < JsonNode_getChildCount(root); i++) {
        JsonNode *node = JsonNode_getChild(root, i);
        JsonNode_deleteTree(node);
    }

    JsonNode_delete(root);

    if (root->m_type == JSON_ROOT) {
        free(root);
    }
}
/* return allocated UT_string */
static UT_string *JsonNode_getJSON_UT(JsonNode *node, String last)
{
    asize_t i, nPairs, nChilds;
    UT_string *buff;
    utstring_new(buff);
    if (buff == NULL) return NULL;

    if (!isNullorEmpty(node->m_name)) {
        utstring_printf(buff, "\"%s\":", node->m_name);
    }

    utstring_printf(buff, "%s\n",  JSON_IS_OBJ(node) ? "{" : "[");
    nPairs = JsonNode_getPairCount(node);
    nChilds = JsonNode_getChildCount(node);

    for (i=0; i < nPairs; i++ ) {
        JsonPair *pair = JsonNode_getPair(node, i);

        if (JSON_IS_ARRAY(node)) {
            utstring_printf(buff, "\"%s\"", pair->key);
        } else {
            utstring_printf(buff, "\"%s\":\"%s\"", pair->key, pair->value);
        }

        utstring_printf(buff, "%s\n", (i < nPairs -1 || nChilds > 0) ? "," : "");
    }

    for (i = 0; i < nChilds; i++) {
        JsonNode* child = JsonNode_getChild(node, i);
        UT_string *childJSON = JsonNode_getJSON_UT(child, i == nChilds - 1 ? "\n" : ",\n");
        if (childJSON != NULL) {
            utstring_concat(buff, childJSON);
            utstring_free(childJSON);
        }
    }

    utstring_printf(buff, "%s%s",  JSON_IS_OBJ(node) ? "}" : "]", last);
    return buff;
}
/* return allocated string */
String JsonNode_getJSON(JsonNode *node)
{
    UT_string *buff = JsonNode_getJSON_UT(node, "\n");
    return utstring_release(buff);
}
/********************************************************************************/
/* Parse JSON                                                                   */
/********************************************************************************/
enum {JSON_ERR_NONE, JSON_ERR_QUOTE, JSON_ERR_COMMA, JSNON_ERR_NOTOBJ, JSON_ERR_SYN};
const char *jsonParser_errlist[] = {
    "Unknown error", "Missing or unexpected quote",
    "Missing or unexpected comma", "Unexpected object end",
    "Unexpected syntax"
};

struct  ParserInternal { /*Jsonlexer*/
    int error;
    int line;
    int quote_begin;
    int is_value;
    UT_string *key;
    UT_string *value;
    cpo_array_t stack;
    struct JsonParser *parser;
    void (*startElem)(struct JsonParser *, const String, int);
    void (*endElem)(struct JsonParser *, const String, int);
    void (*elemData)(struct JsonParser *, const String,  const String);
};

static void JsonParser_internalCreate(struct ParserInternal *pi)
{
    pi->error = JSON_ERR_NONE;
    pi->line = 0;
    utstring_new(pi->key);
    utstring_new(pi->value);
    pi->stack.v = calloc(JSON_STACK_SIZE, sizeof(char*));
    pi->stack.num = 0;
    pi->stack.max = JSON_STACK_SIZE;
    pi->stack.elem_size = sizeof(char*);
    pi->startElem = NULL;
    pi->endElem = NULL;
    pi->elemData = NULL;
}

static void JsonParser_internalReset(struct ParserInternal *pi)
{
    pi->quote_begin = pi->is_value = 0;
    utstring_clear(pi->key);
    utstring_clear(pi->value);
}

static void JsonParser_internalDelete(struct ParserInternal *pi)
{
    utstring_free(pi->key);
    utstring_free(pi->value);
    free(pi->stack.v);
}

static int JsonParser_internalBeginObj(struct ParserInternal *pi, enum eElemType elemType)
{
    char *name = utstring_body(pi->key);
    void *ptr = stack_push_back(&(pi)->stack);
    pi->key->n = 64;
    pi->key->d = (char*)calloc(64, 1);
    pi->key->i = 0;
    ARR_VAL(ptr) = ARR_VAL2PTR(name);
    if (elemType != JSON_ARR_B && elemType != JSON_OBJ_B) {
        pi->error = JSNON_ERR_NOTOBJ;
        return JSON_NOK;
    }
    if (pi->startElem) {
        pi->startElem(pi->parser, name, (elemType == JSON_ARR_B) ? JSON_ARRAY : JSON_OBJ);
    }

    JsonParser_internalReset(pi);
    return JSON_OK;
}

static int JsonParser_internalEndObj(struct ParserInternal *pi, enum eElemType elemType)
{
    char *name;
    void *ptr = stack_pop_back(&(pi)->stack);

    if (elemType != JSON_ARR_E && elemType != JSON_OBJ_E) {
        pi->error = JSNON_ERR_NOTOBJ;
        return JSON_NOK;
    }
    if ((name = (char*)ARR_VAL(ptr))) {
        if (pi->endElem) {
            pi->endElem(pi->parser, name, (elemType == JSON_ARR_E) ? JSON_ARRAY : JSON_OBJ);
        }
        free(name);
    }

    JsonParser_internalReset(pi);
    return 0;
}

static int JsonParser_internalData(struct ParserInternal *pi)
{
    if (pi->quote_begin) {
        pi->error = JSON_ERR_QUOTE;
        return JSON_NOK;
    }

    if (utstring_len(pi->key) > 0 || utstring_len(pi->value) > 0) {

        if (pi->elemData) {
            pi->elemData(pi->parser, utstring_body(pi->key), utstring_body(pi->value) );
        }
    }

    JsonParser_internalReset(pi);
    return 0;
}

static char JsonParser_next_char(const char *p, int pos)
{
    char ch;
    while ( (ch = *(p+ ++pos)) != '\0' ) {
        if (ch != ' ' && ch != '\r' && ch != '\n' && ch != '\t' )
            break;
    }

    return ch;
}

static char JsonParser_prev_char(const char *p, int pos)
{
    char ch;
    while ( (ch = *(p+ --pos)) != '\0' ) {
        if (ch != ' ' && ch != '\r' && ch != '\n' && ch != '\t' )
            break;
    }

    return ch;
}

#define JsonParser_peek_char(p,pos)     *(p + pos + 1)

#define JsonParser_peekObjBegin(p,pos)\
    (JsonParser_next_char(p, pos) == Json_elem(JSON_OBJ_B)\
    || JsonParser_next_char(p, pos) == Json_elem(JSON_ARR_B))

#define JsonParser_peekObjEnd(p, pos)\
    (JsonParser_next_char(p, pos) == Json_elem(JSON_OBJ_E)\
    || JsonParser_next_char(p, pos) == Json_elem(JSON_ARR_E))

#define JsonParser_peekEndLine(p,pos)\
    (JsonParser_peek_char(p,pos) == Json_elem(JSON_CR)\
    || JsonParser_peek_char(p, pos) == Json_elem(JSON_LF))

static int JsonParser_internalParse(struct ParserInternal *pi, const char* json, int len)
{
    int i;
    enum eElemType elemType;
    const char *p = json;

    for (i =0; i < len + 1; i++) {
        char ch;

        if(pi->error != JSON_ERR_NONE) break;

        ch = p[i];

        switch ( elemType = Json_typeOfElem(ch) ) {

        case JSON_OBJ_B:
        case JSON_ARR_B:
            JsonParser_internalBeginObj(pi, elemType);
            break;

        case JSON_OBJ_E:
        case JSON_ARR_E:
            if (pi->is_value) {
                JsonParser_internalData(pi);
            }
            JsonParser_internalEndObj(pi, elemType);
            break;

        case JSON_QUOTE:
            /* escaped quote in value */
            if(JsonParser_prev_char(p, i) == Json_elem(JSON_LEFT)) {
                UT_string *data = (!pi->is_value) ? pi->key : pi->value;
                utstring_addchr(data, Json_elem(JSON_QUOTE));
                break;
            }

            pi->quote_begin = !pi->quote_begin;
            if(pi->quote_begin) {
                char prev = JsonParser_prev_char(p, i);
                if(prev != Json_elem(JSON_COMMA) && prev != Json_elem(JSON_COLON)
                        && prev != Json_elem(JSON_OBJ_B) && prev != Json_elem(JSON_ARR_B)) {
                    pi->error = JSON_ERR_SYN;
                    break;
                }
            }

            if (!pi->quote_begin && JsonParser_peekObjEnd(p,i)) {
                JsonParser_internalData(pi);
            }
            break;
        case JSON_COLON:
            /* Begin value */
            if (!pi->quote_begin) {
                pi->is_value = 1;
            } else {
                /* colon in value */
                UT_string *data = (!pi->is_value) ? pi->key : pi->value;
                utstring_addchr(data, Json_elem(JSON_COLON));
            }
            break;
        case JSON_COMMA:
            if (JsonParser_peekEndLine(p,i) || JsonParser_next_char(p,i) == Json_elem(JSON_QUOTE)) {
                JsonParser_internalData(pi);
            } else {
                /* comma in value */
                UT_string *data = (!pi->is_value) ? pi->key : pi->value;
                utstring_addchr(data, Json_elem(JSON_COMMA));
            }
            break;

        case JSON_CR:/*skip*/
            break;
        case JSON_LF:
            pi->line++;
            if (pi->is_value && !JsonParser_peekObjBegin(p, i) ) {
                JsonParser_internalData(pi);
            }
            break;

        case JSON_BEGIN:
        case JSON_FORMFEED:
        case JSON_LEFT:
        case JSON_RIGHT:
        case JSON_TAB:
        case JSON_HEX:  /*TODO:*/
        case JSON_INVALID:
            if (pi->quote_begin && !pi->is_value) {
                utstring_addchr(pi->key, ch);
            } else if (pi->quote_begin && pi->is_value) {
                utstring_addchr(pi->value, ch);
            } else {
                //printf("[skiped] '%c' [0x%x]\n", ch,ch);
            }
            break;
        }
    }

    return pi->error; /*JSON_OK;*/
}

static void JsonParser_startElem(struct JsonParser *parser, const String name, int type)
{
    void *ptr = NULL;
    JsonNode* parent= NULL, *node=NULL;

    DEBUG_PRINT("Json_startElem %s type %d\n", name, type );

    if (parser->m_nodeStack->num > 0) {
        ptr = stack_back(parser->m_nodeStack);
        parent = (JsonNode*) ARR_VAL(ptr);
    } else {
        parser->m_root = JsonNode_Create();
    }

    if (parent) {
        char *pname = isNullorEmpty(name) ? NULL : (char*)name;
        node = JsonNode_createChild(parent, pname , type);
    } else {
        node = parser->m_root;
    }

    ptr = stack_push_back(parser->m_nodeStack);
    if (ptr != NULL) {
        ARR_VAL(ptr) = ARR_VAL2PTR(node);
    }
}

static void JsonParser_endElem(struct JsonParser *parser, const String name, int type )
{
    DEBUG_PRINT("Json_endElem %s type %d\n", name, type );
    assert( parser->m_nodeStack->num > 0 );
    if (parser->m_nodeStack->num > 0) {
        stack_pop_back(parser->m_nodeStack);
    }
}

static void JsonParser_elemData(struct JsonParser *parser, const String key,  const String value)
{
    DEBUG_PRINT("eleme '%s' => '%s'\n", key, value);
    if (parser->m_nodeStack->num > 0) {
        void *ptr = stack_back(parser->m_nodeStack);
        JsonNode *node = (JsonNode *) ARR_VAL(ptr);
        JsonNode_setPair(node, key, value);
    }
}

String JsonParser_getErrorString(JsonParser *parser)
{
    return parser->m_errorString;
}

int JsonParser_getErrorLine(struct JsonParser *parser)
{
    return parser->m_errorLine;
}

int JsonParser_getErrorLineSet(struct JsonParser *parser)
{
    return parser->m_errorLineSet;
}

JsonNode * JsonParser_parse(struct JsonParser *parser, const char * json)
{
    JsonNode * root = NULL;
    struct ParserInternal pi;
    pi.parser = parser;
    parser->m_errorString = NULL;
    JsonParser_internalCreate(&pi);
    pi.startElem = JsonParser_startElem;
    pi.endElem = JsonParser_endElem;
    pi.elemData = JsonParser_elemData;
    parser->m_nodeStack = cpo_array_create(JSON_STACK_SIZE, sizeof(void*));
    if (JsonParser_internalParse(&pi, json, (int)strlen(json)) == JSON_ERR_NONE) {
        root = parser->m_root;
    } else {
        parser->m_errorString = (char*)jsonParser_errlist[pi.error];
        parser->m_errorLine = pi.line;
        parser->m_errorLineSet = 1;
        //printf("Parser error: %s in line %d\n", parser->m_errorString, pi.line);
    }
    DEBUG_PRINT("Parsed lines %d\n", pi.line);
    JsonParser_internalDelete(&pi);
    cpo_array_destroy(parser->m_nodeStack);
    DEBUG_PRINT("-end-\n");
    return root;
}

static void JsonParser_stripCommentsFromBuffer(char *buff, long size)
{
    long i;
    unsigned char is_string = 0;

    for (i = 0; i < size; i++) {
        if (buff[i] == Json_elem(JSON_QUOTE))
            is_string = !is_string;
        if (is_string)
            continue;
        /* strip // and # comments */
        if ((buff[i] == '/' && buff[i+1] == '/') || buff[i] == '#') {
            while(buff[i] != '\n' && buff[i] != 0) {
                buff[i] = ' ';
                i++;
            }
        }
    }
}

JsonNode * JsonParser_parseFile(struct JsonParser *parser, const char * fileName)
{
    JsonNode * root = NULL;
    FILE *f = fopen (fileName, "rb");

    if (f != NULL) {
        char * buffer;
        long length;
        size_t read = 0;
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = (char*) malloc (length + 1);
        if (buffer != NULL) {
            read = fread (buffer, sizeof(char), length, f);
            buffer[read] = '\0';
        }
        fclose (f);
        if (read == length) {
            JsonParser_stripCommentsFromBuffer(buffer, length);
            root = JsonParser_parse(parser,  buffer);
        } else {
            parser->m_errorString = strerror(errno);
            parser->m_errorLine = 0;
            parser->m_errorLineSet = 0;
        }
        free(buffer);
    } else {
        parser->m_errorString = strerror(errno);
        parser->m_errorLine = 0;
        parser->m_errorLineSet = 0;
        //printf("Error: Cannot read \"%s\"\n", fileName);
    }

    return root;
}
