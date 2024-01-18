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
 * soa_log_common_text.hpp
 *
 *  Created on: Sep 8, 2017
 *      Author: preissne
 */

#ifndef FRAMEWORK_INCLUDE_SOA_LOG_HELPER_HPP_
#define FRAMEWORK_INCLUDE_SOA_LOG_HELPER_HPP_

#include <soa/framework/soa_logger.hpp>
#include <soa/framework/soa_error_code.hpp>
#include <soa/framework/soa_error_code_util.hpp>

namespace fnv {
namespace soa {
namespace framework {


class SoaLogHelper {
    public:
        /**
         * Basic constructor
         * Will log the name of the function/method and enter when called
         *
         * Instantiate this class at the top of a function or method
         *
         * @param logger an instance of a SoaLogger
         * @param func a C++ std::string ref of the function or method name
         */
        SoaLogHelper(const ::soa::logger::SoaLogger & logger, const std::string & func)
            : m_logger(logger), m_func(func), p_errorCode(nullptr) {
            m_logger.funcDebug(m_func, ENTER);
        }
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
        SoaLogHelper(const ::soa::logger::SoaLogger & logger, const std::string & func, const std::string & text)
            : m_logger(logger), m_func(func), p_errorCode(nullptr) {
            m_logger.funcDebug(m_func, "%s %s", ENTER, text.c_str());
        }
        /**
         * Basic constructor for method that returns SoaErrorCode
         * Will log the name of the function/method and enter when called
         * and sets up destructor to log exit value
         * NO_ERROR* uses SoaLogger::funcDebug
         * errors use SoaLogger::funcError
         *
         * Instantiate this class at the top of a function or method
         *
         * @param logger an instance of a SoaLogger
         * @param func a C++ std::string ref of the function or method name
         * @param errorCodePtr pointer to local SoaErrorCode variable assigned the method return value
         */
        SoaLogHelper(const ::soa::logger::SoaLogger & logger, const std::string & func, const SoaErrorCode * const errorCodePtr)
            : m_logger(logger), m_func(func), p_errorCode(errorCodePtr) {
            m_logger.funcDebug(m_func, ENTER);
        }
        /**
         * Constructor for added entry text for method that returns SoaErrorCode
         * Will log the name of the function/method and enter when called
         * and sets up destructor to log exit value
         * NO_ERROR* uses SoaLogger::funcDebug
         * errors use SoaLogger::funcError
         *
         * Instantiate this class at the top of a function or method
         *
         * @param logger an instance of a SoaLogger
         * @param func a string of the function or method name
         * @param text a string to include beside enter
         * @param errorCodePtr pointer to local SoaErrorCode variable assigned the method return value
         */
        SoaLogHelper(const ::soa::logger::SoaLogger & logger, const std::string & func, const std::string & text, const SoaErrorCode * const errorCodePtr)
            : m_logger(logger), m_func(func), p_errorCode(errorCodePtr) {
            m_logger.funcDebug(m_func, "%s %s", ENTER, text.c_str());
        }
        /**
         * Destructor
         * Will log the name of the function/method and leave when called
         */
        ~SoaLogHelper() {
            if (p_errorCode) {
                if (*p_errorCode != SoaErrorCode::NO_ERROR && *p_errorCode != SoaErrorCode::NO_ERROR__GATEWAY_CONNECTED_ONLY) {
                    m_logger.funcError(m_func, "%s %s", LEAVE, SoaErrorCodeUtil::getCString(*p_errorCode));
                } else {
                    m_logger.funcDebug(m_func, "%s %s", LEAVE, SoaErrorCodeUtil::getCString(*p_errorCode));
                }
            } else {
                m_logger.funcDebug(m_func, LEAVE);
            }
        }

        static const char CONSTRUCTOR[];
        static const char DESTRUCTOR[];
        static const char ENTER[];
        static const char LEAVE[];
    protected:
        ::soa::logger::SoaLogger m_logger;
        const std::string m_func;
        const SoaErrorCode *  const p_errorCode;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FRAMEWORK_INCLUDE_SOA_LOG_HELPER_HPP_ */
