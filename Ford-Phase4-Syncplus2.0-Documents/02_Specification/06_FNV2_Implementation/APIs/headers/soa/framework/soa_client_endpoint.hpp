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
 * soa_client_endpoint.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOACLIENTENDPOINT_HPP_
#define FNVSOA_FRAMEWORK_SOACLIENTENDPOINT_HPP_

#include <memory>
#include <unordered_set>
#include <soa/framework/soa_client_endpoint_compile_time.hpp>

namespace fnv {
namespace soa {
namespace framework {

/*
 * Abstract SOA client endpoint class.
 */
class SoaClientEndpoint {
public:
    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaClientEndpoint> SharedPtr;

    /**
     * Typedef of scoped shared pointer type to this class that disallows object modification
     */
    typedef std::shared_ptr<const SoaClientEndpoint> ConstSharedPtr;

	/**
	 * MQTT endpoint factory method
	 * @param endpoint a reference to a endpoint topic string
	 *                 The referenced string is copied into the created endpoint object. The caller
	 *                 is free to destroy the referenced string after calling this factory method.
	 * @return a shared pointer to an MQTT endpoint object if endpoint is not a null reference,
	 *         otherwise returns a nullptr
	 */
	static SharedPtr createClientEndpointMqtt(const std::string & endpoint);

	/**
	 * MQTT endpoint factory method
         * @param endpoint a pointer to a character array holding the topic
	 * @return a shared pointer to an MQTT endpoint object if endpoint is not a null pointer,
	 *         otherwise returns a nullptr
	 */
	static SharedPtr createClientEndpointMqtt(const char * endpoint);

	/**
	 * IPC endpoint factory method - not used
	 * @return nullptr
	 */
	static SharedPtr createClientEndpointIpc(const std::string & endpoint);

	/**
	 * Destructor
	 */
	virtual ~SoaClientEndpoint() = default;

	/**
	 * Returns a hashcode for the endpoint.
	 * The hashcode is created during initialization, so this call does not use CPU
	 * resources to calculate it.
	 * @return The hashcode
	 */
	virtual size_t hashcode() const = 0;

	/**
	 * Returns the endpoint as a string
	 * @return a reference to the string representation of the endpoint contained in
	 *         this object. The string reference is only valid during the lifecycle
	 *         of this endpoint object.
	 */
	virtual const std::string & toString() const = 0;

	/**
	 * Comparison for equality
	 * @param other the other endpoint to compare to this endpoint
	 * @return true if this endpoint equals the other endpoint
	 */
	bool operator==(const SoaClientEndpoint& other) const
    {
             return isEqual(other);
    }
	
	virtual bool isEqual(const SoaClientEndpoint& other) const = 0;

	/**
	 * Comparison for not equality
	 * @param other the other endpoint to compare to this endpoint
	 * @return true if this endpoint not equals the other endpoint
	 */
	bool operator!=(const SoaClientEndpoint& other) const
    {
         return !isEqual(other);
    }

	/**
	 * Comparison method for shared pointers
	 * @param ep1 EP1
	 * @param ep2 EP2
	 * @return bool
	 */
	static bool equal(const SharedPtr & ep1, const SharedPtr & ep2);
	/**
	 * Structure to be used as a hasher function in SoaClientEndpoint::SharedPtr hash set (HashSet).
	 */
	struct Hasher {
		size_t operator()(const SoaClientEndpoint::SharedPtr & obj) const;
	};
	/**
	 * Structure to be used as a comparator function in SoaClientEndpoint::SharedPtr hash set (HashSet).
	 */
	struct Comparator {
		size_t operator()(const SoaClientEndpoint::SharedPtr & obj1, const SoaClientEndpoint::SharedPtr & obj2) const;
	};

	/**
	 * A hash set type where SoaClientEndpoint::SharedPtr instances are resolved by SoaClientEndpoint hash code.
	 */
	typedef std::unordered_set<SoaClientEndpoint::SharedPtr, SoaClientEndpoint::Hasher, SoaClientEndpoint::Comparator> HashSet;

	/**
	 * Empty SoaClientEndpoint::HashSet;
	 * @return SoaClientEndpoint::HashSet
	 */
	const static HashSet emptySet;

	/**
	 * Methods for creating endpoint names at compile time
	 * @param string literal holding endpoint suffix
	 * @return a type containing a character array holding the endpoint name
	 */

	template<int stringLengthWithNull>
	static constexpr auto createRequestEndpointName(const char (&literal)[stringLengthWithNull])
	-> internal::StringArray<stringLengthWithNull + REQUESTPREFIXSIZE - 2> { //subtract 2 to remove the two null characters
		static_assert(stringLengthWithNull > 0, "string length must be greater than 0");
		static_assert(stringLengthWithNull < (1 << 16), "string length must be less than 65536"); //MQTT Topic length < 65536
		return internal::asStringLiteral(REQUESTPREFIX) + internal::StringLiteral<stringLengthWithNull - 1>(literal);
	}

	template<int stringLengthWithNull>
	static constexpr auto createDataEndpointName(const char (&literal)[stringLengthWithNull])
	-> internal::StringArray<stringLengthWithNull + DATAPREFIXSIZE - 2> { //subtract 2 to remove the two null characters
		static_assert(stringLengthWithNull > 0, "string length must be greater than 0");
		static_assert(stringLengthWithNull < (1 << 16), "string length must be less than 65536"); //MQTT Topic length < 65536
		return internal::asStringLiteral(DATAPREFIX) + internal::StringLiteral<stringLengthWithNull - 1>(literal);
	}

	template<int stringLengthWithNull>
	static constexpr auto createResponseEndpointName(const char (&literal)[stringLengthWithNull])
	-> internal::StringArray<stringLengthWithNull + RESPONSEPREFIXSIZE - 2> { //subtract 2 to remove the two null characters
		static_assert(stringLengthWithNull > 0, "string length must be greater than 0");
		static_assert(stringLengthWithNull < (1 << 16), "string length must be less than 65536"); //MQTT Topic length < 65536
		return internal::asStringLiteral(RESPONSEPREFIX) + internal::StringLiteral<stringLengthWithNull - 1>(literal);
	}
	/**
	 * Validates an endpoint.
	 *      1. Publish topic should not contain MQTT wildcards: '#' or '+'
	 * @return True if the endpoint is a valid publish endpoint
     *              "SERVICES/DATA/DATA" returns true
     *              "SERVICES/DATA/DATA_GROUP/#" returns false
     *              "SERVICES/DATA/+/DATA" returns false
	 */
	virtual bool isValidPublishEndpoint() const {
	    // stub implementation
	    return false;
	}
	/**
	 * Checks if an endpoint contains MQTT wildcards: '#' or '+'
	 * @return True if the endpoint contains NQTT wildcards.
	 *              "SERVICES/DATA/DATA" returns false
	 *              "SERVICES/DATA/DATA_GROUP/#" returns true
	 *              "SERVICES/DATA/+/DATA" returns true
	 */
	virtual bool hasWildcards() const {
        // stub implementation
        return false;
	}
protected:
    /**
     * @brief Constructor
     */
    SoaClientEndpoint() = default;
//    {
//        //No implementation needed
//    }
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOACLIENTENDPOINT_HPP_ */
