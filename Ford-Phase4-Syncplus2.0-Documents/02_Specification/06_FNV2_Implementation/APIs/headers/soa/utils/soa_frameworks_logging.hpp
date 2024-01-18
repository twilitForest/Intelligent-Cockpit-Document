//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2019.  Ford Motor Company owns all rights to this work and intends
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
 * soa_framworks_logging.hpp
 *
 *  Created on: mar 11, 2019
 *      Author: sliang11
 */

#ifndef FRAMEWORK_INCLUDE_UTILS_SOA_FRAMEWORKS_LOGGING_HPP_
#define FRAMEWORK_INCLUDE_UTILS_SOA_FRAMEWORKS_LOGGING_HPP_

#include <thread-util/Fnv2log.hpp>

#include <deviceabstraction/device_abstraction_factory.hpp>

namespace fnv {
namespace soa {
namespace framework {

extern fnv::soa::deviceabstraction::DeviceLogger* getSoaFrmwkLogger();

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FRAMEWORK_INCLUDE_UTILS_SOA_FRAMEWORKS_LOGGING_HPP_ */
