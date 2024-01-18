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
 * soa_command_handlers_request_async_helper.hpp
 *
 *  Created on: Aug 29, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_SOA_UTILS_SOA_COMMAND_HANDLERS_REQUEST_ASYNC_HELPER_HPP_
#define INCLUDE_SOA_UTILS_SOA_COMMAND_HANDLERS_REQUEST_ASYNC_HELPER_HPP_

#include <soa/utils/soa_async_helper.hpp>
#include <soa/framework/soa_command_handlers_listener.hpp>
#include <soa/framework/soa_get_command_handlers_action_context.hpp>

namespace fnv {
namespace soa {
namespace framework {

class SoaCommandHandlersRequestAsyncHelper:
		public SoaAsyncHelper<fnv::soa::servicemanager::SoaCommandHandlerMap, SoaGetCommandHandlersActionContext::SharedPtr>,
		public SoaCommandHandlersListener,
		public SoaActionResultListener<SoaGetCommandHandlersActionContext> {
public:
    ~SoaCommandHandlersRequestAsyncHelper() override;
    /**
     * The listener callback method
     * @param handlers A shared pointer to a list of handlers
     */
    void onCommandHandlersListUpdated(fnv::soa::servicemanager::SoaCommandHandlerMap handlers) override;
	/**
	 * A callback method which needs to be implemented by the application developer if
	 * calling a SOA framework method which takes a SoaActionResultListener argument.
	 * @param error The SoaErrorCode enum value will be from a set of possible error codes resulting
	 *              from the specific SOA framework method called that invokes this callback. Check
	 *              the header comments of the SOA framework method being called.
	 * @param actionContext a pointer to a context class containing the context data of the SOA framework
	 *                      method called that invokes this callback.
	 *                      This pointer is always valid.
	 * @see soa_consumer.hpp
	 * @see soa_provider.hpp
	 */
	virtual void onActionCompleted(SoaErrorCode error, SoaGetCommandHandlersActionContext::SharedPtr actionContext) override;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_COMMAND_HANDLERS_REQUEST_ASYNC_HELPER_HPP_ */
