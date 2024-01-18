//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2019.  Ford Motor Company owns all rights to this work and intends
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
 * soa_connection_lost_reason.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: shaghgoo
 */

#ifndef INCLUDE_CPP_SOA_CONNECTION_LOST_REASON_HPP_
#define INCLUDE_CPP_SOA_CONNECTION_LOST_REASON_HPP_

namespace fnv {
namespace soa {
namespace framework {

constexpr const char* SOA_UNKNOWN_STR = "unknown";
constexpr const char* SOA_UNKNOWN_CAUSE_STR = "unknown cause";  // existing API
constexpr const char* SOA_RECONNECTED_STR = "connected";
constexpr const char* SOA_CONNECTION_LOST_STR = "lost";
constexpr const char* SOA_GLOBAL_DISCONNECT_STR = "forced";
constexpr const char* SOA_GATEWAY_RESET_STR = "gateway reset";
constexpr const char* SOA_REQUEST_FAILED_STR = "request failed";
constexpr const char* SOA_REQUEST_REJECTED_STR = "request rejected";
constexpr const char* SOA_LOCAL_DISCONNECT_STR = "local request";
constexpr const char* SOA_TERMINATED_STR = "terminated";
constexpr const char* SOA_NO_CAUSE_PROVIDED_STR = "unknown cause";

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_CPP_SOA_CONNECTION_LOST_REASON_HPP_ */
