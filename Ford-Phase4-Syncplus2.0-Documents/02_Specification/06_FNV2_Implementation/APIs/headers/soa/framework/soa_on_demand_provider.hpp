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
 * soa_on_demand_provider.hpp
 *
 *  Created on: Sep 6, 2017
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_ON_DEMAND_PROVIDER_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_ON_DEMAND_PROVIDER_HPP_

#include <soa/framework/soa_provider.hpp>
#include <soa/framework/soa_handle.hpp>
#include <soa/framework/soa_set_on_demand_broadcast_context.hpp>

namespace fnv {
namespace soa {
namespace framework {

namespace ondemand {
// forward declaration
class SoaOnDemandProviderInternal;
} // namespace ondemand

/**
 * This class definition is a base class for an on-demand data broadcast service provider.
 * It must be derived from before instantiating a on-demand provider object.
 *
 * This class depends on an valid SoaProvider object for communication.
 *
 * This class allows the user to register an on-demand broadcast data service and set up
 * the subscription.  Only one service endpoint is supported through an object of this class,
 * but multiple data endpoints for publishing data to are supported from the service.
 */
class SoaOnDemandProvider {
    public:
        /**
         * @brief Signature for the on demand request handler
         *
         * The SOA client MUST implement this method to handle on
         * demand data requests.
         *
         * @param requestMessage The message containing the on demand data request
         *                This argument will never be null.
         * @param requestHandle correlation handle needed when configuring the on demand broadcast
         */
        virtual void onBroadcastRequestReceived(SoaMessage::SharedPtr requestMessage, SoaHandle requestHandle) = 0;

        /**
         * @brief Signature for the on demand no listeners notification handler
         *
         * The SOA client MUST implement this method to handle notifications
         * that no consumers are listening for data broadcasts.
         *
         * @param dataEndpoint The endpoint which is no longer subscribed to by and listeners
         *                This argument will never be null.
         */
        virtual void onBroadcastNoActiveListeners(SoaClientEndpoint::SharedPtr dataEndpoint) = 0;

        /**
         * @brief Signature for the on demand request cancelled handler
         *
         * The SOA client MUST implement this method to handle responses to the remote calls.
         *
         * @param cancelMessage The message containing the cancellation request
         *                This argument will never be null.
         */
        virtual void onBroadcastCancelled(SoaMessage::SharedPtr cancelMessage) = 0;

        /**
         * Blocking method to register to receive requests from consumers
         * for on-demand data broadcast subscriptions.
         * The SOA client must implement a number of callback methods.
         * This method returns when registration is complete, when the timeout
         * is reached or when an error occurs
         * @param timeout an integer specifying the timeout in ms for register action
         * @return a SoaErrorCode specifying the type of error occurred during register if any
         * @return SoaErrorCode::NO_ERROR                    if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         * @return SoaErrorCode::ERROR_MALFORMED_OBJECT      if the constructor of this class failed to
         */
        SoaErrorCode registerOnDemandDataService(int32_t timeout);

        /**
         * Blocking method to confirm the acceptance of an on-demand data
         * broadcast request.
         * This method generates an acknowledge message to the requesting
         * consumer associated with the requestHandle provided via the
         * request handler.
         * The service will be notified if and when the number of listeners
         * to a previously confirmed data endpoint has reduced to 0. Each call
         * to this method will increment the number of listeners to a specific
         * data endpoint by one. Subscription cancellations automatically decrement
         * the count of listeners.
         * This method will send a the service's opaque confirmation message
         * to the consumer. Typically this response message will include initial
         * or current values with respect to the on-demand data broadcasts being
         * subscribed to by the consumer.
         * This method returns once the response message is sent. when the timeout
         * is reached or if an error occurs
         * @param dataEndpoint a shared pointer to a SoaClientEndpoint that specifies
         *          the endpoint to which the consumer needs to subscribe to receive
         *          the subscribed data messages.
         * @param responseMessage a shared pointer to a SoaMessage whose payload is opaque
         *          to the SOA framework. This is the service specified response message
         *          to the consumer for the request. It can and typically does include
         *          the initial or current values for the data being subscribed to.
         * @param requestHandle a value captured from the request callback which allows
         *          the SOA framework to correlate the confirmation with a specific request
         *          from a specific consumer.
         * @param timeout an integer specifying the timeout in ms for register action
         * @return a SoaErrorCode specifying the type of error occurred during register if any
         * @return SoaErrorCode::NO_ERROR                    if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         * @return SoaErrorCode::ERROR_BAD_HANDLE            if the handle is not known to the SOA framework
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         */
        SoaErrorCode setOnDemandBroadcast(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaMessage::SharedPtr responseMessage,
                        SoaHandle requestHandle,
                        int32_t timeout);

        /**
         * Non-blocking method to register to receive requests from consumers
         * for on-demand data broadcast subscriptions.
         * The SOA client must implement a number of callback methods.
         * Can be called from the callback thread.
         * The user of this method SHOULD implement an override method for onRegisterStatus()
         * to asynchronously receive and handle the result of this call as a SoaErrorCode.
         * This value will be the same as the return code if the user had called the blocking
         * version instead.
         * @param timeout an integer specifying the timeout in ms for register action
         * @see registerOnDemandDataService(...) for possible error codes sent to the
         *          action result listener
         */
        void registerOnDemandDataServiceAsync(int32_t timeout);

        /**
         * Non-blocking method to confirm the acceptance of an on-demand data broadcast request.
         * This method generates an acknowledge message to the requesting
         * consumer associated with the requestHandle provided via the
         * request handler.
         * Can be called from the callback thread.
         * The service will be notified if and when the number of listeners
         * to a previously confirmed data endpoint has reduced to 0. Each call
         * to this method will increment the number of listeners to a specific
         * data endpoint by one. Subscription cancellations automatically decrement
         * the count of listeners.
         * This method will send a the service's opaque confirmation message
         * to the consumer. Typically this response message will include initial
         * or current values with respect to the on-demand data broadcasts being
         * subscribed to by the consumer.
         * The user of this method SHOULD implement an override method for onSetStatus()
         * to asynchronously receive and handle the result of this call as a SoaErrorCode.
         * This value will be the same as the return code if the user had called the blocking
         * version instead.
         * @param dataEndpoint a shared pointer to a SoaClientEndpoint that specifies
         *          the endpoint to which the consumer needs to subscribe to receive
         *          the subscribed data messages.
         * @param responseMessage a shared pointer to a SoaMessage whose payload is opaque
         *          to the SOA framework. This is the service specified response message
         *          to the consumer for the request. It can and typically does include
         *          the initial or current values for the data being subscribed to.
         * @param requestHandle a value captured from the request callback which allows
         *          the SOA framework to correlate the confirmation with a specific request
         *          from a specific consumer.
         * @param timeout an integer specifying the timeout in ms for register action
         * @see setOnDemandBroadcast(...) for possible error codes sent to the
         *          action result listener
         */
        void setOnDemandBroadcastAsync(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaMessage::SharedPtr responseMessage,
                        SoaHandle requestHandle,
                        int32_t timeout);

        /**
         * Publishes a message only if there is one or more subscribers to the endpoint
         * and wait until status of sending the message is known or blanket timeout is reached.
         * @param endpoint a shared pointer to a SoaClientEndpoint specifying the consumer or data endpoint to publish to
         * @param message a shared pointer to a SoaMessage containing the information to be published to the consumer
         * @return a SoaErrorCode specifying the type of error occurred during publish if any
         * @return SoaErrorCode::NO_ERROR                    if successful or there are no subscribers
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         * @return SoaErrorCode::ERROR_EMPTY_TOPIC           if the endpoint topic string is empty
         * @return SoaErrorCode::ERROR_PUBLISH_FAILED        if the MQTT client failed to publish the message
         * @return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if the message object failed to be serialized
         * @see setTimeout
         */
        virtual SoaErrorCode    publishMessage(SoaClientEndpoint::SharedPtr endpoint, SoaMessage::SharedPtr message);

        /**
         * Publishes a message only if there is one or more subscribers to the endpoint
         * and wait until status of sending the message is known or the provided timeout is reached
         * @param endpoint a shared pointer to a SoaClientEndpoint specifying the consumer or data endpoint to publish to
         * @param message a shared pointer to a SoaMessage containing the information to be published to the consumer
         * @param timeout an integer specifying the timeout in ms for publish action
         * @return a SoaErrorCode specifying the type of error occurred during publish if any
         * @return SoaErrorCode::NO_ERROR                    if successful or there are no subscribers
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         * @return SoaErrorCode::ERROR_EMPTY_TOPIC           if the endpoint topic string is empty
         * @return SoaErrorCode::ERROR_PUBLISH_FAILED        if the MQTT client failed to publish the message
         * @return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if the message object failed to be serialized
         */
        virtual SoaErrorCode    publishMessage(SoaClientEndpoint::SharedPtr endpoint,
                                               SoaMessage::SharedPtr message,
                                               int32_t timeout);

        /**
         * Publishes a message only if there is one or more subscribers to the endpoint
         * and get the publish status in the listener
         * Can be called from the callback thread.
         * The user of this method SHOULD implement an override method for onPublishStatus()
         * @param endpoint a shared pointer to a SoaClientEndpoint specifying the consumer or data endpoint to publish to
         * @param message a shared pointer to a SoaMessage containing information to be published to the consumer
         * @see publishMessage(...) for possible error codes sent to the action result listener
         * @see setTimeout
         */
        virtual void            publishMessageAsync(SoaClientEndpoint::SharedPtr endpoint,
                                                    SoaMessage::SharedPtr message);

        /**
         * Publishes a message only if there is one or more subscribers to the endpoint
         * and get the publish status in the listener
         * Can be called from the callback thread.
         * The user of this method SHOULD implement an override method for onPublishStatus()
         * @param endpoint a shared pointer to a SoaClientEndpoint specifying the consumer or data endpoint to publish to
         * @param message a shared pointer to a SoaMessage containing information to be published to the consumer
         * @param timeout an integer specifying the timeout in ms for publish action
         * @see publishMessage(...) for possible error codes sent to the action result listener
         */
        virtual void            publishMessageAsync(SoaClientEndpoint::SharedPtr endpoint,
                                                    SoaMessage::SharedPtr message,
                                                    int32_t timeout);

        /**
         * @brief Signature for the handler of the asynchronous status of the method
         * registerOnDemandDataServiceAsync(...)
         *
         * The user ONLY NEEDS to override this method if calling registerOnDemandDataServiceAsync()
         *
         * @param error The status of the request method call.
         *
         * Error values can be one of the following:
         *  SoaErrorCode::NO_ERROR                    if successful
         *  SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         *  SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         *  SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         *  SoaErrorCode::ERROR_MALFORMED_OBJECT      if the constructor of this class failed to
         */
        virtual void onRegisterStatus(SoaErrorCode error) {
            // Implemented by the user
        }

        /**
         * @brief Signature for the handler of the asynchronous status of the method
         * setOnDemandBroadcastAsync(...)
         *
         * The user ONLY NEEDS to override this method if calling setOnDemandBroadcastAsync()
         *
         * @param error The status of the request method call.
         * @param context the context of the associated call to setOnDemandBroadcastAsync
         *                This will be nullptr if the error is ERROR_MALFORMED_OBJECT as the
         *                error is not related to the context
         *
         * Error values can be one of the following:
         *  SoaErrorCode::NO_ERROR                    if successful
         *  SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         *  SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         *  SoaErrorCode::ERROR_BAD_HANDLE            if the handle is not known to the SOA framework
         *  SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         *  SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         *  SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         *  SoaErrorCode::ERROR_MALFORMED_OBJECT      if the constructor of this class failed to
         */
        virtual void onSetStatus(SoaErrorCode error, std::shared_ptr<SoaSetOnDemandBroadcastContext> context) {
            // Implemented by the user
        }

        /**
         * @brief Signature for the handler of the asynchronous status of the method
         * publishMessageAsync(...)
         *
         * The user ONLY NEEDS to override this method if calling publishMessageAsync()
         *
         * @param error The status of the request method call.
         * @param context the context of the associated call to publishMessageAsync
         *                This will be nullptr if the error is ERROR_MALFORMED_OBJECT as the
         *                error is not related to the context
         *
         * Error values can be one of the following:
         *  SoaErrorCode::NO_ERROR                    if successful or there are no subscribers
         *  SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         *  SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         *  SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout <= 0
         *  SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
         *  SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         *  SoaErrorCode::ERROR_EMPTY_TOPIC           if the endpoint topic string is empty
         *  SoaErrorCode::ERROR_PUBLISH_FAILED        if the MQTT client failed to publish the message
         *  SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if the message object failed to be serialized
         *  SoaErrorCode::ERROR_MALFORMED_OBJECT      if the constructor of this class failed to
         */
        virtual void onPublishStatus(SoaErrorCode error, std::shared_ptr<SoaPublishMessageContext> context) {
            // Implemented by the user
        }

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
         * Destructor
         */
        virtual ~SoaOnDemandProvider();

    protected:
        /**
         * Constructor
         *
         * A service is NOT register by this constructor. The
         * user must call methods of this class to register a service and
         * confirm subscription requests for on-demand data broadcasts.
         *
         * @param provider - a shared pointer to a SoaProvider object
         * @param serviceEndpoint - a shared pointer to the endpoint this service provider
         *                          listens on for on-demand data broadcast subscriptions
         * NOTE: if the provider or serviceEndpoint is null or bad, the methods
         * of this class will return SoaErrorCode::ERROR_MALFORMED_OBJECT
         * or nullptr.
         */
        SoaOnDemandProvider(SoaProvider::SharedPtr provider, SoaClientEndpoint::SharedPtr serviceEndpoint);

    private:
        SoaOnDemandProvider() = delete;                                 // explicitly removed
        SoaOnDemandProvider(const SoaOnDemandProvider &);               // explicitly disabled
        SoaOnDemandProvider & operator= (const SoaOnDemandProvider &);  // explicitly disabled

        std::shared_ptr<ondemand::SoaOnDemandProviderInternal> implementation;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_ON_DEMAND_PROVIDER_HPP_ */
