/**
 * Manual memory ownership toolkit.
 */
#pragma once

#include <QScopedPointer>

/**
 * Tag for pointer owned by someone else. It is recommended to mention owner
 * in comment to make lifetimes manually verifiable.
 */
#define BORROWED

/**
 * Tag for a pointer with static lifetime.
 */
#define STATIC

/**
 * Owned pointer deallocated by automatic destructor.
 *
 * ## Why?
 * `QScopedPointer` is too long and makes the usage in structs hard to use.
 */
template<typename T>
using Box = QScopedPointer<T>;