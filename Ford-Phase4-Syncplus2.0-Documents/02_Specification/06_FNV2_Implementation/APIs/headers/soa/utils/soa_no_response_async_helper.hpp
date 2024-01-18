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
 * soa_no_response_async_helper.hpp
 *
 *  Created on: NOV 9, 2017
 *      Author: KGAO4
 */

#ifndef INCLUDE_UTILS_SOA_ONEWAY_ASYNC_HELPER_HPP_
#define INCLUDE_UTILS_SOA_ONEWAY_ASYNC_HELPER_HPP_

#include <memory>
#include <mutex>
#include <future>
#include <atomic>
#include <soa/framework/soa_action_result.hpp>
#include <soa/utils/soa_timeout.hpp>
#include <soa/framework/soa_action_result_listener.hpp>
#include <soa/framework/soa_error_code_util.hpp>
#include <soa/utils/soa_frameworks_logging.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * This class helps to convert async SOA API calls to blocking
 */
template<class ActionContext>
class SoaNoResponseAsyncHelper: public SoaActionResultListener<ActionContext>  {
private:
    std::promise<SoaErrorCode> m_promise;
    std::future<SoaErrorCode> m_future;
    SoaErrorCode m_actionResult;
protected:
    const std::string k_className;
public:
    explicit SoaNoResponseAsyncHelper(const std::string& logTag):
        m_promise(),
        m_future(m_promise.get_future()),
                m_actionResult(SoaErrorCode::ERROR_RESULT_NOT_AVAILABLE),
        k_className(logTag) {}
    SoaNoResponseAsyncHelper(): SoaNoResponseAsyncHelper(__FUNCTION__) {}
    virtual ~SoaNoResponseAsyncHelper() {
        this->removeResource();
    }

    /**
     * overriding base class callback
     * @param error
     * @param actionContext
     */
    virtual void onActionCompleted(SoaErrorCode error, std::shared_ptr<ActionContext> actionContext) override {
        try {
            m_promise.set_value(error);
        } catch (const std::future_error& e) {
            FNV2LOGE(getSoaFrmwkLogger(), "%s::%s error: %s: error code: %d",
                    k_className.c_str(), __FUNCTION__, e.what(), e.code().value());

        }
    }

    /**
     * Waits for an async SOA action to be completed
     * @param timeout A timeout
     * @return SoaErrorCode
     */
    SoaErrorCode wait(const SoaTimeout& timeout) {
        if (!m_future.valid()) {
            FNV2LOGE(getSoaFrmwkLogger(), "%s::%s %s",
                    k_className.c_str(), __FUNCTION__, "Invalid future state. The helper can only be used once");
            return SoaErrorCode::ERROR_FATAL;
        }

        // Waiting for onActionCompleted to be called
        std::future_status stat = m_future.wait_for(timeout.remained());
        if (stat == std::future_status::ready) {
            m_actionResult = m_future.get();
        } else {
            m_actionResult = SoaErrorCode::ERROR_FATAL;
            FNV2LOGE(getSoaFrmwkLogger(), "%s::%s %s",
                    k_className.c_str(), __FUNCTION__, "onActionCompleted() was not invoked before future timeout has expired");
        }
        return getActionResult();
    }
    /**
     * Waits for an async SOA action to be completed
     * @param timeout A timeout
     * @return SoaErrorCode
     */
    SoaErrorCode wait(SoaTimeout::TimeSpan timeout) {
        SoaTimeout remained(timeout);
        return wait(remained);
    }
    /**
     * Returns a result or nullptr if wait was not successful
     * @return SoaErrorCode
     */
    SoaErrorCode getActionResult() {
        return m_actionResult;
    }


};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_ONEWAY_ASYNC_HELPER_HPP_ */
