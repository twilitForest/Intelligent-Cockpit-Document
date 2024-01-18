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
 * soa_start_service_result_listener_impl.hpp
 *
 *  Created on: Oct 19, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_SOA_UTILS_SOA_START_SERVICE_RESULT_LISTENER_IMPL_HPP_
#define INCLUDE_SOA_UTILS_SOA_START_SERVICE_RESULT_LISTENER_IMPL_HPP_

#include <soa/utils/soa_async_helper.hpp>
#include <soa/framework/soa_start_service_result_listener.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * SoaStartServiceResultListener implementation
 * Allows synchronously waiting for SoaStartServiceResultListener::onResultReceived(SoaErrorCode error, SoaClientEndpoint::SharedPtr service) callback.
 */
class SoaStartServiceResultListenerImpl:
        public framework::SoaAsyncHelper<fnv::soa::framework::protobuf::SoaStartServiceResultProto::ActionStatus, DummyContext::SharedPtr>,
        public SoaStartServiceResultListener {
public:
    virtual ~SoaStartServiceResultListenerImpl();
    /**
     * A listener callback method
     * @param error A request result
     * @param service A service endpoint
     */
    virtual void onResultReceived(SoaErrorCode error, SoaClientEndpoint::SharedPtr service,
            fnv::soa::framework::protobuf::SoaStartServiceResultProto::ActionStatus status);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_START_SERVICE_RESULT_LISTENER_IMPL_HPP_ */
