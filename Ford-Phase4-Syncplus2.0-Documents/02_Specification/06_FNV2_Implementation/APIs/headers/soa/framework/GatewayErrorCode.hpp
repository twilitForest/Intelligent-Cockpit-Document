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
 * GatewayErrorCode.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: frisache
 */

#ifndef GATEWAYERRORCODE_HPP_
#define GATEWAYERRORCODE_HPP_

namespace fnv {
namespace soa {
namespace gateway {

enum class GatewayErrorCode: int {
	/**
	 * Gateway error codes
	 */
	NO_ERROR = 1000,               			//!< NO_ERROR
	ERROR_IPC_SERVER_NOT_STARTED,         	//!< an IPC server was not started
	ERROR_IPC_SERVER_ALREADY_STARTED,     	//!< an IPC server is already started
    ERROR_IPC_SERVER_INITIALIZATION_FAILED, //!< an IPC server initialization error
    ERROR_IPC_SERVER_CONNECTION_FAILED,     //!< an IPC server connection error
	ERROR_IPC_CLIENT_NOT_STARTED,         	//!< an IPC client was not started
	ERROR_IPC_CLIENT_ALREADY_STARTED,     	//!< an IPC client is already started
    ERROR_IPC_CLIENT_INITIALIZATION_FAILED, //!< an IPC client initialization error
    ERROR_IPC_CLIENT_CONNECTION_FAILED,     //!< an IPC client connection error
    ERROR_INITIALIZATION_FAILED,            //!< General initialization failure
    ERROR_STOP_FAILED,                      //!< General  failure while stopping
    ERROR_IPC_INVALID_MESSAGE,              //!< IPC invalid message
    ERROR_IPC_EMPTY_MESSAGE                 //!< IPC empty message

};

} /* namespace gateway */
} /* namespace soa */
} /* namespace fnv */

#endif /* GATEWAYERRORCODE_HPP_ */
