//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2018.  Ford Motor Company owns all rights to this work and intends
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
 * soa_initialized_listener.hpp
 *
 *  Created on: May 14, 2018
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_INITIALIZED_LISTENER_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_INITIALIZED_LISTENER_HPP_

#include <soa/utils/soa_resource_manager.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Common parent call for Consumer and Provider variations
 * Public users will derive from the Consumer and Provider pure virtual sub classes
 */
class SoaInitializedListener : public SoaResourceManager::Resource {
    protected:
        /**
         * Constructor
         */
        explicit SoaInitializedListener(const char * classname): SoaResourceManager::Resource( classname ) {}

    public:
        /**
         * Destructor
         * Removal from the resource manager is done by the derived class implementing
         * the callback
         */
        virtual ~SoaInitializedListener() = default;

};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_INITIALIZED_LISTENER_HPP_ */
