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
 * soa_service_status_listener.hpp
 *
 *  Created on: Mar 23, 2017
 *      Author: KGAO4
 */

#ifndef FNVSOA_FRAMEWORK_SOASERVICESTATUSLISTENER_HPP_
#define FNVSOA_FRAMEWORK_SOASERVICESTATUSLISTENER_HPP_

#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_service_status.hpp>
#include <soa/framework/soa_message_listener.hpp>
#include <soa/utils/soa_resource_manager.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Interface for listening to service status
 *
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
class SoaServiceStatusListener: public SoaMessageListener {
public:
	/**
	 * Creates a named resource
	 */
	SoaServiceStatusListener();
	explicit SoaServiceStatusListener(const std::string & name);
	/**
	 * Deprecated
	 * Callback for service status change
	 * @param endpoint The endpoint for which the status changed
	 * @param status The new status
	 */
	virtual void onStatusChanged(const SoaClientEndpoint::SharedPtr endpoint, SoaServiceStatus status);

    /**
     * Callback for service status change
     * First check the "error"parameter. If error != SoaErrorCode::NO_ERROR the "endpoint" and "status" values are undefined
     * @param error error code
     * @param endpoint The endpoint for which the status changed
     * @param status The new status
     */

	virtual void onStatusChanged(SoaErrorCode error, const SoaClientEndpoint::SharedPtr endpoint, SoaServiceStatus status);

	/**
	 * Destructor
	 */
	~SoaServiceStatusListener() override;

    /**
     * @brief Signature for the message handler
     *
     * @param message The message containing subscribed notification data
     *                This argument will never be null.
     *                Application developers (users of the SOA Framework) do not implement this
     *                method. It is implemented by public derived interface classes provided for handling
     *                specific messages.
     * @param destination
     *                The SoaClientEndpoint created with the topic that the message is received on
     *                Intended for internal use in SoaConsumer implementation only
     *
     */
    void onMessageReceived(SoaMessage::SharedPtr message, SoaClientEndpoint::SharedPtr destination) override final;
private:
    const std::string k_className;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */


#endif /* FNVSOA_FRAMEWORK_SOASERVICESTATUSLISTENER_HPP_ */
