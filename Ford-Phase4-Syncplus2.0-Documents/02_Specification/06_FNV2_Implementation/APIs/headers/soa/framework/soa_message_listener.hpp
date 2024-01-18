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
 * soa_message_listener.hpp
 *
 *  Created on: Apr 5, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOAMESSAGELISTENER_HPP_
#define FNVSOA_FRAMEWORK_SOAMESSAGELISTENER_HPP_

#include <soa/framework/soa_message.hpp>
#include <soa/utils/soa_subscription_tracker.hpp>
#include <soa/utils/soa_resource_manager.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * SoaMessageListener is the common parent class of specific message listeners. The message listener classes
 * are pure-virtual container classes (interfaces) for callback methods which will handle the received message.
 * Application developers (users of the SOA Framework) do not have any need to derive from this class directly.
 * It is used by SOA Framework internals. Developers must create realizations of the specific public interfaces
 * derived from this interface class to be able to handle received messages of specific purposes.
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
class SoaMessageListener:
                public SoaResourceManager::Resource,
                public SoaSubscriptionTracker
{
    public:
        /**
         * Typedef of scoped shared pointer type to this class
         */
        typedef std::shared_ptr<SoaMessageListener> SharedPtr;

        /**
         * Public constructor
         * @param name SoaResourceManager::Resource name
         */
        explicit SoaMessageListener(const std::string & name);

        /**
         * Public constructor.
         * Creates a SoaResourceManager::Resource resource with a default name: SoaMessageListener.
         */
        SoaMessageListener();

        /**
         * Destructor
         */
        virtual ~SoaMessageListener();

        /**
         * @brief Signature for the message handler
         *
         * @param message The message containing subscribed notification data
         *                This argument will never be null.
         *                Application developers (users of the SOA Framework) do not implement this
         *                method. It is implemented by public derived interface classes provided for handling
         *                specific messages.
         * @param destination
         * 				  The SoaClientEndpoint created with the topic that the message is received on
         * 				  Intended for internal use in SoaConsumer implementation only
         *
         */
        virtual void onMessageReceived(SoaMessage::SharedPtr message, SoaClientEndpoint::SharedPtr destination);

    private:
        SoaMessageListener(const SoaMessageListener &);               // explicitly disabled
        SoaMessageListener & operator= (const SoaMessageListener &);  // explicitly disabled
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAMESSAGELISTENER_HPP_ */
