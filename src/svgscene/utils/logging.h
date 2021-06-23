/**
 * Universal wrapper for logging library.
 *
 * Each source file is expected to declare a log category name what is implicitly used for all
 * logging macros. When logging in header files take precaution not to pollute global scope.
 * Either log manually or declare the log within class scope. Log categories can be structured
 * using dots in name: `machine.core.decode`.
 *
 * Logging library (default or provided by the svgscene library user) must provide these macros:
 *
 *    `LOG_CATEGORY(NAME :: string)` which defines category to be used for given cpp file`
 *
 *    `LOG()`, `DEBUG()`, `WARN()`, `ERROR()` which provide stream for given error level with
 *     respect to category defined in the given cpp file. Implementation is allowed to ignore
 *     categories, but they are provided by the library.
 *
 * Svgscene uses Qt logging library by default and it also supports
 * [Necrolog](https://github.com/fvacek/necrolog/) as an alternative.
 *
 * @see
 * https://www.kdab.com/wp-content/uploads/stories/slides/Day2/KaiKoehne_Qt%20Logging%20Framework%2016_9_0.pdf
 *
 * TODO: To decide - should I require only use of stream logging?
 *      Yes: Necrolog compatibility
 *      No: Streams are unreadable and QT allows printf syntax.
 *
 * @file
 */

#pragma once

#if !defined(SVGSCENE_USE_EXTERNAL_LOGGER)
    #include <QLoggingCategory>

    /**
     * Declares logging category for a file (or another scope).
     * Use with care in header files. Preferrably create an extra scope for it, if you use header
     * file implementation, e.g. templates.
     */
    #define LOG_CATEGORY(NAME)                                                                     \
        static QLoggingCategory _loging_category_(NAME) // NOLINT(cert-err58-cpp)

    // Logging level control is kept for the logging library to handle. This is a polyfilly for
    // older Qt versions. The code is identical to the one in Qt5.14 but to has problems in some
    // older versions.
    #if !defined(QT_NO_QDEBUG_MACRO)
        #define QT_NO_QDEBUG_MACRO                                                                 \
            while (false)                                                                          \
            QMessageLogger().noDebug
    #endif

    #if defined(QT_NO_DEBUG_OUTPUT)
        #define DEBUG QT_NO_QDEBUG_MACRO
    #else
        #define DEBUG(...) qCDebug(_loging_category_, __VA_ARGS__)
    #endif
    #define LOG(...) qCInfo(_loging_category_, __VA_ARGS__)
    #define WARN(...) qCWarning(_loging_category_, __VA_ARGS__)
    #define ERROR(...) qCCritical(_loging_category_, __VA_ARGS__)

#elif defined(SVGSCENE_USE_NECROLOG)
    #include <QStringRef>
    #include <necrolog.h>

    #define LOG_CATEGORY(NAME) static constexpr const char *_logging_category_ = NAME
    #define LOG() nCInfo(_logging_category_)
    #define DEBUG() nCDebug(_logging_category_)
    #define WARN() nCWarning(_logging_category_)
    #define ERROR() nCError(_logging_category_)

/**
 * Support for QString logging
 */
inline NecroLog &operator<<(NecroLog log, const QString &s) {
    return log << s.toStdString();
}

/**
 * Support fot QStringRef logging
 */
inline NecroLog &operator<<(NecroLog log, const QStringRef &s) {
    return log << s.toString().toStdString();
}
#endif
