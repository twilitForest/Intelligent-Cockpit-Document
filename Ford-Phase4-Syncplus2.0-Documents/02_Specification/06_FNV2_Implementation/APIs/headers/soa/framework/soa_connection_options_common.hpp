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
 * soa_connection_options_common.hpp
 *
 *  Created on: Nov 30, 2017
 *      Author: drati
 */

#ifndef FNVSOA_FRAMEWORK_SOACONNECTIONOPTIONSCOMMON_HPP_
#define FNVSOA_FRAMEWORK_SOACONNECTIONOPTIONSCOMMON_HPP_

#include <soa/framework/soa_message.hpp>
#include <soa/framework/soa_connection_listener.hpp>
#include <soa/framework/soa_reconnection_listener.hpp>
#include <soa/framework/soa_connection_lost_listener.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Container class used to set connection options.
 */
class SoaConnectionOptionsCommon {

public:

    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaConnectionOptionsCommon> SharedPtr;

    /**
     * Configures keep alive interval
     * @param keepalive a integer specifying keep alive interval in seconds
     */
    virtual void setKeepAliveInterval(int32_t keepalive) = 0;

    /**
     * Returns the connection "keep alive" interval in seconds.
     * @return keep alive interval in seconds
     */
    virtual int32_t getKeepAliveInterval() = 0;

    /**
     * Configures clean session option.
     * If true, the connection state information is discarded at connect and
     * disconnect.
     * If false, stored state information is reinstated based on matching
     * client id and server address.
     * @param isCleanSession true or false
     */
    virtual void setCleanSession(bool isCleanSession) = 0;

    /**
     * Returns the clean session option
     * @return returns clean session
     */
    virtual bool isCleanSession() = 0;

    /**
     * @DEPRECATED
     * This method no longer sets the client id. The client id used for the MQTT broker is
     * now set automatically by the SOA Framework
     */
    virtual void setClientId(const std::string & clientId) = 0;

    /**
     * Get the MQTT client ID
     * @return the client ID
     * @see SoaConnectionOptionsCommon::setClientId
     */
    virtual std::string getClientId() = 0;

    /**
     * Gets the implemented listener to be called on ungraceful disconnect
     * @return implemented ConnectionLostListener if set, else nullptr if none
     */
    virtual SoaConnectionLostListener * getConnectionLostListener() = 0;

    /**
     * Gets the implemented listener to be called on reconnection.
     * @return implemented ReconnectionListener if set, else nullptr if none
     */
    virtual SoaReconnectionListener * getReconnectionListener() = 0;

    /**
     * Gets the implemented listener to be called on connection.
     * @return implemented ConnectionListener if set, else nullptr if none
     */
    virtual SoaConnectionListener * getConnectionListener() = 0;

    /**
     * Sets the implemented listener to be called on ungraceful disconnect
     * The listener is not called when the connection is willfully disconnected
     * by calling disconnect or by destroying the message manager object.
     * The SoaConnectionLostListener::onConnectionLost receives a string
     * that describes the cause of the lost connection
     * @param listener - implemented ConnectionLostListener
     */
    virtual void setConnectionLostListener(SoaConnectionLostListener & listener) = 0;

    /**
     * Sets the implemented listener to be called on reconnection.
     * The SoaReconnectionListener::onReconnect receives an errorCode.
     * The errorCode is ERROR_TIMEOUT if the reconnection times out.
     * The errorCode is ERROR_WRONG_STATE if the state is changed during the reconnect process (usually by a call to disconnect).
     * The errorCode is NO_ERROR if the reconnection is successful.
     * @param listener - implemented ReconnectionListener
     */
    virtual void setReconnectionListener(SoaReconnectionListener & listener) = 0;

    /**
     * Sets the implemented listener to be called on connection.
     * The SoaConnectionListener's onConnect receives an errorCode.
     * The errorCode is ERROR_TIMEOUT if the connection attempt times out.
     * The errorCode is ERROR_WRONG_STATE if the state is changed during the connect process (usually by a call to disconnect).
     * The errorCode is NO_ERROR if the connection is successful.
     * @param listener - implemented ReconnectionListener
     */
    virtual void setConnectionListener(SoaConnectionListener & listener) = 0;

    /**
     * Transport type
     */
    enum class optionType {
        MQTT,
        IPC
    };

    /**
     * Get the Transport option used by these connection options
     */
    virtual optionType getOptionType() = 0;

    /**
     * Default destructor
     */
    virtual ~SoaConnectionOptionsCommon() = default;

    static const std::string CLIENTID_NOT_SET;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOACONNECTIONOPTIONSCOMMON_HPP_ */
