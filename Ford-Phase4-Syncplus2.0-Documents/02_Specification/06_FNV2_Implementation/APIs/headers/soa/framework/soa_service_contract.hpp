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
 * SoaServiceContract.h
 *
 *  Created on: Apr 6, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOASERVICECONTRACT_HPP_
#define FNVSOA_FRAMEWORK_SOASERVICECONTRACT_HPP_

#include <memory>
#include <string>
#include <unordered_set>
#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_service_status.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Service contract class
 * Contract includes:
 *  Service Endpoint aka an addrress consumer can use to deliver service requests
 *  Version - service software version.
 *  Service Host Endpoint - an endpoint of a host hosting the service. Not supported in release 1.0. Can be constructed of any arbitrary string.
 *  FTCP Commands - a set of FTCP commands the service is able to handle. FTCP commands shall be unique across the vehicle.
 *                  Services providing duplicate FTCP commands will not be registered by Service Manager.
 *  Commands - a set of commands the service can handle. There is no restrictions for commands.
 */
class SoaServiceContract {
public:
    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaServiceContract> SharedPtr;

    /**
     * Creates an SoaServiceContract shared pointer
     * @param endpoint an endpoint
     * @param version Service code version
     * @param serviceHostEndpoint An endpoint of an MQTT client hosting the service
     * @returns the pointer
     */
    static SharedPtr makeShared(
    		const SoaClientEndpoint::SharedPtr & endpoint,
			const std::string & version,
			const SoaClientEndpoint::SharedPtr & serviceHostEndpoint);

    /**
     * Creates an SoaServiceContract shared pointer
     * @param endpoint an endpoint
     * @param version Service code version
     * @param serviceHostEndpoint An endpoint of an MQTT client hosting the service
     * @param a list on commands being handled by the service
     * @returns the pointer
     */
    static SharedPtr makeShared(
    		const SoaClientEndpoint::SharedPtr & endpoint,
			const std::string & version,
    		const SoaClientEndpoint::SharedPtr & serviceHostEndpoint,
			const std::unordered_set<std::string> & commands);

    /**
     * Creates an SoaServiceContract shared pointer
     * @param endpoint an endpoint
     * @param version Service code version
     * @param serviceHostEndpoint An endpoint of an MQTT client hosting the service
     * @param commands a list of commands being handled by the service
     * @param ftcpCommands a list of FTCP commands being handled by the service
     * @returns the pointer
     */
    static SharedPtr makeShared(
            const SoaClientEndpoint::SharedPtr & endpoint,
            const std::string & version,
            const SoaClientEndpoint::SharedPtr & serviceHostEndpoint,
            const std::unordered_set<std::string> & commands,
            const std::unordered_set<std::string> & ftcpCommands);
    /**
     * Destructor
     */
	virtual ~SoaServiceContract();
	/**
	 * Return Service endpoint
	 * @return The endpoint
	 */
	SoaClientEndpoint::SharedPtr getServiceEndpoint();

	/**
	 * Service version
	 * @return The version
	 */
	const std::string & getVersion();

	/**
	 * Return Service an endpoint of a client hosting the service
	 * @return The endpoint
	 */
	SoaClientEndpoint::SharedPtr getServiceHostEndpoint();

	/**
	 * Returns a list of commands handled by the service
	 */
	const std::unordered_set<std::string>& getCommands();

    /**
     * Returns a list of FTCP commands handled by the service
     */
    const std::unordered_set<std::string>& getFtcpCommands();
	/**
	 * Returns true if the contract contains the command
	 * @param command a command
	 */
	bool canHandleCommand(const std::string & command);
    /**
     * Returns true if the contract contains the command
     * @param command a command
     */
    bool canHandleFtcpCommand(const std::string & command);

	struct Hasher {
		size_t operator()(const SoaServiceContract::SharedPtr & obj) const;
	};
	/**
	 * Structure to be used as a comparator function in SmInstalledServiceMetadata hash set (HashSet).
	 */
	struct Comparator {
		size_t operator()(const SoaServiceContract::SharedPtr & obj1, const SoaServiceContract::SharedPtr & obj2) const;
	};
	/**
	 * A hash set type where SmInstalledServiceMetadata instances are resolved by service endpoints.
	 */
	typedef std::unordered_set<SoaServiceContract::SharedPtr, SoaServiceContract::Hasher, SoaServiceContract::Comparator> HashSet;

private:
        SoaClientEndpoint::SharedPtr m_endpoint;
        std::string m_version;
        SoaClientEndpoint::SharedPtr m_serviceHostEndpoint;
        std::unordered_set<std::string> m_commands;
        std::unordered_set<std::string> m_ftcpCommands;
        /**
         * Service contract constructor
         * @param endpoint an endpoint
         * @param version Service code version
         * @param serviceHostEndpoint An endpoint of an MQTT client hosting the service
         * @param commands list of command IDs the service can handle
         */
        SoaServiceContract(
                const SoaClientEndpoint::SharedPtr & endpoint,
                const std::string & version,
        		const SoaClientEndpoint::SharedPtr & serviceHostEndpoint,
        		const std::unordered_set<std::string> & commands,
        		const std::unordered_set<std::string> & ftcpCommands);
        static bool validateComponents(
                const SoaClientEndpoint::SharedPtr & endpoint,
                const std::string & version,
        		const SoaClientEndpoint::SharedPtr & serviceHostndpoint);

};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOASERVICECONTRACT_HPP_ */
