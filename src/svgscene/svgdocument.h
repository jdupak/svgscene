/**
 * Entrypoint to SVG dom.
 *
 * Basic implementation only contains only a root subtree of the DOM, however more advanced
 * implementations may contain extra functionalities, such as hashmaps by `id`, `class` or other
 * attributes to speedup the search. The document should then override the search functions (this
 * behavior is not implemented).
 *
 * ## Document traversing API
 * The key component of the API is SvgDomTree, which wraps the QGraphicsItems inSVG aware wrapper
 * . SvgDomTree has methods to read XML and CSS attributes and their values and methods for
 * searching the scene/document tree. Methods find and findAll search child elements using a
 * naive depth-first-search4 and return first, and all matching elements, respectively, wrapped
 * as SvgDomTrees. The find methods accept the type of searched element as a template parameter,
 * and the XML attribute name and value to search as function parameters. All parameters are
 * optional. The default value for the template parameter is QGraphicsItem. The wrapped Qt object
 * can be obtained by a call to the getElement method.
 *
 * @file
 */
#pragma once

#include "svgmetadata.h"

#include <QGraphicsItem>

namespace svgscene {

/**
 * A tree of SVG DOM where each child node can form subtree. This allows chaining of traverse
 * operations.
 * @tparam TT     type of root node of the tree
 */
template<typename TT>
class SvgDomTree {
public:
    explicit SvgDomTree(QGraphicsItem *root);

    /**
     * Unwrap the element from tree. See `find` method example.
     *
     * ## Example
     * ```
     *  auto elem = document.getRoot().getElement();
     * ```

     */
    TT *getElement() const;

    /**
     * Retrieve XML attribute of a element. Return default value if not found.
     *
     * @param element               DOM element
     * @param attr_name             XML attribute name
     * @param defaultValue          value to be returned, when attribute is not present
     */
    QString getAttrValueOr(const QString &attr_name, const QString &default_value = QString());

    /**
     * Retrieve CSS attribute of a element. Return default value if not found.
     *
     * @param element               DOM element
     * @param attr_name             CSS attribute name
     * @param defaultValue          value to be returned, when attribute is not present
     */
    QString getCssValueOr(const QString &attr_name, const QString &default_value = QString());

    /**
     * Search for first occurrences in the subtree, parametrized by any subset od type, attribute
     * name and attribute value.
     *
     * **IMPORTANT:** Order is not guaranteed. Currently used search is BFS.
     *
     * **IMPORTANT:** If attribute name is empty, attribute value is not evaluated at all.
     *
     * @tparam T          type of element to search, QGraphicsItem corresponds to ANY
     * @param attr_name   required attribute name, empty string corresponds to ANT
     * @param attr_value  required attribute value, empty string corresponds to ANY
     * @return            list of found elements wrapped in DOM trees.
     *
     * ## Example
     * ```
     *  document
     *  .getRoot()
     *  .find<QGraphicsItem>("data-component", "data-cache")
     *  .find<SimpleTextItem>()
     *  .getElement();
     * ```
     */
    template<typename T = QGraphicsItem>
    SvgDomTree<T> find(const QString &attr_name = QString(), const QString &attr_value = QString());

    /**
     * Search for all occurrences in the subtree, parametrized by any subset od type, attribute
     * name and attribute value.
     *
     * **IMPORTANT:** If attribute name is empty, attribute value is not evaluated at all.
     *
     * @tparam T          type of element to search, QGraphicsItem corresponds to ANY
     * @param attr_name   required attribute name, empty string corresponds to ANT
     * @param attr_value  required attribute value, empty string corresponds to ANY
     * @return            list of found elements wrapped in DOM trees.
     *
     * ## Example
     * ```
     *  for (auto hyperlink : document.getRoot().findAll<HyperlinkItem>()) {
     *    this->install_hyperlink(hyperlink.getElement());
     *  }
     * ```
     */
    template<typename T = QGraphicsItem>
    QList<SvgDomTree<T>>
    findAll(const QString &attr_name = QString(), const QString &attr_value = QString());

protected:
    /**
     * Alternative find method using `nullptr` as failure reporting mechanism and skips
     * construction of the tree object.
     *
     * ## WHY?
     * WASM does not allows exception catching so we have to handle recoverable errors in a
     * different way - using nullptr.
     *
     * ## NOTE:
     * `findAll` does not need this kind of method as a list can express the absence of result
     * itself.
     */
    template<typename T>
    static T *findFromParentRaw(
        const QGraphicsItem *parent,
        const QString &attr_name = QString(),
        const QString &attr_value = QString());

protected:
    TT *root;
};

/**
 * Simplest implementation of an SVG document. See file description for more details.
 */
class SvgDocument {
public:
    explicit SvgDocument(QGraphicsItem *root);
    SvgDomTree<QGraphicsItem> getRoot() const;

protected:
    SvgDomTree<QGraphicsItem> root;
};

// IMPLEMENTATION OF TEMPLATE FUNCTIONS BELLOW

template<typename T>
bool itemMatchesSelector(
    const QGraphicsItem *item,
    const QString &attr_name,
    const QString &attr_value) {
    if (item == nullptr) {
        throw std::out_of_range("Supplied item is nullptr.");
    }
    if (attr_name.isEmpty()) {
        return true;
    }

    auto attrs = getXmlAttributes(item);
    if (!attrs.contains(attr_name)) {
        return false;
    }
    return attr_value.isEmpty() || attrs.value(attr_name) == attr_value;
}

template<typename T>
SvgDomTree<T>::SvgDomTree(QGraphicsItem *root) : root(dynamic_cast<T *>(root)) {
    if (this->root == nullptr) {
        throw std::out_of_range("Cannot build dom tree with nullptr item.");
    }
}

template<typename TT>
TT *SvgDomTree<TT>::getElement() const {
    return root;
}

template<typename T>
QString SvgDomTree<T>::getAttrValueOr(const QString &attr_name, const QString &default_value) {
    XmlAttributes attrs = getXmlAttributes(root);
    return attrs.value(attr_name, default_value);
}

template<typename T>
QString SvgDomTree<T>::getCssValueOr(const QString &attr_name, const QString &default_value) {
    // This is another function defined in the `svgmetedata.h`, the is no recursion in here.
    return svgscene::getCssValueOr(root, attr_name, default_value);
}

template<typename TT>
template<typename T>
SvgDomTree<T> SvgDomTree<TT>::find(const QString &attr_name, const QString &attr_value) {
    if (!root) {
        throw std::out_of_range("Current element is nullptr.");
    }

    for (QGraphicsItem *_child : root->childItems()) {
        if (T *child = dynamic_cast<T *>(_child)) {
            if (itemMatchesSelector<T>(child, attr_name, attr_value)) {
                return SvgDomTree<T>(child);
            }
        }

        T *found = findFromParentRaw<T>(_child, attr_name, attr_value);
        if (found != nullptr) {
            return SvgDomTree<T>(found);
        }
    }
    throw std::out_of_range("Not found.");
}
template<typename TT>
template<typename T>
QList<SvgDomTree<T>> SvgDomTree<TT>::findAll(const QString &attr_name, const QString &attr_value) {
    QList<SvgDomTree<T>> ret;

    if (!root) {
        return ret;
    }

    for (QGraphicsItem *_child : root->childItems()) {
        if (T *child = dynamic_cast<T *>(_child)) {
            if (itemMatchesSelector<T>(child, attr_name, attr_value)) {
                ret.append(SvgDomTree(child));
            }
        }
        ret.append(SvgDomTree(_child).findAll(_child, attr_name, attr_value));
    }
    return ret;
}

template<typename TT>
template<typename T>
T *SvgDomTree<TT>::findFromParentRaw(
    const QGraphicsItem *parent,
    const QString &attr_name,
    const QString &attr_value) {
    if (!parent) {
        return nullptr;
    }

    for (QGraphicsItem *_child : parent->childItems()) {
        if (T *child = dynamic_cast<T *>(_child)) {
            if (itemMatchesSelector(child, attr_name, attr_value)) {
                return child;
            }
        }
        T *found = findFromParentRaw(_child, attr_name, attr_value);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

} // namespace svgscene
