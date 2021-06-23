#pragma once

#include <QGraphicsItem>
#include <QMap>
#include <QString>

namespace svgscene {

/*
 * Fields that can be found on SVG related `QGraphicsItem`s using the `data`
 * method.
 * Data are stored using QVariant (accessed by index).
 */
enum class MetadataType {
    XmlAttributes = 1,
    CssAttributes,
};

/**
 * Library user can store more values associated with the element. The library guarantees, that
 * any fields larger than this constant are at users disposal.
 */
constexpr MetadataType LAST_VALUE = MetadataType::CssAttributes;

using XmlAttributes = QMap<QString, QString>;

/**
 * Retrieve all XML attributes of a element (including CSS).
 *
 * @param element               DOM element
 * @throws std::out_of_range    if element has no XML data assigned (not even empty) (this happens,
 *                              when the element was not crated by the svgscene)
 */
XmlAttributes getXmlAttributes(const QGraphicsItem *element);

/**
 * Retrieve XML attribute of a element, throws if not found.
 *
 * @param element               DOM element
 * @param attr_name             XML attribute name
 * @throws std::out_of_range    when attribute is not present
 */
QString getXmlAttribute(const QGraphicsItem *element, const QString &name);

/**
 * Retrieve XML attribute of a element. Return default value if not found.
 *
 * @param element               DOM element
 * @param attr_name             XML attribute name
 * @param defaultValue          value to be returned, when attribute is not present
 */
QString getXmlAttributeOr(
    const QGraphicsItem *element,
    const QString &name,
    const QString &defaultValue) noexcept;

/**
 * CSS data of single element.
 *
 * TODO: make struct - number of attributes is fixed
 * TODO: decide, whether to store only supported attributes, in that case exposing css has
 *       probably no value
 * TODO: implement merge as method
 * TODO: use QStringRef for parsing values
 * TODO: all attributes names are known, they can be internalized
 */
using CssAttributes = QMap<QString, QString>;

/**
 * Retrieve all CSS attributes of a element.
 *
 * @param element               DOM element
 * @throws std::out_of_range    if element has no CSS data assigned (not even empty) (this happens,
 *                              when the element was not crated by the svgscene)
 */
CssAttributes getCssAttributes(const QGraphicsItem *element);

/**
 * Retrieve CSS attribute of a element, throws if not found.
 *
 * @param element               DOM element
 * @param attr_name             CSS attribute name
 * @throws std::out_of_range    when attribute is not present
 */
QString getCssValue(const QGraphicsItem *element, const QString &attr_name);

/**
 * Retrieve CSS attribute of a element. Return default value if not found.
 *
 * @param element               DOM element
 * @param attr_name             CSS attribute name
 * @param defaultValue          value to be returned, when attribute is not present
 */
QString getCssValueOr(
    const QGraphicsItem *element,
    const QString &attr_name,
    const QString &defaultValue) noexcept;

} // namespace svgscene

Q_DECLARE_METATYPE(svgscene::XmlAttributes)
// Not necessary, it refers to the same type signature as `XmlAttributes
//Q_DECLARE_METATYPE(svgscene::CssAttributes)
