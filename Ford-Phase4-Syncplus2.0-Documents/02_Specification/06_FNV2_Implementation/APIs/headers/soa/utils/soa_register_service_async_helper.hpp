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
 * soa_register_service_async_helper.hpp
 *
 *  Created on: Jan 12, 2017
 *      Author: KGAO4
 */

#ifndef INCLUDE_UTILS_SOA_REGISTER_SERVICE_ASYNC_HELPER_HPP_
#define INCLUDE_UTILS_SOA_REGISTER_SERVICE_ASYNC_HELPER_HPP_

#include <soa/utils/soa_no_response_async_helper.hpp>
#include <soa/framework/soa_register_service_context.hpp>

namespace fnv {
namespace soa {
namespace framework {
/**
 * This class helps to convert async SOA register service calls to blocking
 */
class SoaRegisterServiceAsyncHelper: public SoaNoResponseAsyncHelper<SoaRegisterServiceContext> {
public:
    SoaRegisterServiceAsyncHelper(): SoaNoResponseAsyncHelper<SoaRegisterServiceContext>(__FUNCTION__) {}
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_REGISTER_SERVICE_ASYNC_HELPER_HPP_ */
