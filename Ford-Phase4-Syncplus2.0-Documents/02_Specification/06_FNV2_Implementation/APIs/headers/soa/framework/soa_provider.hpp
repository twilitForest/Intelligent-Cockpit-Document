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
 * soa_provider.hpp
 *
 *  Created on: Mar 23, 2017
 *      Author: KGAO4
 */

#ifndef FNVSOA_FRAMEWORK_SOAPROVIDER_HPP_
#define FNVSOA_FRAMEWORK_SOAPROVIDER_HPP_

#include <soa/framework/soa_error_code.hpp>
#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_message_manager.hpp>
#include <soa/framework/soa_initialized_listener.hpp>
#include <soa/framework/soa_service_request_listener.hpp>
#include <soa/framework/soa_action_result_listener.hpp>
#include <soa/framework/soa_publish_message_context.hpp>
#include <soa/framework/soa_register_service_context.hpp>
#include <soa/framework/soa_unregister_service_context.hpp>

namespace fnv {
namespace soa {
namespace framework {

/***
 * This class defines the API for a service provider
 */
class SoaProvider {
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
    class SoaInitializedProviderListener : public SoaInitializedListener {
        public:
            /**
             * Constructor
             */
            SoaInitializedProviderListener();

            /**
             * Destructor
             */
            ~SoaInitializedProviderListener() override;

            /**
             * Callback method that must be implemented by user.
             * @param errorCode - NO_ERROR if successful
             */
            virtual void onInitialized(SoaErrorCode errorCode, const SoaProvider * provider);

            /**
             * Callback method that must be implemented by user.
             * @param errorCode - NO_ERROR if successful
             */
            virtual void onDeinitialized(SoaErrorCode errorCode, const SoaProvider * provider);
    };


    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaProvider> SharedPtr;

    /**
     * Factory method for creating a SoaProvider instance
     * @param messageManager a shared pointer to a SoaMessageManager instance
     * @return if the argument is not nullptr returns a shared pointer to a new SoaProvider instance, otherwise returns nullptr
     */
    static SharedPtr createSoaProvider(SoaMessageManager::SharedPtr messageManager);

    /**
     * Initialize provider, establish connection to SOA broker if not established yet
     *
     * WARNING:
     * If calling connectAsync() to pre-connect the SOA app to the broker
     * to avoid failing on calls to the blocking methods SoaConsumer::initialize() or
     * SoaProvider:initialize(), you MUST wait for your implementation of
     * SoaConnectionListener::onConnect(...) to be called before calling initialize().
     *
     * @return SoaErrorCode::NO_ERROR if successful
     * @return SoaErrorCode::ERROR_CONNECT_FAIL if couldn't connect
     * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD if called from the callback thread
     * @return SoaErrorCode::ERROR_TIMEOUT if not complete within the blanket timeout
     * @return SoaErrorCode::ERROR_ALREADY_INITIALIZED if this object was previously successfully initialized
     * @return SoaErrorCode::ERROR_UNKNOWN_EXCEPTION    should not happen
     */
    virtual SoaErrorCode initialize() = 0;

    /**
     * Initialize provider, DO NOT establish connection to SOA broker, but do
     * establish a connection to the gateway if connecting through a gateway.
     * @return SoaErrorCode::NO_ERROR if successful
     * @return SoaErrorCode::ERROR_ALREADY_INITIALIZED if this object was previously successfully initialized
     */
    virtual SoaErrorCode initializeAsync(SoaInitializedProviderListener & listener);

    /**
     * Sets the common timeout for the blocking methods, and the maximum time
     * before an action status callback is called for non-blocking methods.
     * @param timeout The timeout in milliseconds, must be a positive int <= 3600000 mSec (1 hour)
     * @return ERROR_INVALID_TIMEOUT_PARAM  if (timeout <= 0 || timeout > 3600000 mSec (1 hour)).
	 * If this is returned, the timeout is not changed. Otherwise, NO_ERROR is returned.
     */

    virtual SoaErrorCode            setTimeout(int32_t timeout) = 0;

    /**
     * Gets the common timeout value which applies to the blocking methods, and 
     * to the maximum time before an action status callback is called for non-blocking methods.
     * @return the timeout value in milliseconds
     */
    virtual int32_t         getTimeout() = 0;

    /**
     * Publishes a message and get the publish status in the listener
     * Can be called from the callback thread.
     * @param endpoint a shared pointer to a SoaClientEndpoint specifying the consumer or data endpoint to publish to
     * @param message a shared pointer to a SoaMessage containing information to be published to the consumer
     * @param actionResultListener A reference to a listener object that implements a callback method which is
     *                             guaranteed to be called when the status of this action is known
     * @see publishMessage(...) for possible error codes sent to the action result listener
     * @see setTimeout
     */
    virtual void            publishMessageAsync(SoaClientEndpoint::SharedPtr endpoint,
                                                SoaMessage::SharedPtr message,
                                                SoaActionResultListener<SoaPublishMessageContext> & actionResultListener) = 0;

    /**
     * Publishes a message and get the publish status in the listener
     * Can be called from the callback thread.
     * @param endpoint a shared pointer to a SoaClientEndpoint specifying the consumer or data endpoint to publish to
     * @param message a shared pointer to a SoaMessage containing information to be published to the consumer
     * @param actionResultListener A reference to a listener object that implements a callback method which is
     *                             guaranteed to be called when the status of this action is known
     * @param timeout an integer specifying the timeout in ms for publish action
     * @see publishMessage(...) for possible error codes sent to the action result listener
     */
    virtual void            publishMessageAsync(SoaClientEndpoint::SharedPtr endpoint,
                                                SoaMessage::SharedPtr message,
                                                SoaActionResultListener<SoaPublishMessageContext> & actionResultListener,
                                                int32_t timeout) = 0;

    /**
     * Publishes a message and wait until message is delivered or blanket timeout is reached
     * @param endpoint a shared pointer to a SoaClientEndpoint specifying the consumer or data endpoint to publish to
     * @param message a shared pointer to a SoaMessage containing the information to be published to the consumer
     * @return a SoaErrorCode specifying the type of error occurred during publish if any
     * @return SoaErrorCode::NO_ERROR                    if successful
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
    virtual SoaErrorCode    publishMessage(SoaClientEndpoint::SharedPtr endpoint, SoaMessage::SharedPtr message) = 0;

    /**
     * Publishes a message and wait until message is delivered or timeout is reached
     * @param endpoint a shared pointer to a SoaClientEndpoint specifying the consumer or data endpoint to publish to
     * @param message a shared pointer to a SoaMessage containing the information to be published to the consumer
     * @param timeout an integer specifying the timeout in ms for publish action, must be a positive int <= 3600000 mSec (1 hour)
     * @return a SoaErrorCode specifying the type of error occurred during publish if any
     * @return SoaErrorCode::NO_ERROR                    if successful
     * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
     * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
     * @return SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
     * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if (timeout <= 0 || timeout > 3600000 mSec (1 hour))
     * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
     * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
     * @return SoaErrorCode::ERROR_EMPTY_TOPIC           if the endpoint topic string is empty
     * @return SoaErrorCode::ERROR_PUBLISH_FAILED        if the MQTT client failed to publish the message
     * @return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if the message object failed to be serialized
     */
    virtual SoaErrorCode    publishMessage(SoaClientEndpoint::SharedPtr endpoint,
                                           SoaMessage::SharedPtr message,
                                           int32_t timeout) = 0;
    
    /**
     * Subscribes to requests from consumers for remote call services. This method
     * will bind a service request listener object to the endpoint of the service.
     * This method will not block, and provides action status via a callback.
     * Can be called from the callback thread.
     * @param serviceEndpoint a shared pointer to a SoaClientEndpoint that specifies the service to be registered
     * @param requestListener A reference to a listener object that implements a callback method to be
     *                        called when a request is received
     * @param actionResultListener A reference to a listener object that implements a callback method which is
     *                             guaranteed to be called when the status of this action is known
     * @param timeout an integer specifying the timeout in ms for register action
     * @see registerService(...) for possible error codes sent to the action result listener
     */
    virtual void            registerServiceAsync(SoaClientEndpoint::SharedPtr serviceEndpoint,
    		                                     SoaServiceRequestListener & requestListener,
    		                                     SoaActionResultListener<SoaRegisterServiceContext> & actionResultListener,
    		                                     int32_t timeout) = 0;
    
    /**
     * @deprecated { Use version of registerServiceAsync() that takes a timeout}
     *
     */
    virtual void            registerServiceAsync(SoaClientEndpoint::SharedPtr serviceEndpoint,
                                                 SoaServiceRequestListener & requestListener,
                                                 SoaActionResultListener<SoaRegisterServiceContext> & actionResultListener) = 0;

    /**
     * Subscribes to requests from consumers for remote call services. This method
     * will bind a service request listener object to the endpoint of the service.
     * This method returns when registration is complete, when the timeout is reached or when an error occurs
     * @param serviceEndpoint a shared pointer to a SoaClientEndpoint that specifies the service to be registered
     * @param requestListener A reference to a listener object that implements a callback method to be
     *                        called when a request is received
     * @param timeout an integer specifying the timeout in ms for register action, must be a positive int <= 3600000 mSec (1 hour)
     * @return a SoaErrorCode specifying the type of error occurred during register if any
     * @return SoaErrorCode::NO_ERROR                    if successful
     * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
     * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
     * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if the listener param is a null reference
     * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if (timeout <= 0 || timeout > 3600000 mSec (1 hour))
     * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
     * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
     * @return SoaErrorCode::ERROR_PARAM_NULL_REFERENCE  if the listener is a null reference
     */
    virtual SoaErrorCode    registerService(SoaClientEndpoint::SharedPtr serviceEndpoint,
                                            SoaServiceRequestListener & requestListener,
                                            int32_t timeout) = 0;

    /**
     * @deprecated { Use version of registerService() that takes a timeout}
     *
     */
    virtual SoaErrorCode    registerService(SoaClientEndpoint::SharedPtr serviceEndpoint, SoaServiceRequestListener & requestListener) = 0;


    /**
     * Unregisters service from Service Directory and unsubscribes to requests from consumers for remote call services
     * This method will not block, and provides action status via a callback.
     * Can be called from the callback thread.
     * @param serviceEndpoint a shared pointer to a SoaClientEndpoint that specifies the service to be unregistered
     * @param actionResultListener A reference to a listener object that implements a callback method which is
     *                             guaranteed to be called when the status of this action is known
     * @see unregisterService(...) for possible error codes sent to the action result listener
     * @see setTimeout
     */
    virtual void            unregisterServiceAsync(SoaClientEndpoint::SharedPtr serviceEndpoint,
                                                   SoaActionResultListener<SoaUnregisterServiceContext> & actionResultListener) = 0;

    /**
     * Unregisters service from Service Directory and unsubscribes to requests from consumers for remote call services
     * This method will not block, and provides action status via a callback.
     * Can be called from the callback thread.
     * @param serviceEndpoint a shared pointer to a SoaClientEndpoint that specifies the service to be unregistered
     * @param actionResultListener A reference to a listener object that implements a callback method which is
     *                             guaranteed to be called when the status of this action is known
     * @param timeout an integer specifying the timeout in ms for unregister action
     * @see unregisterService(...) for possible error codes sent to the action result listener
     */
    virtual void            unregisterServiceAsync(SoaClientEndpoint::SharedPtr serviceEndpoint,
                                                   SoaActionResultListener<SoaUnregisterServiceContext> & actionResultListener,
                                                   int32_t timeout) = 0;

    /**
     * Unsubscribes to requests from consumers for remote call services
     * This method returns when deregistration is complete, when the blanket timeout is reached or when an error occurs
     * @param serviceEndpoint a shared pointer to a SoaClientEndpoint that specifies the service to be unregistered
     * @return a SoaErrorCode specifying the type of error occurred during unregister if any
     * @return SoaErrorCode::NO_ERROR                    if successful
     * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
     * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
     * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
     * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
     * @see setTimeout
     */
    virtual SoaErrorCode    unregisterService(SoaClientEndpoint::SharedPtr serviceEndpoint) = 0;

    /**
     * Unsubscribes to requests from consumers for remote call services
     * This method returns when unregistration is complete, when the blanket timeout is reached or when an error occurs
     * @param serviceEndpoint a shared pointer to a SoaClientEndpoint that specifies the service to be unregistered
     * @param timeout an integer specifying the timeout in ms for unregister action, must be a positive int <= 3600000 mSec (1 hour)
     * @return a SoaErrorCode specifying the type of error occurred during unregister if any
     * @return SoaErrorCode::NO_ERROR                    if successful
     * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
     * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
     * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if (timeout <= 0 || timeout > 3600000 mSec (1 hour))
     * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
     * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
     */
    virtual SoaErrorCode    unregisterService(SoaClientEndpoint::SharedPtr serviceEndpoint, int32_t timeout) = 0;

    /**
     * Response to remote call request received and monitor response status via callback
     * Can be called from the callback thread.
     * @param endpoint a shared pointer to a SoaClientEndpoint that specifies the consumer to response to
     * @param response a shared pointer to a SoaMessage as response to request with endpoint specified by consumer in request message
     * @param actionResultListener a SoaActionResultListener for the status of this response option
     * @see setTimeout
     * @see remoteCallResponse(...) for possible error codes sent to the action result listener
     */
    virtual void            remoteCallResponseAsync(SoaClientEndpoint::SharedPtr endpoint,
                                                    SoaMessage::SharedPtr response,
                                                    SoaActionResultListener<SoaPublishMessageContext> & actionResultListener) = 0;

    /**
     * Response to remote call request received and monitor response status via callback
     * Can be called from the callback thread.
     * @param endpoint a shared pointer to a SoaClientEndpoint that specifies the consumer to response to
     * @param response a shared pointer to a SoaMessage as response to request with endpoint specified by consumer in request message
     * @param actionResultListener a reference to a SoaActionResultListener for the status of this response option
     * @param timeout a integer specifying the timeout for response action in ms
     * @see remoteCallResponse(...) for possible error codes sent to the action result listener
     */
    virtual void            remoteCallResponseAsync(SoaClientEndpoint::SharedPtr endpoint,
                                                    SoaMessage::SharedPtr response,
                                                    SoaActionResultListener<SoaPublishMessageContext> & actionResultListener,
                                                    int32_t timeout) = 0;

    /**
     * Response to remote call request received, does not return until response sent
     * This method returns when remote call response is complete, when the blanket timeout is reached or when an error occurs
     * @param endpoint a shared pointer to a SoaClientEndpoint that specifies the consumer to response to
     * @param response a shared pointer to a SoaMessage as response to request with endpoint specified by consumer in request message
     * @return a SoaErrorCode specifying the error occurred during response action if any
     * @return SoaErrorCode::NO_ERROR                    if successful
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
    virtual SoaErrorCode    remoteCallResponse(SoaClientEndpoint::SharedPtr endpoint, SoaMessage::SharedPtr response) = 0;

    /**
     * Response to remote call request received, does not return until response sent or timeout reached
     * This method returns when remote call response is complete, when the timeout is reached or when an error occurs
     * @param endpoint a shared pointer to a SoaClientEndpoint that specifies the consumer to response to
     * @param response a shared pointer to a SoaMessage as response to request with endpoint specified by consumer in request message
     * @param timeout a integer specifying the timeout for response action in ms, must be a positive int <= 3600000 mSec (1 hour)
     * @return SoaErrorCode::NO_ERROR                    if successful
     * @return SoaErrorCode::ERROR_ILLEGAL_CALL_THREAD   if called from the callback thread
     * @return SoaErrorCode::ERROR_INVALID_ENDPOINT_PARAM if the endpoint pointer or string is not valid
     * @return SoaErrorCode::ERROR_INVALID_MESSAGE_PARAM if the message pointer is null
     * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if (timeout <= 0 || timeout > 3600000 mSec (1 hour))
     * @return SoaErrorCode::ERROR_TIMEOUT               if not complete within the timeout
     * @return SoaErrorCode::ERROR_NOT_CONNECTED         if not connected to broker
     * @return SoaErrorCode::ERROR_EMPTY_TOPIC           if the endpoint topic string is empty
     * @return SoaErrorCode::ERROR_PUBLISH_FAILED        if the MQTT client failed to publish the message
     * @return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if the message object failed to be serialized
     */
    virtual SoaErrorCode    remoteCallResponse(SoaClientEndpoint::SharedPtr endpoint, SoaMessage::SharedPtr response, int32_t timeout) = 0;

    /**
     * Destructor
     */
    virtual ~SoaProvider() = default;

protected:
    SoaProvider() = default;                        // explicitly protected
private:
    SoaProvider(const SoaProvider &);               // explicitly disabled
    SoaProvider & operator= (const SoaProvider &);  // explicitly disabled
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAPROVIDER_HPP_ */

