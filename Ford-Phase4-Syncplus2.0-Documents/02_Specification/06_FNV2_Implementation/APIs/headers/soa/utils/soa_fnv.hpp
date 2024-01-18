//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2018.  Ford Motor Company owns all rights to this work and intends
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
 * soa_fnv.hpp
 *
 *  Created on: Mar 24, 2018
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_UTILS_SOA_FNV_HPP_
#define INCLUDE_SOA_UTILS_SOA_FNV_HPP_

#include <string>

namespace fnv {
namespace soa {
namespace framework {

class SoaFnv {
public:
    /**
     * Values for this enum are taken from GMRDB (Global Master Reference Data Base)
     */
    enum class EcuType {
        UNKNOWN = 0x00,
        ECG = 0xD8,
        SYNC = 0x89,
        TCU = 0x91,
        LXCLUSTER = 0x60,
    };

    static const std::string ECG_ECU_NAME;
    static const std::string TCU_ECU_NAME;
    static const std::string SYNC_ECU_NAME;
    static const std::string LXCLUSTER_ECU_NAME;
    static const std::string GATEWAY_NAME;
    static const std::string TCU_DEBUG_TOKEN;
    static const std::string SYNC_DEBUG_TOKEN;
    /**
     * Returns an id of the ecu hosting the process
     * @return ECU ID
     */
    const static std::string& ecuId();
    /**
     * Returns an type of the ecu hosting the process
     * @return EcuType
     */
    static EcuType ecuType();
    static constexpr const pid_t INVALID_PID = -1;
    static constexpr const pid_t PID_INITIALIZER = -2;
    static const char k_crashInfoTopicName[];
    /**
     * Returns process UID
     */
    static std::string getUid();
    /**
     * Creates a consumer topic: "SERVICES/RESPONSE/ECU_ID/UID/CONSUMER_ID"
     * @param consumerId a consumer ID
     * @return a topic or an empty string if consumerId is an empty string
     */
    static std::string createConsumerTopic(const std::string& consumerId);
    /**
     * Creates a consumer topic: "SERVICES/RESPONSE/ECU_ID/UID/CONSUMER_ID"
     * @param consumerId a consumer ID
     * @param uid a uid of a process running the consumer
     * @return a topic or an empty string if consumerId is an empty string
     */
    static std::string createConsumerTopic(const std::string& consumerId, const std::string& uid);
    /**
     * Creates an MQTT clientId
     * @param isGateway a flag indicating if it is for gateway clientId
     *        i.e. clientID is for ECG.
     * @return a clientId or an empty string if ecuId is an empty string
     */
    static std::string createClientId(const std::string& ecuId, bool isGateway = false);

    static std::string createClientId(bool isGateway);

private:
    SoaFnv() = delete;
    ~SoaFnv() = delete;

    static constexpr const char k_className[] = "SoaFnv";
    static EcuType ecuTypeInternal();
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_FNV_HPP_ */
