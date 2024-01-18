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
 * soa_function_log.hpp
 *
 *  Created on: Oct 27, 2017
 *      Author: ASERGEE2
 */

#ifndef FRAMEWORK_INCLUDE_UTILS_SOA_FUNCTION_LOG_HPP_
#define FRAMEWORK_INCLUDE_UTILS_SOA_FUNCTION_LOG_HPP_

#include <soa_log_helper.hpp>

namespace fnv {
namespace soa {
namespace framework {

class SoaFunctionLog: public SoaLogHelper {
public:
    /**
     * Basic constructor
     * Will log the name of the function/method and enter when called
     *
     * Instantiate this class at the top of a function or method
     *
     * @param logger an instance of a SoaLogger
     * @param func a C string of the function or method name
     */
    SoaFunctionLog(const ::soa::logger::SoaLogger & logger, const std::string & func):
        SoaLogHelper(logger, func) {}

    /**
     * Constructor for added entry text
     * Will log the name of the function/method and enter when called
     *
     * Instantiate this class at the top of a function or method
     *
     * @param logger an instance of a SoaLogger
     * @param func a string of the function or method name
     * @param text a string to include beside enter
     */
    SoaFunctionLog(const ::soa::logger::SoaLogger & logger, const std::string & func, const std::string & text):
        SoaLogHelper(logger, func, text) {}
    /**
     * A constructor which creates a new SoaLogger instance
     * Will log the name of the function/method and enter when called
     *
     * Instantiate this class at the top of a function or method
     *
     * @param logTag a log tag
     * @param func a C string of the function or method name
     */
    SoaFunctionLog(const std::string & logTag, const std::string & func):
        SoaFunctionLog(::soa::logger::SoaLogger(logTag), func) {}

    /**
     * A constructor which creates a new SoaLogger instance
     * Will log the name of the function/method and enter when called
     *
     * Instantiate this class at the top of a function or method
     *
     * @param logTag a log tag
     * @param func a string of the function or method name
     * @param text a string to include beside enter
     */
    SoaFunctionLog(const std::string & logTag, const std::string & func, const std::string & text):
        SoaFunctionLog(::soa::logger::SoaLogger(logTag), func, text) {}
    /**
     * Logs a critical error message
     * @param format The message format
     */
    template <class ... Params>
    void critical(const std::string & format, Params ... params) {
        m_logger.funcCritical(m_func, format, params ...);
    }

    /**
     * Logs an error message
     * @param format The message format
     */
    template <class ... Params>
    void error(const std::string & format, Params ... params) {
        m_logger.funcError(m_func, format, params ...);
    }

    /**
     * Logs a warning message
     * @param format The message format
     */
    template <class ... Params>
    void warning(const std::string & format, Params ... params) {
        m_logger.funcWarning(m_func, format, params ...);
    }

    /**
     * Logs a info message
     * @param format The message format
     */
    template <class ... Params>
    void info(const std::string & format, Params ... params) {
        m_logger.funcInfo(m_func, format, params ...);
    }

    /**
     * Logs a debug message
     * @param format The message format
     */
    template <class ... Params>
    void debug(const std::string & format, Params ... params) {
        m_logger.funcDebug(m_func, format, params ...);
    }
};

#define FLOG(log, logger) fnv::soa::framework::SoaFunctionLog log(logger, __FUNCTION__)

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FRAMEWORK_INCLUDE_UTILS_SOA_FUNCTION_LOG_HPP_ */
