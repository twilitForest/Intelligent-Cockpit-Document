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
 * SoaErrorCode.h
 *
 *  Created on: Apr 3, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOAERRORCODE_HPP_
#define FNVSOA_FRAMEWORK_SOAERRORCODE_HPP_

namespace fnv {
namespace soa {
namespace framework {

/*
 * SOA action error code
 */
enum class SoaErrorCode {
    NO_ERROR,
    ALREADY_CONNECTED,
    ERROR_ALREADY_CONNECTING,
    ERROR_NOT_IMPLEMENTED,
    ERROR_TIMEOUT,
    ERROR_PARAM_MISSING,
    ERROR_PARAM_BAD,
    ERROR_PARAM_NULL_REFERENCE,
    ERROR_INVALID_TIMEOUT_PARAM,
    ERROR_INVALID_ENDPOINT_PARAM,
    ERROR_INVALID_PUBLISH_ENDPOINT,
    ERROR_INVALID_MESSAGE_PARAM,
    ERROR_MESSAGE_SIZE_BIGGER_THAN_MAXIMUM, // max size is 256MB
    ERROR_INVALID_CALLBACK_PARAM,
    ERROR_MALFORMED_OBJECT,         // constructor set flag that construction failed
    ERROR_UNKNOWN_EXCEPTION,
    ERROR_RESOURCE_UNAVAIL,
    ERROR_NOT_CONNECTED,			// client is not connected to the transport layer (MQTT)
    ERROR_CONNECT_FAIL,
    ERROR_DISCONNECT_FAIL,
    ERROR_TOPIC_PATTERN_ALREADY_REGISTERED,  // unique subscribe failure
    ERROR_LISTENER_ALREADY_REGISTERED_TO_TOPIC_PATTERN,  // non-unique subscribe failure
    ERROR_SOA_MESSAGE_SERIALIZATION_FAILED, // General SOAMessage serialization error
    ERROR_ILLEGAL_CALL_THREAD, //When Synchronous methods are invoked from callback thread or worker thread
    ERROR_SOA_MESSAGE_PARSING_FAILED, // General SOAMessage parsing error
    ERROR_CANNOT_CREATE_CLIENT,
    ERROR_WRONG_STATE,
    ERROR_EMPTY_TOPIC,
    ERROR_EMPTY_TOPIC_PATTERN,
    ERROR_PUBLISH_FAILED,
    ERROR_SUBSCRIBE_FAILED,
    ERROR_UNSUBSCRIBE_FAILED,
    ERROR_TOPIC_NO_SUBSCRIBED,
    ERROR_LISTENER_NOT_BOUND,
    ERROR_CANNOT_EXTRACT_COMMON_NAME,
    ERROR_CLIENT_ID_NOT_SET,
    ERROR_USERNAME_OR_PASSWORD_OR_CLIENTID_TOO_LONG, // max length is 65535 bytes
    ERROR_DUPLICATE_COMMAND_HANDLER,
    ERROR_FAILED_SETTING_CALLBACKS,
    ERROR_ALREADY_MAPPED,   // tried to add a unique listener again
    ERROR_UNIQUENESS_MISMATCH,
    ERROR_INTERNAL_ERROR,
    ERROR_WILDCARD_FIXED_PATTERN_OVERLAP, // a fixed pattern overlaps with a wildcard pattern
    ERROR_SOA_MESSAGE_FACTORY_ERROR,
    ERROR_UNSUPPORTED_VERSION,
    ERROR_REQUIRED_FIELD_MISSING,  // required field in SoaMessage is missing
    ERROR_DEPRECATED,
    ERROR_NOT_INITIALIZED,
    ERROR_ALREADY_INITIALIZED,
    ERROR_BAD_STATE,
    ERROR_MQTT_SERVER_UNAVAILABLE,
    ERROR_RESULT_NOT_AVAILABLE,
    /**
     * On Demand Data Broadcast errors
     */
    ERROR_SUBSCRIPTION_REFUSED,
    ERROR_BAD_COMMAND,
    ERROR_BAD_HANDLE,
    ERROR_PROVIDER_CLOSED,
    ERROR_PROVIDER_DISCONNECTED,
    /**
     * Service Directory Manager errors
     * A command is not supported by the selected command handler
     */
    ERROR_HANDLER_COMMAND_NOT_SUPPORTED,
    ERROR_COMMAND_HANDLER_ERROR,
    ERROR_INITIALIZATION_ERROR,
    ERROR_FATAL,
    ERROR_TRANSACTION_FAILED,
    ERROR_RESPONSE_DATA_BAD,
    ERROR_REQUEST_DATA_BAD,

    /**
     * Special error code for partial connections behind gateway
     */
    ERROR_GATEWAY_CONNECTED_ONLY,   // deprecated, keeping this as it is part of the public API
    NO_ERROR__GATEWAY_CONNECTED_ONLY = ERROR_GATEWAY_CONNECTED_ONLY, // renamed because it was confusing
    ERROR_GATEWAY_UNAVAILABLE,
    ERROR_FAILED_REGISTER_TO_GATEWAY,

    ERROR_TERMINATED,

    ERROR_IPC_ROOT_PATH_UNAVAILABLE,

    WARNING_UNSUPPORTED_VERSION, // deprecated will be removed after changes in unit tests
    WARNING_PROTO_MAJOR_VERSION_MISMATCH,
    ERROR_PROTO_MAJOR_VERSION_MISMATCH,


    // When adding new error codes, please also add them to ECG/soa/src/cpp/soa_error_code_utils.cpp
    ERROR_LAST_ONE__PUT_ALL_BEFORE_THIS
};

} // namespace framework
} // namespace soa
} // namespace fnv

#endif /* FNVSOA_FRAMEWORK_SOAERRORCODE_HPP_ */
