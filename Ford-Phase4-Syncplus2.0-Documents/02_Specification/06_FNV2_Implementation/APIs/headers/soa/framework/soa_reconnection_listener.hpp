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
 * soa_reconnection_listener.hpp
 *
 *  Created on: Oct 2, 2017
 *      Author: dmediati
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_RECONNECTION_LISTENER_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_RECONNECTION_LISTENER_HPP_

#include <soa/utils/soa_resource_manager.hpp>
#include <soa/framework/soa_error_code.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 *
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
class SoaReconnectionListener: public SoaResourceManager::Resource {

    public:
        /**
         * Creates a named resource
         */
        SoaReconnectionListener();

        /**
         * Destructor
         */
        virtual ~SoaReconnectionListener();

        /**
         * Callback method that must be implemented by user.
         * Use SoaConnectionOptions::setReconnectionListener to bind a reference
         * to an instantiation of a concrete user class derived from this interface
         * to have onReconnect be called on reconnection after connection lost,
         * or an error code if reconnection attempts failed.
         * @param errorCode - NO_ERROR if successful, ERROR_TIMEOUT if timed out
         */
        virtual void onReconnect(SoaErrorCode errorCode);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_RECONNECTION_LISTENER_HPP_ */
