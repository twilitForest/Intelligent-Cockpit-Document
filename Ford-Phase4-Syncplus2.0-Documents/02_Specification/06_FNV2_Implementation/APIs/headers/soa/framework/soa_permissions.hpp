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
 * soa_service_permissions.hpp
 *
 *  Created on: Jul 5, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_CPP_SERVICEMANAGER_SERVICE_PERMISSIONS_HPP_
#define INCLUDE_CPP_SERVICEMANAGER_SERVICE_PERMISSIONS_HPP_

#include <memory>
#include <string>

namespace fnv {
namespace soa {
namespace framework {

/*
 * A container class to include service permissions related data.
 * The content is still TBD and will be added in future development phases but it is clear that
 * the class will contain ACL permissions for the service.
 * For now just adding the client ID.
 */
class SoaPermissions {
public:
	/**
	 * Typedef of scoped shared pointer type to this class
	 */
    typedef std::shared_ptr<SoaPermissions> SharedPtr;
    /**
     * Destructor
     */
	virtual ~SoaPermissions();
	/**
	 * Creates a verified SoaServicePermissions::SharedPtr instance.
	 * @param clientId client ID (MQTT client ID)
	 * @return An instance or a nullptr if a valid instance cannot be created
	 */
	static SoaPermissions::SharedPtr makeShared(const std::string & clientId);
	/**
	 * Returns client ID
	 * @return Client ID
	 */
	std::string getClientId();
private:
	std::string m_clientId;
	explicit SoaPermissions(const std::string & clientId);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_CPP_SERVICEMANAGER_SERVICE_PERMISSIONS_HPP_ */
