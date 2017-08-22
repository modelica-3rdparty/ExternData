/* Simple DOM xml tree
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

#ifndef __BSXML_H
#define __BSXML_H

#include "expat.h"
#include "array.h"

#if defined(_MSC_VER)
#define strcasecmp _stricmp
#undef strdup
#define strdup _strdup
#if _MSC_VER < 1900
#define snprintf _snprintf
#endif
#elif defined(__MINGW32__)
#define strcasecmp _stricmp
#endif

enum { XML_NOK = 0, XML_OK };
enum { NODE_NONE, NODE_ROOT, NODE_CHILD };

#define String char*

typedef struct XmlNode * XmlNodeRef;
typedef struct XmlAttribute XmlAttribute;
typedef struct s_array XmlAttributes;
typedef struct s_array XmlStack;
typedef struct s_array XmlNodes;
typedef struct XmlNode XmlNode;
typedef struct XmlParser XmlParser;

struct XmlAttribute {
    String key;
    String value;
};

struct XmlParser
{
    XmlNodeRef  m_root;
    XML_Parser  m_parser;
    String      m_errorString;
    XmlStack   *m_nodeStack;
    XML_Size    m_errorLine;
    int         m_errorLineSet;
};

XmlNodeRef XmlParser_parse_file(XmlParser *parser,  const String fileName );
XmlNodeRef XmlParser_parse(XmlParser *parser,  const char * xml );
const String XmlParser_getErrorString(struct XmlParser *parser);
XML_Size XmlParser_getErrorLine(struct XmlParser *parser);
int XmlParser_getErrorLineSet(struct XmlParser *parser);

struct XmlNode {
    //node type
    int m_type;
    //line of node
    int m_line;
    //node xml tag
    String m_tag;
    //! Content of XML node.
    String m_content;
    //! Parent XML node.
    struct XmlNode *m_parent;
    //Xml childs
    XmlNodes *m_childs;
    //! Xml node attributes.
    XmlAttributes *m_attributes;
};

/*create root element */
struct XmlNode *XmlNode_Create( const String tag );
struct XmlNode *XmlNode_createChild(struct XmlNode *node, const String tag, const String text);
void XmlNode_deleteTree(struct XmlNode *root);
void XmlNode_delete(struct XmlNode *node);
//! Non virtual destructor, XmlNode cannot be used as a base class.
//! Get XML node tag.
String XmlNode_getTag(struct XmlNode * node);
//! Return true if givven tag equal to node tag.
int XmlNode_isTag( struct XmlNode * node, const String tag );
//! Get XML Node attributes.
XmlAttributes * XmlNode_getAttributes(struct XmlNode * node);
//! Get XML Node attribute for specified key.
XmlAttribute * XmlNode_getAttribute(struct XmlNode * node, const String key );
//! Get string value
String XmlNode_getAttributeValue(struct XmlNode *node, const String key);
//! Check if attributes with specified key exist.
int XmlNode_haveAttribute(struct XmlNode * node, const String key );
//! Set new XML Node attribute (or override attribute with same key).
void XmlNode_setAttribute(struct XmlNode * node, const String key, const String  value );
//! Get number of attributes.
asize_t XmlNode_getAttributesCount(struct XmlNode * node);
//! Adds new child node.
void XmlNode_addChild(struct XmlNode * node, const XmlNodeRef child );

//! Get number of child XML nodes.
asize_t XmlNode_getChildCount(struct XmlNode * node);

//! Get XML Node child nodes.
XmlNodeRef XmlNode_getChild(struct XmlNode * node, asize_t i );

//! Find node with specified tag.
XmlNodeRef XmlNode_findChild(struct XmlNode * node, const String tag );

//! Get parent XML node.
XmlNodeRef  XmlNode_getParent(struct XmlNode * node);

//! Returns content of this node.
String XmlNode_getContent(struct XmlNode * node);

//! Returns line number for XML tag.
int XmlNode_getLine(struct XmlNode * node);
//! Set line number in xml.
void XmlNode_setLine(struct XmlNode * node, int line );

//! Returns XML of this node and sub nodes.
String XmlNode_getXML(struct XmlNode * node);
void XmlNode_toFile(struct XmlNode *node, const char *fileName);
//! Get value of node.
void XmlNode_getValue(struct XmlNode * node, String *value );
void XmlNode_getValueInt(struct XmlNode * node, int *value );
void XmlNode_getValueFloat(struct XmlNode *node, float *value );

//! Set value of node.
void XmlNode_setValue(struct XmlNode * node, const String value );
void XmlNode_setValueInt(struct XmlNode * node, int value );
//void setValue( float value );

//! Get value of sub node.
int XmlNode_getSubNodeValue(struct XmlNode * node, const String tag, String *value );
int XmlNode_getSubNodeValueInt(struct XmlNode * node, const String tag, int *value );
int XmlNode_getSubNodeValueFloat(struct XmlNode * node, const String tag,float *value );

//! Set value of sub node (Create sub node if neccesarry).
void XmlNode_setSubNodeValue(struct XmlNode * node, const String tag, const String value );
void XmlNode_setSubNodeValueInt(struct XmlNode * node, const String tag, int value );
void XmlNode_setSubNodeValueFloat(struct XmlNode * node, const String tag, float value );

#endif // __BSXML_H
