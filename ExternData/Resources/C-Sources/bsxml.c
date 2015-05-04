/* bsxml.c - Simple DOM xml tree
 *
 * Copyright (C) 2014 Borislav Sapundzhiev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or (at
 * your option) any later version.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "bsstr.h"
#include "bsxml.h"
#include "ModelicaUtilities.h"
/* initial size */
#define XMLTREE_CHILDSIZE	8
#define XMLTREE_ATTRSIZE 	4
#define XMLTREE_STACKSIZE 	32

#define ENC_TYPE_UTF8 	"UTF-8"

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

int XmlNode_getAttributesCount(struct XmlNode * node)
{
    return node->m_attributes->num;
}

String XmlNode_getTag(struct XmlNode * node)
{
    return node->m_tag;
}

int	XmlNode_getChildCount(struct XmlNode * node)
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
    int i;
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
    int i;
    if (root == NULL) return;
    for (i=0 ; i < root->m_childs->num; i++) {
        XmlNode *node = cpo_array_get_at(root->m_childs, i);
        XmlNode_deleteTree(node);
    }

    XmlNode_delete(root);

    if (root->m_type == NODE_ROOT) {
        free(root);
        root = NULL;
    }
}

void XmlNode_print(struct XmlNode *root)
{
    int i;
    for (i=0 ; i < root->m_childs->num; i++) {
        XmlNode *node = cpo_array_get_at(root->m_childs, i);
        XmlNode_print(node);
    }

    printf("Node %s content:%s", root->m_tag , root->m_content != NULL ? root->m_content : NULL);
}

int XmlNode_isTag(struct XmlNode *node, const String tag )
{
    return strcasecmp(tag, node->m_tag) == XML_NOK;
}

static int XmlAttribute_comparer(const void *a, const void *b)
{
    return strcmp(((XmlAttribute *) a)->key, ((XmlAttribute *) b)->key);
}

String XmlNode_getAttribute(struct XmlNode *node, const String key )
{
    XmlAttribute a;
    a.key = (String)key;
    return (String)cpo_array_bsearch(node->m_attributes, &a, XmlAttribute_comparer);
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

XmlNodeRef XmlNode_getChild(struct XmlNode *node, int i)
{
    assert( i >= 0 && i < node->m_childs->num );
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
            if (!new) return;
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
        XmlNode_getValue(child , value );
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
    if (node) {
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
/* return allocated string */
String XmlNode_getXML(struct XmlNode *node)
{
    String xmlStr = NULL;
    bsstr *buff = bsstr_create("");
    int i;
    if (node->m_attributes->num == 0) {
        bsstr_printf(buff, "<%s>\n", node->m_tag);
    } else {
        bsstr_printf(buff, "<%s ",node->m_tag);
        // Put attributes.
        for (i =0; i< node->m_attributes->num; i++) {
            XmlAttribute *a = cpo_array_get_at(node->m_attributes, i);
            bsstr_printf(buff, "%s =\"%s\" ",a->key , a->value);
        }

        if (isNullorEmpty(node->m_content) && !node->m_childs->num) {
            bsstr_printf(buff, "/>\n");
            xmlStr = bsstr_release(buff);
            return xmlStr;
        }
        bsstr_printf(buff, ">\n");
    }

    if (!isNullorEmpty(node->m_content)) {

        size_t pos;
        char *p = node->m_content;
        for (pos = 0; pos < strlen(node->m_content); ++pos) {
            switch (p[pos]) {
            case '&':
                bsstr_printf(buff,"&amp;");
                break;
            case '\"':
                bsstr_printf(buff,"&quot;");
                break;
            case '\'':
                bsstr_printf(buff,"&apos;");
                break;
            case '<':
                bsstr_printf(buff,"&lt;");
                break;
            case '>':
                bsstr_printf(buff,"&gt;");
                break;
            default:
                bsstr_printf(buff,"%c", p[pos]);
                break;
            }
        }
        bsstr_printf(buff,"\n");
    }

    for (i = 0; i < node->m_childs->num; i++) {
        XmlNodeRef child = cpo_array_get_at(node->m_childs, i);
        String childXML = XmlNode_getXML(child);
        bsstr_add(buff, childXML);
        free(childXML);
    }

    // End tag.
    bsstr_printf(buff, "</%s>\n", node->m_tag);
    xmlStr = bsstr_release(buff);
    return xmlStr;
}

void XmlNode_toFile(struct XmlNode *node, const char *fileName)
{
    FILE *f = fopen (fileName, "w+b");
    if (f) {
        char *buffer = XmlNode_getXML(node);
        size_t len = strlen(buffer);

        if (buffer) {
            fprintf(f, "<?xml version=\"1.0\" encoding=\"%s\"?>", ENC_TYPE_UTF8);
            fwrite(buffer, sizeof(char),len , f);
            free(buffer);
        }
        fclose (f);
    } else {
        ModelicaFormatError("Error: Cannot write to \"%s\"\n", fileName);
    }
}
/*parser */
static void startElement(void *userData, const char *name, const char **atts)
{
    int i = 0;
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
        node = XmlNode_createChild(parent, (const String)name , NULL);
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
    char *str = (char*)malloc(len + 1);
    if (!str) return;
    strncpy(str,s,len );
    str[len] = 0;
    if (parser->m_nodeStack->num > 0) {
        void *ptr =  stack_back(parser->m_nodeStack);
        XmlNode *node = (XmlNode*) ARR_VAL(ptr);
        XmlNode_setValue(node, str);
        free(str);
    }
}

String XmlParser_getErrorString(struct XmlParser *parser)
{
    return parser->m_errorString;
}

/* return root elem */
XmlNodeRef XmlParser_parse(XmlParser *parser,  const char * xml )
{
    XmlNodeRef root = NULL;
    parser->m_nodeStack= cpo_array_create(XMLTREE_STACKSIZE, sizeof(void*));
    /*expat parser*/
    parser->m_parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser->m_parser, parser );
    XML_SetElementHandler(parser->m_parser, startElement, endElement );
    XML_SetCharacterDataHandler(parser->m_parser, characterData );

    if (XML_Parse(parser->m_parser, xml, strlen(xml), XML_TRUE)) {
        root = parser->m_root;
    } else {
        ModelicaFormatError("XML Error: %s at line %ld\n",
               XML_ErrorString(XML_GetErrorCode(parser->m_parser)),
               XML_GetCurrentLineNumber(parser->m_parser));
    }

    XML_ParserFree(parser->m_parser);
    cpo_array_destroy(parser->m_nodeStack);
    return root;
}

XmlNodeRef XmlParser_parse_file(struct XmlParser *parser,  const String fileName )
{
    char * buffer = 0;
    long length =0;
    XmlNodeRef root = NULL;
    FILE *f = fopen (fileName, "rb");

    if (f) {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = (char*) malloc (length + 1);
        if (buffer) {
            fread (buffer, sizeof(char), length, f);
            buffer[length] = '\0';
        }
        fclose (f);
        root = XmlParser_parse(parser,  buffer);
        free(buffer);
    } else {
        ModelicaFormatError("Error: Cannot read \"%s\"\n", fileName);
    }

    return root;
}
