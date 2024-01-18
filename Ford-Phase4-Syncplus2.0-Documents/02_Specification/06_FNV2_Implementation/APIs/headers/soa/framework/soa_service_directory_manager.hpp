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
 * SoaServiceDirectoryManager.h
 *
 *  Created on: Apr 5, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOASERVICEDIRECTORYMANAGER_HPP_
#define FNVSOA_FRAMEWORK_SOASERVICEDIRECTORYMANAGER_HPP_

#include <string>

#include <map>
#include <soa/framework/soa_message_manager.hpp>
#include <soa/framework/soa_service_status.hpp>
#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_service_contract.hpp>
#include <soa/framework/soa_service_status_listener.hpp>
#include <soa/framework/soa_service_contract_listener.hpp>
#include <soa/framework/soa_error_code.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Local SOA Service Directory manager.
 * The class is a central point for clients to send requests to Service Directory Service.
 * The class will cache the latest service status and contract to avoid extra SOA traffic.
 */
class SoaServiceDirectoryManager {
public:
    /**
     * Typedef of scoped shared pointer type to this class
     */
	typedef std::shared_ptr<SoaServiceDirectoryManager> SharedPtr;

	/**
	 * Factory method to create an object of this type
	 * @param messageManager
	 * @param endpoint
	 * @return a shared pointer to a SoaServiceDirectoryManager object
	 */
	static SharedPtr create(SoaMessageManager::SharedPtr messageManager, SoaClientEndpoint::SharedPtr endpoint);

    /**
     * Starts the manager: Makes the manager a SOA client listening to the endpoint provided in the constructor
     * @return
     */
	virtual SoaErrorCode start() = 0;

	/**
	 * Stops the manager from listening to the endpoint
	 * @return
	 */
	virtual SoaErrorCode stop() = 0;

	/**
	 * Not Implemented
	 * Use this API to statically register system services you are going to access in you code.
	 * System service assumption:
	 * 		It is a well knows service with the contract defined in a header file
	 * 		THe service has global lifecycle
	 * @param contract The contract
	 * @return SoaErrorCode::ERROR_NOT_IMPLEMENTED
	 */
	virtual SoaErrorCode registerSystemService(SoaServiceContract::SharedPtr contract, SoaServiceStatus status) = 0;

	/**
     * destructor
     */
    virtual ~SoaServiceDirectoryManager() = default;

};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOASERVICEDIRECTORYMANAGER_HPP_ */
