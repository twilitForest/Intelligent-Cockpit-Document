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
 * soa_action_result_listener.hpp
 *
 *  Created on: Apr 6, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOAACTIONRESULTLISTENER_HPP_
#define FNVSOA_FRAMEWORK_SOAACTIONRESULTLISTENER_HPP_

#include <soa/framework/soa_error_code.hpp>
#include <memory>
#include <soa/utils/soa_resource_manager.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Generic listener class template.
 * Used to receive action success/error code and the caller context data of a called SOA method.
 * @param class Context The appropriate context class required by the SOA framework method
 *                      being called that invokes the callback implemented for the realized
 *                      template instantiation class.
 * @see soa_consumer.hpp
 * @see soa_provider.hpp
 * !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!!
 * IMPORTANT: Always unsubscribe / unregister your listener before destroying it.
 *
 * IMPORTANT: When implementing a listener class derived from this class,
 *            to eliminate the risk of crashing from a segmentation fault
 *            resulting from a potential race condition between an incoming
 *            message being dispatched to the callback method and the destruction
 *            of this object, implement a non-default destructor and include
 *            the following method call as the first line in your destructor.
 *            It's vital that this get called first by the most derived class'
 *            destructor to ensure callbacks are not dispatched to your listener
 *            during destruction.
 *
 *              this->removeResource();
 *
 *     NOTE: This requirement can be addressed by wrapping your derived
 *           listener class with the SoaListenerLifecycleHelper class template.
 *           The wrapper automates the addition of a call to removeResource()
 *           for you in the most derived class, and the wrapper supports
 *           listener classes which implement multiple callbacks being derived
 *           from multiple SOA listener classes.
 *           The required header file is already included by this header file.
 *           Refer to soa_listener_lifecycle_helper.hpp for instructions on how
 *           to use the wrapper.
 */
template <class Context>
class SoaActionResultListener: public SoaResourceManager::Resource {
public:
	/**
	 * Creates a named resource
	 */
	SoaActionResultListener(): SoaActionResultListener(__FUNCTION__) {}
	explicit SoaActionResultListener(const std::string & name): SoaResourceManager::Resource(name) {}
	/**
	 * Destructor
	 */
	virtual ~SoaActionResultListener() {
		this->removeResource();
	}
	/**
	 * A callback method which needs to be implemented by the application developer if
	 * calling a SOA framework method which takes a SoaActionResultListener argument.
	 * @param error The SoaErrorCode enum value will be from a set of possible error codes resulting
	 *              from the specific SOA framework method called that invokes this callback. Check
	 *              the header comments of the SOA framework method being called.
	 * @param actionContext a pointer to a context class containing the context data of the SOA framework
	 *                      method called that invokes this callback.
	 *                      This pointer is always valid.
	 * @see soa_consumer.hpp
	 * @see soa_provider.hpp
	 */
	virtual void onActionCompleted(SoaErrorCode error, std::shared_ptr<Context> actionContext) {
		// intentional empty implementation instead of pure virtual
	}
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAACTIONRESULTLISTENER_HPP_ */
