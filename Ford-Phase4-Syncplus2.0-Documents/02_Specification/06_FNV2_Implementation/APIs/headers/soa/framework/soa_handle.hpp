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
 * soa_handle.hpp
 *
 *  Created on: Aug 28, 2017
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_HANDLE_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_HANDLE_HPP_

#include <string>
#include <sstream>

namespace fnv {
namespace soa {
namespace framework {

typedef std::size_t SoaHandle;

class SoaHandleString {
    public:
        static std::string toHexString(SoaHandle handle) {
            std::stringstream strstrm;
            strstrm << std::hex << handle;
            return strstrm.str();
        }
};

const static SoaHandle HANDLE_NOT_ASSIGNED = static_cast<SoaHandle>(-1LL);

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_HANDLE_HPP_ */
