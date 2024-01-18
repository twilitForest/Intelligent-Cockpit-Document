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
 * SoaPublishMessageContext.h
 *
 *  Created on: May 3, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOAPUBLISHMESSAGECONTEXT_HPP_
#define FNVSOA_FRAMEWORK_SOAPUBLISHMESSAGECONTEXT_HPP_

#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_message.hpp>

namespace fnv {
namespace soa {
namespace framework {

/*
 * Context for async message publish methods.
 */
class SoaPublishMessageContext {
    public:
    	/**
    	 * Typedef of scoped shared pointer type to this class
    	 */
    	typedef std::shared_ptr<SoaPublishMessageContext> SharedPtr;
        /**
         * @return a shared pointer to the destination endpoint, may be nullptr
         */
        virtual SoaClientEndpoint::SharedPtr getEndpoint() = 0;
        /**
         * @return a shared pointer to the message, may be nullptr
         */
        virtual SoaMessage::SharedPtr getMessage() = 0;
        /**
         * Destructor
         */
        virtual ~SoaPublishMessageContext() = default;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAPUBLISHMESSAGECONTEXT_HPP_ */
