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
 * soa_consumer.hpp
 *
 *  Created on: Mar 23, 2017
 *      Author: KGAO4
 */

#ifndef FNVSOA_FRAMEWORK_SOACONSUMER_HPP_
#define FNVSOA_FRAMEWORK_SOACONSUMER_HPP_

#include <vector>
#include <tuple>
#include <soa/framework/soa_message.hpp>
#include <soa/framework/soa_error_code.hpp>
#include <soa/framework/soa_action_result.hpp>
#include <soa/framework/soa_action_result_listener.hpp>
#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_service_status.hpp>
#include <soa/framework/soa_service_directory_manager.hpp>
#include <soa/framework/soa_data_listener.hpp>
#include <soa/framework/soa_initialized_listener.hpp>
#include <soa/framework/soa_remote_call_listener.hpp>
#include <soa/framework/soa_service_status_listener.hpp>
#include <soa/framework/soa_subscribe_to_data_context.hpp>
#include <soa/framework/soa_remote_call_context.hpp>
#include <soa/framework/soa_request_service_status_context.hpp>
#include <soa/framework/soa_onewaycall_remote_call_context.hpp>
#include <soa/framework/soa_subscribe_to_service_status_context.hpp>
#include <soa/framework/soa_unsubscribe_to_data_context.hpp>
#include <soa/framework/soa_unsubscribe_to_service_status_context.hpp>
#include <soa/framework/soa_service_metadata_list.hpp>
#include <soa/framework/soa_command_handlers_listener.hpp>
#include <soa/framework/soa_get_command_handlers_action_context.hpp>
#include <soa/framework/soa_start_service_result_listener.hpp>
#include <soa/servicemanager/get_command_handlers_response_message_payload.hpp>
#include <soa/servicemanager/get_command_handles_request_message_payload.hpp>
#include <soa/idl/SoaStartServiceResponsePayload.pb.h>

namespace fnv {
namespace soa {
namespace framework {
/**
 * This class defines the API for a service consumer
 */
class SoaConsumer {
    public:
        /**
         * For applications to asynchronously get results when SOA client objects
         * make non-blocking calls to initializeAsync(listener), this is the
         * interface class declaring the callback method those
         * applications must implement to receive a notification.
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
        class SoaInitializedConsumerListener : public SoaInitializedListener {
            public:
                /**
                 * Constructor
                 */
                SoaInitializedConsumerListener();

                /**
                 * Destructor
                 */
                ~SoaInitializedConsumerListener() override;

                /**
                 * Callback method that must be implemented by user.
                 * @param errorCode - NO_ERROR if successful
                 */
                virtual void onInitialized(SoaErrorCode errorCode, const SoaConsumer * consumer);

                /**
                 * Callback method that must be implemented by user.
                 * @param errorCode - NO_ERROR if successful
                 */
                virtual void onDeinitialized(SoaErrorCode errorCode, const SoaConsumer * consumer);
        };


        /**
         * Typedef of scoped shared pointer type to this class
         */
        typedef std::shared_ptr<SoaConsumer> SharedPtr;
        /**
         * Factory method for creating a SoaConsumer object
         * @param messageManager a shared pointer to a SoaMessageManager instance
         * @param serviceDirectoryManager a shared pointer to a SoaServiceDirectoryManager instance
         *                                can be a nullptr if the consumer does not need access to Service Manager Services
         *                                Supported Service Manager APIs:
         *                                  getCommandHandlers
         *                                  getCommandHandlersAsync
         *                                Not supported Service Manager APIS
         *                                  requestServiceStatusCached
         *                                  requestServiceStatusAsync
         *                                  requestServiceStatus
         *                                  subscribeToServiceStatusUpdateAsync
         *                                  subscribeToServiceStatusUpdate
         *                                  unsubscribeToServiceStatusUpdateAsync
         *                                  unsubscribeToServiceStatusUpdate
         *                                  startService
         *                                  startServiceAsync
         * @param consumerId a consumer ID. Must be unique string for the component
         * @return  a shared pointer to a new SoaConsumer instance
         *          a nullptr if
         *              1. messageManager is a nullptr
         *              2. serviceDirectoryManager is a nullptr
         *              3. consumerId is an empty string
         */
        static SharedPtr createSoaConsumer(
                const SoaMessageManager::SharedPtr& messageManager,
                const SoaServiceDirectoryManager::SharedPtr& serviceDirectoryManager,
                const std::string& consumerId);
        /**
         * @deprecated
         * Factory method for creating a SoaConsumer object
         * @param messageManager a shared pointer to a SoaMessageManager instance
         * @param serviceDirectoryManager a shared pointer to a SoaServiceDirectoryManager instance
         *                                can be a nullptr if the consumer does not need access to Service Manager Services
         *                                Supported Service Manager APIs:
         *                                  getCommandHandlers
         *                                  getCommandHandlersAsync
         *                                Not supported Service Manager APIS
         *                                  requestServiceStatusCached
         *                                  requestServiceStatusAsync
         *                                  requestServiceStatus
         *                                  subscribeToServiceStatusUpdateAsync
         *                                  subscribeToServiceStatusUpdate
         *                                  unsubscribeToServiceStatusUpdateAsync
         *                                  unsubscribeToServiceStatusUpdate
         *                                  startService
         *                                  startServiceAsync
         * @param consumerEndpoint a shared pointer to a SoaClientEndpoint specifying the endpoint of this consumer
         * @return if all arguments are not nullptr returns a shared pointer to a new SoaConsumer instance, otherwise returns nullptr
         */
        static SharedPtr createSoaConsumer(
                        SoaMessageManager::SharedPtr messageManager,
                        SoaServiceDirectoryManager::SharedPtr serviceDirectoryManager,
                        SoaClientEndpoint::SharedPtr consumerEndpoint);

        /**
         * Initialize consumer, subscribe to consumer endpoint and establish connection to SOA broker if it is not established yet
         *
         * WARNING:
         * If calling connectAsync() to pre-connect the SOA app to the broker
         * to avoid failing on calls to the blocking methods SoaConsumer::initialize() or
         * SoaProvider:initialize(), you MUST wait for your implementation of
         * SoaConnectionListener::onConnect(...) to be called before calling initialize().
         *
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD if called from the callback thread
         * @return SoaErrorCode::ERROR_CONNECT_FAIL if couldn't connect
         * @return SoaErrorCode::ERROR_PARAM_BAD if the consumer endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the blanket timeout
         * @return SoaErrorCode::ERROR_EMPTY_TOPIC_PATTERN   if missing pattern param
         * @return SoaErrorCode::ERROR_SUBSCRIBE_FAILED      if MQTT subscribe failed
         * @return SoaErrorCode::ERROR_TOPIC_PATTERN_ALREADY_REGISTERED if endpoint subscription would overlap with reserved subscription
         * @return SoaErrorCode::ERROR_ALREADY_INITIALIZED if this object was previously successfully initialized
         * @return SoaErrorCode::ERROR_UNKNOWN_EXCEPTION    should not happen
         */
        virtual SoaErrorCode initialize() = 0;

        /**
         * Initialize consumer, DO NOT establish connection to SOA broker, but do
         * establish a connection to the gateway if connecting through a gateway.
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ALREADY_INITIALIZED if this object was previously successfully initialized
         */
        virtual SoaErrorCode initializeAsync(SoaInitializedConsumerListener & listener);

        /**
         * Sets the default timeout common to the blocking and non-blocking methods.
         * Timeouts for non-blocking methods refers to the time to call the callback
         * @param timeout The timeout in milliseconds, must be a positive int <= 3600000 mSec (1 hour)
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout less than or equal to 0 or
         *                                                      timeout greater than 3600000 ms (1 hr)
         * If this is returned, the timeout is not changed. Otherwise, NO_ERROR is returned.
         */
        virtual SoaErrorCode setTimeout(int32_t timeout) = 0;

        /**
         * Gets the common timeout value which applies to the blocking and non-blocking methods
         * @return the timeout value in milliseconds
         */
        virtual int32_t getTimeout() = 0;

        /**
         * Subscribes to provider specified by dataEndpoint. The message is received by the listener and
         * the subscription status is received by the status callback.   The message listener is guaranteed to
         * not be called before the status callback is called and returns.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * subscription has failed (the framework guarantees no subscription is created if a timeout occurs)
         * Can be called from the callback thread.
         * @param dataEndpoint a shared pointer to a SoaClientEndpoint specifying the data endpoint to subscribe to
         * @param dataListener a SoaDataListener that listens to incoming message from provider
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @see setTimeout
         * @see subscribeToData(...) for possible error codes sent to the action result listener
         */
        virtual void subscribeToDataAsync(SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaDataListener & dataListener,
                        SoaActionResultListener<SoaSubscribeToDataContext> & actionResultListener) = 0;

        /**
         * Subscribes to provider specified by dataEndpoint. The message is received by the listener and
         * the subscription status is received by the status callback.   The message listener is guaranteed to
         * not be called before the status callback is called and returns.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * subscription has failed (the framework guarantees no subscription is created if a timeout occurs)
         * Can be called from the callback thread.
         * @param dataEndpoint a shared pointer to a SoaClientEndpoint specifying the data endpoint to subscribe to
         * @param dataListener a SoaDataListener that listens to incoming message from provider
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @param timeout a integer specifying the subscription timeout in ms
         * @see subscribeToData(...) for possible error codes sent to the action result listener
         */
        virtual void subscribeToDataAsync(SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaDataListener & dataListener,
                        SoaActionResultListener<SoaSubscribeToDataContext> & actionResultListener,
                        int32_t timeout) = 0;
        /**
         * Subscribes to a provider specified by dataEndpoint. The call is blocking until an error code is returned
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * subscription has failed (the framework guarantees no subscription is created if a timeout occurs)
         * @param dataEndpoint a shared pointer to SoaClientEndpoint specifying the data endpoint to subscribe to
         * @param dataListener a SoaDataListener that listens to incoming message from provider
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if the listener param is a null reference
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         * @return SoaErrorCode::ERROR_EMPTY_TOPIC_PATTERN   if missing pattern param
         * @return SoaErrorCode::ERROR_SUBSCRIBE_FAILED      if MQTT subscribe failed
         * @return SoaErrorCode::ERROR_TOPIC_PATTERN_ALREADY_REGISTERED if endpoint subscription would overlap with reserved subscription
         * @return SoaErrorCode::ERROR_WILDCARD_FIXED_PATTERN_OVERLAP if endpoint subscription would overlap with reserved subscription
         * @return SoaErrorCode::ERROR_UNKNOWN_EXCEPTION     if should not happen
         * @see setTimeout
         */
        virtual SoaErrorCode subscribeToData(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaDataListener& dataListener) = 0;

        /**
         * Subscribes to a dataEndpoint and wait up to the given timeout in ms to get an error code.
         * The message is received by the subListener. The timeout provided is for this subscription request only
         * and will not impact the blanket timeout.
         * The timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * subscription has failed (the framework guarantees no subscription is created if a timeout occurs)
         * @param dataEndpoint a shared pointer to a SoaClientEndpoint specifying the data endpoint to subscribe to
         * @param dataListener a SoaDataListener that listens to incoming message from provider
         * @param timeout a integer specifying the subscription timeout in ms, must be a positive int <= 3600000 mSec (1 hour)
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if the listener param is a null reference
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout less than or equal to 0 or
         *                                                      timeout greater than 3600000 ms (1 hr)
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
         * @return SoaErrorCode::ERROR_EMPTY_TOPIC_PATTERN   if missing pattern param
         * @return SoaErrorCode::ERROR_SUBSCRIBE_FAILED      if MQTT subscribe failed
         * @return SoaErrorCode::ERROR_TOPIC_PATTERN_ALREADY_REGISTERED if endpoint subscription would overlap with reserved subscription
         * @return SoaErrorCode::ERROR_WILDCARD_FIXED_PATTERN_OVERLAP if endpoint subscription would overlap with reserved subscription
         * @return SoaErrorCode::ERROR_UNKNOWN_EXCEPTION     if should not happen
         */
        virtual SoaErrorCode subscribeToData(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaDataListener& dataListener, int32_t timeout) = 0;

        /**
         * Requests a service and expect a response received by the listeners
         * Can be called from the callback thread.
         * @param providerEndpoint a shared pointer to a SoaClientEndpoint specifying the provider endpoint to send to
         * @param request a shared pointer to a SoaMessage specifying the request details e.g. CommandID request endpoint, and response endpoint
         * @param callListener a SoaRemoteCallListener that listens for response message
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @param timeout a integer specifying the call/reply timeout in ms
         * @see remoteCall(..., timeout) for possible error codes sent to the action result listener
         */
        virtual void remoteCallAsync(SoaClientEndpoint::SharedPtr providerEndpoint,
                        SoaMessage::SharedPtr request,
                        SoaRemoteCallListener& callListener,
                        SoaActionResultListener<SoaRemoteCallContext>& actionResultListener,
                        int32_t timeout) = 0;

        /**
         * @deprecated { Use version of remoteCallAsync() that takes a timeout}
         *
         */
        virtual void remoteCallAsync(SoaClientEndpoint::SharedPtr providerEndpoint,
                        SoaMessage::SharedPtr request,
                        SoaRemoteCallListener& callListener,
                        SoaActionResultListener<SoaRemoteCallContext>& actionResultListener) = 0;
        /**
         * Initiates a remote call to a service provider that will not provide a response
         * This is a non-blocking method. The status of the remote call attempt is sent to the callback.
         * Can be called from the callback thread.
         * @param providerEndpoint a shared pointer to a SoaClientEndpoint specifying the provider endpoint to send to
         * @param request a shared pointer to a SoaMessage specifying the request details e.g. CommandID request endpoint
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @see remoteCall(...) for possible error codes sent to the action result listener
         * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if the listener param is a null reference
         */
        virtual void remoteCallAsync(SoaClientEndpoint::SharedPtr providerEndpoint,
                        SoaMessage::SharedPtr request,
                        SoaActionResultListener<SoaOneWayRemoteCallContext>& actionResultListener) = 0;
        /**
         * Requests service and block until the response is received or timeout is reached.
         * If successful, a message is included in the return container object. If an error is returned,
         * the contained message pointer will be the nullptr
         * @param providerEndpoint a shared pointer to a SoaClientEndpoint specifying the provider endpoint to send to
         * @param request a shared pointer to a SoaMessage specifying the request details e.g. CommandID request endpoint, and response endpoint
         * @param timeout a integer specifying the call/reply timeout in ms, must be a positive int <= 3600000 mSec (1 hour)
         * @return SoaErrorCode::NO_ERROR and a response message if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout less than or equal to 0 or
         *                                                      timeout greater than 3600000 ms (1 hr)
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_EMPTY_TOPIC           if the endpoint topic string is empty
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected
         * @return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if the message object failed to serialize
         * @return SoaErrorCode::ERROR_PUBLISH_FAILED        if the MQTT publish failed
         */
        virtual SoaActionResult<SoaMessage>::SharedPtr remoteCall(SoaClientEndpoint::SharedPtr providerEndpoint,
                        SoaMessage::SharedPtr request, int32_t timeout) = 0;
        /**
         * Initiates a remote call to a service provider that will not provide a response, and block until local status of attempt is known.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * sending of the remote call has failed
         * @param providerEndpoint a shared pointer to a SoaClientEndpoint specifying the provider endpoint to send to
         * @param request a shared pointer to a SoaMessage specifying the request details e.g. CommandID request endpoint
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_EMPTY_TOPIC           if the endpoint topic string is empty
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected
         * @return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if the message object failed to serialize
         * @return SoaErrorCode::ERROR_PUBLISH_FAILED        if the MQTT publish failed
         */
        virtual SoaErrorCode remoteCall(SoaClientEndpoint::SharedPtr providerEndpoint,
                        SoaMessage::SharedPtr request) = 0;

        /**
         * Requests the service status from the Service Directory AND creates an explicit subscription for subsequent status updates
         * such that all status values are provided to the status listener except when connection is down.
         * One should assume all services are down when SoaConnectionLostListener is called,
         * and listen for status updates in SoaServiceStatusListener after connection is restored.
         * The explicit subscription will also
         * ensure the cached status is always valid, unless an unsubscribe status method is called.
         * The status listener is guaranteed to not be called before the status callback is called and returns.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * subscription has failed (the framework guarantees no subscription is created if a timeout occurs)
         * Can be called from the callback thread.
         * @param serviceEndpoint a SoaClientEndpoint specifying the service interested in
         * @param listener a SoaServiceStatusListener to listen for a service status update
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @see subscribeToServiceStatusUpdate(...) for possible error codes sent to the action result listener
         * @see setTimeout
         */
        virtual void subscribeToServiceStatusUpdateAsync(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener,
                        SoaActionResultListener<SoaSubscribeToServiceStatusContext> & actionResultListener) = 0;

        /**
         * Requests the service status from the Service Directory AND creates an explicit subscription for subsequent status updates
         * such that all status values are provided to the status listener except when connection is down.
         * One should assume all services are down when SoaConnectionLostListener is called,
         * and listen for status updates in SoaServiceStatusListener after connection is restored
         * The explicit subscription will also
         * ensure the cached status is always valid, unless an unsubscribe status method is called.
         * The status listener is guaranteed to not be called before the status callback is called and returns.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * subscription has failed (the framework guarantees no subscription is created if a timeout occurs)
         * Can be called from the callback thread.
         * @param serviceEndpoint a SoaClientEndpoint specifying the service interested in
         * @param listener a SoaServiceStatusListener to listen for a service status update
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @param timeout a integer specifying a timeout to the subscription action in ms
         * @see subscribeToServiceStatusUpdate(...) for possible error codes sent to the action result listener
         */
        virtual void subscribeToServiceStatusUpdateAsync(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener,
                        SoaActionResultListener<SoaSubscribeToServiceStatusContext> & actionResultListener,
                        int32_t timeout) = 0;

        /**
         * This method creates an explicit service status subscription and blocks until
         * Requests the service status from the Service Directory AND creates an explicit subscription for subsequent status updates
         * such that all status values are provided to the status listener except when connection is down.
         * One should assume all services are down when SoaConnectionLostListener is called,
         * and listen for status updates in SoaServiceStatusListener after connection is restored.
         * The explicit subscription will also
         * ensure the cached status is always valid, unless an unsubscribe status method is called.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * subscription has failed (the framework guarantees no subscription is created if a timeout occurs)
         * @param serviceEndpoint a SoaClientEndpoint specifying the service interested in
         * @param listener a SoaServiceStatusListener to listen for a service status update
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_NOT_INITIALIZED       if service manager object not initialized
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if the listener param is a null reference
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected
         * @see setTimeout
         */
        virtual SoaErrorCode subscribeToServiceStatusUpdate(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener) = 0;

        /**
         * This method creates an explicit service status subscription and blocks until
         * Requests the service status from the Service Directory AND creates an explicit subscription for subsequent status updates
         * such that all status values are provided to the status listener except when connection is down.
         * One should assume all services are down when SoaConnectionLostListener is called,
         * and listen for status updates in SoaServiceStatusListener after connection is restored.
         * The explicit subscription will also
         * ensure the cached status is always valid, unless an unsubscribe status method is called.
         * The timeout provided is for this subscription request only and will not impact the blanket timeout.
         * The timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * subscription has failed (the framework guarantees no subscription is created if a timeout occurs)
         * @param serviceEndpoint a SoaClientEndpoint specifying the service interested in
         * @param listener a SoaServiceStatusListener to listen for a service status update
         * @param timeout a integer specifying a timeout to the subscription action in ms, must be a positive int <= 3600000 mSec (1 hour)
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_NOT_INITIALIZED       if service manager object not initialized
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if the listener param is a null reference
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout less than or equal to 0 or
         *                                                      timeout greater than 3600000 ms (1 hr)
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected
         */
        virtual SoaErrorCode subscribeToServiceStatusUpdate(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener, int32_t timeout) = 0;

        /**
         * Unsubscribes to a data endpoint.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The service listener may still get called.)
         * Can be called from the callback thread.
         * @param dataEndpoint a SoaClientEndpoint specifying the data endpoint no longer interested in
         * @param dataListener The listener provided in a subscribeToData method.
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @see unsubscribeToData(...) for possible error codes sent to the action result listener
         * @see setTimeout
         */
        virtual void unsubscribeToDataAsync(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaDataListener & dataListener,
                        SoaActionResultListener<SoaUnsubscribeToDataContext> & actionResultListener) = 0;

        /**
         * Unsubscribes to a data endpoint.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The service listener may still get called.)
         * Can be called from the callback thread.
         * @param dataEndpoint a SoaClientEndpoint specifying the data endpoint no longer interested in
         * @param dataListener The listener provided in a subscribeToData method.
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @param timeout a integer specifying a timeout to the unsubscribe action in ms
         * @see unsubscribeToData(...) for possible error codes sent to the action result listener
         */
        virtual void unsubscribeToDataAsync(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaDataListener & dataListener,
                        SoaActionResultListener<SoaUnsubscribeToDataContext> & actionResultListener,
                        int32_t timeout) = 0;

        /**
         * Unsubscribes to a data endpoint, returns only when  done unsubscribing or an error has occurred
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The service listener may still get called.)
         * @param dataEndpoint a SoaClientEndpoint specifying the data endpoint no longer interested in
         * @param dataListener The listener provided in a subscribeToData method.
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if the listener param is a null reference
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected
         * @see setTimeout
         */
        virtual SoaErrorCode unsubscribeToData(
                SoaClientEndpoint::SharedPtr dataEndpoint,
                SoaDataListener & dataListener) = 0;

        /**
         * Unsubscribes to a data endpoint, returns only when done unsubscribing, when error has occurred, or timeout is reached.
         * The timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The service listener may still get called.)
         * @param dataEndpoint a SoaClientEndpoint specifying the data endpoint no longer interested in
         * @param dataListener The listener provided in a subscribeToData method.
         * @param timeout a integer specifying a timeout to the unsubscribe action in ms, must be a positive int <= 3600000 mSec (1 hour)
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if the listener param is a null reference
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout less than or equal to 0 or
         *                                                      timeout greater than 3600000 ms (1 hr)
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected
         * @return SoaErrorCode::ERROR_EMPTY_TOPIC_PATTERN   if missing pattern param
         * @return SoaErrorCode::ERROR_UNSUBSCRIBE_FAILED    if MQTT unsubscribe failed
         * @return SoaErrorCode::ERROR_LISTENER_NOT_BOUND    if endpoint data listener not bound to the endpoint
         * @return SoaErrorCode::ERROR_UNKNOWN_EXCEPTION     if should not happen
         *
         */
        virtual SoaErrorCode unsubscribeToData(
                        SoaClientEndpoint::SharedPtr dataEndpoint,
                        SoaDataListener & dataListener,
                        int32_t timeout) = 0;


        /**
         * Unsubscribes to the status update of a specific service. This method cancels the implicit subscription
         * and immediately makes the cached status invalid (i.e. unavailable) regardless of the success or failure of
         * the unsubscribe of an explicit subscription.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The status update listener may still get called.)
         * Can be called from the callback thread.
         * @param serviceEndpoint a SoaClientEndpoint specifying the service no longer interested in
         * @param listener The listener provided in a subscribe method.
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @see unsubscribeToServiceStatusUpdate(...) for possible error codes sent to the action result listener
         * @see setTimeout
         */
        virtual void unsubscribeToServiceStatusUpdateAsync(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener,
                        SoaActionResultListener<SoaUnsubscribeToServiceStatusContext> & actionResultListener);

        /**
         * Unsubscribes to the status update of a specific service. This method cancels the implicit subscription
         * and immediately makes the cached status invalid (i.e. unavailable) regardless of the success or failure of
         * the unsubscribe of an explicit subscription.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The status update listener may still get called.)
         * Can be called from the callback thread.
         * @param serviceEndpoint a SoaClientEndpoint specifying the service no longer interested in
         * @param listener The listener provided in a subscribe method.
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         * @param timeout an integer specifying a timeout to the unsubscribe action in ms
         * @see unsubscribeToServiceStatusUpdate(...) for possible error codes sent to the action result listener
         */
        virtual void unsubscribeToServiceStatusUpdateAsync(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener,
                        SoaActionResultListener<SoaUnsubscribeToServiceStatusContext> & actionResultListener,
                        int32_t timeout);

        /**
         * Unsubscribes to the status update of a specific service, returns only when done unsubscribing or when an error has occurred
         * This method cancels the implicit subscription and immediately makes the cached status invalid
         * (i.e. unavailable) regardless of the success or failure of the unsubscribe of an explicit subscription.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The status update listener may still get called.)
         * @param serviceEndpoint a SoaClientEndpoint specifying the service no longer interested in
         * @param listener The listener provided in a subscribe method.
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_NOT_INITIALIZED       if service manager object not initialized
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected
         * @see setTimeout
         */
        virtual SoaErrorCode unsubscribeToServiceStatusUpdate(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener);

        /**
         * Unsubscribes to the status update of a specific service, returns only when done unsubscribing or when an error has occurred
         * This method cancels the implicit subscription and immediately makes the cached status invalid
         * (i.e. unavailable) regardless of the success or failure of the unsubscribe of an explicit subscription.
         * The timeout provided is for this subscription request only and will not impact the blanket timeout.
         * The timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The status update listener may still get called.)
         * @param serviceEndpoint a SoaClientEndpoint specifying the service no longer interested in
         * @param listener The listener provided in a subscribe method.
         * @param timeout an integer specifying a timeout to the unsubscribe action in ms, must be a positive int <= 3600000 mSec (1 hour)
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
         * @return SoaErrorCode::ERROR_NOT_INITIALIZED       if service manager object not initialized
         * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout less than or equal to 0 or
         *                                                      timeout greater than 3600000 ms (1 hr)
         * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the timeout
         * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected
         */
        virtual SoaErrorCode unsubscribeToServiceStatusUpdate(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener,
                        int32_t timeout);

        /**
         * Destructor
         */
        virtual ~SoaConsumer() = default;

        /* Deprecated APis returning SoaErrorCode::ERROR_DEPRECATED */

        /**
         * Deprecated
         *
         * Unsubscribes to the status update of a specific service. This method cancels the implicit subscription
         * and immediately makes the cached status invalid (i.e. unavailable) regardless of the success or failure of
         * the unsubscribe of an explicit subscription.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The status update listener may still get called.)
         * Can be called from the callback thread.
         * @param serviceEndpoint a SoaClientEndpoint specifying the service no longer interested in
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         *                     Always geta called with SoaErrorCode::ERROR_DEPRECATED
         * @see unsubscribeToServiceStatusUpdate(...) for possible error codes sent to the action result listener
         * @see setTimeout
         */
        virtual void unsubscribeToServiceStatusUpdateAsync(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaActionResultListener<SoaUnsubscribeToServiceStatusContext> & actionResultListener) = 0;

        /**
         * Deprecated
         *
         * Unsubscribes to the status update of a specific service. This method cancels the implicit subscription
         * and immediately makes the cached status invalid (i.e. unavailable) regardless of the success or failure of
         * the unsubscribe of an explicit subscription.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The status update listener may still get called.)
         * Can be called from the callback thread.
         * @param serviceEndpoint a SoaClientEndpoint specifying the service no longer interested in
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known.
         *                     Always geta called with SoaErrorCode::ERROR_DEPRECATED
         * @param timeout an integer specifying a timeout to the unsubscribe action in ms
         * @see unsubscribeToServiceStatusUpdate(...) for possible error codes sent to the action result listener
         */
        virtual void unsubscribeToServiceStatusUpdateAsync(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaActionResultListener<SoaUnsubscribeToServiceStatusContext> & actionResultListener,
                        int32_t timeout) = 0;

        /**
         * Deprecated
         *
         * Unsubscribes to the status update of a specific service, returns only when done unsubscribing or when an error has occurred
         * This method cancels the implicit subscription and immediately makes the cached status invalid
         * (i.e. unavailable) regardless of the success or failure of the unsubscribe of an explicit subscription.
         * The blanket timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The status update listener may still get called.)
         * @param serviceEndpoint a SoaClientEndpoint specifying the service no longer interested in
         * @return SoaErrorCode::ERROR_DEPRECATED
         * @see setTimeout
         */
        virtual SoaErrorCode unsubscribeToServiceStatusUpdate(
                        SoaClientEndpoint::SharedPtr serviceEndpoint) = 0;

        /**
         * Deprecated
         *
         * Unsubscribes to the status update of a specific service, returns only when done unsubscribing or when an error has occurred
         * This method cancels the implicit subscription and immediately makes the cached status invalid
         * (i.e. unavailable) regardless of the success or failure of the unsubscribe of an explicit subscription.
         * The timeout provided is for this subscription request only and will not impact the blanket timeout.
         * The timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * unsubscribe can be assumed to have failed. (i.e. The status update listener may still get called.)
         * @param serviceEndpoint a SoaClientEndpoint specifying the service no longer interested in
         * @param timeout an integer specifying a timeout to the unsubscribe action in ms, must be a positive int <= 3600000 mSec (1 hour)
         * @return SoaErrorCode::ERROR_DEPRECATED
         */
        virtual SoaErrorCode unsubscribeToServiceStatusUpdate(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        int32_t timeout) = 0;

        /**
         * Deprecated
         * Returns information about services being able to handle the specified command.
         * @param commands a hashset of IDs of the commands need to be handled
         * @param timeout an integer specifying a timeout to the unsubscribe action in ms, must be a positive int <= 3600000 mSec (1 hour)
         * @return a GetCommandHandlersResult containing a error code to specify the type of error occurred if any and
         * a SoaServiceMetadataList object.
         * @return SoaActionResult with SoaErrorCode::ERROR_DEPRECATED
         */
        virtual SoaActionResult<fnv::soa::servicemanager::SoaCommandHandlerMap>::SharedPtr getCommandHandlers(
                        fnv::soa::servicemanager::SoaCommandHashSet commands, int32_t timeout) = 0;

        /**
         * Deprecated
         * Asynchronously returns information about services being able to handle the specified command.
         * Can be called from the callback thread.
         * @param commands a hashset of IDs of the commands need to be handled
         * @param listener a listener to asynchronously receive the result
         * @param actionResultListener to asynchronously receive the action status
         *                              Always invoked with SoaErrorCode::ERROR_DEPRECATED
         * @param timeout an integer specifying a timeout to the unsubscribe action in ms
         * @see getCommandHandlers(...) for possible error codes sent to the action result listener
         */
        virtual void getCommandHandlersAsync(
                fnv::soa::servicemanager::SoaCommandHashSet commands,
                SoaCommandHandlersListener & listener,
                SoaActionResultListener<SoaGetCommandHandlersActionContext> & actionResultListener,
                int32_t timeout) = 0;

        /* end of deprecated APIs*/

        /* Not implemented APis returning SoaErrorCode::ERROR_NOT_IMPLEMENTED */
        /**
         * Not implemented
         * Returns the last known status of a given service without blocking, if available. Else an error code is
         * returned without blocking.
         * A cached status is only available if this or one of the related methods has already been called
         * causing an explicit or implicit subscription for service status, and the subscription has provided at
         * least one status notification. Subsequent explicit or implicit status update notifications will
         * ensure the cached value is always valid. Unsubscribing to service status will immediately invalidate
         * the cached status.
         * Can be called from the callback thread.
         * @param serviceEndpoint a shared pointer to a SoaClientEndpoint specifying the service interested in
         * @return a SoaActionResult containing a error code to specify the type of error occurred if any and a SoaServiceStatus
         *                              Always invoked with SoaErrorCode::ERROR_NOT_IMPLEMENTED
         */
        virtual SoaActionResult<SoaServiceStatus>::SharedPtr requestServiceStatusCached(
                        SoaClientEndpoint::SharedPtr serviceEndpoint) = 0;

        /**
         * Not implemented
         * Requests the service status from the Service Directory. The status is received by the provided status listener,
         * but only once to provide the current status. This method does not create an explicit persistent subscription such that
         * the listener is called again. It does, however, create an implicit persistent subscription such that status updates
         * will be received by the framework and the cached status will remain valid unless an unsubscribe status method is called.
         * Calling this method after explicitly subscribing to status updates will not cancel the explicit status subscription.
         * The status listener is guaranteed to not be called before the status callback is called and returns.
         * Can be called from the callback thread.
         * @param serviceEndpoint a shared pointer to a SoaClientEndpoint specifying the service interested in
         * @param listener a SoaServiceStatusListener to listen for service status returned from Service Directory
         * @param actionResultListener A callback object that implements a callback method which is
         *                     guaranteed to be called when the status of this action is known
         *                     Always invoked with SoaErrorCode::ERROR_NOT_IMPLEMENTED
         * @param timeout a integer specifying request timeout in ms
         * @see requestServiceStatus(...) for possible error codes sent to the action result listener
         */
        virtual void requestServiceStatusAsync(
                        SoaClientEndpoint::SharedPtr serviceEndpoint,
                        SoaServiceStatusListener& listener,
                        SoaActionResultListener<SoaRequestServiceStatusContext> & actionResultListener,
                        int32_t timeout) = 0;

        /**
         * Not implemented
         * Gets the service status from the Service Directory and blocks until status is received, or the timeout
         * expires. This method has the side effect of creating an implicit persistent subscription such that status updates
         * will be received by the framework and the cached status will remain valid unless an unsubscribe status method is called.
         * Calling this method after explicitly subscribing to status updates will not cancel the explicit status subscription.
         * The timeout provided is for this subscription request only and will not impact the blanket timeout.
         * The timeout may expire resulting in an error code indicating a timeout occurred, in which case the
         * no implicit subscription is created
         * @param serviceEndpoint a shared pointer to a SoaClientEndpoint specifying the service interested in
         * @param timeout a integer specifying request timeout in ms, must be a positive int <= 3600000 mSec (1 hour)
         * @return a SoaActionResult containing with SoaErrorCode::ERROR_NOT_IMPLEMENTED and null result
         */
        virtual SoaActionResult<SoaServiceStatus>::SharedPtr requestServiceStatus(
                        const SoaClientEndpoint::SharedPtr serviceEndpoint,
                        int32_t timeout) = 0;

        /**
         * Not implemented
         * A blocking method.
         * Sends a start service request to Service Manager
         * @param service A service to start
         * @param timeout A transaction timeout, this is an RPC call to Service Manager, must be a positive int <= 3600000 mSec (1 hour)
         * @return SoaActionResult including action status and start service result
         * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if timeout less than or equal to 0 or
         *                                                      timeout greater than 3600000 ms (1 hr)
         */
        virtual SoaActionResult<fnv::soa::framework::protobuf::SoaStartServiceResultProto::ActionStatus>::SharedPtr startService(
                SoaClientEndpoint::SharedPtr service, int32_t timeout) = 0;
        /**
         * Not implemented
         * Asynchronously sends a start service request to Service Manager
         * @param service a service to start
         * @param listener a listener to asynchronously receive the result
         * @param timeout a transaction timeout
         */
        virtual void startServiceAsync(SoaClientEndpoint::SharedPtr service, SoaStartServiceResultListener & listener, int32_t timeout) = 0;

        /* end of not implemented APIs*/

    protected:
        SoaConsumer() = default;                        // explicitly protected
    private:
        SoaConsumer(const SoaConsumer &);               // explicitly disabled
        SoaConsumer & operator= (const SoaConsumer &);  // explicitly disabled
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOACONSUMER_HPP_ */

