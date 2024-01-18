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
 * soa_async_helper.hpp
 *
 *  Created on: Aug 4, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_UTILS_SOA_ASYNC_HELPER_HPP_
#define INCLUDE_UTILS_SOA_ASYNC_HELPER_HPP_

#include <memory>
#include <future>
#include <tuple>
#include <mutex>
#include <soa/framework/soa_action_result.hpp>
#include <soa/utils/soa_timeout.hpp>
#include <soa/framework/soa_action_result_listener.hpp>
#include <soa/framework/soa_error_code_util.hpp>
#include <soa/utils/soa_frameworks_logging.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * An empty class to allow creating SoaAsyncHelper without context
 */
class DummyContext {
public:
    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<DummyContext> SharedPtr;

};
/**
 * This class helps to convert async SOA API calls to blocking
 */
template<class ActionResult, class ActionContext>
class SoaAsyncHelper {
private:
    std::promise<SoaErrorCode> m_actionPromise;
    std::promise<SoaErrorCode> m_resultPromise;
    bool m_valid;
    ActionResult m_result;
    ActionContext m_context;
    std::mutex m_resultMutex;
    std::mutex m_contextMutex;

protected:

    const char * const k_className;
	void onActionStarted(SoaErrorCode error, ActionContext context) {
		{
            std::lock_guard<std::mutex> lock(m_contextMutex);
			m_context = context;
		}
		m_actionPromise.set_value(error);
	}
	void onResultReceived(ActionResult result) {
		{
            std::lock_guard<std::mutex> lock(m_resultMutex);
			m_result = result;
		}
		m_resultPromise.set_value(SoaErrorCode::NO_ERROR);
	}
public:
	SoaAsyncHelper(): m_valid(true), k_className("SOA_ASYNC_HELPER") {

	}
	virtual ~SoaAsyncHelper() = default;

	/**
	 * Checks if the helper can be used.
	 * A helper object can be only use once.
	 * @return bool
	 */
	bool valid() {
		return m_valid;
	}
	/**
	 * Waits for an async SOA action to be completed
	 * @param timeout A timeout
	 * @return SoaErrorCode
	 */
	SoaErrorCode wait(int32_t timeout_mSec) {
		return wait(std::chrono::milliseconds(timeout_mSec));
	}
	/**
	 * Async action result callback.
	 * @param error A error
	 * @param result A result, can be a nullptr if the error != SoaErrorCode::NO_ERROR
	 * @param context An action context
	 */
	virtual void onActionResult(SoaErrorCode error, ActionResult result, ActionContext context) {
		// Implement is a derived class if needed
	}

	/**
	 * Waits for an async SOA action to be completed
	 * @param timeout A timeout
	 * @return SoaErrorCode
	 */
	SoaErrorCode wait(SoaTimeout::TimeSpan timeout) {
		if (!valid()) {
			FNV2LOGE(getSoaFrmwkLogger(), "%s::%s %s",
			        k_className, __FUNCTION__, "SoaAsyncHelper object can be used only one time");
			return SoaErrorCode::ERROR_BAD_STATE;
		}
		// the same object cannot be used for the second time.
		m_valid = false;

		SoaTimeout remained(timeout);

		// First waiting for a SoaActionResultListener to be called
		std::future<SoaErrorCode> startActionFuture = m_actionPromise.get_future();
		if (startActionFuture.wait_for(remained.remained()) != std::future_status::ready) {
			FNV2LOGE(getSoaFrmwkLogger(), "%s::%s %s", k_className, __FUNCTION__, "Async action start timeout");
            std::lock_guard<std::mutex> resultLock(m_resultMutex);
            std::lock_guard<std::mutex> contextLock(m_contextMutex);
			onActionResult(SoaErrorCode::ERROR_FATAL, m_result, m_context);
			return SoaErrorCode::ERROR_FATAL;
		}
		SoaErrorCode startActionResult = startActionFuture.get();
		if (startActionResult != SoaErrorCode::NO_ERROR) {
			FNV2LOGW(getSoaFrmwkLogger(), "%s::%s Async action error: %s",
			        k_className, __FUNCTION__, SoaErrorCodeUtil::getCString(startActionResult));
            std::lock_guard<std::mutex> resultLock(m_resultMutex);
            std::lock_guard<std::mutex> contextLock(m_contextMutex);
			onActionResult(startActionResult, m_result, m_context);
			return startActionResult;
		}

		// Now waiting for the result listener to be called
		std::future<SoaErrorCode> resultFuture = m_resultPromise.get_future();
		if (resultFuture.wait_for(remained.remained()) != std::future_status::ready) {
			FNV2LOGE(getSoaFrmwkLogger(), "%s::%s %s", k_className, __FUNCTION__, "Async result timeout");
            std::lock_guard<std::mutex> resultLock(m_resultMutex);
            std::lock_guard<std::mutex> contextLock(m_contextMutex);
			onActionResult(SoaErrorCode::ERROR_TIMEOUT, m_result, m_context);
			return SoaErrorCode::ERROR_TIMEOUT;
		}
        std::lock_guard<std::mutex> resultLock(m_resultMutex);
        std::lock_guard<std::mutex> contextLock(m_contextMutex);
		onActionResult(SoaErrorCode::NO_ERROR, m_result, m_context);
		return resultFuture.get();
	}
	/**
	 * Returns a result or nullptr if wait was not successful
	 * @return ActionResult
	 */
	ActionResult getResult() {
        std::lock_guard<std::mutex> lock(m_resultMutex);
		return m_result;
	}

	/**
	 * Returns an action context
	 * @return ActionContext
	 */
	ActionContext getContext() {
        std::lock_guard<std::mutex> lock(m_contextMutex);
		return m_context;
	}

};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_ASYNC_HELPER_HPP_ */
