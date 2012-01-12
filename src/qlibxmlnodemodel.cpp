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
 * This function is called by the QtXmlPatterns query engine when it
 * wants to move to the next node in the model. It moves along an \a
 * axis, \e from the node specified by \a nodeIndex.
 */
QXmlNodeModelIndex
QLibXmlNodeModel::nextFromSimpleAxis(SimpleAxis axis, const QXmlNodeModelIndex &nodeIndex) const
{
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
    return QUrl();
}

/*!
 * Returns a value indicating the kind of node identified by \a ni.
 * The caller guarantees that \a ni is not null and that it identifies
 * a node in this node model. This function maps to the \c
 * dm:node-kind() accessor.
 */
QXmlNodeModelIndex::NodeKind
QLibXmlNodeModel::kind(const QXmlNodeModelIndex &node) const
{
    return QXmlNodeModelIndex::Element;
}

/*!
 * This function returns the relative document order for the
 * nodes indexed by \a ni1 and \a ni2. It is used for the \c Is
 * operator and for sorting nodes in document order.
 */
QXmlNodeModelIndex::DocumentOrder QLibXmlNodeModel::compareOrder(const QXmlNodeModelIndex&, const QXmlNodeModelIndex&) const
{
    return QXmlNodeModelIndex::Precedes;
}

/*!
 * Returns the name of \a node. The caller guarantees that \a node is
 * not null and that it is contained in this node model.
 */
QXmlName QLibXmlNodeModel::name(const QXmlNodeModelIndex &node) const
{
    return QXmlName(namePool(), QString());
}

/*!
 * Returns the root node of the tree that contains the node whose index
 * is \a n. The caller guarantees that \a n is not \c null and that it
 * identifies a node in this node model.
 */
QXmlNodeModelIndex QLibXmlNodeModel::root(const QXmlNodeModelIndex &node) const
{
    Q_UNUSED(node);
    return node;
}

/*!
 * Returns the typed value for \a node, which must be either an
 * attribute or an element. The QVariant returned represents the atomic
 * value of an attribute or the atomic value contained in an element.
 *
 * If the QVariant is returned as a default constructed variant,
 * it means that \a node has no typed value.
 */
QVariant QLibXmlNodeModel::typedValue(const QXmlNodeModelIndex &node) const
{
    Q_ASSERT_X(false, Q_FUNC_INFO, "Invalid typed value.");
    return QString();
}

/*!
 * Returns the attributes of \a element. The caller guarantees
 * that \a element is an element in this node model.
 */
QVector<QXmlNodeModelIndex> QLibXmlNodeModel::attributes(const QXmlNodeModelIndex &element) const
{
    QVector<QXmlNodeModelIndex> result;

    return result;
}
