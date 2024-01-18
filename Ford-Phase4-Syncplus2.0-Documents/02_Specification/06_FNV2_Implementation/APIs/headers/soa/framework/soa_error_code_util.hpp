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
 * soa_error_code_util.hpp
 *
 *  Created on: Jun 15, 2017
 *      Author: PREISSNE
 */

#ifndef ECGSOA_FRAMEWORK_SOA_ERROR_CODE_UTIL_HPP_
#define ECGSOA_FRAMEWORK_SOA_ERROR_CODE_UTIL_HPP_

#include <string>
#include <soa/framework/soa_error_code.hpp>

namespace fnv {
namespace soa {
namespace framework {

class SoaErrorCodeUtil {
    public:
        static std::string getString(SoaErrorCode code);
        static const char * getCString(SoaErrorCode code);
        static constexpr const char * unknownErrorCode = "<unknown error code>";
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* ECGSOA_FRAMEWORK_SOA_ERROR_CODE_UTIL_HPP_ */
