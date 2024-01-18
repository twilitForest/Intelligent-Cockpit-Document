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
 * soa_service_status_util.hpp
 *
 *  Created on: Jul 20, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_CPP_SOA_SERVICE_STATUS_UTIL_HPP_
#define INCLUDE_CPP_SOA_SERVICE_STATUS_UTIL_HPP_

#include <string>
#include <soa/framework/soa_service_status.hpp>

namespace fnv {
namespace soa {
namespace framework {

class SoaServiceStatusUtil {
public:
    static std::string getString(SoaServiceStatus status);
    static const char * getCString(SoaServiceStatus status);
    /**
     * Returns true if a status is either AVAILABLE or RUNNING
     * @param status A status
     * @return bool
     */
    static bool isProvisionedStatus(SoaServiceStatus status);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_CPP_SOA_SERVICE_STATUS_UTIL_HPP_ */
