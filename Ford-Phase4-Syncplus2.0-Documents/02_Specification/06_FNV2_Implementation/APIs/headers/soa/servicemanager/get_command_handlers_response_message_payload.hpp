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
 * get_command_handlers_response_message_payload.hpp
 *
 *  Created on: Jul 27, 2017
 *      Author: ASERGEE2
 */

#ifndef CLIENT_SRC_SMMAIN_SOAPUBLICINCLUDES_SERVICEMANAGER_GET_COMMAND_HANDLERS_RESPONSE_MESSAGE_PAYLOAD_HPP_
#define CLIENT_SRC_SMMAIN_SOAPUBLICINCLUDES_SERVICEMANAGER_GET_COMMAND_HANDLERS_RESPONSE_MESSAGE_PAYLOAD_HPP_

#include <string>
#include <unordered_map>
#include <soa/framework/soa_message_payload.hpp>
#include <soa/framework/soa_client_endpoint.hpp>

namespace fnv {
namespace soa {
namespace servicemanager {

/**
 * A map containing list of services being able to handle commands
 * A map key: command ID
 * A map value: a hash set of service endpoints
 */
typedef std::unordered_map<std::string, fnv::soa::framework::SoaClientEndpoint::HashSet> SoaCommandHandlerMap;

} /* namespace servicemanager */
} /* namespace soa */
} /* namespace fnv */

#endif /* CLIENT_SRC_SMMAIN_SOAPUBLICINCLUDES_SERVICEMANAGER_GET_COMMAND_HANDLERS_RESPONSE_MESSAGE_PAYLOAD_HPP_ */
