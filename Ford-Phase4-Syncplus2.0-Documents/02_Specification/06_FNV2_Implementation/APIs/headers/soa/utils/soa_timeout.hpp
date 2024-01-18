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
 * soa_timeout.hpp
 *
 *  Created on: Aug 4, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_UTILS_SOA_TIMEOUT_HPP_
#define INCLUDE_UTILS_SOA_TIMEOUT_HPP_

#include <chrono>

namespace fnv {
namespace soa {
namespace framework {

/**
 * A helper class to track remaining timeout
 */
class SoaTimeout {
public:
	/**
	 * A timeout unit typedef
	 */
	typedef std::chrono::milliseconds TimeSpan;
	/**
	 * Creates a timeout with initial value
	 * @param initial An initial timeout
	 */
	explicit SoaTimeout(TimeSpan initial);
    /**
     * Creates a timeout with initial value
     * @param initial An initial timeout
     */
    explicit SoaTimeout(int32_t initial);
	/**
	 * Returns remaining timeout
	 * @return TimeSpan
	 */
	TimeSpan remained() const;
private:
	TimeSpan m_initialValue;
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;


};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_TIMEOUT_HPP_ */
