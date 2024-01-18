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
 * SoaOneWayRemoteCallContext.h
 *
 *  Created on: May 3, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOAONEWAYCALLREMOTECALLCONTEXT_HPP_
#define FNVSOA_FRAMEWORK_SOAONEWAYCALLREMOTECALLCONTEXT_HPP_

#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_message.hpp>

namespace fnv {
namespace soa {
namespace framework {


/*
 * Context for a remote call to a service provider that will not provide a response
 */
class SoaOneWayRemoteCallContext {
    public:
        /**
         * @return a shared pointer to the service provider endpoint, may be nullptr
         */
        virtual SoaClientEndpoint::SharedPtr getProviderEndpoint() = 0;
        /**
         * @return a shared pointer to the request message, may be nullptr
         */
        virtual SoaMessage::SharedPtr getRequestMessage() = 0;
        /**
         * Destructor
         */
        virtual ~SoaOneWayRemoteCallContext() = default;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAONEWAYCALLREMOTECALLCONTEXT_HPP_ */
