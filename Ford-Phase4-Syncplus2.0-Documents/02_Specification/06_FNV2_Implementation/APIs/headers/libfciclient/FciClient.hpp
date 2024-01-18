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
 * FciClient.hpp
 *
 *  Created on: Nov 26, 2017
 *      Author: mseman3
 */

#ifndef FCICLIENT_HPP
#define FCICLIENT_HPP

#include <memory>
#include <string>
#include <unordered_set>
#include <soa/framework/soa_consumer.hpp>
#include <soa/framework/soa_provider.hpp>
#include <soa/framework/soa_message.hpp>
#include <vim/idl/vim_if.pb.h>

namespace fnv {
namespace fci {
namespace client {

/**
 * @brief
 * Enumeration of possible error codes returned by some methods in this library
 */
enum class FciErrorCode {
    FCI_NO_ERROR,
    FCI_NO_COMMANDS_REGISTERED,
    FCI_INVALID_PARAMETER,
    FCI_SERVICE_MANAGER_ERROR,
    FCI_GENERAL_ERROR
};

/**
 * @brief
 * Enumeration values to configure Alert message DRX
 */
enum class FciDrxConfig : unsigned char {
    DRXCONFIG_0 = 0, // Alert is never sent in DRX
    DRXCONFIG_1 = 1, // Alert is held, send is delayed until next DRX Wakeup, IGN ON, etc...
    DRXCONFIG_2 = 2, // Alert sent immediately: DRX reset
    DRXCONFIG_3 = 3, // Alert sent immediately: DRX reset only if VBATT is not low, otherwise connection time extended
    DRXCONFIG_4 = 4, // Alert sent immediately: DRX reset only if VBATT is not low; Connection time not extended
    DRXCONFIG_5 = 5, // Alert sent immediately: DRX continues; Connection time extended
    DRXCONFIG_6 = 6, // Alert sent immediately: DRX continues; Connection time extended only if VBATT is not low
    DRXCONFIG_7 = 7 // Alert sent immediately: DRX continues; Connection time not extended
};

/**
 * @brief
 * Enumeration values to configure Alert 'quality of service'
 */
enum class FciQos : unsigned char {
    QOS_1 = 1, // at least once
    QOS_2 = 2 // exactly once
};

/**
 * @brief
 * Enumeration of possible connection interface types SDN can be connected over
 */
enum class FciSdnConnectionInterfaceType : unsigned char {
    UNKNOWN = 0,
    TCU_CELL = 1,
    TCU_WIFI = 2,
    SYNC_WIFI = 3,
    SYNC_SDL = 4
};

/**
 * @brief
 * Enumeration of possible vehicle power state indications that can be
 * broadcast via BroadcastMessageListener
 */
enum class FciPowerStateIndication : unsigned char {
    UNKNOWN = 0,
    FPO     = 1, // Full Power Igintion ON
    FPC     = 2, // Full Power Igition OFF, Charging ON
    FPS     = 3, // Full Power Ignition OFF, Charging OFF
    LPR     = 4, // Low Power Registered
    DWU     = 5, // DRX Wake Up
    HSLP    = 6, // High-Scan Sleep
    LSLP    = 7, // Low-Scan Sleep
    DSLP    = 8  // Deep Sleep
};

/**
 * @brief
 * Enumeration of FTCP message type to be sent (in delivery status)
 */
enum class FciFtcpMessageType : unsigned char {
    UNKNOWN          = 0,
    COMMAND_RESPONSE = 1,
    ALERT            = 2,
    QUERY            = 3
};

/**
 * @brief
 * Enumeration of possible message delivery outcomes (in delivery status)
 */
enum class FciMessageDeliveryStatus : unsigned char {
    UNKNOWN             = 0,
    SEND_SUCCEEDED      = 1, // message sent to FCI
    SEND_FAILED        = 2,  // message cannot be sent to FCI
    DELIVERY_SUCCEEDED  = 3, // message delivered to SDN
    DELIVERY_FAILED     = 4, // message cannot be delivered to SDN
    DELIVERY_FAILED_INVALID_FORMAT = 5, // message discarded due to invalid format
    DELIVERY_QUEUE_OVERFLOW  = 6 // message dropped due to internal buffer overflow
};

/**
 * @brief
 * Enumeration of SMS type 2 message payload type
 */
enum class SmsType2PayloadType : unsigned char {
    HURL = 1,
    FAPN = 2,
    HURL_FAPN = 3
};

/**
 * @brief
 * Enumeration of authorization status
 */
enum class AuthorizationStatus : unsigned char {
    NOT_AUTHORIZED = 0,
    AUTHORIZED = 1
};

/**
 * @brief
 * Enumeration of anonymization status
 */
enum class AnonymizationStatus : unsigned char {
    ANONY_OFF = 0,
    ANONY_ON = 1
};

/**
 * @brief
 * Structure of DateTime returned from getXxxDateTime
 */
struct DateTime {
    int32_t mYear;
    int32_t mMonth;
    int32_t mDay;
    int32_t mHour;
    int32_t mMin;
    int32_t mSecond;
    int32_t mMillis;
};

/**
 * @brief
 * Abstract class to hold FTCP message data.
 * You need to provide your own definition/implementation of this class.
 * Transaction ID is set by FCI on messages received from SDN (e.g. FTCP command).
 * Your response to received messages should be set to the same transaction ID value (e.g. FTCP command response).
 * You must set your own transaction ID for messages originated from your app (e.g. FTCP Query).
 * The response to that message (e.g. FTCP Query Response) will have that same transaction ID,
 * allowing you to know which message was responded to.
 * Transaction ID on outgoing messages will also allow you to track delivery status per message.
 * Payload is serialized FTCP message. It is up to your app to deserialize and consume received
 * messages and to assemble and serialize transmitted messages.
 */
class FciData {
public:
    explicit FciData(const std::string & transactionId);
    virtual ~FciData();

    /**
     * @brief
     * Return message transaction ID.
     * @return
     */
    virtual const std::string & getTransactionId() const;

    /**
     * @brief
     * Set FTCP message transaction ID.
     */
    virtual void setTransactionId(const std::string & transactionId);

    /**
     * @brief
     * Return FTCP message that is serialized.
     * @return
     */
    virtual std::shared_ptr<std::string> getPayload() const;

    /**
     * @brief
     * Set FTCP message that is serialized.
     * Using this method transfers ownership of the payload to the FciData object.
     */
    virtual void setPayload(std::shared_ptr<std::string> payload);

protected:
    explicit FciData();
private:
    class FciDataImpl;
    std::unique_ptr<FciDataImpl> m_dataImpl;
};

/**
 * @brief
 * Abstract class to be implemented by your app if you are interested in one or more of
 * the information broadcasted by FCI. Implement one or more of the callbacks according
 * which information you are interested in.
 */
class BroadcastMessageListener {
public:
    virtual ~BroadcastMessageListener() = default;

    /**
     * @brief
     * SDN connection change callback
     * This callback provides information on SDN connectivity acquired (connected = true)
     * or lost (connected = false)
     */
    virtual void onSdnConnectionStatusChanged(FciSdnConnectionInterfaceType interface,
            bool connected);

    /**
     * @brief
     * Vehicle power state change callback
     * This callback provides information when new power state takes effect on the vehicle
     */
    virtual void onPowerStateChanged(FciPowerStateIndication state);

    /**
     * @brief
     * SMS type 2 Message broadcast callback
     * This callback provides SMS type 2 message payload which provide either home URL or Ford APN or both
     */
    virtual void onSmsType2MessageReceived(SmsType2PayloadType type, const std::string & homeUrl, const std::string & fordApn);
};

/**
 * @brief
 * This Listener is deprecated. Should use DeliveryStatusListener instead.
 * Abstract class to be implemented by your app if you would like to receive confirmation
 * of messages sent by you
 */
class DeliveryConfirmationListener {
public:
    virtual ~DeliveryConfirmationListener() = default;

    /**
     * @brief
     * Listener method to provide the client with additional information in case
     * the message delivery failed.
     * @param transactionId - transaction id of the failed message
     * @param status - indicates the outcome of attempted message delivery (success / failure)
     */
    virtual void onDeliveryConfirmed(const std::string & transactionId, FciMessageDeliveryStatus status) = 0;
};

/**
 * @brief
 * Abstract class to be implemented by your app if you would like to receive delivery status
 * of messages sent by you
 */
class DeliveryStatusListener {
public:
    virtual ~DeliveryStatusListener() = default;

    /**
     * @brief
     * Listener method to provide the client with the message delivery status.
     * The message can be identified by using message type and transaction id.
     * @param msgType - the message type
     * @param transactionId - transaction id of the message
     * @param status - indicates the outcome of attempted message delivery
     */
    virtual void onDeliveryStatusReceived(FciFtcpMessageType msgType, const std::string & transactionId, FciMessageDeliveryStatus status) = 0;
};

/**
 * @brief
 * Abstract class to be implemented by your app if you would like to receive FTCP messages from SDN
 */
class FtcpMessageListener {
public:
    virtual ~FtcpMessageListener() = default;
    /**
     * @brief
     * Listener method to be invoked when FTCP message (e.g.: Command or Query Response) is received.
     * @param messageId - FTCP command id or Query_Response id
     * @param ftcpCommand - FciData object instance
     */
    virtual void onMessageReceived(const std::string & messageId, std::shared_ptr<FciData> fciData) = 0;
};

/**
 * @brief
 * Function registerForFtcpCommands/registerForFtcpCommandsAsync removed
 */
class FtcpCommandRegistrationListener {
public:
    virtual ~FtcpCommandRegistrationListener() = default;
    /**
     * @brief
     * Will never be called
     */
    virtual void onCommandRegistrationComplete(FciErrorCode error) = 0;
};

/**
 * @class FciClient
 * Public library to facilitate the use of the Ford Cloud Interface.
 */
class FciClient {
public:
    /**
     * @brief
     * Returns FciClient object pointer AND ownership. Caller responsible for deletion.
     * Each call of this method creates a new instance.
     *
     * Requires properly initialized SoaConsumer and SoaProvider objects. The caller has
     * ownership of the SoaConsumer and Provider objects. FciClient uses weak pointers
     * to keep SoaConsumer and Provider references.
     *
     * You should not need to make subsequent calls to createInstance() if an instance
     * was already created for you successfully. If you do, you must supply new provider
     * and consumer references (as opposed to reusing the ones used before).
     *
     * @param consumer - the shared pointer of SoaConsumer
     * @param provider - the shared pointer of SoaProvider
     * @param uniqueServiceId - service id MUST be unique among SOA components
     *      The name of your app/service can be used. All capital letters.
     * @return new instance of FciClient object if the parameters are not null,
     *          nullptr otherwise
     */
    static std::shared_ptr<FciClient> createInstance(
            std::shared_ptr<fnv::soa::framework::SoaConsumer> consumer,
            std::shared_ptr<fnv::soa::framework::SoaProvider> provider,
            const std::string & uniqueServiceId);

    virtual ~FciClient() = default;

    /**
     * @brief
     * This method clears all listeners previously set and revokes all FTCP command
     * registrations.
     *
     * This method MUST be called before the object returned from createInstance()
     * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
     * is deleted, or the reference to it goes out of scope.
     * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
     *
     * However, it is possible to continue using the FciClient instance after cleanup
     * was called. You'd need to re-set listeners and re-register commands in that case.
     */
    virtual void cleanup() = 0;

    /**
     * @brief
     * Set listener that will be invoked each time when FTCP message is received.
     * The caller has ownership of the listener objects and FciClient uses weak
     * pointers to store the reference.
     * Set nullptr or delete the listener object to unregister.
     *
     * Each param is optional; you only need to set those listeners which you need.
     *
     * @param cmdReceived - Listener to be invoked when the FTCP command registered
     *                      through registerFTCPCommand() is received.
     * @param queryRespReceived - Listener to be invoked when the Query_Response is received.
     * @param alertRespReceived - Listener to be invoked when the Alert_Response is received.
     * @return true if succeeded, false otherwise
     */
    virtual bool setFtcpMessageListeners(   std::shared_ptr<FtcpMessageListener> cmdReceived = nullptr,
                                            std::shared_ptr<FtcpMessageListener> queryRespReceived = nullptr,
                                            std::shared_ptr<FtcpMessageListener> alertRespReceived = nullptr) = 0;

    /**
     * @brief
     * This method is deprecated. Should use setDeliveryStatusListener instead.
     * Set listener that will be invoked when FCI receives a delivery token after sending
     * FTCP message to SDN broker.
     * The caller has ownership of the listener object and FciClient uses weak
     * pointer to store the reference. Set nullptr or delete the listener object to unregister.
     * @param deliveryConfirmed - Listener to be invoked when a delivery confirmation message is received.
     * @return true if succeeded, false otherwise
     */
    virtual bool setDeliveryConfirmationListener(std::shared_ptr<DeliveryConfirmationListener> deliveryConfirmed) = 0;

    /**
     * @brief
     * Set listener that will be invoked when
     * - The client sends a message to FCI successfully.
     * - The client cannot send a message to FCI.
     * - FCI delivers the message to the cloud successfully.
     * - FCI cannot deliver the message to the cloud.
     * - FCI drops the message as the message format is invalid.
     * - FCI drops the message due to FCI sender queue overflow.
     *
     * The caller has ownership of the listener object and FciClient uses weak
     * pointer to store the reference. Set nullptr or delete the listener object to unregister.
     * @param deliveryStatusReceived - Listener to be invoked when a delivery status message is received.
     * @return true if succeeded, false otherwise
     */
    virtual bool setDeliveryStatusListener(std::shared_ptr<DeliveryStatusListener> deliveryStatusReceived) = 0;

    /**
     * @brief
     * Set listener that will be invoked for FCI broadcast message callbacks
     * The caller has ownership of the listener object and FciClient uses weak
     * pointer to store the reference. Set nullptr or delete the listener object to unregister.
     * @param messageReceived - Listener to be invoked for implemented callbacks.
     * @return true if succeeded, false otherwise
     */
    virtual bool setBroadcastMessageListener(std::shared_ptr<BroadcastMessageListener> messageReceived) = 0;

    /**
     * Function deprecated
     */
    virtual FciErrorCode registerForFtcpCommands(const std::unordered_set<std::string> & cmdNames) = 0;

    /**
     * Function deprecated
     */
    virtual FciErrorCode registerForFtcpCommandsAsync(const std::unordered_set<std::string> & cmdNames,
            std::shared_ptr<FtcpCommandRegistrationListener> registrationComplete) = 0;

    /**
     * Function deprecated
     */
    virtual FciErrorCode deregisterFromAllFTCPCommands() = 0;

    /**
     * @brief
     * Synchronous call to send a FTCP command response
     * @param cmdResponse - FciData containing the transaction ID from the received command and your response payload
     * @param deliveryStatus - true to get delivery notification through DeliveryStatusListener.
     * @return
     */
    virtual FciErrorCode sendCommandResponse(const FciData & cmdReponse, bool deliveryStatus) = 0;

    /**
     * @brief
     * Synchronous call to send an FTCP alert. Option to indicate when a response is expected.
     * FciClient will create an endpoint for delivery status.
     * @param alert - FciData containing your transaction ID and the alert payload
     * @param deliveryStatus - true to get delivery notification through DeliveryStatusListener
     * @param drxConfig - enum value to define the behavior in drx mode
     * @param qos - Quality of Service level. If there is no particular reason otherwise, use 1.
     * @param expectAlertResponse - true to indicate the sender expects SDN to send back alert response message
     * @return
     */
    virtual FciErrorCode sendAlert(const FciData & alert,
                             bool deliveryStatus,
                             FciDrxConfig drxConfig,
                             FciQos qos = FciQos::QOS_1,
                             bool expectAlertResponse = false) = 0;

    /**
     * @brief
     * Synchronous call to send a FTCP SDN query.
     * FciClient will create an endpoint for QUERY_RESPONSE and delivery status.
     * @param query - FciData containing your transaction ID and the query payload
     * @param deliveryStatus - true to get delivery notification through DeliveryStatusListener
     * @return
     */
    virtual FciErrorCode sendQuery(const FciData & query, bool deliveryStatus) = 0;

    /**
     * @brief
     * Synchronous request to connect SDN using the provided network interface type.
     * if ConnectionStatusListener is registered, the listener method will be invoked when sdn connection is established.
     * @param interface - network interface type, cannot be UNKNOWN
     * @param url - SDN url  (e.g. tcp://VSDNStub.azure-devices.net:1883, ssl://VSDNStub.azure-devices.net:8883)
     * @param wifiSsid - wifi AP ssid. This param is only applicable if wifi interface is indicated (ignored otherwise)
     * @param wifiPassword - wifi AP password. This param is only applicable if wifi interface is indicated (ignored otherwise)
     * @param withReceiptConfirmation - if false, this method sends the connect request and returns
     *                                - if true, the method will wait for acknowledgement of request receipt from FCI
     *                                  before returning, blocking until ack is received (i.e. longer time)
     * @return  FCI_INVALID_PARAMETER if request is for interface type UNKNOWN
     *          FCI_GENERAL_ERROR if protobuf parsing error occurs, or
     *                            receipt acknowledgement failure to sent request, or
     *                            receipt acknowledgement request sent, but not acknowledged
     *          FCI_NO_ERROR if disconnection request was sent (withReceiptConfirmation false)
     *                       or received successfully (withReceiptConfirmation true)
     */
    virtual FciErrorCode requestConnectSdn(FciSdnConnectionInterfaceType interface,
                                const std::string & url,
                                const std::string & wifiSsid,
                                const std::string & wifiPassword,
                                bool withReceiptConfirmation) = 0;

    /**
     * @brief
     * Request to disconnect SDN
     * if ConnectionStatusListener is registered, the listener method will be invoked when sdn connection is closed.
     * @param interface - network interface type, cannot be UNKNOWN
     * @param withReceiptConfirmation - if false, this method sends the disconnect request and returns
     *                                - if true, the method will wait for acknowledgement of request receipt from FCI
     *                                  before returning, blocking until ack is received (i.e. longer time)
     * @return  FCI_INVALID_PARAMETER if request is for interface type UNKNOWN
     *          FCI_GENERAL_ERROR if protobuf parsing error occurs, or
     *                            receipt acknowledgement failure to sent request, or
     *                            receipt acknowledgement request sent, but not acknowledged
     *          FCI_NO_ERROR if disconnection request was sent (withReceiptConfirmation false)
     *                       or received successfully (withReceiptConfirmation true)
     */
    virtual FciErrorCode requestDisconnectSdn(FciSdnConnectionInterfaceType interface, bool withReceiptConfirmation) = 0;

    /**
     * @brief
     * Request to update authorization status, synchronized
     * Only SPCM is allowed to make this function call
     * 
     * @param authStatus - Authorization Status Enum
     * 
     * @return  FCI_GENERAL_ERROR if protobuf parsing error occurs, or
     *                            receipt acknowledgement failure to sent request, or
     *                            receipt acknowledgement request sent, but not acknowledged, or
     *                            caller is not SPCM
     *          FCI_NO_ERROR if sent successfully
     */
    virtual FciErrorCode updateAuthorizationStatus(AuthorizationStatus authStatus) = 0;

    /**
     * @brief
     * Request to update anonymization status, synchronized
     * Only SPCM is allowed to make this function call
     * 
     * @param anonyStatus - Anonymization status Enum
     * 
     * @return  FCI_GENERAL_ERROR if protobuf parsing error occurs, or
     *                            receipt acknowledgement failure to sent request, or
     *                            receipt acknowledgement request sent, but not acknowledged, or
     *                            caller is not SPCM
     *          FCI_NO_ERROR if sent successfully
     */
    virtual FciErrorCode updateAnonymizationStatus(AnonymizationStatus anonyStatus) = 0;

    /**
     * @brief
     * Given a set of VimPrimitiveDynamicInfo, parse them and insert them into the given messages.
     * Only signals that exist in the protobuf message will be populated.
     * Returns true or false depending on whether success or not
     *
     * @param logTag - Logging Tag of the app the output should be logged under
     * @param vimDynInfoFields - Set of VimPrimitiveDynamicInfo fields
     * @param pMessage - VIM signal data will be inserted into the VehicleStatus message,
     *                   caller need to allocate this message
     * @return  true if message is filled up
     *          false if any error occurred. 
     */
    static bool createFtcpVehicleStatus(const std::string & logTag,
                                        const google::protobuf::RepeatedPtrField<vimsoainterface::VimPrimitiveDynamicInfo> & vimDynInfoFields,
                                        google::protobuf::Message* pMessage);

    /**
     * Returns UTC time
     * @return DateTime with UTC time
     *         DateTime.mMillis always equals to zero in the return, no need to parse
     */
    static DateTime getUtcDateTime();

    /**
     * Returns RTC time
     * @return DateTime with RTC time
     *         DateTime.mMillis always equals to zero in the return, no need to parse
     */
    static DateTime getRtcDateTime();

protected:
    FciClient();
    FciClient(const FciClient&);
    FciClient& operator=(const FciClient & ref);
};

} /* namespace client */
} /* namespace fci */
} /* namespace fnv */

#endif /* FCICLIENT_HPP */
