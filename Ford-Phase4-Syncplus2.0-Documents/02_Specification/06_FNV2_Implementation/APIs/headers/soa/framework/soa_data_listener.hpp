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
 * soa_data_listener.hpp
 *
 *  Created on: Mar 23, 2017
 *      Author: PREISSNE
 */

#ifndef FNVSOA_FRAMEWORK_SOASUBSCRIBELISTENER_HPP_
#define FNVSOA_FRAMEWORK_SOASUBSCRIBELISTENER_HPP_

#include <soa/framework/soa_message_listener.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * @class SoaDataListener
 *
 * 		This class declares a callback for the handler of in incoming
 * 		subscribed message.
 * 		The SOA client must implement an instance of this class to be able to
 * 		receive data messages.
 * 		This is an abstract interface class.
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
class SoaDataListener : public SoaMessageListener
{
    public:
        /**
         * @brief Constructor
         */
        SoaDataListener();

        /**
         * @brief Destructor
         */
        ~SoaDataListener() override;

        /**
         * @brief Override of the parent message handler
         *
         * This method implementation of the generic message handler routes
         * the flow control to the handler specific to this listener class.
         *
         * @param message The message
         *                This argument will never be null.
         * @param destination
         * 				  The SoaClientEndpoint created with the topic that the message is received on
         * 				  Intended for internal use in SoaConsumer implementation only
         */
        void onMessageReceived(SoaMessage::SharedPtr message, SoaClientEndpoint::SharedPtr destination) final;

        /**
         * @brief Signature for the subscription notification message handler
         *
         * The SOA client must implement this method to handle subscribed data.
         *
         * @param message The message containing subscribed data
         *                This argument will never be null.
         */
        virtual void onDataReceived(SoaMessage::SharedPtr message);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOASUBSCRIBELISTENER_HPP_ */
