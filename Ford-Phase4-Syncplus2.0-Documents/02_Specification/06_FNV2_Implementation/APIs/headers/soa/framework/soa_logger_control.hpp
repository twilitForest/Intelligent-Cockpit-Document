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
 * soa_logger_control.hpp
 *
 *  Created on: Jun 20, 2017
 *      Author: PREISSNE
 */

#ifndef INCLUDE_CPP_FRAMEWORK_SOA_LOGGER_CONTROL_HPP_
#define INCLUDE_CPP_FRAMEWORK_SOA_LOGGER_CONTROL_HPP_

namespace fnv {
namespace soa {
namespace framework {

/**
 * Programmatic global control over SOA debug level log messages
 * The default setting for debug level logging is true or ON.
 * They must be explicitly disabled if you want to suppress them.
 * Other levels of logging are not impacted by this control
 */
class SoaLoggerControl {
    public:
        /**
         * Enable SOA Framework debug logging (only applies to debug level logs)
         * @param enable optional, default = true, true to enable, false to disable
         */
        static void On(bool enable = true);
        /**
         * Disable SOA Framework logging
         */
        static void Off();
        /**
         * Get enabled state of SOA Framework logging
         * @return true if enabled, false if disabled
         */
        static bool isEnabled();
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_CPP_FRAMEWORK_SOA_LOGGER_CONTROL_HPP_ */
