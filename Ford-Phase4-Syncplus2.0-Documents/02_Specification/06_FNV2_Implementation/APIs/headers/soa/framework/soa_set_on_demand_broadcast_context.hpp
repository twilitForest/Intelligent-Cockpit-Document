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
 * soa_set_on_demand_broadcast_context.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_SET_ON_DEMAND_BROADCAST_CONTEXT_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_SET_ON_DEMAND_BROADCAST_CONTEXT_HPP_

#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_message.hpp>
#include <soa/framework/soa_handle.hpp>


namespace fnv {
namespace soa {
namespace framework {

class SoaSetOnDemandBroadcastContext {
    public:
        /**
         * Typedef of scoped shared pointer type to this class
         */
        typedef std::shared_ptr<SoaSetOnDemandBroadcastContext> SharedPtr;
        /**
         * @return a shared pointer to the data endpoint, may be nullptr
         */
        virtual SoaClientEndpoint::SharedPtr getDataEndpoint() = 0;
        /**
         * @return a shared pointer to the message, may be nullptr
         */
        virtual SoaMessage::SharedPtr getResponseMessage() = 0;
        /**
         * @return the request handle
         */
        virtual SoaHandle getHandle() = 0;
        /**
         * Destructor
         */
        virtual ~SoaSetOnDemandBroadcastContext() = default;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_SET_ON_DEMAND_BROADCAST_CONTEXT_HPP_ */
