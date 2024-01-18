//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2017.  Ford Motor Company owns all rights to this work and intends
// to maintain it in confidence to preserve its trade secret status.
// Ford Motor Company reserves the right to protect this work as an
// unpublished copyrighted work in the event of an inadvertent or
// deliberate unauthorized publication.  Ford Motor Company also
// reserves its rights under the copyright laws to protect this work
// as a published work.  Those having access to this work may not copy
// it, use it, or disclose the information contained in it without
// the written authorization of Ford Motor Company.
//

/*
 * soa_logger.hpp
 *
 *  Created on: Mar 21, 2017
 *      Author: ASERGEE2
 */

#ifndef ECGSOA_FRAMEWORK_SOALOGGER_H_
#define ECGSOA_FRAMEWORK_SOALOGGER_H_

#include <string>
#include <iomanip>
#include <sstream>
#include <functional>
#include <soa/utils/soa_singleton_factory.hpp>

using LogFunc = std::function<void(const char *)>;

namespace soa {
namespace logger {

/*
 * This class either can be used directly for logging or as a base class for other classes willing to log
 */
class SoaLogger {
public:
    /**
     * SOALogger constructor
     * @param name The logger name. Log messages will start with the name
     */
    explicit SoaLogger(const std::string & name);
    virtual ~SoaLogger(void);

    /*
     * The following template function parameters cannot use reference (ex. const Format & format, const Arg & ... arg) since
     * the compiler deduces the type for string literals ("string") as fixed size char arrays(char [6]) which causes a large
     * number of template instantiations to be created increasing the footprint
     * note: empty string literal must be added as an additional argument after "arg ..." in snprintf so the compiler never
     * encounters a case where there are no arguments
     */

    /*
     * Logs a critical error message
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Format, typename ... Arg>
    void logCritical(Format format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        mlog(LogLevelWrapper::getInstance().logCritical, line);
    }
    //specialization for std::string format
    template <typename ... Arg>
    void logCritical(const std::string & format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        mlog(LogLevelWrapper::getInstance().logCritical, line);
    }

    /**
     * Logs an error message
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Format, typename ... Arg>
    void logError(Format format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        mlog(LogLevelWrapper::getInstance().logError, line);
    }
    //specialization for std::string format
    template <typename ... Arg>
    void logError(const std::string & format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        mlog(LogLevelWrapper::getInstance().logError, line);
     }

    /**
     * Logs a warning message
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Format, typename ... Arg>
    void logWarning(Format format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        mlog(LogLevelWrapper::getInstance().logWarning, line);
    }
    //specialization for std::string format
    template <typename ... Arg>
    void logWarning(const std::string & format, Arg ... arg) const {
         char line[messageMaxSize];
         snprintf( line, sizeof(line), format.c_str(), arg ..., "");
         mlog(LogLevelWrapper::getInstance().logWarning, line);
     }

    /**
     * Logs a info message
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Format, typename ... Arg>
    void logInfo(Format format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        mlog(LogLevelWrapper::getInstance().logInfo, line);
    }
    //specialization for std::string format
    template <typename ... Arg>
    void logInfo(const std::string & format, Arg ... arg) const {
         char line[messageMaxSize];
         snprintf( line, sizeof(line), format.c_str(), arg ..., "");
         mlog(LogLevelWrapper::getInstance().logInfo, line);
     }

    /**
     * Logs a debug message
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Format, typename ... Arg>
    void logDebug(Format format, Arg ... arg) const {
        if(isDebugEnabled()) {
            char line[messageMaxSize];
            snprintf( line, sizeof(line), format, arg ..., "");
            mlog(LogLevelWrapper::getInstance().logDebug, line);
        }
    }
    //specialization for std::string format
    template  <typename ... Arg>
    void logDebug(const std::string & format, Arg ... arg) const {
        if(isDebugEnabled()) {
            char line[messageMaxSize];
            snprintf( line, sizeof(line), format.c_str(), arg ..., "");
            mlog(LogLevelWrapper::getInstance().logDebug, line);
        }
    }

    /**
     * Logs a critical error message in a format: "funcName critical error: message"
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename FuncName, typename Format, typename ... Arg>
    void funcCritical(FuncName funcName, Format format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        func_mlog(LogLevelWrapper::getInstance().logCritical, funcName, line);
    }
    //specialization for std::string format
    template <typename FuncName, typename ... Arg>
    void funcCritical(FuncName funcName, const std::string & format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        func_mlog(LogLevelWrapper::getInstance().logCritical, funcName, line);
    }

    /**
     * Logs an error message in a format: "funcName error: message"
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename FuncName, typename Format, typename ... Arg>
    void funcError(FuncName funcName, Format format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        func_mlog(LogLevelWrapper::getInstance().logError, funcName, line);
    }
    //specialization for std::string format
    template <typename FuncName, typename ... Arg>
    void funcError(FuncName funcName, const std::string & format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        func_mlog(LogLevelWrapper::getInstance().logError, funcName, line);
    }

    /**
     * Logs a warning message in a format: "funcName warning: message"
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename FuncName, typename Format, typename ... Arg>
    void funcWarning(FuncName funcName, Format format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        func_mlog(LogLevelWrapper::getInstance().logWarning, funcName, line);
    }
    //specialization for std::string format
    template <typename FuncName, typename ... Arg>
    void funcWarning(FuncName funcName, const std::string & format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        func_mlog(LogLevelWrapper::getInstance().logWarning, funcName, line);
    }

    /**
     * Logs an info message in a format: "funcName: message"
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename FuncName, typename Format, typename ... Arg>
    void funcInfo(FuncName funcName, Format format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        func_mlog(LogLevelWrapper::getInstance().logInfo, funcName, line);
    }
    //specialization for std::string format
    template <typename FuncName, typename ... Arg>
    void funcInfo(FuncName funcName, const std::string & format, Arg ... arg) const {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        func_mlog(LogLevelWrapper::getInstance().logInfo, funcName, line);
    }

    /**
     * Logs a debug message in a format: "funcName: message"
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename FuncName, typename Format, typename ... Arg>
    void funcDebug(FuncName funcName, Format format, Arg ... arg) const {
        if(isDebugEnabled()) {
            char line[messageMaxSize];
            snprintf( line, sizeof(line), format, arg ..., "");
            func_mlog(LogLevelWrapper::getInstance().logDebug, funcName, line);
        }
    }
    //specialization for std::string format
    template <typename FuncName, typename ... Arg>
    void funcDebug(FuncName funcName, const std::string & format, Arg ... arg) const {
        if(isDebugEnabled()) {
            char line[messageMaxSize];
            snprintf( line, sizeof(line), format.c_str(), arg ..., "");
            func_mlog(LogLevelWrapper::getInstance().logDebug, funcName, line);
        }
    }

    /*
     * Find out if debug logging is enabled
     * @return true if debug logging is enabled on both SoaLogger and SoaLogAdapterTelemetry, false otherwise
     */
    static bool isDebugEnabled(void);

    /**
     * Enable SOA Framework logger
     * @param enable true to enable, false to disable
     */
    static void enable(bool enable) { enabled = enable; }

    /**
     * Find out if SOA Framework logger is enabled
     * note: returns the SoaLogger settings, but does not include debug settings in SoaLogAdapterTelemetry
     * @return true if enabled, false if disabled
     */
    static bool getIsEnabled(void) { return enabled; }

    /**
     * Returns log tag
     * @return const std::string &
     */
    const std::string & getTag(void) const;


    //Static versions of log methods

    /**
     * Logs a critical error message
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename Format, typename ... Arg>
    static void sCritical(Tag tag, Format format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        slog(LogLevelWrapper::getInstance().logCritical, tag, line);
    }
    //specialization for std::string format
    template <typename Tag, typename ... Arg>
    static void sCritical(Tag tag, const std::string & format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        slog(LogLevelWrapper::getInstance().logCritical, tag, line);
    }

    /**
     * Logs an error message
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename Format, typename ... Arg>
    static void sError(Tag tag, Format format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        slog(std::move(LogLevelWrapper::getInstance().logError), tag, line);
    }
    //specialization for std::string format
    template <typename Tag, typename ... Arg>
    static void sError(Tag tag, const std::string & format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        slog(std::move(LogLevelWrapper::getInstance().logError), tag, line);
    }

    /**
     * Logs a warning message
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename Format, typename ... Arg>
    static void sWarning(Tag tag, Format format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        slog(LogLevelWrapper::getInstance().logWarning, tag, line);
    }
    //specialization for std::string format
    template <typename Tag, typename ... Arg>
    static void sWarning(Tag tag, const std::string & format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        slog(LogLevelWrapper::getInstance().logWarning, tag, line);
    }

    /**
     * Logs a info message
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename Format, typename ... Arg>
    static void sInfo(Tag tag, Format format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        slog(LogLevelWrapper::getInstance().logInfo, tag, line);
    }
    //specialization for std::string format
    template <typename Tag, typename ... Arg>
    static void sInfo(Tag tag, const std::string & format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        slog(LogLevelWrapper::getInstance().logInfo, tag, line);
    }

    /**
     * Logs a debug message
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename Format, typename ... Arg>
    static void sDebug(Tag tag, Format format, Arg ... arg) {
       if(isDebugEnabled()) {
           char line[messageMaxSize];
           snprintf( line, sizeof(line), format, arg ..., "");
           slog(LogLevelWrapper::getInstance().logDebug, tag, line);
       }
    }
    //specialization for std::string format
    template <typename Tag, typename ... Arg>
    static void sDebug(Tag tag, const std::string & format, Arg ... arg) {
       if(isDebugEnabled()) {
           char line[messageMaxSize];
           snprintf( line, sizeof(line), format.c_str(), arg ..., "");
           slog(LogLevelWrapper::getInstance().logDebug, tag, line);
       }
    }

    /**
     * Logs a critical error message in a format: "funcName critical error: message"
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename FuncName, typename Format, typename ... Arg>
    static void sfCritical(Tag tag, FuncName funcName, Format format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        func_slog(LogLevelWrapper::getInstance().logCritical, tag, funcName, line);
    }
    //specialization for std::string format
    template <typename Tag, typename FuncName, typename ... Arg>
    static void sfCritical(Tag tag, FuncName funcName, const std::string & format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        func_slog(LogLevelWrapper::getInstance().logCritical, tag, funcName, line);
    }

    /**
     * Logs an error message in a format: "funcName error: message"
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename FuncName, typename Format, typename ... Arg>
    static void sfError(Tag tag, FuncName funcName, Format format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        func_slog(LogLevelWrapper::getInstance().logError, tag, funcName, line);
    }
    //specialization for std::string format
    template <typename Tag, typename FuncName, typename ... Arg>
    static void sfError(Tag tag, FuncName funcName, const std::string & format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        func_slog(LogLevelWrapper::getInstance().logError, tag, funcName, line);
    }

    /**
     * Logs a warning message in a format: "funcName warning: message"
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename FuncName, typename Format, typename ... Arg>
    static void sfWarning(Tag tag, FuncName funcName, Format format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        func_slog(LogLevelWrapper::getInstance().logWarning, tag, funcName, line);
    }
    //specialization for std::string format
    template <typename Tag, typename FuncName, typename ... Arg>
    static void sfWarning(Tag tag, FuncName funcName, const std::string & format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        func_slog(LogLevelWrapper::getInstance().logWarning, tag, funcName, line);
    }

    /**
     * Logs an info message in a format: "funcName: message"
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename FuncName, typename Format, typename ... Arg>
    static void sfInfo(Tag tag, FuncName funcName, Format format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format, arg ..., "");
        func_slog(LogLevelWrapper::getInstance().logInfo, tag, funcName, line);
    }
    //specialization for std::string format
    template <typename Tag, typename FuncName, typename ... Arg>
    static void sfInfo(Tag tag, FuncName funcName, const std::string & format, Arg ... arg) {
        char line[messageMaxSize];
        snprintf( line, sizeof(line), format.c_str(), arg ..., "");
        func_slog(LogLevelWrapper::getInstance().logInfo, tag, funcName, line);
    }

    /**
     * Logs a debug message in a format: "funcName: message"
     * @param tag - the message tag (must be [const] std::string [&] or [const] char * types)
     * @param funcName - The name of the function (must be [const] std::string [&] or [const] char * types)
     * @param format - The message format (must be [const] std::string [&] or [const] char * types)
     * @param ... arg - The message arguments
     */
    template <typename Tag, typename FuncName, typename Format, typename ... Arg>
    static void sfDebug(Tag tag, FuncName funcName, Format format, Arg ... arg) {
        if(isDebugEnabled()) {
            char line[messageMaxSize];
            snprintf( line, sizeof(line), format, arg ..., "");
            func_slog(LogLevelWrapper::getInstance().logDebug, tag, funcName, line);
        }
    }
    //specialization for std::string format
    template <typename Tag, typename FuncName, typename ... Arg>
    static void sfDebug(Tag tag, FuncName funcName, const std::string & format, Arg ... arg) {
        if(isDebugEnabled()) {
            char line[messageMaxSize];
            snprintf( line, sizeof(line), format.c_str(), arg ..., "");
            func_slog(LogLevelWrapper::getInstance().logDebug, tag, funcName, line);
        }
    }
protected:
    std::string mName;
    const static int messageMaxSize = 256;
    static bool enabled;
private:
    std::string mAdornedName;
    static std::string threadInfo(void);    ///< return string with thread info as [name.id]

    void func_mlog(const LogFunc & method, const std::string & funcName, const char * line) const;
    void mlog(const LogFunc & method, const char * line) const;
    static void slog(const LogFunc & method, const std::string & tag, const char * line);
    static void func_slog(const LogFunc & method, const std::string & tag, const std::string & funcName, const char * line);

    /*
     * This singleton is used because the templates need access to the function objects. This allows the templates to pass the
     * functions without the need to assign them for every soa logger
     */
    class LogLevelWrapper{
        public:
            static LogLevelWrapper& getInstance(void);

            LogLevelWrapper(const LogLevelWrapper&) = delete;
            LogLevelWrapper& operator=(const LogLevelWrapper&) = delete;

            LogFunc logCritical;
            LogFunc logError;
            LogFunc logWarning;
            LogFunc logInfo;
            LogFunc logDebug;
        private:
            LogLevelWrapper(void);
            friend class ::fnv::soa::framework::SingletonFactory<LogLevelWrapper>;
    };

};

} /* namespace logger */
} /* namespace soa */

#endif /* ECGSOA_FRAMEWORK_SOALOGGER_H_ */
