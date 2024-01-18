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
 * SoaConnectionOptions.h
 *
 *  Created on: Mar 23, 2017
 *      Author: KGAO4
 */

#ifndef FNVSOA_FRAMEWORK_SOACONNECTIONOPTIONS_HPP_
#define FNVSOA_FRAMEWORK_SOACONNECTIONOPTIONS_HPP_

#include <soa/framework/soa_message.hpp>
#include <soa/framework/soa_connection_options_common.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Container class used to set connection options.
 */
class SoaConnectionOptions: public SoaConnectionOptionsCommon {

public:
    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaConnectionOptions> SharedPtr;

    /**
     * Create container with default connection options.
     */
    static SharedPtr createConnectionOptions();

    /**
     * Create container of MQTT default connection options
     */
    static SharedPtr createConnectionOptionsMqtt();

    /**
     * Returns the connection timeout value in seconds.
     * @return connection timeout value in seconds
     */
    virtual int32_t getConnectionTimeout() = 0;

    /**
     * Returns password associated with username applied to the connection
     * @return user password
     */
    virtual std::string getPassword() = 0;

    /**
     * Not used
     * Returns true if the response channel has been setup (for RPC)
     * @return is channel setup
     */
    virtual bool isResponseChannel() = 0;

    /**
     * Returns username associated with this connection
     * @return username
     */
    virtual std::string getUserName() = 0;

    /**
     * Get clean session option.
     * @return bool true or false
     */
    bool isCleanSession() override = 0;

    /**
     * Configures connection timeout
     * @param connTimeout a integer specifying connection timeout in seconds
     */
    virtual void setConnectionTimeOut(int32_t connTimeout) = 0;

    /**
     * Sets password to be used for connection
     * @param password a password string
     */
    virtual void setPassword(const std::string & password) = 0;

    /**
     * Sets username to be used for connection
     * @param username a username string
     */
    virtual void setUserName(const std::string & username) = 0;

    /**
     * Configures for a request-response channel
     * @param isReqRespChannel a boolean that should be set to true if
     *                         a request and/or response channel need to
     *                         be created (for RPC)
     */
    virtual void setRequestResponseChannel(bool isReqRespChannel) = 0;

    /**
     * Option set to automatically attempt reconnect after an unrequested disconnection
     * Use setRecconectionTimeOptions to set timing options
     * Use setConnectionLostListener to be notified when connection is lost
     * Use setReconnectionListener to be notified of the status of the reconnection upon completion (NO_ERROR or ERROR_TIMEOUT)
     * Reconnect can be called manually by using reconnect() from soa_message_manager.hpp
     * @param autoReconnect - true to automatically attempt reconnect, false otherwise
     */
    virtual void setAutoReconnect(bool autoReconnect) = 0;

    /**
     * Is option set to automatically attempt reconnect after an unrequested disconnection
     * @return true if reconnect will be attempted automatically, false otherwise
     */
    virtual bool isAutoReconnect() = 0;

    /**
     * The reconnection time options are used to algorithmically define the wait times between failed connection attempts.
     * The options create a single algorithm which is shared between asynchronous (non-blocking) SoaMessageManager::connectAsync()
     * requests and auto-reconnect attempts which are triggered by a connection lost notification or can be initiated by the
     * application via a call to SoaMessageManager::reconnect() if the application knows the connection was unwillingly lost.
     *
     * System continually attempts to reconnect until connection is successful as long as the reconnect timeout would not be reached,
     * where the interval between reconnection attempts is specified by the retry interval arguments. minRetryInterval is the
     * starting interval between reconnect attempts, but the interval can be configured to grow at an exponential rate using
     * the interval multiplier argument. maxRetryInterval is the maximum interval time that will be used if the multiplier
     * is greater than 1. If the next interval calculation exceeds maxRetryInterval, the interval will be set to maxRetryInterval
     * and will not grow between successive attempts. The multiplier argument MUST be greater than or equal to 1.
     *
     * In all cases - for reconnect, auto-reconnect or connectAsync - the first attempt by the algorithm to connect
     * is done immediately upon being initiated. If this initial attempt fails due to the server being unavailable, then
     * the minimum retry interval time is waited before trying again.
     * Note: after failing an attempt to connect due to server not available, if the calculated next time to try to
     * connect is greater than or equal (>=) to the reconnectTimeout, no further attempt will be made. An initial attempt
     * to connect is guaranteed if the options are accepted as valid.
     *
     * All time options are in milliseconds (must be greater than 0 ) for the reconnection system to function.
     * If all time values are 0, NO_ERROR is returned, but reconnection will not be attempted and the callback error code
     * will be ERROR_TIMED_OUT.
     * @param reconnectTimeout - (milliseconds) System continually reconnects after every interval till the overall time exceeds the timeout. 3600000ms (1 hour) by default.
     * @param minRetryInterval - (milliseconds) the starting interval time between reconnect attempts, multiplied by retryIntervalMultiplier after each attempt up to maxRetryInterval (must be less than or equal to max and timeout). 500ms (0.5 seconds) by default.
     * @param maxRetryInterval - (milliseconds) maximum interval time between reconnection attempts (must be greater than or equal to min). 60000ms (1 minute) by default.
     * @param retryIntervalMultiplier - increases interval time after each attempt by multiplying the current interval time (must be greater than or equal to 1). 2 by default.
     * @return SoaErrorCode::NO_ERROR if all parameters can be used together
     * @return SoaErrorCode::ERROR_INVALID_TIMEOUT_PARAM if the reconnectTimeout is less than 0
     * @return SoaErrorCode::ERROR_PARAM_BAD if the multiplier is less than 1.0 or the interval values do not make sense,
     *           such as the interval params are less than or equal to 0, or the minimum retry interval is greater than
     *           the maximum retry interval or the reconnect timeout, or the minimum (initial) retry interval is 0
     */
    virtual SoaErrorCode setReconnectionTimeOptions(int32_t reconnectTimeout, int32_t minRetryInterval,
                                                    int32_t maxRetryInterval, float retryIntervalMultiplier) = 0;

    /**
     * Get the timeout for reconnection in milliseconds
     * @return reconnection timeout value in milliseconds
     */
    virtual int32_t getReconnectTimeout() = 0;

    /**
     * Get the minimum retry interval between reconnection attempts in milliseconds
     * @return minimum retry interval value in milliseconds
     */
    virtual int32_t getMinRetryInterval() = 0;

    /**
     * Get the maximum retry interval between reconnection attempts in milliseconds
     * @return maximum retry interval value in milliseconds
     */
    virtual int32_t getMaxRetryInterval() = 0;

    /**
     * Get the multiplier to modify the retry interval up to the max retry interval
     * @return retry interval multiplier value
     */
    virtual float getRetryIntervalMultiplier() = 0;


    /**
     * Get if stored session exist
     * @return bool
     */

    virtual bool isSessionPresent() = 0;

    /**
     * Default destructor
     */
    ~SoaConnectionOptions() override = default;

    /**
     * FOR DEBUGGING ONLY!!!
     * Set the URL for your MQTT broker
     * During development the default is now "tcp://127.0.0.1:1883"
     * User this method to override
     * @param brokerUrl the IP address with port number of your MQTT broker ex. "tcp://10.1.0.200:1883"
     */
    virtual void setDebugBrokerUrl(const std::string & brokerUrl) = 0;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOACONNECTIONOPTIONS_HPP_ */
