/* bsxml.c - Simple DOM xml tree
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
#include <ctype.h>
#include <errno.h>
#define oom() break
#include "utstring.h"
#include "bsxml.h"
/* initial size */
#define XMLTREE_CHILDSIZE   8
#define XMLTREE_ATTRSIZE    4
#define XMLTREE_STACKSIZE   32

#define ENC_TYPE_UTF8   "UTF-8"

#define isNullorEmpty(str) \
    (str == NULL || str[0] == '\0')

#define isAlpha(c) \
    ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))

#define isDigit(c) \
    (c >= '0' && c <= '9')

#define isAlphaNumeric(c) \
    (isDigit(c) || isAlpha(c))

XmlNode * XmlNode_Create(const String tag)
{
    XmlNode * node = (XmlNode *) malloc( sizeof(struct XmlNode) );
    if (!node) return NULL;
    node->m_tag = strdup( tag );
    node->m_type = NODE_ROOT;
    node->m_parent = 0;
    node->m_content = NULL;
    node->m_childs = cpo_array_create(XMLTREE_CHILDSIZE, sizeof(struct XmlNode));
    node->m_attributes = cpo_array_create(XMLTREE_ATTRSIZE, sizeof( struct XmlAttribute) );
    return node;
}

XmlAttributes *XmlNode_getAttributes(struct XmlNode * node)
{
    return node->m_attributes;
}

asize_t XmlNode_getAttributesCount(struct XmlNode * node)
{
    return node->m_attributes->num;
}

String XmlNode_getTag(struct XmlNode * node)
{
    return node->m_tag;
}

asize_t XmlNode_getChildCount(struct XmlNode * node)
{
    return node->m_childs->num;
}

XmlNodeRef XmlNode_getParent(struct XmlNode * node)
{
    return node->m_parent;
}

String XmlNode_getContent(struct XmlNode * node)
{
    return node->m_content;
}

int XmlNode_getLine(struct XmlNode * node)
{
    return node->m_line;
}

void XmlNode_setLine(struct XmlNode * node, int line )
{
    node->m_line = line;
}

void XmlNode_delete(struct XmlNode *node)
{
    asize_t i;
    if (node == NULL) return;
    //printf("delete %s\n", node->m_tag);
    for (i=0; i < node->m_attributes->num; i++) {
        XmlAttribute *attr = cpo_array_get_at(node->m_attributes, i);
        free(attr->key);
        free(attr->value);
    }

    if (node->m_childs) {
        cpo_array_destroy(node->m_childs);
    }

    if (node->m_attributes) {
        cpo_array_destroy(node->m_attributes);
    }

    if (node->m_content)
        free(node->m_content);

    free(node->m_tag);
}

void XmlNode_deleteTree(struct XmlNode *root)
{
    asize_t i;
    if (root == NULL) return;
    for (i=0 ; i < root->m_childs->num; i++) {
        XmlNode *node = cpo_array_get_at(root->m_childs, i);
        XmlNode_deleteTree(node);
    }

    XmlNode_delete(root);

    if (root->m_type == NODE_ROOT) {
        free(root);
    }
}

void XmlNode_print(struct XmlNode *root)
{
    asize_t i;
    for (i=0 ; i < root->m_childs->num; i++) {
        XmlNode *node = cpo_array_get_at(root->m_childs, i);
        XmlNode_print(node);
    }

    printf("Node %s content:%s", root->m_tag, root->m_content != NULL ? root->m_content : NULL);
}

int XmlNode_isTag(struct XmlNode *node, const String tag )
{
    return strcasecmp(tag, node->m_tag) == XML_NOK;
}

static int XmlAttribute_comparer(const void *a, const void *b)
{
    return strcmp(((XmlAttribute *) a)->key, ((XmlAttribute *) b)->key);
}

XmlAttribute *XmlNode_getAttribute(struct XmlNode *node, const String key)
{
    XmlAttribute a;
    a.key = (String)key;
    return (XmlAttribute*)cpo_array_bsearch(node->m_attributes, &a, XmlAttribute_comparer);
}

String XmlNode_getAttributeValue(struct XmlNode *node, const String key)
{
    String value = NULL;
    XmlAttribute *attr = XmlNode_getAttribute(node, key);
    if(attr) {
        value = attr->value;
    }

    return value;
}

int XmlNode_haveAttribute(struct XmlNode *node, const String key )
{
    return ( XmlNode_getAttribute(node, key ) ) ?  XML_OK : XML_NOK;
}

void XmlNode_setAttribute(struct XmlNode *node, const String key, const String value )
{
    XmlAttribute *a = (XmlAttribute *)cpo_array_push( node->m_attributes );
    a->key =  strdup(key);
    a->value =  strdup(value);
}

static int XmlNode_comparer(const void *a, const void *b)
{
    return strcmp(((XmlNode *) a)->m_tag, ((XmlNode *) b)->m_tag);
}

XmlNodeRef XmlNode_findChild(struct XmlNode *node, const String tag )
{
    XmlNode tmpNode = {NODE_CHILD, 0, (String)tag};
    XmlNodeRef ret = (XmlNodeRef)cpo_array_bsearch(node->m_childs, &tmpNode, XmlNode_comparer);
    return ret;
}

XmlNode * XmlNode_createChild(struct XmlNode *node, const String tag, const String text)
{
    XmlNodeRef child = (XmlNodeRef)cpo_array_push( node->m_childs );
    child->m_tag = strdup( tag );
    child->m_type = NODE_CHILD;
    child->m_content = NULL;
    child->m_parent = node;
    if (text) {
        XmlNode_setValue(child, text );
    }

    child->m_childs = cpo_array_create(XMLTREE_CHILDSIZE , sizeof(struct XmlNode));
    child->m_attributes = cpo_array_create(XMLTREE_ATTRSIZE , sizeof( struct XmlAttribute) );
    return child;
}

void XmlNode_addChild(struct XmlNode *node, const XmlNodeRef child )
{
    XmlNodeRef ref = (XmlNodeRef)cpo_array_push( node->m_childs );
    if (ref) {
        *ref = *child;
        ref->m_parent = node;
    }
}

XmlNodeRef XmlNode_getChild(struct XmlNode *node, asize_t i)
{
    assert( i < node->m_childs->num );
    return (XmlNodeRef)cpo_array_get_at(node->m_childs, i);
}

void XmlNode_getValue(struct XmlNode *node, String *value )
{
    *value = node->m_content;
}

void XmlNode_getValueInt(struct XmlNode *node, int *value )
{
    *value = atoi(node->m_content);
}

void XmlNode_getValueFloat(struct XmlNode *node, float *value )
{
    *value = (float)atof(node->m_content);
}

static int XmlNode_isEmptyValue(const char * s)
{
    const char *p;
    for (p=s; *p != '\0'; p++) {
        if ( *p != ' ' && *p != '\r' && *p != '\n' && *p != '\t' ) {
            return XML_NOK;
        }
    }
    return XML_OK;
}

void XmlNode_setValue(struct XmlNode *node, const String value )
{
    if (!isNullorEmpty(value) && !XmlNode_isEmptyValue(value)) {
        //printf("value for %s = '%s'\n",node->m_tag, value );
        if (node->m_content != NULL) {
            size_t end = strlen(node->m_content);
            size_t len = strlen(value);
            unsigned last = isAlphaNumeric(*(node->m_content + end-1)) ? 2:1;
            char *new = realloc(node->m_content,(end + len + last));
            if (new == NULL) return;
            node->m_content = new;

            if (last == 2) {
                strcat(node->m_content, " ");
                end+=1;
            }
            strncpy(node->m_content + end, value, len+1);
        } else {
            node->m_content = strdup(value);
        }
    }
}

void XmlNode_setValueInt(struct XmlNode *node, int value )
{
    char str[128];
    snprintf( str, sizeof(str),"%d",value );
    XmlNode_setValue(node, str);
}

void XmlNode_setValueFloat(struct XmlNode *node, float value )
{
    char str[128];
    snprintf( str,sizeof(str),"%f",value );
    XmlNode_setValue(node, str);
}

int XmlNode_getSubNodeValue(struct XmlNode *node, const String tag, String *value )
{
    XmlNodeRef child = XmlNode_findChild(node, tag );
    if (child) {
        XmlNode_getValue(child, value );
        return XML_OK;
    }
    return XML_NOK;
}

int XmlNode_getSubNodeValueInt(struct XmlNode *node, const String tag, int *value )
{
    XmlNodeRef child = XmlNode_findChild(node, tag );
    if (child) {
        XmlNode_getValueInt(child, value );
        return XML_OK;
    }
    return XML_NOK;
}

int XmlNode_getSubNodeValueFloat(struct XmlNode *node, const String tag, float *value )
{
    XmlNodeRef child = XmlNode_findChild(node, tag );
    if (child) {
        XmlNode_getValueFloat(child, value );
        return XML_OK;
    }
    return XML_NOK;
}

void XmlNode_setSubNodeValue(struct XmlNode *node, const String tag, const String value )
{
    XmlNodeRef child = XmlNode_findChild(node, tag );
    if (!child) {
        child = XmlNode_createChild(node, tag, NULL);
    }

    XmlNode_setValue(child, value);
}

void XmlNode_setSubNodeValueInt(struct XmlNode *node, const String tag, int value )
{
    XmlNodeRef child = XmlNode_findChild(node, tag );
    if (!child) {
        child = XmlNode_createChild(node, tag, NULL);
    }

    XmlNode_setValueInt(child, value);
}

void XmlNode_setSubNodeValueFloat(struct XmlNode *node, const String tag, float value )
{
    XmlNodeRef child = XmlNode_findChild(node, tag );
    if (!child) {
        child = XmlNode_createChild(node, tag, NULL);
    }

    XmlNode_setValueFloat(child, value);
}
/* return allocated UT_string */
static UT_string *XmlNode_getXML_UT(struct XmlNode *node)
{
    asize_t i;
    UT_string *buff;
    utstring_new(buff);
    if (buff == NULL) return NULL;
    if (node->m_attributes->num == 0) {
        utstring_printf(buff, "<%s>", node->m_tag);
    } else {
        utstring_printf(buff, "<%s ",node->m_tag);
        // Put attributes.
        for (i =0; i< node->m_attributes->num; i++) {
            XmlAttribute *a = cpo_array_get_at(node->m_attributes, i);
            utstring_printf(buff, "%s =\"%s\" ",a->key , a->value);
        }

        if (isNullorEmpty(node->m_content) && !node->m_childs->num) {
            utstring_printf(buff, "/>\n");
            return buff;
        }
        utstring_printf(buff, ">");
    }

    if (node->m_childs->num) {
        utstring_printf(buff, "\n");
    }

    if (!isNullorEmpty(node->m_content)) {
        size_t pos;
        char *p = node->m_content;
        for (pos = 0; pos < strlen(node->m_content); ++pos) {
            switch (p[pos]) {
            case '&':
                utstring_printf(buff,"&amp;");
                break;
            case '\"':
                utstring_printf(buff,"&quot;");
                break;
            case '\'':
                utstring_printf(buff,"&apos;");
                break;
            case '<':
                utstring_printf(buff,"&lt;");
                break;
            case '>':
                utstring_printf(buff,"&gt;");
                break;
            default:
                utstring_printf(buff,"%c", p[pos]);
                break;
            }
        }
    }

    for (i = 0; i < node->m_childs->num; i++) {
        XmlNodeRef child = cpo_array_get_at(node->m_childs, i);
        UT_string* childXML = XmlNode_getXML_UT(child);
        if (childXML != NULL) {
            utstring_concat(buff, childXML);
            utstring_free(childXML);
        }
    }

    // End tag.
    utstring_printf(buff, "</%s>\n", node->m_tag);
    return buff;
}
/* return allocated string */
String XmlNode_getXML(struct XmlNode *node)
{
    UT_string *buff = XmlNode_getXML_UT(node);
    return utstring_release(buff);
}

void XmlNode_toFile(struct XmlNode *node, const char *fileName)
{
    FILE *f = fopen (fileName, "w+b");
    if (f != NULL) {
        UT_string *buff = XmlNode_getXML_UT(node);
        if (buff != NULL) {
            if (utstring_body(buff) != NULL) {
                fprintf(f, "<?xml version=\"1.0\" encoding=\"%s\"?>\n", ENC_TYPE_UTF8);
                fwrite(utstring_body(buff), sizeof(char), utstring_len(buff), f);
            }
            utstring_free(buff);
        }
        fclose (f);
    } else {
        //printf("Error: Cannot write to \"%s\"\n", fileName);
    }
}
/*parser */
static void startElement(void *userData, const char *name, const char **atts)
{
    asize_t i = 0;
    void *ptr = NULL;
    XmlNodeRef parent= NULL, node=NULL;
    XmlParser *parser = (XmlParser *)userData;

    if (parser->m_nodeStack->num > 0) {
        ptr = stack_back(parser->m_nodeStack);
        parent = (XmlNodeRef) ARR_VAL(ptr);
    } else {
        parser->m_root = XmlNode_Create((const String) name);
    }

    if (parent) {
        node = XmlNode_createChild(parent, (const String)name, NULL);
    } else {
        node = parser->m_root;
    }

    ptr = stack_push_back(parser->m_nodeStack);
    if (ptr != NULL) {
        ARR_VAL(ptr) = ARR_VAL2PTR(node);
    }

    XmlNode_setLine(node, XML_GetCurrentLineNumber( parser->m_parser ) );

    // Call start element callback.
    while (atts[i] != 0) {
        XmlNode_setAttribute(node, (const String)atts[i], (const String)atts[i+1] );
        i += 2;
    }
}

static void endElement(void *userData, const char *name )
{
    XmlParser *parser = (XmlParser *)userData;

    assert( parser->m_nodeStack->num > 0 );
    if (parser->m_nodeStack->num > 0) {
        stack_pop_back(parser->m_nodeStack);
    }
}

static void characterData( void *userData, const char *s, int len )
{
    XmlParser *parser = (XmlParser *)userData;

    if (parser->m_nodeStack->num > 0) {
        void *ptr = stack_back(parser->m_nodeStack);
        XmlNode *node = (XmlNode*) ARR_VAL(ptr);
        char *str = (char*)malloc(len + 1);
        if (!str) return;
        strncpy(str,s,len);
        str[len] = 0;
        XmlNode_setValue(node, str);
        free(str);
    }
}

const String XmlParser_getErrorString(struct XmlParser *parser)
{
    return parser->m_errorString;
}

XML_Size XmlParser_getErrorLine(struct XmlParser *parser)
{
    return parser->m_errorLine;
}

int XmlParser_getErrorLineSet(struct XmlParser *parser)
{
    return parser->m_errorLineSet;
}

/* return root elem */
XmlNodeRef XmlParser_parse(XmlParser *parser,  const char * xml )
{
    XmlNodeRef root = NULL;
    parser->m_errorString = NULL;
    parser->m_nodeStack= cpo_array_create(XMLTREE_STACKSIZE, sizeof(void*));
    /*expat parser*/
    parser->m_parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser->m_parser, parser );
    XML_SetElementHandler(parser->m_parser, startElement, endElement );
    XML_SetCharacterDataHandler(parser->m_parser, characterData );

    if (XML_Parse(parser->m_parser, xml, (int)strlen(xml), XML_TRUE)) {
        root = parser->m_root;
    } else {
        parser->m_errorString = (char*)XML_ErrorString(XML_GetErrorCode(parser->m_parser));
        parser->m_errorLine = XML_GetCurrentLineNumber(parser->m_parser);
        parser->m_errorLineSet = 1;
        //printf("XML Error: %s at line %ld\n",
        //    XmlParser_getErrorString(parser),
        //    XML_GetCurrentLineNumber(parser->m_parser));
    }

    XML_ParserFree(parser->m_parser);
    cpo_array_destroy(parser->m_nodeStack);
    return root;
}

XmlNodeRef XmlParser_parse_file(struct XmlParser *parser,  const String fileName )
{
    XmlNodeRef root = NULL;
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
            root = XmlParser_parse(parser,  buffer);
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
