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
 * soa_publish_async_helper.hpp
 *
 *  Created on: Nov 9, 2017
 *      Author: KGAO4
 */

#ifndef INCLUDE_UTILS_SOA_PUBLISH_ASYNC_HELPER_HPP_
#define INCLUDE_UTILS_SOA_PUBLISH_ASYNC_HELPER_HPP_

#include <soa/framework/soa_publish_message_context.hpp>
#include <soa/utils/soa_no_response_async_helper.hpp>

namespace fnv {
namespace soa {
namespace framework {
/**
 * This class helps to convert async SOA publish calls to blocking
 */
class SoaPublishAsyncHelper: public SoaNoResponseAsyncHelper<SoaPublishMessageContext> {
public:
    SoaPublishAsyncHelper(): SoaNoResponseAsyncHelper<SoaPublishMessageContext>(__FUNCTION__) {}
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_PUBLISH_ASYNC_HELPER_HPP_ */
