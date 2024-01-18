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
 * soa_service_metadata.hpp
 *
 *  Created on: May 31, 2017
 *      Author: asergee2
 */

#ifndef INCLUDE_CPP_FRAMEWORK_SOA_SERVICE_METADATA_HPP_
#define INCLUDE_CPP_FRAMEWORK_SOA_SERVICE_METADATA_HPP_

#include <memory>
#include <soa/framework/soa_service_contract.hpp>
#include <soa/framework/soa_service_status.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Class container for SoaServiceContract and SoaServiceStatus
 */
class SoaServiceMetadata {
    public:
        /**
         * Typedef of scoped shared pointer type to this class
         */
        typedef std::shared_ptr<SoaServiceMetadata> SharedPtr;

        /**
         * The constructor
         * @param serviceContract a service contract
         * @param serviceStatus a service status
         */
        inline SoaServiceMetadata(SoaServiceContract::SharedPtr serviceContract, SoaServiceStatus serviceStatus):
                m_serviceContract(serviceContract), m_serviceStatus(serviceStatus) { }
        /**
         * Destructor
         */
        ~SoaServiceMetadata() = default;

        /**
         * Returns service contract
         */
        inline SoaServiceContract::SharedPtr getContract() {
            return m_serviceContract;
        }

        /**
         * returns service status
         */
        inline SoaServiceStatus getStatus() {
            return m_serviceStatus;
        }

        /**
         * Returns true is the other endpoint is equal to this
         * @param other another endpoint
         * @returns the result of comparison
         */
        inline bool endpointEquals(const SoaServiceMetadata & other) {
            return m_serviceContract->getServiceEndpoint()->hashcode() == other.m_serviceContract->getServiceEndpoint()->hashcode();
        }

    private:
        SoaServiceContract::SharedPtr m_serviceContract;
        SoaServiceStatus m_serviceStatus;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_CPP_FRAMEWORK_SOA_SERVICE_METADATA_HPP_ */
