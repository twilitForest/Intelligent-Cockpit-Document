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
 * SoaServiceStatus.h
 *
 *  Created on: Apr 6, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOASERVICESTATUS_HPP_
#define FNVSOA_FRAMEWORK_SOASERVICESTATUS_HPP_

#include <unordered_set>
#include <soa/idl/soa_service_status.pb.h>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Service Status enum
 */
enum class SoaServiceStatus {
	/**
	 * There is no service record in Service Directory
	 */
	NOT_AVAILABLE = fnv::soa::framework::protobuf::ServiceStatus::NOT_AVAILABLE,
	/**
	 * service is temporarily cannot be started for reasons:
     *      1. The app is being updated
     *      2. The app is about to be uninstalled. If uninstall is canceled the status will be changed to "AVAILABLE"
	 */
    TEMPORARILY_NOT_AVAILABLE = fnv::soa::framework::protobuf::ServiceStatus::TEMPORARILY_NOT_AVAILABLE,
	/**
	 * Service is installed and provisioned and can be started on demand
	 */
	AVAILABLE = fnv::soa::framework::protobuf::ServiceStatus::AVAILABLE,
	/**
	 * Service is running and waiting for service requests
	 */
	RUNNING = fnv::soa::framework::protobuf::ServiceStatus::RUNNING
};

/**
 * Structure to be used as a hasher function in SoaServiceStatusHashSet hash set.
 */
struct SoaServiceStatusHasher {
	size_t operator()(const SoaServiceStatus & status) const {
		return static_cast<std::size_t>(status);
	}
};

/**
 * SoaServiceStatus hash set definition
 */
typedef std::unordered_set<SoaServiceStatus, SoaServiceStatusHasher> SoaServiceStatusHashSet;

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOASERVICESTATUS_HPP_ */
