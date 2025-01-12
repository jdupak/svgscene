﻿/**
 * SVG code parsing and processing.
 */

#pragma once

#include "svgdocument.h"
#include "svgmetadata.h"

#include <QFile>
#include <QMap>
#include <QPen>
#include <QStack>
#include <utility>

class QXmlStreamReader;
class QXmlStreamAttributes;
class QGraphicsScene;
class QGraphicsItem;
class QGraphicsSimpleTextItem;
class QGraphicsTextItem;
class QAbstractGraphicsShapeItem;

namespace svgscene {

/**
 * Entrypoint for svgscene usage taking file name.
 *
 * @param scene     scene where produced elements will be placed
 * @param filename  path to a SVG file
 * @return          an svg document, see `svgdocument.h`
 */
SvgDocument parseFromFileName(QGraphicsScene *scene, const QString &filename);

/**
 * Entrypoint for svgscene usage taking QFile handle.
 *
 * @param scene     scene where produced elements will be placed
 * @param filename  path to a SVG file
 * @return          an svg document, see `svgdocument.h`
 */
SvgDocument parseFromFile(QGraphicsScene *scene, QFile *file);

// TODO: make the handler private, i.e. not exposed a in header file.
//      The above entrypoint makes it uninteresting for library users.
class SvgHandler {
public:
    struct SvgElement {
        QString name;
        XmlAttributes xmlAttributes;
        CssAttributes styleAttributes;
        bool itemCreated = false;

        SvgElement() = default;
        explicit SvgElement(QString n, bool created = false)
            : name(std::move(n))
            , itemCreated(created) {}

        static SvgElement initial_element();
    };

public:
    explicit SvgHandler(QGraphicsScene *scene);
    virtual ~SvgHandler();

    void load(QXmlStreamReader *data, bool is_skip_definitions = false);

    static QString point2str(QPointF r);
    static QString rect2str(QRectF r);

    SvgDocument getDocument() const;

protected:
    virtual QGraphicsItem *createGroupItem(const SvgElement &el);
    QGraphicsItem *createHyperlinkItem(const SvgElement &el);
    virtual void installVisuController(QGraphicsItem *it, const SvgElement &el);
    virtual void setElementMetadata(QGraphicsItem *item, const SvgElement &svg_element);

    QGraphicsScene *m_scene;

private:
    void parse();
    static CssAttributes
    parseXmlAttributes(const QXmlStreamAttributes &attributes, CssAttributes &css);
    static void mergeCSSAttributes(
        CssAttributes &css_attributes,
        const QString &attr_name,
        const XmlAttributes &xml_attributes);

    static void setTransform(QGraphicsItem *it, const QString &str_val);
    static void setStyle(QAbstractGraphicsShapeItem *it, const CssAttributes &attributes);
    static void setTextStyle(QFont &font, const CssAttributes &attributes);
    static void setTextStyle(QGraphicsSimpleTextItem *text, const CssAttributes &attributes);
    static void setTextStyle(QGraphicsTextItem *text, const CssAttributes &attributes);

    bool startElement();
    void addItem(QGraphicsItem *it);

private:
    QGraphicsItem *root = nullptr;
    QStack<SvgElement> m_elementStack;
    QGraphicsItem *m_topLevelItem = nullptr;
    QXmlStreamReader *m_xml = nullptr;
    QPen m_defaultPen;
    bool m_skipDefinitions = false;

protected:
    /**
     * Allows subclasses of the handler to save more data to each graphics item.
     *
     * @param item          qt dom item
     * @param svg_element   svg processed element
     */
    virtual void setCustomElementMetadata(QGraphicsItem *item, const SvgElement &svg_element);
};

} // namespace svgscene