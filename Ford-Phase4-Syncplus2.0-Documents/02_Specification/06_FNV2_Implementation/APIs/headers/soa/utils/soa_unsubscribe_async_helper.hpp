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
 * soa_unsubscribe_async_helper.hpp
 *
 *  Created on: Nov 9, 2017
 *      Author: KGAO4
 */

#ifndef INCLUDE_UTILS_SOA_UNSUBSCRIBE_ASYNC_HELPER_HPP_
#define INCLUDE_UTILS_SOA_UNSUBSCRIBE_ASYNC_HELPER_HPP_

#include <soa/framework/soa_unsubscribe_to_data_context.hpp>
#include <soa/framework/soa_unsubscribe_to_service_status_context.hpp>
#include <soa/utils/soa_no_response_async_helper.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * This class helps to convert async SOA unsubscribe calls to blocking
 */
class SoaUnsubscribeAsyncHelper: public SoaNoResponseAsyncHelper<SoaUnsubscribeToDataContext> {
public:
    SoaUnsubscribeAsyncHelper(): SoaNoResponseAsyncHelper<SoaUnsubscribeToDataContext>(__FUNCTION__) {}
};

/**
 * This class helps to convert async SOA unsubscribe to service status updates calls to blocking
 */
class SoaServiceStatusUnsubscribeAsyncHelper: public SoaNoResponseAsyncHelper<SoaUnsubscribeToServiceStatusContext> {
public:
        SoaServiceStatusUnsubscribeAsyncHelper(): SoaNoResponseAsyncHelper<SoaUnsubscribeToServiceStatusContext>(__FUNCTION__) {}
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_UNSUBSCRIBE_ASYNC_HELPER_HPP_ */
