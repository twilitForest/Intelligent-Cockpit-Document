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
 * SoaActionResult.h
 *
 *  Created on: Apr 6, 2017
 *      Author: ASERGEE2
 */

#ifndef ECGSOA_FRAMEWORK_SOAACTIONRESULT_HPP_
#define ECGSOA_FRAMEWORK_SOAACTIONRESULT_HPP_

#include <memory>
#include <soa/framework/soa_error_code.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Generic SOA result class template.
 * Container class used to receive both the error and/or result object(s) as a
 * return value from blocking SOA framework methods.
 * @param class Result A container class for the result data of the specific SOA framework
 *                     method called that returns an object of the instantiated class from
 *                     this template.
 * @see soa_consumer.hpp
 * @see soa_message.hpp
 * @see soa_message_payload.hpp
 */
template <class Result>
class SoaActionResult {
private:
	std::shared_ptr<Result> mResult;
	SoaErrorCode mError;
public:
    /**
     * Shared pointer type for this class
     */
    typedef std::shared_ptr<SoaActionResult<Result>> SharedPtr;

    /**
     * Action result object factory, provided all arguments
     * @param result a pointer to object of the class the SoaActionResult class template is
     *               instantiated with
     * @param error a SoaErrorCode value
     * @return a shared pointer to the created SoaActionResult<> object
     */
    static SharedPtr createResult(const std::shared_ptr<Result> & result, SoaErrorCode error) {
        return std::make_shared<SoaActionResult>(result, error);
    }

    /**
     * Convenience action result object factory, provided only an error value.
     * The result pointer is set to nullptr
     * @param error a SoaErrorCode value
     * @return a shared pointer to the created SoaActionResult<> object
     */
    static SharedPtr createResult(SoaErrorCode error) {
        return createResult(nullptr, error);
    }

    /**
     * Convenience action result object factory for "success" cases, provided only a result object.
     * The error value is set to SoaErrorCode::NO_ERROR
     * @param result a pointer to object of the class the SoaActionResult class template is
     *               instantiated with
     * @return a shared pointer to the created SoaActionResult<> object
     */
    static SharedPtr createResult(const std::shared_ptr<Result> & result) {
        return createResult(result, SoaErrorCode::NO_ERROR);
    }

    /**
     * Convenience action result object factory for "success" cases, provided only a result object.
     * The error value is set to SoaErrorCode::NO_ERROR
     * Internally a shared pointer of "result" is created: std::make_shared<Result>(result)
     * @param result a pointer to object of the class the SoaActionResult class template is
     *               instantiated with
     * @return a shared pointer to the created SoaActionResult<> object
     */
    static SharedPtr createResult(const Result & result) {
        return createResult(std::make_shared<Result>(result), SoaErrorCode::NO_ERROR);
    }

    /**
     * The constructor
     * @param result a pointer to object of the class the SoaActionResult class template is
     *               instantiated with
     * @param error a SoaErrorCode value
     */
	SoaActionResult(const std::shared_ptr<Result> & result, SoaErrorCode error) {
		mResult = result;
		mError = error;
	}

	/**
	 * Destructor
	 */
	virtual ~SoaActionResult() = default;

	/**
	 * Default copy constructor and assignment operator
	 */
	SoaActionResult(const SoaActionResult &) = default;
	SoaActionResult & operator=(const SoaActionResult &) = default;

    /**
     * Returns the action result or NULL
     * @return a shared pointer to an object of the class the SoaActionResult class template is instantiated with
     *         The pointer may be nullptr
     */
	inline std::shared_ptr<Result> getResult() {
		return mResult;
	}
	/**
	 * Returns the error code
	 * @return the stored SoaErrorCode value
	 */
	inline SoaErrorCode getError() {
		return mError;
	}

	/**
	 * Returns true is getError() == SoaErrorCode::NO_ERROR
	 * @return bool
	 */
	inline bool success() {
		return mError == SoaErrorCode::NO_ERROR;
	}


};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* ECGSOA_FRAMEWORK_SOAACTIONRESULT_HPP_ */
