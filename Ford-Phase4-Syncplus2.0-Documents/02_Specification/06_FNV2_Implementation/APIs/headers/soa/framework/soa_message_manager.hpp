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
 * SoaMessageManager.h
 *
 *  Created on: Mar 23, 2017
 *      Author: PREISSNE
 */

#ifndef FNVSOA_FRAMEWORK_SOAMESSAGEMANAGER_HPP_
#define FNVSOA_FRAMEWORK_SOAMESSAGEMANAGER_HPP_

#include <soa/framework/soa_error_code.hpp>
#include <soa/framework/soa_connection_state.hpp>
#include <soa/framework/soa_connection_options.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * @class SoaMessageManager
 *
 * The SOA client must instantiate an object of this class to communicate with
 * other clients in the system.  There are two choices for the message transport
 * mechanism: MQTT and IPC.
 * This class includes factory methods for creating the message manager object
 * needed by the SOA client. The factory methods return a object of this type.
 * This class represents an opaque reference (empty interface) to the message
 * manager object, whose private methods are used by the SOA Middleware layer.
 *
 * A SOA client does not need to instantiate more than one message manager object
 * per transport type for a set of connection options.
 *
 */

class SoaMessageManager {
public:
    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaMessageManager> SharedPtr;

    /**
     * @brief createMessageManagerMqtt - MQTT message manager factory method
     * @deprecated This factory have been replace by the createMessageManager. The interface will
     *             be specified by the type setup in the options.
     * @param options Reference to a connection option object specifying options to be
     *                used for the connection.
     *                NOTE: The options object is copied by this method, so it is safe to destroy
     *                the options object after this method returns. Any changes made to options
     *                after this method is called are not visible to the message manager.
     * @return a pointer to a SoaMessageManager if the SoaConnectionOptions reference is not null,
     *         otherwise return nullptr
     */
    static SoaMessageManager::SharedPtr createMessageManagerMqtt(SoaConnectionOptions & options);

    /**
     * @brief createMessageManager - Message Manager factory method
     * @param - SoaConnectionOptionsCommon - create the options specific to the interface being used.
     * @return a pointer to a SoaMessageManager
     */
    static SoaMessageManager::SharedPtr createMessageManager(SoaConnectionOptionsCommon & options);

    /**
     * @brief Connect to the transport layer after an explicitly requested disconnect
     *
     * IMPORTANT: This method cannot be called to create an first time connection.
     * To create a first time connection,
     *    call either SoaConsumer::initialize() or SoaProvider::initialize()
     *    This method has no effect unless the transport layer has been disconnected
     *    after a client class has been initialized. Therefore this method is used to
     *    connect all disconnected previously-initialized clients.
     * @return SoaErrorCode::NO_ERROR if successful
     * @return SoaErrorCode::ERROR_NOT_INITIALIZED if called before a client has initialized itself
     * @return SoaErrorCode::ERROR_CLIENT_ID_NOT_SET if the client id has not been set
     * @return SoaErrorCode::ERROR_CANNOT_CREATE_CLIENT if the MQTT client object creation fails
     * @return SoaErrorCode::ERROR_WRONG_STATE if already connected
     * @return SoaErrorCode::ERROR_FAILED_SETTING_CALLBACKS if binding internal functions to MQTT client fails
     * @return SoaErrorCode::ERROR_MQTT_SERVER_UNAVAILABLE if the server is unavailable
     * @return SoaErrorCode::ERROR_CONNECT_FAIL if the MQTT client fails to connect for another reason
     */
    virtual SoaErrorCode connect() = 0;

    /**
     * @brief
     * Connect to the transport layer asynchronously.
     * Do not block the calling thread.
     *
     * $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
     *
     * This method is used differently on TCU from other ECUs such as SYNC and ECG.
     *
     * TCU manages its SOA gateway clients using a master/slave model. Only the
     * tcumaind app is aware of the power states. It is the master and the only
     * TCU app that is allowed to call connectAsync() to trigger the gateway to
     * connect to the MQTT broker. All other SOA client apps are slaves such that
     * they wait to be connected.
     * Only the master on TCU calls disconnectGlobalAsync() to force the gateway to
     * disconnect from the broker.
     *
     * Other ECUs don't have a master process, and generally don't need to be
     * aware that a gateway may or may not be involved with their connection to
     * the MQTT broker, or that other apps exist. Each of these call connectAsync().
     *
     * On an ECU with a gateway, only the first will trigger the gateway to connect
     * to the broker. Subsequent apps will have accessed the gateway while in a
     * connecting or connected state.
     * These SOA clients MUST NOT call disconnectGlobalAsync() as this is
     * reserved for a master process to disconnect the gateway from the broker.
     * Peer gateway clients call disconnect() or disconnectAsync() to disconnect
     * from the gateway. Note, the gateway automatically disconnects from the
     * broker when the last client disconnects from the gateway.
     *
     * On the ECG ECU, each SOA app connects itself to the broker via connectAsync().
     * These apps disconnect via disconnect() or disconnectAsync()
     *
     * $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
     *
     * If attempts to connect fail because the server is unavailable, waits and
     * tries again according to the algorithm configured via the connection
     * options auto-reconnect parameters.
     * If a configuration error is found which precludes attempting to connect
     * to the broker such as no connection listener is assigned, this method
     * will return immediately with an error identifying the issue.
     * If a connection listener has been assigned, and a configuration error is
     * found, the error will be reported immediately via the listener callback
     * method.
     * If the server is unavailable, connection will be repeatedly reattempted
     * according to the configured auto-reconnect algorithm configured via the
     * connection options parameters, or the default algorithm is used.
     *
	 * If not connecting directly to the broker, but instead connecting through
	 * a gateway, this method will initiate a request for the gateway to connect
	 * to the broker.
	 *
     * Note:
     * If already connected when this method is called, NO_ERROR is returned and
     * the connection listener callback is called on the callback thread with a
     * status of NO_ERROR
     *
     * WARNING:
     * If calling connectAsync() to pre-connect the SOA app to the broker
     * to avoid failing on calls to the blocking methods SoaConsumer::initialize() and
     * SoaProvider:initialize(), you MUST wait for your implementation of
     * SoaConnectionListener::onConnect(...) to be called before calling initialize().
     *
     * @return SoaErrorCode::NO_ERROR if no configuration errors exist which would
     *             preclude the connect algorithm from executing in the background.
     * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if connection options
     *            connectionListener is not set
     * @return SoaErrorCode::ERROR_CLIENT_ID_NOT_SET if the client id has not been set
     * @return SoaErrorCode::ERROR_CANNOT_CREATE_CLIENT if the MQTT client object creation fails
     * @return SoaErrorCode::ERROR_WRONG_STATE if connectAsync() or reconnect() already called, or
     *                auto-reconnect is enabled and a background attempt to connect is
     *                currently active.
     * @return SoaErrorCode::ERROR_FAILED_SETTING_CALLBACKS if binding internal functions to MQTT client fails
     * @return SoaErrorCode::ERROR_CONNECT_FAIL if the MQTT client fails to connect for another reason
     *
     * One of the following error codes is passed to the connection options
     * connection listener.
     * SoaErrorCode::NO_ERROR if connection was established successfully
     * SoaErrorCode::ERROR_WRONG_STATE
     * SoaErrorCode::ERROR_TIMEOUT if the reconnect timeout has been reached while
     *             repeated attempts to connect have failed due to server unavailable
     * SoaErrorCode::ERROR_CONNECT_FAIL if the MQTT client fails to connect for
     *             a reason other than server unavailable
     */
    virtual SoaErrorCode connectAsync();

    /**
     * @brief disconnectAsync - Disconnect the local process' message manager from the transport layer
     * This method may also be called to cancel an active background attempt to
     * connect via connectAsync() or reconnect() or auto-reconnect.
     * If a disconnect listener has been configured in the connection options, it will be called,
     * but only if this method returns NO_ERROR.
     * If not connected directly to the broker, but instead connected through a gateway,
     * this method will disconnect from the gateway. The gateway will not disconnect from
     * the broker if there are any other gateway clients connected.
     * @return SoaErrorCode::NO_ERROR if connected and disconnect initiated
     * @return SoaErrorCode::ERROR_NOT_CONNECTED if not connected
     */
    virtual SoaErrorCode disconnectAsync();

    /**
     * @brief disconnect - Disconnect the local process' message manager from the transport layer
     * This method may also be called to cancel an active background attempt to 
     * connect via connectAsync() or reconnect() or auto-reconnect.
     * If not connected directly to the broker, but instead connected through a gateway,
     * this method will disconnect from the gateway. The gateway will not disconnect from
     * the broker if there are any other gateway clients connected.
     * @return SoaErrorCode::NO_ERROR if successful
     * @return SoaErrorCode::ERROR_NOT_CONNECTED if not connected
     * @return SoaErrorCode::ERROR_DISCONNECT_FAIL if MQTT client fails to disconnect
     */
    virtual SoaErrorCode disconnect() = 0;

    /**
     * @brief reconnect - Starts asynchronous reconnect attempts, only works if the connection
     * was unwillingly lost. This method returns an error if called after the connection
     * was explicitly disconnected by a call to disconnect() while connected.
     * Auto-reconnect is not a prerequisite to call reconnect, but the reconnection
     * timeout parameter must not be 0 and a reconnection listener must be assigned in
     * the connection options.
     * If already connected, this method returns NO_ERROR, and the reconnection listener
     * is called on the callback thread with an error code of NO_ERROR
     * @return SoaErrorCode::NO_ERROR if the reconnection attempt has begun or already connected
     * @return SoaErrorCode::ERROR_WRONG_STATE if the connection has not been lost
     * @return SoaErrorCode::ERROR_INVALID_CALLBACK_PARAM if no reconnection listener set
     *
     * One of the following error codes is passed to the connection options
     * reconnection listener.
     * SoaErrorCode::NO_ERROR if connection was established successfully
     * SoaErrorCode::ERROR_WRONG_STATE
     * SoaErrorCode::ERROR_TIMEOUT if the reconnect timeout has been reached while
     *             repeated attempts to connect have failed due to server unavailable
     * SoaErrorCode::ERROR_CONNECT_FAIL if the MQTT client fails to connect for
     *             a reason other than server unavailable
     */
    virtual SoaErrorCode reconnect() = 0;

    /**
     * @brief disconnectGlobalAsync - Disconnect the all processes' message manager from the transport layer
     * This method is only used for the master/dependent model where a master process disconnects
     * the gateway from the broker, and dependent processes co-resident on the machine will be notified
     * by the gateway.
     * This method may also be called to cancel an active background attempt to
     * connect via connectAsync() or reconnect() or auto-reconnect.
     * If a disconnect listener has been configured in the connection options of any dependent process'
     * message manager, and the respective dependent processes are connected to the gateway,
     * the disconnect listener callback will be called, but only if this method returns NO_ERROR.
     * @return SoaErrorCode::NO_ERROR if successful
     * @return SoaErrorCode::ERROR_NOT_CONNECTED if not connected
     */
    virtual SoaErrorCode disconnectGlobalAsync();

    /**
     * @brief connectDependentAsync - non-blocking connect of a process to the gateway
     * This method is used for the master/dependent model where a dependent process connects
     * to the gateway without generating a request for the gateway to connect to the broker.
     * REQUIREMENTS:
     * connectionListener configured
     * - when connected to the gateway only, callback gets an error code of GATEWAY_CONNECTION_ONLY
     *
     */
    virtual SoaErrorCode connectDependentAsync();

    /**
     * @brief disconnectDependentAsync - pass-through wrapper for disconnectAsync
     */
    virtual SoaErrorCode disconnectDependentAsync();

    /**
     * @brief getConnectedState
     * Get the current connection state
     * @return CONNECTED - the connection is active; methods which require the connection may be called
     * @return DISCONNECTED - the connection is not active; methods which require the connection will
     *                        fail and return NOT_CONNECTED
     * @return CONNECTING - the library is attempting to connect, but the connection is not active; methods
     *                      which require the connection will fail and return NOT_CONNECTED. Additional
     *                      attempts to connect will be rejected.
     * @return CONNECTION_LOST - The connection has been active but was lost
     */
    virtual SoaConnectionState getConnectionState();

    /**
     * Create an endpoint
     * @param topic a string defining the topic of the endpoint
     * @return a a shared pointer to a SoaClientEndpoint or nullptr if topic is empty
     */
    virtual SoaClientEndpoint::SharedPtr createClientEndpoint(const std::string & topic);

    /**
     * Create an endpoint
     * @param topic a StringArray created by one of the SoaClientEndpoint constexpr methods
     *              createDataEndpointName, createRequestEndpointName or createResonseEndpointName
     * @return a a shared pointer to a SoaClientEndpoint or nullptr if topic is empty
     */
    template <int stringLength>
    SoaClientEndpoint::SharedPtr createClientEndpoint(const internal::StringArray<stringLength> & topic) {
        return createClientEndpoint( std::string( topic ) );
    }

    /**
     * @brief Destructor
     */
    virtual ~SoaMessageManager();
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAMESSAGEMANAGER_HPP_ */
