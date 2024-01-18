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
 * soa_command_handler_list.hpp
 *
 *  Created on: May 31, 2017
 *      Author: asergee2
 */

#ifndef INCLUDE_CPP_FRAMEWORK_SOA_COMMAND_HANDLER_LIST_HPP_
#define INCLUDE_CPP_FRAMEWORK_SOA_COMMAND_HANDLER_LIST_HPP_

#include <memory>
#include <vector>
#include <soa/framework/soa_service_metadata.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Class container for service metada list
 */
class SoaServiceMetadataList {
    public:
        /**
         * Typedef of scoped shared pointer type to this class
         */
        typedef std::shared_ptr<SoaServiceMetadataList> SharedPtr;

        /**
         * The constructor
         */
        SoaServiceMetadataList();

        /**
         * The destructor
         */
        virtual ~SoaServiceMetadataList();

        /**
         * Returns the list size
         */
        size_t size();

        /**
         * Adds an item to the list.
         * @param item a shared pointer to a service meta data object
         * @returns True if item has been added successfully, false if the same item already exists
         */
        bool add(SoaServiceMetadata::SharedPtr item);

        /**
         * Returns a shared pointer to the service meta data object at specified index
         * @param index an index
         * @returns the item or empty shared pointer if the index is out of range.
         */
        SoaServiceMetadata::SharedPtr get(size_t index);

    private:
        std::vector<SoaServiceMetadata::SharedPtr> m_list;
        bool hasItem(const SoaServiceMetadata & item);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_CPP_FRAMEWORK_SOA_COMMAND_HANDLER_LIST_HPP_ */
