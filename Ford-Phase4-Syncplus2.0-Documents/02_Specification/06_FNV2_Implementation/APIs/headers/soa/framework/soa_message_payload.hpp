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
 * soa_message_payload.hpp
 *
 *  Created on: Jul 5, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_CPP_FRAMEWORK_SOA_MESSAGE_PAYLOAD_HPP_
#define INCLUDE_CPP_FRAMEWORK_SOA_MESSAGE_PAYLOAD_HPP_

#include <type_traits>
#include <soa/framework/soa_action_result.hpp>
#include <soa/framework/soa_error_code_util.hpp>

namespace fnv {
namespace soa {
namespace framework {

/*
 * SOA message payload base class.
 */
class SoaMessagePayload {
public:
    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaMessagePayload> SharedPtr;
	// Default virtual destructor
	virtual ~SoaMessagePayload() = default;
	// Default copy constructor
	SoaMessagePayload(const SoaMessagePayload &) = delete;
	// Default assignment operator
	SoaMessagePayload & operator = (const SoaMessagePayload &) = delete;

	/**
	 * Serializes the payload to a string
	 * @return An SoaActionResult including the action status and a string representing a serialized payload
	 */
	SoaActionResult<std::string>::SharedPtr serializeAsString();

    /**
     * Serializes the payload to a string
     * @return a string representing a serialized payload
     *          an empty string in case of serialization error.
     */
    std::string serializeToString();
protected:
    const std::string k_className;
	virtual std::string serializeAsStringInternal() = 0;
	SoaMessagePayload();
    /**
     * Constructor
     * @param logTag a log tag
     */
    explicit SoaMessagePayload(const std::string & logTag);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */


#endif /* INCLUDE_CPP_FRAMEWORK_SOA_MESSAGE_PAYLOAD_HPP_ */
