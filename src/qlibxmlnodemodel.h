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

#include <QSimpleXmlNodeModel>
#include <QVector>

class QLibXmlNodeModelPrivate;

class QLibXmlNodeModel : public QSimpleXmlNodeModel
{
    friend class QLibXmlNodeModelPrivate;

public:
    QLibXmlNodeModel(const QXmlNamePool&, const QByteArray&, const QUrl&);
    ~QLibXmlNodeModel();

    inline QXmlNodeModelIndex dom() const { return root(QXmlNodeModelIndex()); }

    virtual QXmlNodeModelIndex::DocumentOrder compareOrder(const QXmlNodeModelIndex&, const QXmlNodeModelIndex&) const;
    virtual QXmlName name(const QXmlNodeModelIndex&) const;
    virtual QUrl documentUri(const QXmlNodeModelIndex&) const;
    virtual QXmlNodeModelIndex::NodeKind kind(const QXmlNodeModelIndex&) const;
    virtual QXmlNodeModelIndex root(const QXmlNodeModelIndex&) const;
    virtual QVariant typedValue(const QXmlNodeModelIndex&) const;
    virtual QString stringValue (const QXmlNodeModelIndex&) const;

protected:
    virtual QVector<QXmlNodeModelIndex> attributes(const QXmlNodeModelIndex&) const;
    virtual QXmlNodeModelIndex nextFromSimpleAxis(SimpleAxis, const QXmlNodeModelIndex&) const;

private:
    QLibXmlNodeModelPrivate *d;
};

