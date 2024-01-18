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
 * soa_rpc_async_helper.hpp
 *
 *  Created on: Aug 4, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_UTILS_SOA_RPC_ASYNC_HELPER_HPP_
#define INCLUDE_UTILS_SOA_RPC_ASYNC_HELPER_HPP_

#include <soa/utils/soa_async_helper.hpp>
#include <soa/framework/soa_message.hpp>
#include <soa/framework/soa_remote_call_context.hpp>
#include <soa/framework/soa_remote_call_listener.hpp>
#include <soa/framework/soa_listener_lifecycle_helper.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * This class helps to convert async SOA RPC calls to blocking
 */
class SoaRpcAsyncHelperBase:
		public SoaAsyncHelper<SoaMessage::SharedPtr, SoaRemoteCallContext::SharedPtr>,
		public SoaRemoteCallListener,
		public SoaActionResultListener<SoaRemoteCallContext> {
public:
    ~SoaRpcAsyncHelperBase() override = default;

    /**
     * @brief Signature for the remote call response handler
     *
     * The SOA client must implement this method to handle responses to the remote calls.
     *
     * @param message The message containing the remote call response
     *                This argument will never be null.
     */
    void onResponseReceived(SoaMessage::SharedPtr message) override;
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
	virtual void onActionCompleted(SoaErrorCode error, SoaRemoteCallContext::SharedPtr actionContext) override;
};

typedef SoaListenerLifecycleHelper<
    SoaRpcAsyncHelperBase,
    SoaRemoteCallListener,
    SoaActionResultListener<SoaRemoteCallContext>
        > SoaRpcAsyncHelper;

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_RPC_ASYNC_HELPER_HPP_ */
