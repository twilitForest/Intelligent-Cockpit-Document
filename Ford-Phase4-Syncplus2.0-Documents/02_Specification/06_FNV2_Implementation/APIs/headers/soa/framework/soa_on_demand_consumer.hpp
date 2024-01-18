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
 * soa_on_demand_consumer.hpp
 *
 *  Created on: Sep 6, 2017
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_ON_DEMAND_CONSUMER_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_ON_DEMAND_CONSUMER_HPP_

#include <soa/framework/soa_consumer.hpp>

namespace fnv {
namespace soa {
namespace framework {

namespace ondemand {
// forward declaration
class SoaOnDemandConsumerInternal;
} // namespace ondemand

/**
 * This class definition is a base class for an on-demand data broadcast service consumer.
 * It must be derived from before instantiating a on-demand consumer object.
 *
 * This class depends on an valid SoaConsumer object for communication.
 *
 * This class allows the user to request an on-demand broadcast data subscription and cancel
 * the subscription.  Only one subscription can be active at a time through an object of this class
 */
class SoaOnDemandConsumer {
    public:
        /**
         * @brief Signature for the subscription notification message handler
         *
         * The SOA client MUST implement this method to handle subscribed data.
         *
         * @param error   When a subscribed data message is received via this callback,
         *                this will be SoaErrorCode::NO_ERROR, and message will be non-null
         *                Otherwise, if error != NO_ERROR, the callback was called for an
         *                error event where the error value indicates the nature of the error,
         *                and in this case, message is NULL.
         *                The intended use for error event callbacks is to notify the
         *                consumer it will no longer receive data broadcasts from the provider.
         *                The error code is SoaErrorCode::ERROR_PROVIDER_DISCONNECTED
         * @param message The message containing subscribed data
         *                This argument is NULL if error != SoaErrorCode::NO_ERROR
         */
        virtual void onDataReceived(SoaErrorCode error, SoaMessage::SharedPtr message) = 0;

        /**
         * Blocking method to simplify "on-demand" data broadcast subscriptions.
         * This class is to be used for a single active request for on-demand
         * broadcasts. New requests must not be
         * A remote call to the service provider is made with the request message.
         * The request message is opaque to the SOA framework. Its format and
         * content must be as per the service provider's specifications.
         * The service provider will respond to the remote call with an endpoint
         * for this consumer to subscribe to. This method handles the receipt of
         * the response message and the subscription to the data endpoint, where the
         * user implemented message handler is assigned as the handler for the data broadcasts.
         * Data messages are received by the handler method and the request status
         * is returned by this method. The message handler is guaranteed to
         * not be called before the status is returned.
         * The call blocks the calling thread until a status code is returned.
         * @param requestMessage a shared pointer to a SoaMessage specifying the request details
         * @param timeout a integer specifying the subscription timeout in ms
         * @return an object the includes the status of the request, and if successful a response
         *         message
         * Status values can be one of the following:
         *  SoaErrorCode::NO_ERROR                    if successful
         *  SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         *  SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         *  SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         *  SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         *  SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         *  SoaErrorCode::ERROR_REQUEST_IN_FLIGHT     if called before a previous call to
         *                                               requestDataBroadcastAsync() for the same
         *                                               object has not yet yielded a status. The
         *                                               status of the async call will not be impacted
         *                                               by this.
         *  SoaErrorCode::ERROR_ON_DEMAND_ACTIVE      if called while there is already an on-demand
         *                                               data broadcast subscription active via this
         *                                               object. An active subscription coordinated through
         *                                               this object must be cancelled before a new one can
         *                                               be requested.
         *  SoaErrorCode::ERROR_MALFORMED_OBJECT      if the constructor of this class failed to
         *                                               create a useable object
         */
        SoaActionResult<SoaMessage>::SharedPtr requestDataBroadcast(SoaMessage::SharedPtr requestMessage, int32_t timeout);

        /**
         * Blocking method to cancel an "on-demand" data broadcast subscription.
         * A remote call to the service provider is made with the cancel message.
         * The cancel message is opaque to the SOA framework. Its format and
         * content must be as per the service provider's specifications.
         * The call blocks the calling thread until an error code is returned.
         * @param cancelMessage a shared pointer to a SoaMessage specifying the request details
         * @param timeout a integer specifying the subscription timeout in ms
         * @return a SoaErrorCode specifying the type of error occurred during register if any
         * @return SoaErrorCode::NO_ERROR                    if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         * @return SoaErrorCode::ERROR_NO_ACTIVE_SUBSCRIPTION if no subscription to cancel
         * @return SoaErrorCode::ERROR_MALFORMED_OBJECT      if the constructor of this class failed to
         *                                                      create a useable object
         */
        SoaErrorCode cancelDataBroadcast(SoaMessage::SharedPtr cancelMessage, int32_t timeout);

        /**
         * Non-blocking method to simplify "on-demand" data broadcast subscriptions.
         * A remote call to the service provider is made with the request message.
         * The request message is opaque to the SOA framework. Its format and
         * content must be as per the service provider's specifications.
         * The service provider will respond to the remote call with an endpoint
         * for this consumer to subscribe to. This method handles the receipt of
         * the response message and the subscription to the data endpoint, where the
         * user implemented message handler is assigned as the handler for the data broadcasts.
         * Data messages are received by the handler method and the rSoaOnDemandConsumerequest status
         * is passed to the callback method onRequestStatus(). The message handler
         * is guaranteed to not be called before the status is returned.
         * This method can be called from the callback thread.
         * The user of this method SHOULD implement an override method for onRequestStatus()
         * to asynchronously receive and handle the result of this call as a SoaErrorCode.
         * This value will be the same as the return code if the user had called the blocking
         * version instead.
         * @param requestMessage a shared pointer to a SoaMessage specifying the request details
         * @param timeout a integer specifying the subscription timeout in ms
         * @see onRequestStatus() for details about the status received in the status callback
         */
        void requestDataBroadcastAsync(SoaMessage::SharedPtr requestMessage, int32_t timeout);

        /**
         * Non-blocking method to cancel an "on-demand" data broadcast subscription.
         * A remote call to the service provider is made with the cancel message.
         * The cancel message is opaque to the SOA framework. Its format and
         * content must be as per the service provider's specifications.
         * Can be called from the callback thread.
         * The user of this method SHOULD implement an override method for onCancelStatus()
         * to asynchronously receive and handle the result of this call as a SoaErrorCode.
         * This value will be the same as the return code if the user had called the blocking
         * version instead.
         * @param cancelMessage a shared pointer to a SoaMessage specifying the request details
         * @param timeout a integer specifying the subscription timeout in ms
         * @see cancelDataBroadcast() for details about the status received in the status callback
         */
        void cancelDataBroadcastAsync(SoaMessage::SharedPtr cancelMessage, int32_t timeout);


        /**
         * @brief Signature for the handler of the asynchronous status of the method
         * requestDataBroadcastAsync(...)
         *
         * The user ONLY NEEDS to override this method if calling requestDataBroadcastAsync()
         *
         * @param error The status of the request method call.
         *
         * Error values can be one of the following:
         *  SoaErrorCode::NO_ERROR                    if successful
         *  SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         *  SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         *  SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         *  SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         *  SoaErrorCode::ERROR_REQUEST_IN_FLIGHT     if requestDataBroadcastAsync() was called before
         *                                               a previous call to requestDataBroadcastAsync() for the
         *                                               same object has not yet yielded a status via this handler. The
         *                                               status of the earlier call will not be impacted by this.
         *  SoaErrorCode::ERROR_ON_DEMAND_ACTIVE      if requestDataBroadcastAsync was called while there is
         *                                               already an on-demand data broadcast subscription active via this
         *                                               object. An active subscription coordinated through
         *                                               this object must be cancelled before a new one can
         *                                               be requested.
         *  SoaErrorCode::ERROR_MALFORMED_OBJECT      if the constructor of this class failed to
         *                                               create a useable object
         */
        virtual void onRequestStatus(SoaErrorCode error) {
            // Implemented by the user
        }

        /**
         * @brief Signature for the handler of the asynchronous status of the method
         * cancelDataBroadcastAsync(...)
         *
         * The user ONLY NEEDS to override this method if calling cancelDataBroadcastAsync()
         *
         * @param error The status of the cancel request method call.
         *
         * Error values can be one of the following:
         * @return a SoaErrorCode specifying the type of error occurred during register if any
         *  SoaErrorCode::NO_ERROR                    if successful
         *  SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         *  SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         *  SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         *  SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         *  SoaErrorCode::ERROR_NO_ACTIVE_SUBSCRIPTION if no subscription to cancel
         *  SoaErrorCode::ERROR_MALFORMED_OBJECT      if the constructor of this class failed to
         *                                               create a useable object
         */

        virtual void onCancelStatus(SoaErrorCode error) {
            // Implemented by the user
        }

        /*
         * Implementation TBD
        SoaActionResult<SoaMessage>::SharedPtr updateDataBroadcast(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaMessage::SharedPtr requestMessage,
                        int32_t timeout) = 0;

        void updateDataBroadcastAsync(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaMessage::SharedPtr requestMessage,
                        int32_t timeout) = 0;
        */

        /**
         * Get the service endpoint
         * which disallows modification of the endpoint
         *
         * This method is provided to to access the context of the subscription
         * This method is fully executed on the user thread and is non-blocking.
         *
         * @return a shared pointer to the service endpoint, or nullptr if this
         *         object is malformed
         */
        SoaClientEndpoint::ConstSharedPtr getServiceEndpoint();

        /**
         * Get the subscription request message
         * which disallows modification of the message
         *
         * This method is provided to to access the context of the subscription
         * This method is fully executed on the user thread and is non-blocking.
         * If no request message has been provided to this object via a subscription
         * request method, the referenced message will be empty.
         * If the there is no subscription active because cancel was called, this
         * message is the last request message provided.
         *
         * @return a shared pointer to the request message, or nullptr if this
         *         object is malformed
         */
        SoaMessage::ConstSharedPtr getRequestMessage();

        /**
         * Get the subscription cancel message
         * which disallows modification of the message
         *
         * This method is provided to to access the context of the subscription
         * This method is fully executed on the user thread and is non-blocking.
         * If no cancel message has been provided to this object via a subscription
         * cancel method, the referenced message will be empty.
         * If a new subscription has become active since the last cancel request, this
         * message is the last cancel message provided.
         *
         * @return a shared pointer to the cancel message, or nullptr if this
         *         object is malformed
         */
        SoaMessage::ConstSharedPtr getCancelMessage();


        /**
         * Destructor
         */
        virtual ~SoaOnDemandConsumer();

    protected:
        /**
         * Constructor
         *
         * A subscription is NOT created by this constructor. The
         * user must call methods of this class to subscribe to and
         * cancel subscriptions for on-demand data broadcasts.
         *
         * @param consumer - a shared pointer to a SoaConsumer object
         * @param serviceEndpoint - a shared pointer to an endpoint for the service provider
         *                          with which an on-demand data broadcast subscription is
         *                          to be created.
         * NOTE: if the consumer or serviceEndpoint is null or bad, the methods
         * of this class will return SoaErrorCode::ERROR_MALFORMED_OBJECT
         * or nullptr.
         */
        SoaOnDemandConsumer(SoaConsumer::SharedPtr consumer, SoaClientEndpoint::SharedPtr serviceEndpoint);


    private:
        SoaOnDemandConsumer() = delete;                                 // explicitly removed
        SoaOnDemandConsumer(const SoaOnDemandConsumer &);               // explicitly disabled
        SoaOnDemandConsumer & operator= (const SoaOnDemandConsumer &);  // explicitly disabled

        std::shared_ptr<ondemand::SoaOnDemandConsumerInternal> implementation;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_ON_DEMAND_CONSUMER_HPP_ */
