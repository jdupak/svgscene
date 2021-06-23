/**
 * Data from the SVG specification.
 *
 * @file
 */
#pragma once

#include <QSet>

namespace svgscene { namespace svgspec {

    /**
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute
     */
    static const QSet<QString> presentation_attributes {
        // NOLINT(cert-err58-cpp)
        QStringLiteral("alignment-baseline"),
        QStringLiteral("baseline-shift"),
        QStringLiteral("clip"),
        QStringLiteral("clip-path"),
        QStringLiteral("clip-rule"),
        QStringLiteral("color"),
        QStringLiteral("color-interpolation"),
        QStringLiteral("color-interpolation-filters"),
        QStringLiteral("color-profile"),
        QStringLiteral("color-rendering"),
        QStringLiteral("cursor"),
        QStringLiteral("direction"),
        QStringLiteral("display"),
        QStringLiteral("dominant-baseline"),
        QStringLiteral("enable-background"),
        QStringLiteral("fill"),
        QStringLiteral("fill-opacity"),
        QStringLiteral("fill-rule"),
        QStringLiteral("filter"),
        QStringLiteral("flood-color"),
        QStringLiteral("flood-opacity"),
        QStringLiteral("font-family"),
        QStringLiteral("font-size"),
        QStringLiteral("font-size-adjust"),
        QStringLiteral("font-stretch"),
        QStringLiteral("font-style"),
        QStringLiteral("font-variant"),
        QStringLiteral("font-weight"),
        QStringLiteral("glyph-orientation-horizontal"),
        QStringLiteral("glyph-orientation-vertical"),
        QStringLiteral("image-rendering"),
        QStringLiteral("kerning"),
        QStringLiteral("letter-spacing"),
        QStringLiteral("lighting-color"),
        QStringLiteral("marker-end"),
        QStringLiteral("marker-mid"),
        QStringLiteral("marker-start"),
        QStringLiteral("mask"),
        QStringLiteral("opacity"),
        QStringLiteral("overflow"),
        QStringLiteral("pointer-events"),
        QStringLiteral("shape-rendering"),
        QStringLiteral("stop-color"),
        QStringLiteral("stop-opacity"),
        QStringLiteral("stroke"),
        QStringLiteral("stroke-dasharray"),
        QStringLiteral("stroke-dashoffset"),
        QStringLiteral("stroke-linecap"),
        QStringLiteral("stroke-linejoin"),
        QStringLiteral("stroke-miterlimit"),
        QStringLiteral("stroke-opacity"),
        QStringLiteral("stroke-width"),
        QStringLiteral("text-anchor"),
        QStringLiteral("text-decoration"),
        QStringLiteral("text-rendering"),
        QStringLiteral("transform"),
        QStringLiteral("transform-origin"),
        QStringLiteral("unicode-bidi"),
        QStringLiteral("vector-effect"),
        QStringLiteral("visibility"),
        QStringLiteral("word-spacing"),
        QStringLiteral("writing-mode"),
    };
}} // namespace svgscene::svgspec