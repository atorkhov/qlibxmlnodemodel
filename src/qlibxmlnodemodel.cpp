/*
 * qlibxmlnodemodel - A QAbstractXmlNodeModel that uses libxml2 library
 * Copyright (C) 2012 Alexey Torkhov
 * Copyright (C) 2011 Jonas Gehring
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Based on qhtmlnodemodel by Jonas Gehring and QT examples
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
    xmlNode *root_element;

    QLibXmlNodeModelPrivate(QLibXmlNodeModel *model)
        : model(model), doc(NULL), root_element(NULL)
    {
    }

    // Parses the given source tree
    void parse(const QByteArray &source)
    {
        doc = htmlReadMemory(source.data(), source.size(), uri.toString().toUtf8(), "utf-8", 0);

        if (doc == NULL) {
            qDebug() << "could not parse source" << source;
        }

        root_element = xmlDocGetRootElement(doc);
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
};

/*!
 * Constructor passes \a pool to the base class
 */
QLibXmlNodeModel::QLibXmlNodeModel(const QXmlNamePool& namePool, const QByteArray &source, const QUrl &uri)
    : QSimpleXmlNodeModel(namePool), d(new QLibXmlNodeModelPrivate(this))
{
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
    qDebug() << "nextFromSimpleAxis()" << node << axis;
    if (!node) {
        qDebug() << "Invalid node";
        return QXmlNodeModelIndex();
    }

    switch (axis) {
        case Parent:
            qDebug() << "Parent " << node->parent;
            if (!node->parent) {
                return QXmlNodeModelIndex();
            }
            return d->toNodeIndex(node->parent);

        case FirstChild:
            qDebug() << "Child " << node->children;
            if (!node->children) {
                return QXmlNodeModelIndex();
            }
            return d->toNodeIndex(node->children);

        case PreviousSibling:
            qDebug() << "Prev " << node->prev;
            if (!node->prev) {
                return QXmlNodeModelIndex();
            }
            return d->toNodeIndex(node->prev);

        case NextSibling:
            qDebug() << "Next " << node->next;
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
    qDebug() << "kind()" << node;
    if (!node) {
        qDebug() << "Invalid node";
        return QXmlNodeModelIndex::Element;
    }

    qDebug() << "Node type" << node->type;

    switch (node->type) {
        case XML_ELEMENT_NODE:
            return QXmlNodeModelIndex::Element;
        case XML_ATTRIBUTE_NODE:
            return QXmlNodeModelIndex::Attribute;
        case XML_TEXT_NODE:
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
    }

    qDebug() << "Invalid node type" << node->type;
    return QXmlNodeModelIndex::Element;
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
    qDebug() << "compareOrder()" << node1 << node2;
    return QXmlNodeModelIndex::Precedes;
}

/*!
 * Returns the name of \a node. The caller guarantees that \a node is
 * not null and that it is contained in this node model.
 */
QXmlName QLibXmlNodeModel::name(const QXmlNodeModelIndex &nodeIndex) const
{
    xmlNode *node = d->toNode(nodeIndex);
    qDebug() << "name()" << node;
    if (!node) {
        qDebug() << "Invalid node";
        return QXmlName(namePool(), QString());
    }

    qDebug() << "Node name" << (const char *)node->name;

    return QXmlName(namePool(), QString((const char *)node->name));
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
    qDebug() << "typedValue()" << node;
    if (!node) {
        qDebug() << "Invalid node";
        return QVariant();
    }

    qDebug() << "Node typed value" << (const char *)node->name;

    return QString((const char *)node->name);
}

/*!
 * Returns the attributes of \a element. The caller guarantees
 * that \a element is an element in this node model.
 */
QVector<QXmlNodeModelIndex> QLibXmlNodeModel::attributes(const QXmlNodeModelIndex &nodeIndex) const
{
    xmlNode *node = d->toNode(nodeIndex);
    qDebug() << "attributes()" << node;

    QVector<QXmlNodeModelIndex> result;

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
    qDebug() << "stringValue()" << node;

    if (node->type == XML_TEXT_NODE) {
        xmlChar *buf = xmlNodeGetContent(node);
        QString str((const char *)buf);
        xmlFree(buf);
        return str;
    }

    return QSimpleXmlNodeModel::stringValue(nodeIndex);
}
