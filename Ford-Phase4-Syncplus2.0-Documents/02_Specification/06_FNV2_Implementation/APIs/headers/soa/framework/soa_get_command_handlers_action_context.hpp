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
 * soa_get_command_handlels_action_context.hpp
 *
 *  Created on: May 30, 2017
 *      Author: asergee2
 */

#ifndef INCLUDE_CPP_FRAMEWORK_SOA_GET_COMMAND_HANDLERS_ACTION_CONTEXT_HPP_
#define INCLUDE_CPP_FRAMEWORK_SOA_GET_COMMAND_HANDLERS_ACTION_CONTEXT_HPP_

#include <soa/framework/soa_command_handlers_listener.hpp>
#include <soa/servicemanager/get_command_handles_request_message_payload.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Context for SoaConsumer::getCommandHandlersAsync action
 */
class SoaGetCommandHandlersActionContext {
public:
	/**
	 * Typedef of scoped shared pointer type to this class
	*/
	typedef std::shared_ptr<SoaGetCommandHandlersActionContext> SharedPtr;
    virtual ~SoaGetCommandHandlersActionContext() = default;
    /**
     * returns the command ID
     */
    virtual fnv::soa::servicemanager::SoaCommandHashSet getCommandIds() = 0;
    /**
     * returns the listener
     */
    virtual SoaCommandHandlersListener& getCommandHandlersListener() = 0;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_CPP_FRAMEWORK_SOA_GET_COMMAND_HANDLERS_ACTION_CONTEXT_HPP_ */
