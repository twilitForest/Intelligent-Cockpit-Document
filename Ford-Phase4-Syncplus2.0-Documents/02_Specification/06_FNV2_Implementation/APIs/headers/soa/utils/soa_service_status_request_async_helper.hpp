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
 * soa_service_status_request_async_helper.hpp
 *
 *  Created on: Aug 9, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_UTILS_SOA_SERVICE_STATUS_REQUEST_ASYNC_HELPER_HPP_
#define INCLUDE_UTILS_SOA_SERVICE_STATUS_REQUEST_ASYNC_HELPER_HPP_

#include <soa/utils/soa_async_helper.hpp>
#include <soa/framework/soa_service_status_listener.hpp>
#include <soa/framework/soa_request_service_status_context.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * This class helps to convert async SOA ServiceStatusRequest calls to blocking
 */
class SoaServiceStatusRequestAsyncHelper:
		public SoaAsyncHelper<std::pair<SoaClientEndpoint::SharedPtr, SoaServiceStatus>, SoaRequestServiceStatusContext::SharedPtr> {
public:
    SoaServiceStatusRequestAsyncHelper();
    SoaServiceStatusListener& getResultListener();
    SoaActionResultListener<SoaRequestServiceStatusContext>& getActionStatusListener();
private:
	class SoaServiceStatusListenerImpl: public SoaServiceStatusListener {
	public:
	    ~SoaServiceStatusListenerImpl() override {
            this->removeResource();
        }
	    void setAsyncHelper(SoaServiceStatusRequestAsyncHelper* helper);
	    /**
	     * Callback for service status change
	     * @param endpoint The endpoint for which the status changed
	     * @param status The new status
	     */
	    void onStatusChanged(const SoaClientEndpoint::SharedPtr endpoint, SoaServiceStatus status) override;
	private:
	    SoaServiceStatusRequestAsyncHelper* m_helper;
	};
	class SoaActionResultListenerImpl: public SoaActionResultListener<SoaRequestServiceStatusContext> {
    public:
	    void setAsyncHelper(SoaServiceStatusRequestAsyncHelper* helper);
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
	    virtual void onActionCompleted(SoaErrorCode error, SoaRequestServiceStatusContext::SharedPtr actionContext);
    private:
        SoaServiceStatusRequestAsyncHelper* m_helper;
	};
	SoaServiceStatusListenerImpl m_resultListener;
	SoaActionResultListenerImpl m_actionStatusListener;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_SERVICE_STATUS_REQUEST_ASYNC_HELPER_HPP_ */
