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
 * get_command_handles_request_message_payload.hpp
 *
 *  Created on: Jul 27, 2017
 *      Author: ASERGEE2
 */

#ifndef CLIENT_SRC_SMMAIN_SOAPUBLICINCLUDES_SERVICEMANAGER_GET_COMMAND_HANDLES_REQUEST_MESSAGE_PAYLOAD_HPP_
#define CLIENT_SRC_SMMAIN_SOAPUBLICINCLUDES_SERVICEMANAGER_GET_COMMAND_HANDLES_REQUEST_MESSAGE_PAYLOAD_HPP_

#include <unordered_set>
#include <string>
#include <soa/framework/soa_message_payload.hpp>

namespace fnv {
namespace soa {
namespace servicemanager {

/**
 * A hash set containing list of SOA service commands
 */
typedef std::unordered_set<std::string> SoaCommandHashSet;

} /* namespace servicemanager */
} /* namespace soa */
} /* namespace fnv */

#endif /* CLIENT_SRC_SMMAIN_SOAPUBLICINCLUDES_SERVICEMANAGER_GET_COMMAND_HANDLES_REQUEST_MESSAGE_PAYLOAD_HPP_ */
