/*
 * qlibxmlnodemodel - A QAbstractXmlNodeModel for using with libxml2 library
 * Copyright (C) 2012 Alexey Torkhov
 * Copyright (C) 2011 Jonas Gehring
 * All rights reserved.
 *
 * Based on qhtmlnodemodel by Jonas Gehring
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holders nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <QDebug>

#include <libxml/HTMLparser.h>
#include <libxml/tree.h>

#include "qlibxmlnodemodel.h"

// Internal private data
class QLibXmlNodeModelPrivate
{
public:
    QLibXmlNodeModel *model;
    QUrl uri;

    xmlDoc *doc;

    QLibXmlNodeModelPrivate(QLibXmlNodeModel *model)
        : model(model), doc(NULL)
    {
    }

    ~QLibXmlNodeModelPrivate()
    {
        xmlFreeDoc(doc);
        doc = NULL;
    }

    // Parses the given source tree
    void parse(const QByteArray &source)
    {
        doc = htmlReadMemory(source.data(), source.size(), uri.toString().toUtf8(), "utf-8", 0);

        if (doc == NULL) {
            qDebug() << "could not parse source" << source;
        }
    }

    // Converts a model index to a HTML node
    xmlNode *toNode(const QXmlNodeModelIndex &index) const
    {
        return (xmlNode *)index.internalPointer();
    }

    // Converts a HTML node to a model index
    QXmlNodeModelIndex toNodeIndex(xmlNode *node) const
    {
        return model->createIndex((void *)node);
    }
    QXmlNodeModelIndex toNodeIndex(xmlDoc *node) const
    {
        return model->createIndex((void *)node);
    }
    QXmlNodeModelIndex toNodeIndex(xmlAttr *node) const
    {
        return model->createIndex((void *)node);
    }
};

void libXmlErrorFunc(void *ctx, const char *msg, ...)
{
   char buf[1024];
   va_list args;
   va_start(args, msg);
   vsnprintf(buf, 1024, msg, args);
   va_end(args);

   qDebug() << buf;
   return;
}

/*!
 * Constructor passes \a pool to the base class
 */
QLibXmlNodeModel::QLibXmlNodeModel(const QXmlNamePool& namePool, const QByteArray &source, const QUrl &uri)
    : QSimpleXmlNodeModel(namePool), d(new QLibXmlNodeModelPrivate(this))
{
    xmlSetGenericErrorFunc(NULL, libXmlErrorFunc);

    d->uri = uri;
    d->parse(source);
}

/*!
 * Destructor
 */
QLibXmlNodeModel::~QLibXmlNodeModel()
{
    delete d;
}

/*!
 * This function is called by the QtXmlPatterns query engine when it
 * wants to move to the next node in the model. It moves along an \a
 * axis, \e from the node specified by \a nodeIndex.
 */
QXmlNodeModelIndex
QLibXmlNodeModel::nextFromSimpleAxis(SimpleAxis axis, const QXmlNodeModelIndex &nodeIndex) const
{
    xmlNode *node = d->toNode(nodeIndex);
    //qDebug() << "nextFromSimpleAxis()" << node << axis;
    if (!node) {
        //qDebug() << "Invalid node";
        return QXmlNodeModelIndex();
    }

    switch (axis) {
        case Parent:
            //qDebug() << "Parent " << node->parent;
            if (!node->parent) {
                return QXmlNodeModelIndex();
            }
            return d->toNodeIndex(node->parent);

        case FirstChild:
            //qDebug() << "Child " << node->children;
            if (!node->children) {
                return QXmlNodeModelIndex();
            }
            return d->toNodeIndex(node->children);

        case PreviousSibling:
            //qDebug() << "Prev " << node->prev;
            if (!node->prev) {
                return QXmlNodeModelIndex();
            }
            return d->toNodeIndex(node->prev);

        case NextSibling:
            //qDebug() << "Next " << node->next;
            if (!node->next) {
                return QXmlNodeModelIndex();
            }
            return d->toNodeIndex(node->next);
    }

    Q_ASSERT_X(false, Q_FUNC_INFO, "Invalid axis!");
    return QXmlNodeModelIndex();
}

/*!
 * Returns the document URI of \a n. The document URI identifies the
 * resource which is the document. For example, the document could be a
 * regular file, e.g., \c{file:/}, or it could be the \c{http://} URL of
 * the location of a file. The document URI is used for resolving URIs
 * and to simply know where the document is.
 */
QUrl QLibXmlNodeModel::documentUri(const QXmlNodeModelIndex &node) const
{
    Q_UNUSED(node);
    return d->uri;
}

/*!
 * Returns a value indicating the kind of node identified by \a ni.
 * The caller guarantees that \a ni is not null and that it identifies
 * a node in this node model. This function maps to the \c
 * dm:node-kind() accessor.
 */
QXmlNodeModelIndex::NodeKind
QLibXmlNodeModel::kind(const QXmlNodeModelIndex &nodeIndex) const
{
    xmlNode *node = d->toNode(nodeIndex);
    //qDebug() << "kind()" << node;
    if (!node) {
        //qDebug() << "Invalid node";
        return QXmlNodeModelIndex::Document;
    }

    //qDebug() << "Node type" << node->type;

    switch (node->type) {
        case XML_ELEMENT_NODE:
            return QXmlNodeModelIndex::Element;
        case XML_ATTRIBUTE_NODE:
            return QXmlNodeModelIndex::Attribute;
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
            return QXmlNodeModelIndex::Text;
        case XML_COMMENT_NODE:
            return QXmlNodeModelIndex::Comment;
        case XML_DOCUMENT_NODE:
        case XML_HTML_DOCUMENT_NODE:
            return QXmlNodeModelIndex::Document;
        case XML_NAMESPACE_DECL:
            return QXmlNodeModelIndex::Namespace;
        case XML_PI_NODE:
            return QXmlNodeModelIndex::ProcessingInstruction;
        // Don't show message
        case XML_DTD_NODE:
            return QXmlNodeModelIndex::Text;
    }

    //qDebug() << "Node type is not handled properly in kind()" << node->type;
    return QXmlNodeModelIndex::Text;
}

/*!
 * This function returns the relative document order for the
 * nodes indexed by \a ni1 and \a ni2. It is used for the \c Is
 * operator and for sorting nodes in document order.
 */
QXmlNodeModelIndex::DocumentOrder QLibXmlNodeModel::compareOrder(const QXmlNodeModelIndex &nodeIndex1, const QXmlNodeModelIndex &nodeIndex2) const
{
    xmlNode *node1 = d->toNode(nodeIndex1);
    xmlNode *node2 = d->toNode(nodeIndex2);
    if (!node1) {
        //qDebug() << "Invalid node";
        return QXmlNodeModelIndex::Precedes;
    }
    if (!node2) {
        //qDebug() << "Invalid node";
        return QXmlNodeModelIndex::Follows;
    }

    xmlNode *cur;

    //qDebug() << "compareOrder()" << node1 << node2;

    // Check is nodes are root
    if (node1->parent == NULL) {
        return QXmlNodeModelIndex::Precedes;
    }
    if (node2->parent == NULL) {
        return QXmlNodeModelIndex::Follows;
    }

    // Find nodes which has common parent
    if (node1->parent != node2->parent) {
        QList<xmlNode*> tree1;
        QList<xmlNode*> tree2;

        // Build trees for node1 and node2
        for (cur = node1->parent; cur != NULL; cur = cur->parent) {
            tree1.push_front(cur);
        }
        for (cur = node2->parent; cur != NULL; cur = cur->parent) {
            tree2.push_front(cur);
        }

        // Find last common element in lists (common parent of nodes)
        QList<xmlNode*>::const_iterator iter1;
        QList<xmlNode*>::const_iterator iter2;
        node1 = node2 = NULL;
        for (iter1 = tree1.begin(), iter2 = tree2.begin(); iter1 != tree1.end() && iter2 != tree2.end(); ++iter1, ++iter2) {
            if (*iter1 != *iter2) {
                break;
            }
        }

        // One node is successor of another
        if (iter1 == tree1.end()) {
            return QXmlNodeModelIndex::Precedes;
        }
        if (iter2 == tree2.end()) {
            return QXmlNodeModelIndex::Follows;
        }

        // Asign nodes from nodes' tree which are children of common parent
        node1 = *iter1;
        node2 = *iter2;

        //qDebug() << tree1 << tree2 << node1 << node2;
        Q_ASSERT_X(node1 && node2, Q_FUNC_INFO, "Nodes has no common parent");
    }

    Q_ASSERT_X(node1 != node2, Q_FUNC_INFO, "Different nodes' parent is equal");
    Q_ASSERT_X(node1->parent == node2->parent, Q_FUNC_INFO, "Common parent is found improperly");
    Q_ASSERT_X(node1->parent->children, Q_FUNC_INFO, "Node parent has no children");

    // Find which node precedes
    for (cur = node1->parent->children; cur != NULL; cur = cur->next) {
        if (cur == node1) {
            return QXmlNodeModelIndex::Precedes;
        } else if (cur == node2) {
            return QXmlNodeModelIndex::Follows;
        }
    }

    Q_ASSERT_X(false, Q_FUNC_INFO, "Should not get here");
    return QXmlNodeModelIndex::Precedes;
}

/*!
 * Returns the name of \a node. The caller guarantees that \a node is
 * not null and that it is contained in this node model.
 */
QXmlName QLibXmlNodeModel::name(const QXmlNodeModelIndex &nodeIndex) const
{
    xmlNode *node = d->toNode(nodeIndex);
    //qDebug() << "name()" << node;
    if (!node) {
        //qDebug() << "Invalid node";
        return QXmlName(namePool(), QString());
    }

    //qDebug() << "Node name" << (const char *)node->name;
    return QXmlName(namePool(), QString::fromUtf8((const char *)node->name));
}

/*!
 * Returns the root node of the tree that contains the node whose index
 * is \a n. The caller guarantees that \a n is not \c null and that it
 * identifies a node in this node model.
 */
QXmlNodeModelIndex QLibXmlNodeModel::root(const QXmlNodeModelIndex &nodeIndex) const
{
    Q_UNUSED(nodeIndex);
    return d->toNodeIndex(d->doc);
}

/*!
 * Returns the typed value for \a node, which must be either an
 * attribute or an element. The QVariant returned represents the atomic
 * value of an attribute or the atomic value contained in an element.
 *
 * If the QVariant is returned as a default constructed variant,
 * it means that \a node has no typed value.
 */
QVariant QLibXmlNodeModel::typedValue(const QXmlNodeModelIndex &nodeIndex) const
{
    xmlNode *node = d->toNode(nodeIndex);
    //qDebug() << "typedValue()" << node;
    Q_ASSERT_X(node, Q_FUNC_INFO, "Invalid node");

    xmlChar *buf = xmlNodeGetContent(node);
    QString str = QString::fromUtf8((const char *)buf);
    xmlFree(buf);

    //qDebug() << "Attribute typed value" << str;
    return str;
}

/*!
 * Returns the attributes of \a element. The caller guarantees
 * that \a element is an element in this node model.
 */
QVector<QXmlNodeModelIndex> QLibXmlNodeModel::attributes(const QXmlNodeModelIndex &nodeIndex) const
{
    xmlNode *node = d->toNode(nodeIndex);
    //qDebug() << "attributes()" << node;
    Q_ASSERT_X(node, Q_FUNC_INFO, "Invalid node");

    QVector<QXmlNodeModelIndex> result;
    for (xmlAttr *cur = node->properties; cur != NULL; cur = cur->next) {
        result += d->toNodeIndex(cur);
    }

    return result;
}

/*!
 * Returns the string value for node \a n.
 *
 * The caller guarantees that \a n is not \c null and that it belong to
 * this QAbstractXmlNodeModel instance.
 *
 * This function maps to the \c dm:string-value() accessor, which the
 * specification completely specifies. Here's a summary:
 *
 * \list
 * \o For processing instructions, the string value is the data
 * section(excluding any whitespace appearing between the name and the
 * data).
 * \o For text nodes, the string value equals the text node.
 * \o For comments, the content of the comment
 * \o For elements, the concatenation of all text nodes that are
 * descendants. Note, this is not only the children, but the
 * childrens' childrens' text nodes, and so forth.
 * \o For document nodes, the concatenation of all text nodes in the
 * document.
 * \endlist
 */
QString QLibXmlNodeModel::stringValue (const QXmlNodeModelIndex &nodeIndex) const
{
    xmlNode *node = d->toNode(nodeIndex);
    //qDebug() << "stringValue()" << node;

    if (node->type == XML_TEXT_NODE ||
        node->type == XML_CDATA_SECTION_NODE ||
        node->type == XML_COMMENT_NODE) {
        xmlChar *buf = xmlNodeGetContent(node);
        QString str = QString::fromUtf8((const char *)buf);
        xmlFree(buf);

        return str;
    }

    if (node->type == XML_ELEMENT_NODE ||
        node->type == XML_ATTRIBUTE_NODE) {
        return QSimpleXmlNodeModel::stringValue(nodeIndex);
    }

    // TODO: handle other node types
    //qDebug() << "Node type is not handled properly in stringValue()" << node->type;

    return QString();
}
