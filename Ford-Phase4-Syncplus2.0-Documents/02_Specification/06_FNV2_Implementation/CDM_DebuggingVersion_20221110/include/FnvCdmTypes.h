#ifndef FNVCDM_TYPES_H
#define FNVCDM_TYPES_H

namespace fnv {
namespace cdm {

const int32_t TIMEOUT_SHORT  = 5000;
const int32_t TIMEOUT_MEDIUM = TIMEOUT_SHORT << 1;
const int32_t TIMEOUT_LONG   = TIMEOUT_SHORT << 2;

enum class CdmReturnCode : uint8_t {
    // Success
    Success = 0,

    // Failure
    ErrorOccurred,

    // Failure due to invalid parameter
    ErrorInvalidParameter,

    // Failure due to communication error with server
    ErrorCommunicationLinkFailure,

    // Timed out
    ErrorTimeOut,

    // Not initialized
    ErrorNotInitialized,

    // Some other error
    ErrorOther = 0xFF
};

// Cdm Connect Results
typedef enum {
    MSG_INIT = 0,

    MSG_CONNECTING = 1,
    MSG_CONNECTED_GATEWAY,
    MSG_CONNECTED_ECG,
    MSG_DISCONNECTED,

    MSG_CONNECT_FAILED = 5,
    MSG_DISCONNECT_FAILED,
    MSG_CONNECT_LOST,
    MSG_RECONNECTING,

    MSG_ALREADY_CONNECTED = 9,
    MAG_NOT_CONNECTED,

    MSG_CONSUMER_INIT = 11,
    MSG_CONSUMER_INIT_FAILED,

    MSG_CONSUMER_DEINIT = 13,
    MSG_CONSUMER_DEINIT_FAILED,

    MSG_SERVICE_NOT_AVAILABLE = 15,
    MSG_SERVICE_TEMPORARILY_NOT_AVAILABLE,
    MSG_SERVICE_AVAILABLE,
    MSG_SERVICE_RUNNING,

    MSG_SUBSCRIBED = 19,
    MSG_SUBSCRIBE_FAILED,

    MSG_UNSUBSCRIBED = 21,
    MSG_UNSUBSCRIBE_FAILED,

    MSG_REMOTECALL_GETCDMPROFILE_SUCCESS = 23,
    MSG_REMOTECALL_SETACTIVECDMPROFILE_SUCCESS,
    MSG_REMOTECALL_SETCDMSETTINGS_SUCCESS,

    MSG_REMOTECALL_GETCDMPROFILE_FAILED = 26,
    MSG_REMOTECALL_SETACTIVECDMPROFILE_FAILED,
    MSG_REMOTECALL_SETCDMSETTINGS_FAILED,

    MSG_REMOTECALL_SUCCESS = 29,
    MSG_REMOTECALL_FAILED,

    MSG_OTHERS

} CdmConnectResult;

enum class CdmProfileNumberEnum : uint8_t {
    PROFILENUMBERINVALID = 0,
    PROFILENUMBER1 = 1,
    PROFILENUMBER2 = 2,
    PROFILENUMBER3 = 3,
    PROFILENUMBER4 = 4,
    PROFILENUMBER5 = 5,
    PROFILENUMBER6 = 6
};

enum class CdmFeatureObjectId : uint8_t {
    INVALID         = 0,
    DRIVEMODE       = 1,
    POWERTRAIN      = 2,
    EXHAUST         = 3,
    STOPSTART       = 4,
    STEERING        = 5,
    SUSPENSION      = 6,
    CHASSIS         = 7,
    TRACTIONCONTROL = 8,
    DISPLAY         = 9,
    STABILIZERBAR   = 10
};

enum class CdmSettingNumberEnum : uint8_t {
    SETTINGINVALID = 0,
    SETTING01 = 1,
    SETTING02 = 2,
    SETTING03 = 3,
    SETTING04 = 4,
    SETTING05 = 5,
    SETTING06 = 6,
    SETTING07 = 7,
    SETTING08 = 8,
    SETTING09 = 9,
    SETTING10 = 10,
    SETTING11 = 11,
    SETTING12 = 12,
    SETTING13 = 13,
    SETTING14 = 14,
    SETTING15 = 15,
    SETTING16 = 16,
    SETTING17 = 17,
    SETTING18 = 18,
    SETTING19 = 19,
    SETTING20 = 20,
    SETTING21 = 21,
    SETTING22 = 22,
    SETTING23 = 23,
    SETTING24 = 24,
    SETTING25 = 25,
    SETTING26 = 26,
    SETTING27 = 27,
    SETTING28 = 28,
    SETTING29 = 29,
    SETTING30 = 30
};
} /* namespace cdm */
} /* namespace fnv */

#endif // FNVCDM_TYPES_H
