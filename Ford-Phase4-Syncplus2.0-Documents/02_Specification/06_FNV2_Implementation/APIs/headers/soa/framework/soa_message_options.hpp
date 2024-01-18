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
 * SoaMessageOptions.h
 *
 *  Created on: Apr 04, 2017
 *      Author: PREISSNE
 */

#ifndef FNVSOA_FRAMEWORK_SOAMESSAGEOPTIONS_HPP_
#define FNVSOA_FRAMEWORK_SOAMESSAGEOPTIONS_HPP_

#include <memory>
namespace fnv {
namespace soa {
namespace framework {

/**
 * @class SoaMessageOptions
 *
 * Container class for message send options
 */
class SoaMessageOptions
{
private:
	int32_t qos;
	bool retained;
public:
    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaMessageOptions> SharedPtr;

	/**
	 * @Default constructor
	 */
	SoaMessageOptions(){
	    qos = 0;
	    retained = false;
	}

	/**
	 * @Constructor that sets qos and retained with given value
	 * @param qos a integer specifying the desired message QoS
	 * @param retained a boolean specifying if this message should be retained
	 */
	SoaMessageOptions(int32_t in_qos, bool in_retained)
	    :
	    qos(in_qos), retained(in_retained)
	{
	    // nothing else to do
	}
	/**
	 * @brief Quality of service option getter
	 *
	 * @return The QOS (Quality of service) value (0, 1, 2)
	 */
	int32_t getQos() const {
		return qos;
	}

	/**
	 * @brief Quality of service option setter
	 * The qos is set to a value of 0, 1 or 2. Any value provided less than 0
	 * sets it to 0. Any value greater than 2 sets it to 2.
	 *
	 * @param qos The QOS (Quality of service) value to set
	 */
	void setQos(int32_t qos) {
	    if (qos < 0) {
	        qos = 0;
	    } else if (qos > 2) {
	        qos = 2;
	    }
		this->qos = qos;
	}

	/**
	 * @brief Boolean getter for retained setting
	 *
	 * @return true if retained, false otherwise
	 */
	bool isRetained() const {
		return retained;
	}

	/**
	 * @brief Setter for retained option
	 *
	 * @param retained If the message is to be retained or not
	 */
	void setRetained(bool retained) {
		this->retained = retained;
	}
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif // FNVSOA_FRAMEWORK_SOAMESSAGEOPTIONS_HPP_

