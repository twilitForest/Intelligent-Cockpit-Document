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
 * soa_command_handlers_listener.hpp
 *
 *  Created on: May 30, 2017
 *      Author: asergee2
 */

#ifndef INCLUDE_CPP_FRAMEWORK_SOA_COMMAND_HANDLERS_LISTENER_HPP_
#define INCLUDE_CPP_FRAMEWORK_SOA_COMMAND_HANDLERS_LISTENER_HPP_

#include <soa/framework/soa_service_status.hpp>
#include <soa/framework/soa_service_contract.hpp>
#include <soa/framework/soa_service_metadata_list.hpp>
#include <soa/servicemanager/get_command_handlers_response_message_payload.hpp>
#include <soa/utils/soa_resource_manager.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Interface to listen to available command handlers updates or requests
 */
/**
 * @class SoaCommandHandlersListener
 *
 * 		This interface class defines the signature of the callback method for
 * 		the non-blocking get command handlers method.
 * 		The SOA client application developer must implement a concrete class
 * 		derived from this interface to be able to receive the asynchronous
 * 		response with a collection of command handler identifiers.
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
class SoaCommandHandlersListener: public SoaResourceManager::Resource {
public:
	/**
	 * Creates a named resource
	 */
	SoaCommandHandlersListener();
    /**
     * The destructor
     */
    virtual ~SoaCommandHandlersListener();

    /**
     * The listener callback method
     * @param handlers A shared pointer to a list of handlers
     */
    virtual void onCommandHandlersListUpdated(fnv::soa::servicemanager::SoaCommandHandlerMap handlers);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_CPP_FRAMEWORK_SOA_COMMAND_HANDLERS_LISTENER_HPP_ */
