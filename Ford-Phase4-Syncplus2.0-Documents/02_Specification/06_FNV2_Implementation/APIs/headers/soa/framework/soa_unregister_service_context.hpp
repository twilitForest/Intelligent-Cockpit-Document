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
 * SoaUnregisterServiceContext.h
 *
 *  Created on: May 9, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOAUNREGISTERSERVICECONTEXT_HPP_
#define FNVSOA_FRAMEWORK_SOAUNREGISTERSERVICECONTEXT_HPP_

#include <soa/framework/soa_client_endpoint.hpp>

namespace fnv {
namespace soa {
namespace framework {

/*
 *
 */
class SoaUnregisterServiceContext {
public:
    /**
     * Shared pointer type for this class
     */
    typedef std::shared_ptr<SoaUnregisterServiceContext> SharedPtr;

    /**
     * @return a shared pointer to the service provider endpoint, may be nullptr
     */
    virtual SoaClientEndpoint::SharedPtr getServiceEndpoint() = 0;
    /**
     * Destructor
     */
	virtual ~SoaUnregisterServiceContext() = default;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAUNREGISTERSERVICECONTEXT_HPP_ */
